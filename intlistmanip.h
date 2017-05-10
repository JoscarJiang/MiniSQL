#ifndef INTLISTMANIP_H
#define INTLISTMANIP_H
#include "structs.h"

IntList DecreaseIntList(IntList il);
IntList IncreaseIntList(IntList il);
IntList IntListInsert(IntList il, int x);
IntList IntListDestroy(IntList il);
IntList IntListDeleteNode(IntList, IntList);
PVoidList PVoidListDestroy(PVoidList pvl);
PVoidList PVoidListInsert(char*, int, PVoidList,char);

#endif
