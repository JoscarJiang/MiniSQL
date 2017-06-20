#include <iostream>
#include "RecordManager.h"
#include <cstring>
#include "API.h"

//create table
int RecordManager::tableCreate(string tableName)
{
	string tableFileName = tableFileNameGet(tableName);

	FILE *fp;
	fp = fopen(tableFileName.c_str(),"w+");
	if(fp == NULL)
	{
		return 0;
	}
	fclose(fp);
	return 1;
}

//drop the table
int RecordManager::tableDrop(string tableName)
{
	string tableFileName = tableFileNameGet(tableName);
	bm.delete_fileNode(tableFileName.c_str());
	if(remove(tableFileName.c_str()))
	{
		cout <<"droping table:"<< tableFileName << endl;
		return 0;
	}
	return 1;
}

//create a index
/*
*
*
***********************************************/
int RecordManager::indexCreate(string indexName)
{
	string indexFileName = indexFileNameGet(indexName);

	FILE *fp;
	fp = fopen(indexFileName.c_str(),"w+");
	if(fp == NULL)
	{
		return 0;
	}
	fclose(fp);
	return 1;
}

int RecordManager::indexDrop(string indexName)
{
	string indexFileName = indexFileNameGet(indexName);
	bm.delete_fileNode(indexFileName.c_str());
	if(remove(indexFileName.c_str()))
	{
		return 0;
	}
	return 1;
}

//insert the recoder
int RecordManager::recordInsert(string tableName, char* record, int recordSize)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* btmp = bm.getBlockHead(ftmp);
	while(1)//find the next not full block
	{
		if(btmp == NULL)
		{
			return -1;
		}
		if(bm.get_usingSize(*btmp) <= bm.getBlockSize() - recordSize)//可用-现在需要用的
		{
			char* addressBegin;
			addressBegin = bm.get_content(*btmp) + bm.get_usingSize(*btmp);//block's address in the memory + the using size
			memcpy(addressBegin, record, recordSize);
			bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + recordSize);
			bm.set_dirty(*btmp);
			return btmp->offsetNum;
		}
		else
		{
			btmp = bm.getNextBlock(ftmp, btmp);
		}
	}
	return -1;
}

//print all record of a table meet requirement
int RecordManager::recordAllShow(string tableName, vector<string>* attributeNameVector,  vector<Condition>* conditionVector)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* btmp = bm.getBlockHead(ftmp);
	int count = 0;
	while(1)
	{
		if(btmp == NULL)
		{
			return -1;
		}
		if(!btmp->ifbottom)
		{
			int recordBlockNum = recordBlockShow(tableName, attributeNameVector, conditionVector, btmp);
			count += recordBlockNum;
			btmp = bm.getNextBlock(ftmp, btmp);
		}
		else 
		{
			int recordBlockNum = recordBlockShow(tableName, attributeNameVector, conditionVector, btmp);
			count += recordBlockNum;
			return count;
		}
	}
	return -1;
}

//print record of a table in a block
int RecordManager::recordBlockShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector, int blockOffset)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* block = bm.getBlockByOffset(ftmp, blockOffset);
	if(block == NULL)
	{
		return -1;
	}
	else
	{
		return  recordBlockShow(tableName, attributeNameVector, conditionVector, block);
	}
}

//print record of a table in a block
int RecordManager::recordBlockShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector, blockNode* block)
{
	if(block == NULL)//if block is null;
	{
		return -1;
	}

	int count = 0;

	char* recordBegin = bm.get_content(*block);
	vector<Attribute> attributeVector;
	int recordSize = api->recordSizeGet(tableName);

	api->attributeGet(tableName, &attributeVector);
	char* blockBegin = bm.get_content(*block);
	size_t usingSize = bm.get_usingSize(*block);

	while(recordBegin - blockBegin < usingSize)//not full
	{
		if(conditionVector && recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector))
		{
			count ++;
			recordPrint(recordBegin, recordSize, &attributeVector, attributeNameVector);
			printf("\n");
		}
		recordBegin += recordSize;
	}
	return count;
}

//find the number of all record of a table meet requirement
int RecordManager::recordAllFind(string tableName, vector<Condition>* conditionVector)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* btmp = bm.getBlockHead(ftmp);
	int count = 0;
	while(1)
	{
		if(btmp == NULL) return -1;//if the node is NULL
		//if it is in the end of the list
		if(btmp->ifbottom)
		{
			int recordBlockNum = recordBlockFind(tableName, conditionVector, btmp);
			count += recordBlockNum;
			return count;//return the number of the 
		}
		else
		{
			int recordBlockNum = recordBlockFind(tableName, conditionVector, btmp);
			count += recordBlockNum;
			btmp = bm.getNextBlock(ftmp, btmp);
		}
	}
	return -1;
}

//fide the number of record of a table in a block
int RecordManager::recordBlockFind(string tableName, vector<Condition>* conditionVector, blockNode* block)
{
	if(block == NULL) return -1;//the blocknode is NULL

	int count = 0;

	char* recordBegin = bm.get_content(*block);
	vector<Attribute> attributeVector;
	int recordSize = api->recordSizeGet(tableName);

	api->attributeGet(tableName, &attributeVector);

	while(recordBegin - bm.get_content(*block) < bm.get_usingSize(*block))
	{
		if(recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector)) count++;
		recordBegin += recordSize; //move the begin point
	}
	return count; //return the number of the record

}

//delete all record which meet the requirement
int RecordManager::recordAllDelete(string tableName, vector<Condition>* conditionVector)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* btmp = bm.getBlockHead(ftmp);

	int count = 0;
	while(1)
	{
		if(btmp == NULL) return -1;
		int recordBlockNum = recordBlockDelete(tableName, conditionVector, btmp);
		count += recordBlockNum;

		if(btmp -> ifbottom)//if it is the last block
			return count;
		else btmp = bm.getNextBlock(ftmp, btmp);
	}
	return -1;
}

//delete record of a table in a block by offset
int RecordManager::recordBlockDelete(string tableName, vector<Condition>* conditionVector, int blockOffset)
{
	fileNode* ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode* block = bm.getBlockByOffset(ftmp, blockOffset);
	if(block == NULL) return -1;
	else return recordBlockDelete(tableName, conditionVector, block);
}

//delete record of a tbable by blocknode, return the number of the record meet requirements in the block
//连续存放，删除后移动之后的record
int RecordManager::recordBlockDelete(string tableName,  vector<Condition>* conditionVector, blockNode* block)
{
	//if block is null, return -1
	if(block == NULL) return -1;
	int count = 0;

	char* recordBegin = bm.get_content(*block);
	vector<Attribute> attributeVector;
	int recordSize = api->recordSizeGet(tableName);

	api->attributeGet(tableName, &attributeVector);

	while(recordBegin - bm.get_content(*block) < bm.get_usingSize(*block))
	{
		if(recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector))
		{
			count++;
			api->recordIndexDelete(recordBegin, recordSize, &attributeVector, block->offsetNum);
			int i;
			for(i=0; i + recordSize + recordBegin - bm.get_content(*block) < bm.get_usingSize(*block); i++)
			{
				recordBegin[i] = recordBegin[i + recordSize];//move the record
			}
			memset(recordBegin+1, 0, recordSize);
			bm.set_usingSize(*block, bm.get_usingSize(*block) - recordSize);
			bm.set_dirty(*block);//the content changed
		}
		else recordBegin += recordSize;
	}
	return count;
}

//insert all record of the table into the index
int RecordManager::indexRecordAllAlreadyInsert(string tableName,string indexName)
{
	fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);
	int count = 0;
	while (true)
	{
		if (btmp == NULL)
		{
			return -1;
		}
		int recordBlockNum = indexRecordBlockInsert(tableName, indexName, btmp);
		count += recordBlockNum;
		if (btmp->ifbottom) return count;
		else btmp = bm.getNextBlock(ftmp, btmp);
	} 
	return -1;
}


int RecordManager::indexRecordBlockInsert(string tableName,string indexName,  blockNode* block)
{
	if(!block) return- 1;
	int count = 0;

	char* recordBegin = bm.get_content(*block);
	vector<Attribute> attributeVector;
	int recordSize = api->recordSizeGet(tableName);

	api->attributeGet(tableName, &attributeVector);

	int type;
	int typeSize;
	char* contentBegin;

	while(recordBegin - bm.get_content(*block) < bm.get_usingSize(*block))
	{
		contentBegin = recordBegin;//point the head
		for(int i=0; i<attributeVector.size(); i++)
		{
			type = attributeVector[i].type;//get the type
			typeSize = api->typeSizeGet(type);
			//find the index of the record, insert it in to index tree
			if(attributeVector[i].index == indexName)
			{
				api->indexInsert(indexName, contentBegin, type, block->offsetNum);
				count;
			}
			contentBegin += typeSize; //change the contentBegin
		}
		recordBegin += recordSize; //change the recordBegin
	}
	return count;
}

 //judge the record that meet the requirement
 bool RecordManager::recordConditionFit(char* recordBegin, int recordSize, vector<Attribute>* attributeVector, vector<Condition>* conditionVector)
 {
	 if(conditionVector == NULL) return true;
	 int type;
	 string attributeName;
	 int typeSize;
	 char content[255];

	 char* contentBegin = recordBegin;
	 for(int i=0; i<attributeVector->size();i++)
	 {
		 type = (*attributeVector)[i].type;
		 attributeName = (*attributeVector)[i].name;
		 typeSize = api->typeSizeGet(type);

		 memset(content, 0, 255);//init content
		 memcpy(content, contentBegin, typeSize);
		 for(int j=0; j<(*conditionVector).size();j++)
		 {
			 if((*conditionVector)[j].attributeName == attributeName)
			 {
				 if(!contentConditionFit(content, type, &(*conditionVector)[j])) return false;
			 }
		 }
		 contentBegin += typeSize;
	 }
	 return true;
 }

//print the record
void RecordManager::recordPrint(char* recordBegin, int recordSize, vector<Attribute>* attributeVector, vector<string> *attributeNameVector)
{
	int type;
	string attributeName;
	int typeSize;
	char content[255];

	char* contentBegin = recordBegin;
	for(int i = 0; i < attributeVector->size(); i++)
	{
		type = (*attributeVector)[i].type;
		typeSize = api->typeSizeGet(type);

		memset(content, 0, 255);

		memcpy(content, contentBegin, typeSize);

		for(int j=0; j<(*attributeNameVector).size(); j++)
		{
			if((*attributeNameVector)[j] == (*attributeVector)[i].name)
			{
				contentPrint(content, type);
				break;
			}
		}
		contentBegin += typeSize;
	}
}

void RecordManager::contentPrint(char* content, int type)
{
	if(type == TYPE_INT) 
	{
		int tmp = *((int*) content);
		printf("%d\t", tmp);

	}
	else if(type == TYPE_FLOAT)
	{
		float tmp = *((float*) content);
		printf("%f\t", tmp);
	}
	else
	{
		string tmp = content;
		printf("%s \t", tmp.c_str());
	}
}

/**
 *
 * judge if the content meet the requirement
 * @param content: point to content
 * @param type: type of content
 * @param condition: condition
 * @return bool: the content if meet
 */
bool RecordManager::contentConditionFit(char* content,int type,Condition* condition)
{
	if (type == TYPE_INT)
	{
		//if the content is a int
		int tmp = *((int *) content);   //get content value by point
		return condition->ifRight(tmp);
	}
	else if (type == TYPE_FLOAT)
	{
		//if the content is a float
		float tmp = *((float *) content);   //get content value by point
		return condition->ifRight(tmp);
	}
	else
	{
		//if the content is a string
		return condition->ifRight(content);
	}
	return true;
}

string RecordManager::indexFileNameGet(string indexName)
{
	string tmp = "";
	return "INDEX_"+indexName;
}

string RecordManager::tableFileNameGet(string tableName)
{
	string tmp = "";
	return tmp + "TABLE_" + tableName;
}