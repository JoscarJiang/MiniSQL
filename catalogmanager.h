#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H
#include "structs.h"

int GetIID(char* name);

int AllocateIID(char* name);

void WriteIMeta(int IID, short TID, short AID, char fixed);

char GetAID(int TID, char* name);

int DeleteIMeta(int IID);

void DeleteIIDMeta(int IID);

void DropIndex(char* in);

int ifexist(char* in, char* tn, char* an, char force);

void CreateIndex(char* in, char* tn, char* an, char force);

int FindIID(int TID, int AID);

int GetTID(char* name);

int AllocateTID(char* name);

void WriteTMeta(short TID, MetaTable T);

void CreateTable(MetaTable T);

void DeleteTIDMeta(short TID);

void DeleteTMeta(short TID);

void DropTable(char* tn);

void DroptheTable(char* tn,int TID);

void GetTable(short TID, char* table);

#endif

