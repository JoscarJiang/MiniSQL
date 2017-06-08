#define _CRT_SECURE_NO_WARNINGS 1
#ifndef _STRUCTS_H
#define _STRUCTS_H
#include "macro.h"
typedef struct values *Values;
typedef struct cond *Cond;
typedef struct attr *Attr;
typedef struct pVoidList *PVoidList;
typedef struct intList *IntList;
typedef struct metaTable *MetaTable;
typedef struct tuples *Tuples;
typedef struct rslt *Rslt;
typedef struct buf *Buf;
typedef struct bpNode *BpNode;
typedef struct Bplus_Node_Pointer *BNpointer;
typedef struct Bplus_File_Pointer *BFpointer;
typedef struct block *block;


struct intList{
        int data;
        IntList next;
};

struct attr {
	char pk; // 是否主键 
	char unique; // 是否独一无二 
	char type; // 数据类型 
	unsigned char size; // 数据大小 
	char name[MAX_ATTR_NAME];
};

struct bpNode {
	short flag;
	int isroot;

	void* K[MAX_K];
	BNpointer	Node_P[MAX_ATTR];
	BFpointer File_P[MAX_ATTR];
	int offset[MAX_ATTR];
};

struct metaTable {
	char tn[MAX_TABLE_NAME];
	int valid; // 一共多少个属性 
	Attr a[MAX_ATTR];
};

struct pVoidList {
	void* data;
	struct pVoidList* next;
};


typedef struct buf {
	int valid;
	int size;
	char* fileName;
	int line;
	char* data;
	struct buf* next;
} Bufs;

struct cond {
	char an[MAX_ATTR_NAME]; // 列名
	char op;
	// op 约定如下： 
	// 000 =
	// 001 <=
	// 011 <
	// 100 >
	// 110 >=
	// 111 <>
	Values value; // 此结构中的next可以不使用 
	Cond next; // 下一个条件，遇到一个and就分配一个 
};
struct values {
	char type; // 00 int, 01 float, 11 char(n)
	char size; // int = float = 4; char = n
	void* data;
	Values next;
} ; 


struct tuples {
	void* data[MAX_DATA];
	Tuples next;

};

struct rslt {
	MetaTable mt;
	Tuples t;
};
struct writeBufStruct {
	char* data;
	int size;
};

struct globalBuffer {
	Buf head;
	Buf tail;
};

struct Bplus_Node_Pointer {
	int vaild;
	BNpointer next_node;
};

struct Bplus_File_Pointer {
	int vaild;
	int lines;

};

struct block {
	int value_int[MAX_ATTR];
	int file_vaild[MAX_ATTR];
	int lines[MAX_ATTR];
	int type;
	int flag;
	int isroot;
	float value_float[MAX_ATTR];
	char value_ch[MAX_ATTR];
	int offset[MAX_ATTR];
	BNpointer node_vaild[MAX_ATTR];
};
struct writeBufStruct* writeBuf;
struct globalBuffer* GlobalBuffer;
char readBuf[16][MAX_ATTR];

Buf bufs[16];
#endif

