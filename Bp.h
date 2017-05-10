#ifndef BP_H
#define BP_H
#include "structs.h"
void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, PVoidList values);
void BpDestroy(BpNode,int IID, unsigned char type, unsigned char size);
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value);
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
int BpEqual(int IID, unsigned char type, unsigned char size, void* value);
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il);
 
#endif
