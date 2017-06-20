#include "API.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "IndexManager.h"
#include <stdlib.h>


CatalogManager *cm;
IndexManager* im;

//drop一个table
void API::tableDrop(string tableName)
{
	if (!tableExist(tableName)) return;
	
	vector<string> indexNameVector;
	
	//get all index in the table, and drop them all
	indexNameListGet(tableName, &indexNameVector);
	for (int i = 0; i < indexNameVector.size(); i++)
	{
		printf("%s", indexNameVector[i].c_str());
		
		indexDrop(indexNameVector[i]);
	}
	
	//delete a table file
	if(rm->tableDrop(tableName))
	{
		//delete a table information
		cm->dropTable(tableName);
		printf("Drop table %s successfully\n", tableName.c_str());
	}
}

//drop一个index
void API::indexDrop(string indexName)
{
	if (cm->findIndex(indexName) != INDEX_FILE)
	{
		printf("There is no index %s \n", indexName.c_str());
		return;
	}
	
	
	if (rm->indexDrop(indexName))
	{
		int indexType = cm->getIndexType(indexName);
		if (indexType == -2)
		{
			cerr<<"Drop Index Failed!\n";
			return;
		}
		//删除一个index文件
		cm->dropIndex(indexName);
		
		//删除index树
		im->dropIndex(rm->indexFileNameGet(indexName), indexType);
		printf("Drop index %s successfully\n", indexName.c_str());
	}
}

//创建一个index
void API::indexCreate(string indexName, string tableName, string attributeName)
{
	//已存在
	if (cm->findIndex(indexName) == INDEX_FILE)
	{
		cout << "There is index " << indexName << " already" << endl;
		return;
	}
	
	//没有这个表
	if (!tableExist(tableName)) return;
	
	//
	vector<Attribute> attributeVector;
	cm->attributeGet(tableName, &attributeVector);
	int i;
	int type = 0;
	//获得attribute的type
	for (i = 0; i < attributeVector.size(); i++)
	{
		if (attributeName == attributeVector[i].name)
		{
			if (!attributeVector[i].ifUnique)
			{
				cout << "the attribute is not unique" << endl;
				return;
			}
			type = attributeVector[i].type;
			break;
		}
	}

	//若不存在这个属性
	if (i == attributeVector.size())
	{
		cout << "there is not this attribute in the table" << endl;
		return;
	}
	
	//新建一个index文件
	if (rm->indexCreate(indexName))
	{
		cm->addIndex(indexName, tableName, attributeName, type);

		int indexType = cm->getIndexType(indexName);
		if (indexType == -2)
		{
			cerr << "Error in getting index type!\n";
			return;
		}
		
		//indexManager to create a index tress
		im->createIndex(rm->indexFileNameGet(indexName), indexType);
		
		//用recordmanager创建一棵树
		rm->indexRecordAllAlreadyInsert(tableName, indexName);
		printf("Create index %s successfully\n", indexName.c_str());
	}
	else
	{
		cout << "Create index " << indexName << " fail" << endl;
	}
}

//创建table
void API::tableCreate(string tableName, vector<Attribute>* attributeVector, string primaryKeyName,int primaryKeyLocation)
{
	//找不到表
	if(cm->findTable(tableName) == TABLE_FILE)
	{
		cout << "There is a table " << tableName << " already" << endl;
		return;
	}
	
	if(rm->tableCreate(tableName))
	{
		cm->addTable(tableName, attributeVector, primaryKeyName, primaryKeyLocation);
   
		printf("Create table %s successfully\n", tableName.c_str());
	}
	else return;
	
	if (primaryKeyName != "")
	{
		string indexName = primaryIndexNameGet(tableName);
		indexCreate(indexName, tableName, primaryKeyName);
	}
}

//show all the record in the table//
void API::recordShow(string tableName, vector<string>* attributeNameVector)
{
	vector<Condition> conditionVector;
	recordShow(tableName, attributeNameVector, &conditionVector);
}

void API::recordShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector)
{
	if (cm->findTable(tableName) == TABLE_FILE)
	{
		int num = 0;
		vector <Attribute> attributeVector;
		attributeGet(tableName, &attributeVector);//get the attribute

		vector<string> allAttributeName;
		if (attributeNameVector == NULL)
		{
			for (Attribute attribute : attributeVector)//for each
			{
				allAttributeName.insert(allAttributeName.end(), attribute.name);//insert the name into the end of the vector
			}

			attributeNameVector = &allAttributeName;
		}

		tableAttributePrint(attributeNameVector);//print the trributeName

		for (string name : (*attributeNameVector))
		{
			int i = 0;
			for (i = 0; i < attributeVector.size(); i++)//foreacah
			{
				if (attributeVector[i].name == name)
				{
					break;//judge if the attribute exist
				}
			}

			if (i == attributeVector.size())
			{
				cout << "the attribute which you want to print is not exist in the table" << endl;
				return;
			}
		}

		int blockOffset = -1;
		if (conditionVector != NULL)
		{
			for (Condition condition : *conditionVector)//for each conditon
			{
				int i = 0;
				for (i = 0; i < attributeVector.size(); i++)
				{
					if (attributeVector[i].name == condition.attributeName)
					{
						if (condition.operate == OPERATOR_EQUAL && attributeVector[i].index != "")
						{
							blockOffset = im->searchIndex(rm->indexFileNameGet(attributeVector[i].index), condition.value, attributeVector[i].type);//if the attribute has index
						}
						break;
					}
				}

				if (i == attributeVector.size())
				{
					cout << "the attribute is not exist in the table" << endl;
					return;
				}
			}
		}

		if (blockOffset == -1)
		{
			num = rm->recordAllShow(tableName, attributeNameVector, conditionVector);
		}
		else
		{
			//find the block by index,search in the block
			num = rm->recordBlockShow(tableName, attributeNameVector, conditionVector, blockOffset);
		}

		printf("%d records selected\n", num);
	}
	else
	{
		cout << "There is no table " << tableName << endl;
	}
}

//insert a record to a table
void API::recordInsert(string tableName, vector<string>* recordContent)
{
	if (tableExist(tableName))
	{
		string indexName;

		//deal if the record could be insert (if index is exist)
		vector<Attribute> attributeVector;

		vector<Condition> conditionVector;

		attributeGet(tableName, &attributeVector);//get the attributeGet
		for (int i = 0; i < attributeVector.size(); i++)
		{
			indexName = attributeVector[i].indexNameGet();
			if (indexName != "")//if the attribute has a index
			{
				int blockoffest = im->searchIndex(rm->indexFileNameGet(indexName), (*recordContent)[i], attributeVector[i].type);//insert the record in to the indexfile

				if (blockoffest != -1)
				{
					//if the value has exist in index tree then fail to insert the record
					cout << "insert fail because index value exist" << endl;
					return;
				}
			}
			else if (attributeVector[i].ifUnique)
			{
				//if the attribute is unique but not index
				Condition condition(attributeVector[i].name, (*recordContent)[i], OPERATOR_EQUAL);
				conditionVector.insert(conditionVector.end(), condition);
			}
		}

		if (conditionVector.size() > 0)

			for (int i = 0; i < conditionVector.size(); i++)
			{
				vector<Condition> conditionTmp;
				conditionTmp.insert(conditionTmp.begin(), conditionVector[i]);//insert the condition

				if (rm->recordAllFind(tableName, &conditionTmp) > 0) //there have record
				{
					cout << "insert fail because unique value exist" << endl;
					return;
				}
			}

		char recordString[2000];//store the record
		memset(recordString, 0, 2000);

		//CatalogManager to get the record string
		cm->recordStringGet(tableName, recordContent, recordString);

		//RecordManager to insert the record into file; and get the position of block being insert
		int recordSize = cm->calcuteLenth(tableName);
		int blockOffset = rm->recordInsert(tableName, recordString, recordSize);

		if (blockOffset >= 0)
		{
			recordIndexInsert(recordString,recordSize, &attributeVector, blockOffset);//insert into the recordIndex
			cm->insertRecord(tableName, 1);
			//	        printf("insert record into table %s successful\n", tableName.c_str());
			cout << ".";
		}
		else
		{
			cout << "insert record into table " << tableName << " fail" << endl;
		}
	}
	else return;
}

//删除一张表所有的record
void API::recordDelete(string tableName)
{
	vector<Condition> conditionVector;
	recordDelete(tableName, &conditionVector);
}

//删除表中符合筛选条件的record？？？
void API::recordDelete(string tableName, vector<Condition>* conditionVector)
{
	if (!tableExist(tableName)) return;
	
	int num = 0;
	vector<Attribute> attributeVector;
	attributeGet(tableName, &attributeVector);

	int blockOffset = -1;
	if (conditionVector != NULL)
	{
		for (Condition condition : *conditionVector)
		{
			if (condition.operate == OPERATOR_EQUAL)
			{
				for (Attribute attribute : attributeVector)
				{
					if (attribute.index != "" && attribute.name == condition.attributeName)
					{
						blockOffset = im->searchIndex(rm->indexFileNameGet(attribute.index), condition.value, attribute.type);
					}
				}
			}
		}
	}

	
	if (blockOffset == -1)
	{
		//如果无法通过index来找到这个表，就要从全部的表里面找
		num = rm->recordAllDelete(tableName, conditionVector);
	}
	else
	{
		//在index里找
		num = rm->recordBlockDelete(tableName, conditionVector, blockOffset);
	}
	
	//删除记录
	cm->deleteValue(tableName, num);
	printf("delete %d record in table %s\n", num, tableName.c_str());
}

//
int API::recordNumGet(string tableName)
{
	if (!tableExist(tableName)) return 0;
	
	return cm->getRecordNum(tableName);
}

int API::recordSizeGet(string tableName)
{
	if (!tableExist(tableName)) return 0;
	
	return cm->calcuteLenth(tableName);
}

int API::typeSizeGet(int type)
{
	return cm->calcuteLenth2(type);
}

int API::indexNameListGet(string tableName, vector<string>* indexNameVector)
{
	if (!tableExist(tableName)) {
		return 0;
	}
	return cm->indexNameListGet(tableName, indexNameVector);
}

//得到所有index的名字
void API::allIndexAddressInfoGet(vector<IndexInfo> *indexNameVector)
{
	cm->getAllIndex(indexNameVector);//vector中存储了所有的index名
	//更新
	for (IndexInfo &i : *indexNameVector)
	{
		strcpy(i.indexName, rm->indexFileNameGet(i.indexName).c_str());
	}
}

int API::attributeGet(string tableName, vector<Attribute>* attributeVector)
{
	if (!tableExist(tableName)) {
		return 0;
	}
	return cm->attributeGet(tableName, attributeVector);
}


//把所有的record插入到index树中
void API::recordIndexInsert(char* recordBegin,int recordSize, vector<Attribute>* attributeVector,  int blockOffset)
{
	char* contentBegin = recordBegin;
	for (int i = 0; i < (*attributeVector).size() ; i++)
	{
		int type = (*attributeVector)[i].type;
		int typeSize = typeSizeGet(type);
		if ((*attributeVector)[i].index != "")
		{
			indexInsert((*attributeVector)[i].index, contentBegin, type, blockOffset);
		}
		
		contentBegin += typeSize;
	}
}

/**
 *
 * insert a value to index tree
 * @param indexName: name of index
 * @param contentBegin: address of content
 * @param type: the type of content
 * @param blockOffset: the block offset num
 */
void API::indexInsert(string indexName, char* contentBegin, int type, int blockOffset)
{
/*    string content= "";
	stringstream tmp;
	//if the attribute has index
	///这里传*attributeVector)[i].index这个index的名字, blockOffset,还有值
	if (type == Attribute::TYPE_INT)
	{
		int value = *((int*)contentBegin);
		tmp << value;
	}
	else  if (type == Attribute::TYPE_FLOAT)
	{
		float value = *((float* )contentBegin);
		tmp << value;
	}
	else
	{
		char value[255];
		memset(value, 0, 255);
		memcpy(value, contentBegin, sizeof(type));
		string stringTmp = value;
		tmp << stringTmp;
	}
	tmp >> content;
	im->insertIndex(rm->indexFileNameGet(indexName), content, blockOffset, type);*/
}

//删除index树里所有的record值
void API::recordIndexDelete(char* recordBegin,int recordSize, vector<Attribute>* attributeVector, int blockOffset)
{
	 
	char* contentBegin = recordBegin;
	for (auto &i : *attributeVector)
	{
		int type = i.type;
		int typeSize = typeSizeGet(type);
		string content;
		stringstream tmp;
		if (i.index != "")//如果这个属性有index
		{
			if (type == TYPE_INT)
			{
				int value = *((int*)contentBegin);
				tmp << value;
			}
			else if (type == TYPE_FLOAT)
			{
				float value = *((float*)contentBegin);
				tmp << value;
			}
			else//字符串类型
			{
				char *value = new char[strlen(contentBegin)+2];
				memcpy(value, contentBegin, sizeof(type));
				string stringTmp = value;
				tmp << stringTmp;
				delete value;
			}
			tmp >> content;
			im->deleteIndexByKey(rm->indexFileNameGet(i.index), content, type);
		}
		contentBegin += typeSize;
	}
}

//查看table是否存在
int API::tableExist(string tableName)
{
	if (cm->findTable(tableName) != TABLE_FILE)
	{
		cout << "There is no table " << tableName << endl;
		return 0;
	}
	else
	{
		return 1;
	}
}

//获取primaryindex的名字
string API::primaryIndexNameGet(string tableName)
{
	return  "PRIMARY_" + tableName;
}

//打印表的所有属性
void API::tableAttributePrint(vector<string>* attributeNameVector)
{
	int i = 0;
	for ( i = 0; i < attributeNameVector->size(); i++)
	{
		printf("%s ", (*attributeNameVector)[i].c_str());
	}
	if (i != 0)
		printf("\n");
}

