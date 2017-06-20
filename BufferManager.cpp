#include "BufferManager.h"
#include "Minisql.h"
#include <stdlib.h>
#include <string>
#include <cstring>
#include <queue>

static int replaced_block = -1;

//Constructor Function: allocate memories for the pools and init the variable values
BufferManager::BufferManager():total_block(0),total_file(0),fileHead(NULL)
{
	int i;
	for(i=0; i<MAX_FILE_NUM; i++)
	{
		file_pool[i].fileName = new char[MAX_FILE_NAME];
		if(file_pool[i].fileName == NULL)
		{
			printf("Can't allocate memory to initing the file pool!\n");
			exit(1);
		}
		init_file(file_pool[i]);
	}
	for(i=0; i<MAX_BLOCK_NUM; i++)
	{
		block_pool[i].address = new char[BLOCK_SIZE];
		if(block_pool[i].address == NULL)
		{
			printf("Can't allocate memory to initing the file pool!\n");
			exit(1);
		}
		block_pool[i].fileName = new char[MAX_FILE_NAME];
		if(block_pool[i].fileName == NULL)
		{
			printf("Can't allocate memory to initing the file pool!\n");
			exit(1);
		}
		init_block(block_pool[i]);
	}
}


//init the fileNode 
void BufferManager::init_file(fileNode &file)
{
	file.nextFile = NULL;
	file.preFile = NULL;
	file.blockHead = NULL;
	file.pin = false;
	memset(file.fileName,0,MAX_FILE_NAME);
}

//init_block
void BufferManager::init_block(blockNode &block)
{
	memset(block.address,0,BLOCK_SIZE);//block_size = 4k
	size_t init_usage = 0;
	memcpy(block.address, (char*)&init_usage, sizeof(size_t)); // set the block head

	block.using_size = sizeof(size_t);
	block.dirty = false;
	block.nextBlock = NULL;
	block.preBlock = NULL;
	block.offsetNum = -1;//initial the offset
	block.pin = false;//don't pinned
	block.reference = false;
	block.ifbottom = false;

	memset(block.fileName,0,MAX_FILE_NAME);
}

//Destructor function
BufferManager::~BufferManager()
{
	writtenBackToDiskAll();
	for (int i = 0; i < MAX_FILE_NUM; i ++)
	{
		delete [] file_pool[i].fileName;
	}
	for (int i = 0; i < MAX_BLOCK_NUM; i ++)
	{
		delete [] block_pool[i].address;
	}
}

//write all block nodes in the list into the disk
void BufferManager::writtenBackToDiskAll()
{
	blockNode *btmp = NULL;
	fileNode *ftmp = NULL;
	if(fileHead)
	{
		for(ftmp = fileHead;ftmp != NULL;ftmp = ftmp ->nextFile)
		{
			if(ftmp->blockHead)
			{
				for(btmp = ftmp->blockHead;btmp != NULL;btmp = btmp->nextBlock)
				{
					if(btmp->preBlock)init_block(*(btmp -> preBlock));
					writtenBackToDisk(btmp->fileName, btmp);
				}
			}
		}
	}
}

//write the blocks into the disk, if the block is not dirty, do nothing
void BufferManager::writtenBackToDisk(const char* fileName,blockNode* block)
{
	if(!block->dirty)//if the block not changed don't write to 
	{
		return;
	}
	else
	{
		FILE* fp = NULL;
		if((fp = fopen(fileName,"rb+")) != NULL)
		{
			if(fseek(fp, block->offsetNum*BLOCK_SIZE, 0) == 0)
			{
				if(fwrite(block->address, block->using_size+sizeof(size_t), 1, fp) != 1)
				{
					printf("Error writing the file %s in writtenBackToDisk",fileName);
					fclose(fp);
					exit(1);
				}
				else
				{
					fclose(fp);
					return;
				}
			}
			else
			{
				printf("Error seeking the file %s in writtenBackToDisk",fileName);
				fclose(fp);
				exit(1);
			}
		}
		else 
		{
			printf("Error in opening the file %s in writtenBackToDisk",fileName);
			fclose(fp);
			exit(1);
		}
	}
}

//get the head block of the file
blockNode* BufferManager::getBlockHead(fileNode * file)
{
	if(file->blockHead != NULL)
	{
		if(file->blockHead->offsetNum == 0) //the head should in the 0 offset place
		{
			return (file->blockHead);
		}
		else 
		{
			return(getBlock(file, NULL));//ReGet the HeadBlock
		}
	}
	else 
	{
		return(getBlock(file,NULL));//get the HeadBlock
	}
}

//get next block of the node
blockNode* BufferManager::getNextBlock(fileNode* file,blockNode* block)
{
	if(block->nextBlock == NULL)
	{
		if(block->ifbottom) block->ifbottom = false;
		return getBlock(file, block);
	}
	else //nextblock  not null
	{
		if(block->offsetNum == block->nextBlock->offsetNum - 1)
		{
			return block->nextBlock;
		}
		else //the block list is not in the right place
		{
			return getBlock(file, block);
		}
	}
}

//get a block node
//if the block is already in the list, return it
//if not in the list, using LRU, if need allocate more space
blockNode* BufferManager::getBlock(fileNode * file,blockNode *position, bool pin)
{
	const char * fileName = file->fileName;
	blockNode * btmp = NULL;

	btmp = replaceBlock();
	//add the block into the block list
	addBlocktoList(file, position, btmp);

	if(strlen(fileName) + 1 > MAX_FILE_NAME)
	{
		printf("文件名长度过长，最高不能超过%d",MAX_FILE_NAME);
		exit(3);
	}
	set_pin(*btmp, pin);
	strncpy(btmp->fileName, fileName, MAX_FILE_NAME);
	//get the file content to the block
	FILE* fp;
	if((fp = fopen(fileName, "ab+")) !=NULL)
	{
		if(fseek(fp, btmp->offsetNum*BLOCK_SIZE, 0) == 0)
		{
			if(fread(btmp->address, 1, BLOCK_SIZE, fp) == 0) btmp->ifbottom = true;
			btmp->using_size = getUsingSize(btmp);
			fclose(fp);
		}
		else
		{
			printf("Error in seeking the file %s in reading",fileName);
			fclose(fp);
			exit(1);
		}
	}
	else
	{
		printf("Error in opening the file %s in writtenBackToDisk",fileName);
		fclose(fp);
		exit(1);
	}
	return btmp;
}

//repalace the block using LUR
blockNode* BufferManager::replaceBlock()
{
	blockNode * btmp = NULL;
	if(total_block == 0)//if the pool is empty
	{
		btmp = &block_pool[0];
		total_block++;
	}
	else if(total_block < MAX_BLOCK_NUM)//pool is not full
	{
		for(int i=0;i < MAX_FILE_NUM; i++)
		{
			if(block_pool[i].offsetNum == -1)//find the empty block
			{
				btmp = &block_pool[i];
				total_block++;
				break;
			}
			//else continue;
		}
	}
	else if(total_block >= MAX_BLOCK_NUM)
	{
		int i = replaced_block;
		while(1)//LRU, find the next not used block.
		{
			i++;
			if(i >= total_block) i =0;
			if(!block_pool[i].pin)
			{
				if(block_pool[i].reference == true) block_pool[i].reference = false;
				else
				{
					btmp = &block_pool[i];
					if(btmp->nextBlock) btmp -> nextBlock -> preBlock = btmp -> preBlock;
					if(btmp->preBlock) btmp -> preBlock -> nextBlock = btmp -> nextBlock;
					replaced_block = i;//record the replaced block and begin from the next block the next time.

					writtenBackToDisk(btmp->fileName, btmp);
					init_block(*btmp);
					break;
				}
			}
			else continue;//locked
		}
	}
	return btmp;
}

//add block to the blocklist
void BufferManager::addBlocktoList(fileNode* file,blockNode* position, blockNode* btmp)
{
	if(position != NULL && position->nextBlock == NULL)
	{
		btmp -> preBlock = position;
		position -> nextBlock = btmp;
		btmp -> offsetNum = position -> offsetNum + 1;	
	}
	else if(position != NULL && position->nextBlock != NULL)
	{
		btmp->preBlock = position;
		btmp->nextBlock = position->nextBlock;
		position->nextBlock->preBlock = btmp;
		position->nextBlock = btmp;
		btmp -> offsetNum = position -> offsetNum + 1;
	}
	else //head of the list
	{
		btmp->offsetNum = 0;
		if(file->blockHead)
		{
			file->blockHead->preBlock = btmp;
			btmp->nextBlock = file->blockHead;
		}
		file->blockHead = btmp;
	}
}

// Get the block of the file by offset
blockNode* BufferManager::getBlockByOffset(fileNode* file, int offsetNumber)
{
	blockNode* btmp = NULL;
	if(offsetNumber == 0) return getBlockHead(file);
	else
	{
		btmp = getBlockHead(file);
		while( offsetNumber > 0)
		{
			btmp = getNextBlock(file, btmp);
			offsetNumber --;
		}
		return btmp;
	}
}

//get a FileNode
//if not in the list, replace it;
fileNode* BufferManager::getFile(const char * fileName, bool pin)
{
	blockNode* btmp = NULL;
	fileNode * ftmp = NULL;
	if(fileHead!=NULL)
	{
		for(ftmp = fileHead; ftmp != NULL; ftmp = ftmp->nextFile)
		{
			if(!strcmp(fileName, ftmp->fileName))
			{
				ftmp->pin = pin;
				return ftmp;
			}
		}
	}
	//if fileNode is not in the list
	if(total_file == 0)
	{
		ftmp = &file_pool[total_file];
		total_file ++;
		fileHead = ftmp;
	}
	else if(total_file < MAX_BLOCK_NUM)
	{
		ftmp = &file_pool[total_file];//add the filenode into the tail of the lis
		file_pool[total_file - 1].nextFile = ftmp;
		ftmp->preFile = &file_pool[total_file-1];
		total_file++;
	}
	else
	{
		ftmp = fileHead;
		while(ftmp->pin)
		{
			if(ftmp->nextFile)ftmp = ftmp->nextFile;
			else
			{
				cout << "There are no enough file node in the pool!"<< endl;
				exit(1);
			}
		}
		for(btmp = ftmp->blockHead; btmp != NULL; btmp = btmp->nextBlock)
		{
			if(btmp->preBlock)
			{
				init_block(*(btmp->preBlock));
				total_block --;
			}
			writtenBackToDisk(btmp->fileName,btmp);
		}
		init_file(*ftmp);
	}
	if(strlen(fileName) + 1 > MAX_FILE_NAME)
	{
		printf("文件名长度过长，最高不能超过%d\n",MAX_FILE_NAME);
		exit(1);
	}
	strncpy(ftmp->fileName, fileName,MAX_FILE_NAME);
	set_pin(*ftmp, pin);
	return ftmp;
}

//delete the file node and its block node
void BufferManager::delete_fileNode(const char * fileName)
{
	fileNode* ftmp = getFile(fileName);
	blockNode* btmp = getBlockHead(ftmp);
	queue<blockNode*> blockQ;
	while(1)
	{
		if(btmp == NULL) return;
		blockQ.push(btmp);
		if(btmp->ifbottom) break;
		btmp = getNextBlock(ftmp, btmp);
	}
	total_block -= blockQ.size();
	while(!blockQ.empty())
	{
		init_block(*blockQ.back());//reset the blocks
		blockQ.pop();
	}
	if(ftmp->preFile) ftmp->preFile->nextFile = ftmp->nextFile;
	if(ftmp->nextFile) ftmp->nextFile->preFile = ftmp->preFile;
	if(fileHead == ftmp) fileHead = ftmp->nextFile;
	init_file(*ftmp);
	total_file --;
}


int BufferManager::getBlockSize() //Get the size of the block that others can use.Others cannot use the block head
{
	return BLOCK_SIZE - sizeof(size_t);
}

void BufferManager::set_pin(blockNode &block,bool pin)
{
	block.pin = pin;
	if(!pin)
		block.reference = true;
}

void BufferManager::set_pin(fileNode &file,bool pin)
{
	file.pin = pin;
}

void BufferManager::set_dirty(blockNode &block)
{
	block.dirty = true;
}

void BufferManager::clean_dirty(blockNode &block)
{
	block.dirty = false;
}


size_t BufferManager::getUsingSize(blockNode* block)
{
	return *(size_t*)block->address;
}

void BufferManager::set_usingSize(blockNode & block,size_t usage)
{
	block.using_size = usage;
	memcpy(block.address,(char*)&usage,sizeof(size_t));
}

size_t BufferManager::get_usingSize(blockNode & block)
{
	return block.using_size;
}

char* BufferManager::get_content(blockNode& block)
{
	return block.address + sizeof(size_t);
}



void Write(char* fn,int line, char* data, int size){ // 需要从形式上，经过了write buffer 
	FILE* fp = fopen(fn,"rb+");
	fseek(fp,line,0);
	fwrite(data,size,1,fp);
	int i;
	int valid;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn," ")) valid = 0; // 此处可优化，即附加行之前的完整页仍然是不变的，
	}
	fclose(fp);
}

void DeleteLine(char* fn, int line, int unit, int howmany){ // 从底部抽上来。会乱序 

	char bufin[4096];
	char writeinfo[4096];
	int thissize = howmany>4096?4096:howmany;
	thissize = (thissize/unit) * unit;
	int i,j,k;
	for(i = 0 ,j=0;i<thissize;){
		char* start;
		start = writeinfo+i;
		for(k=0;k<unit;k++){
			start[k] = bufin[thissize-(j+1)*unit + k];
		}
		i+=unit;
		j++;
	}
	Write(fn,line,writeinfo,thissize); 
	if(howmany>4096) DeleteLine(fn,line+4096,unit,howmany-thissize);
}

void DeleteLines(char* fn, int il, int unit){

	int killer = il;
	while(killer){
		int line = 0;
		DeleteLine(fn,line,unit,1*unit);
		killer ++;
	}
}

void ReWrite(char* fn, char* data, int size){
	FILE* fp = fopen(fn,"wb");
	int residual = size;
	while(residual){
		int thissize = 4096>residual?residual:4096;
		size = thissize;
		memcpy(data,data,thissize);
		fwrite(data,size,1,fp);	
		residual -= thissize;
	}	int i;

	int valid = 1;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn," ")) valid = 0; // 此处可优化，即附加行之前的完整页仍然是不变的，可不取 
	}

	fclose(fp);
}