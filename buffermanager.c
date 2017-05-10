#include "buffermanager.h"
#include "intlistmanip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void DoAppend(char* fn){  // 将写缓冲区的文件附加到文件后 
	FILE* fp = fopen(fn,"ab");
	fwrite(writeBuf->data,writeBuf->size,1,fp);
	fclose(fp);

	struct buf* pb = GlobalBuffer->head;
	int i;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) 
			pb->valid = 0; // 此处可优化，即附加行之前的完整页仍然是不变的，可不取 
		pb = pb->next;
	}
	return;
} 


// 当程序其他部件需要从硬盘上读取数据时，调用这个函数，传入（文件名，从第几个字节开始读），会得到一块存有目的数据的缓冲区
// 该区内有4096B。使用readBuf[返回值]可以得到存有数据的char数组。 
int GetReadBuffer(char* FileName, int line){ // 返回数据存进了16个readbuffer中的哪一个 ，若已到文件尾部，则返回-1 
	int result = -1;
	struct buf* pb; 
	struct buf* ppb;
	ppb = pb = (GlobalBuffer->head);

	// 如果需要的数据，已经在某个buffer中 
	while( pb && !(pb->valid && pb->line==line && !strcmp(pb->fileName,FileName))){ // 可以改进的地方：如果已有line500，则要求line600的时候，也要判定存在 
		ppb = pb;
		pb = pb->next;
	}
	if(pb && 0){
		if(pb == ppb){
			GlobalBuffer->tail->next = pb;
			GlobalBuffer->tail = pb;
			GlobalBuffer->head = pb->next;
			pb->next = 0;
		}else{
			GlobalBuffer->tail->next = pb;
			GlobalBuffer->tail = pb;
			ppb->next = pb->next;
			pb->next = 0;
		}
//		printf("H");
		return (pb->data  - readBuf[0]) / BUFFER_SIZE;
	}else
	
	// 需要从给定文件、行号中取数据。 
	{
		

		pb = GlobalBuffer->head; 
		memset(pb->data,0,BUFFER_SIZE);
		FILE *fp = fopen(FileName,"rb");
		

		int fs = GetFileSize11(FileName);
		if(line>=fs){
			fclose(fp);
			return -1;	
		} 
		fseek(fp,line,0); //移动到要开始读取的行  
		int rest = fs - line;
		fread(pb->data,1,BUFFER_SIZE,fp); //一次读4096B
		fclose(fp);
		pb->size = rest > BUFFER_SIZE ? BUFFER_SIZE : rest;
		pb->valid=1; // 此数据有效 
		strcpy(pb->fileName,FileName); //保存此缓冲区的信息
		pb->line =  line;
		GlobalBuffer->tail->next = pb;
		GlobalBuffer->tail = pb;
		GlobalBuffer->head = pb->next;
		pb->next = 0;
//		printf("BelowHere\n");
		return (pb->data - readBuf[0]) / BUFFER_SIZE;
	}
}

int Append(char* fn, char* data, int size){ // 在fn文件末尾，从data开始增附size个字节
	memset(writeBuf->data,0,BUFFER_SIZE);
	int k;
	int round = 0;
	while(size>0){
		int thissize = size>BUFFER_SIZE?BUFFER_SIZE:size;
		for(k = 0; k<thissize;k++){
			writeBuf->data[k] = data[round*BUFFER_SIZE+k];
		}
		writeBuf->size = thissize;
		DoAppend(fn);
		size-=thissize;
		round++;
	}
}

#if 1
int Read(char* fn, int line, char* bufin){ // 在fn文件第line行开始读数据，读到bufin中，返回值是读了多少字节，最多BUFFER_SIZE，若-1表示已经无内容 
	int size;
	struct buf* pb = GlobalBuffer->head;

	int rbf = GetReadBuffer(fn,line);

	if(rbf==-1)return -1;
	int i;
	for(i=0;i<16;i++){
		if(rbf== (pb->data - readBuf[0]) / BUFFER_SIZE){
			size = pb->size;
			break;
		} 
		pb = pb->next;
	}
	for(i=0;i<size;i++){
		bufin[i] = readBuf[rbf][i];
	}
	return size;
}
#endif

#if 0
int Read(char* fn, int line, char* bufin){
	FILE* fp = fopen(fn,"rb");
	int fs = GetFileSize11(fn);
	fseek(fp,line,0);
	if(fs<line){
		fclose(fp);
		return-1;
	}if(fs-line>=4096){
		fread(bufin,1,4096,fp);
		fclose(fp);
		fp=0;
		return 4096;
	}else{
		fread(bufin,1,fs-line,fp);
		fclose(fp);
		fp=0;
		return fs-line;		
	}
}
#endif
void BufsInit(){
	writeBuf = (struct writeBufStruct*) malloc(sizeof(struct writeBufStruct));
	GlobalBuffer = (Bufs*) malloc(sizeof(Bufs));
	GlobalBuffer->head = (struct buf*) malloc(sizeof(struct buf) * 16);
	int i;
	for(i =0 ; i<16;i++){
		((GlobalBuffer->head)+i)->next = ((i!=15) ? ((GlobalBuffer->head) + i+1) : 0);
		((GlobalBuffer->head)+i)->valid = 0; // 【应】【为】【0】【！】 
		((GlobalBuffer->head)+i)->size = 0;
		((GlobalBuffer->head)+i)->line = 0;
		((GlobalBuffer->head)+i)->data = readBuf[i]; // 分配一块读缓冲区 
//		char tmp[5];
//		sprintf(tmp,"%d",i*2);
//		strcpy( ((GlobalBuffer->head)+i)->fileName, tmp);
	}
	GlobalBuffer->tail = (GlobalBuffer->head) + 15;
}

void Write(char* fn,int line, char* data, int size){ // 需要改成从形式上，经过了write buffer 
	FILE* fp = fopen(fn,"rb+");
	fseek(fp,line,0);
	fwrite(data,size,1,fp);
	int i;
	struct buf* pb = GlobalBuffer->head;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) pb->valid = 0; // 此处可优化，即附加行之前的完整页仍然是不变的，可不取 
		pb = pb->next;
	}
	fclose(fp);
}

void DeleteLine(char* fn, int line, int unit, int howmany){ // 从底部抽上来。会乱序 
	int fs = GetFileSize11(fn);
	char bufin[BUFFER_SIZE];
	char writeinfo[BUFFER_SIZE];
	int thissize = howmany>BUFFER_SIZE?BUFFER_SIZE:howmany;
	thissize = (thissize/unit) * unit;
	Read(fn,fs-thissize,bufin);
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
	truncate(fn,fs-thissize);
	if(howmany>BUFFER_SIZE) DeleteLine(fn,line+BUFFER_SIZE,unit,howmany-thissize);
}

void DeleteLines(char* fn, IntList il, int unit){
	il = DecreaseIntList(il);
	IntList killer = il;
	while(killer){
		int line = killer->data;
		DeleteLine(fn,line,unit,1*unit);
		killer = killer->next;
	}
}

void ReWrite(char* fn, char* data, int size){
	FILE* fp = fopen(fn,"wb");
	int residual = size;
	while(residual){
		int thissize = BUFFER_SIZE>residual?residual:BUFFER_SIZE;
		writeBuf->size = thissize;
		memcpy(writeBuf->data,data,thissize);
		fwrite(writeBuf->data,writeBuf->size,1,fp);	
		residual -= thissize;
	}	int i;

	struct buf* pb = GlobalBuffer->head;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) pb->valid = 0; // 此处可优化，即附加行之前的完整页仍然是不变的，可不取 
		pb = pb->next;
	}

	fclose(fp);
}
