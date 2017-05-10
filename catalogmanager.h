#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H
#include "structs.h"

int GetIID(char* name);

int AllocIID(char* name);

void WriteIMeta(int IID, short TID, short AID, char fixed);

char GetAID(int TID, char* name);

int DeleteIMeta(int IID);

void DeleteIIDMeta(int IID);

void DropIndex(char* in);

void CreateIndex(char* in, char* tn, char* an, char force);

int FindIID(int TID, int AID);

int GetTID(char* name);

int AllocTID(char* name);

void WriteTMeta(short TID, MetaTable T);

void CreateTable(MetaTable T);

void DeleteTIDMeta(short TID);

void DeleteTMeta(short TID);

void DropTable(char* tn);

void GetTable(short TID, char* table);

#endif

