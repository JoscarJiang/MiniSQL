#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include"structs.h"

void SelectFromWhere(char* tn, Cond cond);
void DeleteFromWhere(char* tn, Cond cond);
void InsertIntoValues(char* tn, Values v);

#endif 
