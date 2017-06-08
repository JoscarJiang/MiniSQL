#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include"structs.h"
#include "intlistmanip.h"
#include "Bp.h"
#include "check.h"
#include "filemanip.h"
void SelectFromWhere(char* tn, Cond cond);
void DeleteFromWhere(char* tn, Cond cond);
void InsertIntoValues(char* tn, Values v);

#endif 
