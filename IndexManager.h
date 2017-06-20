
#ifndef __Minisql__IndexManager__
#define __Minisql__IndexManager__

#include <stdio.h>
#include <map>
#include <string>
#include <sstream>
#include "Attribute.h"
#include "BPlusTree.h"
#include <cstring>
class API;

class IndexManager{
private:
    typedef map<string,BPlusTree<int> *> intMap;
    typedef map<string,BPlusTree<string> *> stringMap;
    typedef map<string,BPlusTree<float> *> floatMap;

    // other values mean the size of the char.Eg, 4 means char(4);
    
    API *api; // to call the functions of API
    
    intMap indexIntMap;
    stringMap indexStringMap;
    floatMap indexFloatMap;
    struct keyTmp{
        int intTmp;
        float floatTmp;
        string stringTmp;
    }; // the struct to help to convert the inputed string to specfied type
    struct keyTmp kt;
    
    int getDegree(int type);

    int getKeySize(int type);
   
    void setKey(int type,string key);
    
    
public:
    IndexManager(API *api);
    ~IndexManager();

    void createIndex(string filePath,int type);
    
    void dropIndex(string filePath,int type);
    
    offsetNumber searchIndex(string filePath,string key,int type);

    void insertIndex(string filePath,string key,offsetNumber blockOffset,int type);
    
    void deleteIndexByKey(string filePath,string key,int type);
};





class IndexInfo
{
public:
	IndexInfo(string i, string t, string a, int ty)
	{
		strcpy(indexName, i.c_str()); strcpy(tableName, t.c_str()); strcpy(Attribute, a.c_str()); type = ty;
	}
	char indexName[50];
	char tableName[50];
	char Attribute[50];
	int type;
};


#endif /* defined(__Minisql__IndexManager__) */
