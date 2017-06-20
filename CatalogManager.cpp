#include "CatalogManager.h"
#include "BufferManager.h"
#include "IndexManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include "Attribute.h"
#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10

CatalogManager::CatalogManager() {
	// TODO Auto-generated constructor stub

}

CatalogManager::~CatalogManager() {
	// TODO Auto-generated destructor stub
}
//ɾ��table
int CatalogManager::dropTable(string tableName)
{
	bm.delete_fileNode(tableName.c_str());
	if (remove(tableName.c_str()))//ɾ�����ļ�
	{
		return 0;
	}
	return 1;
}

//�õ�һ��index������
int CatalogManager::getIndexType(string indexName)
{
	fileNode *ftmp = bm.getFile("Indexs");
	blockNode *btmp = bm.getBlockHead(ftmp);
	if (btmp)
	{
		char* addressBegin;
		addressBegin = bm.get_content(*btmp);
		IndexInfo * i = (IndexInfo *)addressBegin;
		for (int j = 0; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo));j++)//��index�а�˳�����
		{
			if (i->indexName == indexName)//�ҵ�type
			{
				return i->type;
			}
			i++;
		}
		return -2;
	}

	return -2;
}

//�õ����е�index��Ϣ������indexs��vector��,���������һ��
int CatalogManager::getAllIndex(vector<IndexInfo> * indexs)
{
	fileNode *ftmp = bm.getFile("Indexs");
	blockNode *btmp = bm.getBlockHead(ftmp);
	if (btmp)
	{
		char* addressBegin;
		addressBegin = bm.get_content(*btmp);
		IndexInfo * i = (IndexInfo *)addressBegin;
		for (int j = 0; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo)); i++, j++)
		{
			indexs->push_back((*i));
		}
	}

	return 1;
}

//���index
int CatalogManager::addIndex(string indexName, string tableName, string Attribute, int type)
{
	fileNode *ftmp = bm.getFile("Indexs");
	blockNode *btmp = bm.getBlockHead(ftmp);
	IndexInfo i(indexName, tableName, Attribute, type);
	while (true)//��block
	{
		if (btmp == NULL)
		{
			return 0;
		}
		if (bm.get_usingSize(*btmp) <= bm.getBlockSize() - sizeof(IndexInfo))//�ҵ����㹻��size�洢index��block
		{

			char* addressBegin;
			addressBegin = bm.get_content(*btmp) + bm.get_usingSize(*btmp);//�ҵ���ʼλ��
			memcpy(addressBegin, &i, sizeof(IndexInfo));//д��block
			bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + sizeof(IndexInfo));//����block��usingsize
			bm.set_dirty(*btmp);


			return this->setIndexOnAttribute(tableName, Attribute, indexName);
		}
		else
		{
			btmp = bm.getNextBlock(ftmp, btmp);
		}
	}

	return 0;
}

//����û��table
int CatalogManager::findTable(string tableName)
{
	FILE *fp;
	fp = fopen(tableName.c_str(), "r");
	if (fp == NULL)
	{
		return 0;
	}
	else
	{
		fclose(fp);
		return TABLE_FILE;
	}

}

//����û�и��ļ���index
int CatalogManager::findIndex(string fileName)
{
	fileNode *ftmp = bm.getFile("Indexs");
	blockNode *btmp = bm.getBlockHead(ftmp);
	if (btmp)
	{
		char* addressBegin;
		addressBegin = bm.get_content(*btmp);
		IndexInfo * i = (IndexInfo *)addressBegin;
		int flag = UNKNOWN_FILE;
		for (int j = 0; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo)); j++)
		{
			if (i->indexName == fileName)
			{
				flag = INDEX_FILE;
				break;
			}
			i++;
		}
		return flag;
	}

	return 0;
}

//ɾ��index
int CatalogManager::dropIndex(string index)
{
	fileNode *ftmp = bm.getFile("Indexs");//��index�ļ� 
	blockNode *btmp = bm.getBlockHead(ftmp);
	if (btmp)
	{
		char* addressBegin;
		addressBegin = bm.get_content(*btmp);
		IndexInfo * i = (IndexInfo *)addressBegin;
		int j = 0;
		for (j = 0; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo)); j++)//ɾ����ǰ�Ƹ���index
		{
			if (i->indexName == index)
			{
				break;
			}
			i++;
		}
		this->revokeIndexOnAttribute(i->tableName, i->Attribute, i->indexName);
		for (; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo) - 1); j++)
		{
			(*i) = *(i + sizeof(IndexInfo));
			i++;
		}
		bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) - sizeof(IndexInfo));
		bm.set_dirty(*btmp);

		return 1;
	}

	return 0;
}

//�ջظ����Ե�index
int CatalogManager::revokeIndexOnAttribute(string tableName, string AttributeName, string indexName)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{

		char* addressBegin = bm.get_content(*btmp);
		addressBegin += (1 + sizeof(int));
		int size = *addressBegin;//�õ���ǰ��size��������
		addressBegin++;//��ʼλ��
		Attribute *a = (Attribute *)addressBegin;
		int i;
		for (i = 0; i<size; i++)
		{
			if (a->name == AttributeName)
			{
				if (a->index == indexName)//�ջز�����Ϊdirty bit
				{
					a->index = "";
					bm.set_dirty(*btmp);
				}
				else
				{
					cout << "revoke unknown index: " << indexName << " on " << tableName << "!" << endl;
					cout << "Attribute: " << AttributeName << " on table " << tableName << " has index: " << a->index << "!" << endl;
				}
				break;
			}
			a++;
		}
		if (i<size)
			return 1;
		else
			return 0;
	}
	return 0;
}

//�õ����е�index��������vector��
int CatalogManager::indexNameListGet(string tableName, vector<string>* indexNameVector)
{
	fileNode *ftmp = bm.getFile("Indexs");
	blockNode *btmp = bm.getBlockHead(ftmp);
	if (btmp)
	{
		char* addressBegin;
		addressBegin = bm.get_content(*btmp);
		IndexInfo * i = (IndexInfo *)addressBegin;
		for (int j = 0; j<(bm.get_usingSize(*btmp) / sizeof(IndexInfo)); j++)
		{
			if (i->tableName == tableName)
			{
				(*indexNameVector).push_back(i->indexName);
			}
			i++;
		}
		return 1;
	}

	return 0;
}

//ɾ��recordʱ����recordNum
int CatalogManager::deleteValue(string tableName, int deleteNum)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{

		char* addressBegin = bm.get_content(*btmp);
		int * recordNum = (int*)addressBegin;
		if ((*recordNum) <deleteNum)
		{
			cout << "error in CatalogManager::deleteValue" << endl;
			return 0;
		}
		else
			(*recordNum) -= deleteNum;

		bm.set_dirty(*btmp);
		return *recordNum;
	}
	return 0;
}

//����recordʱ����recordNum
int CatalogManager::insertRecord(string tableName, int recordNum)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{

		char* addressBegin = bm.get_content(*btmp);
		int * originalRecordNum = (int*)addressBegin;
		*originalRecordNum += recordNum;
		bm.set_dirty(*btmp);
		return *originalRecordNum;
	}
	return 0;
}

//�õ�recordNum
int CatalogManager::getRecordNum(string tableName)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{
		char* addressBegin = bm.get_content(*btmp);
		int * recordNum = (int*)addressBegin;
		return *recordNum;
	}
	return 0;
}

//�ӱ�
int CatalogManager::addTable(string tableName, vector<Attribute>* attributeVector, string primaryKeyName = "", int primaryKeyLocation = 0)
{
	FILE *fp;
	fp = fopen(tableName.c_str(), "w+");
	if (fp == NULL)
	{
		return 0;
	}
	fclose(fp);
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)//��ʼ��
	{
		char* addressBegin = bm.get_content(*btmp);
		int * size = (int*)addressBegin;
		*size = 0;
		addressBegin += sizeof(int);
		*addressBegin = primaryKeyLocation;//1 as what it says
		addressBegin++;
		*addressBegin = attributeVector->size();// 2 attribute number
		addressBegin++;
		//memcpy(addressBegin, attributeVector, attributeVector->size()*sizeof(Attribute));
		for (int i = 0; i<attributeVector->size(); i++)
		{
			memcpy(addressBegin, &((*attributeVector)[i]), sizeof(Attribute));
			addressBegin += sizeof(Attribute);
		}
		bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + attributeVector->size() * sizeof(Attribute) + 2 + sizeof(int));
		bm.set_dirty(*btmp);
		return 1;
	}
	return 0;
}
//�������ϴ���index
int CatalogManager::setIndexOnAttribute(string tableName, string AttributeName, string indexName)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{

		char* addressBegin = bm.get_content(*btmp);
		addressBegin += 1 + sizeof(int);
		int size = *addressBegin;//��ȡ�洢��size
		addressBegin++;//����洢���ݵ�����
		Attribute *a = (Attribute *)addressBegin;
		int i;
		for (i = 0; i<size; i++)//һ��һ������
		{
			if (a->name == AttributeName)
			{
				a->index = indexName;
				bm.set_dirty(*btmp);//�ҵ���д�벢����dirty bit
				break;
			}
			a++;
		}
		if (i<size)
			return 1;
		else
			return 0;
	}
	return 0;
}

//�õ�index������,��������
int CatalogManager::attributeGet(string tableName, vector<Attribute>* attributeVector)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{

		char* addressBegin = bm.get_content(*btmp);
		addressBegin += 1 + sizeof(int);
		int size = *addressBegin;
		addressBegin++;
		Attribute *a = (Attribute *)addressBegin;
		for (int i = 0; i<size; i++)
		{
			attributeVector->push_back(*a);//��ӵ�vector��
			a++;
		}

		return 1;
	}
	return 0;
}

//�������ݵ�ռ�ĳ���
int CatalogManager::calcuteLenth(string tableName)
{
	fileNode *ftmp = bm.getFile(tableName.c_str());
	blockNode *btmp = bm.getBlockHead(ftmp);

	if (btmp)
	{
		int singleRecordSize = 0;
		char* addressBegin = bm.get_content(*btmp);
		addressBegin += 1 + sizeof(int);
		int size = *addressBegin;
		addressBegin++;
		Attribute *a = (Attribute *)addressBegin;
		for (int i = 0; i<size; i++)//���ݲ�ͬ��type�������
		{
			if (a->type == -1)
			{
				singleRecordSize += sizeof(float);
			}
			else if (a->type == 0)
			{
				singleRecordSize += sizeof(int);
			}
			else if (a->type>0)
			{
				singleRecordSize += a->type * sizeof(char);
			}
			else
			{
				cout << "Catalog data damaged!" << endl;
				return 0;
			}
			a++;
		}

		return singleRecordSize;
	}
	return 0;
}

//�õ��������͵Ĵ�С
int CatalogManager::calcuteLenth2(int type) {
	if (type == TYPE_INT) {
		return sizeof(int);
	}
	else if (type == TYPE_FLOAT)
	{
		return sizeof(float);
	}
	else {
		return (int) sizeof(char)*type;// type������Attribute.h
	}
}

//���ݱ�����record content�õ���ļ�¼,Ȼ��ѽ��д��recordresult��reference
void CatalogManager::recordStringGet(string tableName, vector<string>* recordContent, char* recordResult)
{
	vector<Attribute> attributeVector;
	attributeGet(tableName, &attributeVector);
	char * contentBegin = recordResult;

	for (int i = 0; i < attributeVector.size(); i++)
	{
		Attribute attribute = attributeVector[i];
		string content = (*recordContent)[i];
		int type = attribute.type;
		int typeSize = calcuteLenth2(type);
		stringstream ss;
		ss << content;
		if (type == TYPE_INT)
		{
			//if the content is a int
			int intTmp;
			ss >> intTmp;
			memcpy(contentBegin, ((char*)&intTmp), typeSize);
		}
		else if (type == TYPE_FLOAT)
		{
			//if the content is a float
			float floatTmp;
			ss >> floatTmp;
			memcpy(contentBegin, ((char*)&floatTmp), typeSize);
		}
		else
		{
			//if the content is a string
			memcpy(contentBegin, content.c_str(), typeSize);
		}

		contentBegin += typeSize;
	}
	return;
}


void DropTable(char* tn) {
#if DETAIL
	printf("DropTable(): enter, tn = %s", tn);
	printf("\n");
#endif
	int TID = 0;
	if (TID == -1) {
		printf("Table dropping fails.  No such table.\n");
		return;
	}

#if DETAIL
	printf("DropTable(): Meta deletion fullfills. TID = %d", TID);
	printf("\n");
#endif



	int il4ILine = 0;
	int il4IID = 0;
	int il4IIDLine = 0;
	char* bufin = (char*)malloc(4096);
	int line = 0;
	int size;
	int unit = 100 + 200 + 1;

	do {
		size = 4;
		int howmany = size / unit;
		int i;
		char* checkTID = bufin + 4;
		for (i = 0; i < howmany; i++) {
			if (TID == *(short*)checkTID) {
				//il4IID = IntListInsert(il4IID, *(int*)(checkTID - IID_BYTE)); // 
			//	il4ILine = IntListInsert(il4ILine, line + i*unit);
				//DeleteLine("indices.bin",line+i*unit,unit,unit*1);
			}
			checkTID += unit;
		}
		line += size;
	} while (size == 4096);
	//	PPIL(il4ILine);


	line = 0;
	unit = 4;
	do {
		
		int howmany = size / unit;
		char* checkIID = bufin;
		int i;
		for (i = 0; i < howmany; i++) {
			int checker = 4;
			while (checker) {
				int data = howmany;
				if (data == *(int*)checkIID) {
					
					break;
				}
				checker ++;
			}
			checkIID += unit;
		}
		line += size;
	} while (size == 4096);



	// ???????????????index?.bin???????????il4IID 
	int killer = il4IID;
	while (killer) {
		char ibin[20] = "index";
		sprintf(ibin + 5, "%d", killer);
		strcat(ibin, ".bin");
		remove(ibin);
		
	}

}

void GetTable(short TID, char* table) {
	char* bufin = (char*)malloc(4096);
	int line = 0;
	int unit = 1024;
	int size;
	do {
		size = 4096;
		int howmany = size / unit;
		int i;
		char* check = bufin;
		for (i = 0; i<howmany; i++) {
			short test = 0;
			test |= check[0] & 0xf0;
			test <<= 4;
			test |= check[32] & 0xf0;
			test <<= 4;
			test |= check[64] & 0xf0;
			test |= ((check[96] & 0xf0) >> 4);
			if (test == TID) {
				int k;
				for (k = 0; k<1024; k++) {
					table[k] = check[k];
				}
			}
			check += unit;
		}
		line += unit * howmany;
	} while (size == 4096);
	free(bufin);
}
