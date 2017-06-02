#include "catalogmanager.h"
#include "intlistmanip.h"
#include "buffermanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetIID(char* name){ // �������ڴ�����������-1 
	char bufin[BUFFER_SIZE];
	int line = 0;
//	int sizes=2000 * 64;
	int size = 0;
	int unit = I_LENGTH;
	do{
//		sizes-=BUFFER_SIZE;
		size = Read("indexID.bin",line,bufin);

		int howmany = size/unit;
		int i;
		char* checkname = bufin+IID_BYTE;
		for(i=0;i<howmany;i++){
			if(strcmp(checkname,name)){
				checkname += unit;
				continue;
			}else{
				return *(int*)(checkname - IID_BYTE);
			}
		}
		line += unit * howmany;
	} while(size>=BUFFER_SIZE);
	return -1;
}

#if 0 
int GetIID(char* name){ // �������ڴ�����������-1 
	char bufin[BUFFER_SIZE];
	int line = 0;
	int size=1000 * 64;
	int unit = I_LENGTH;
	do{
//		size-=BUFFER_SIZE;
//		Read("indexID.bin",line,bufin);
		size = Read("indexID.bin",line,bufin);
		int howmany = size/unit;
		int i;
		char* checkname = bufin+IID_BYTE;
		for(i=0;i<howmany;i++){
			if(strcmp(checkname,name)){
				checkname += unit;
				continue;
			}else{
				return *(int*)(checkname - IID_BYTE);
			}
		}
		line += unit * howmany;
	} while(size>=BUFFER_SIZE);
	return -1;
}
#endif

int AllocateIID(char* name){ // ʹ�ô˺���ǰ��������GetIIDȷ�ϲ����ڴ�������ʹ�ú�������������indices.bin��д�������Ϣ 

	int fs = GetFileSize11("indexID.bin");
	int marksize = fs / I_LENGTH + 1;
	char* mark = (char*)malloc(marksize);
	memset(mark,1,marksize);


	int line = 0;
	int unit = I_LENGTH;
	int size;
	do{
		char bufin[BUFFER_SIZE];
		char* ptn = bufin;
		size = Read("indexID.bin",line,bufin);
		int i = 0;
		int howmany = size/unit;
		for(i=0;i<howmany;i++){
			int thisIID = *(int*)ptn;
			if(thisIID < fs+1){
				mark[thisIID] = 0;
			}
			ptn += unit;
		}
		line += unit * howmany;
	}while(size==BUFFER_SIZE);

	char* writeinfo = (char*) malloc(I_LENGTH+1); 
	memset(writeinfo,0,I_LENGTH);
	int j;
	for(j = 0; j < marksize; j++){
		if(mark[j]){ // �ҵ�����С����IID 
			*(unsigned int*)writeinfo = j;
			strcpy(writeinfo+IID_BYTE,name);
			Append("indexID.bin",writeinfo,I_LENGTH);
			if(mark)free(mark);
			free(writeinfo);
			return j;
		}
	}
	if(mark)free(mark);
	free(writeinfo);
}

void WriteIMeta(int IID, short TID, short AID, char fixed){
	int size = IID_BYTE+TID_BYTE+1; // �洢�ṹΪ ����ID����ID������ID�����λΪ1�򲻿�ɾ�� 
	char writeinfo[IID_BYTE+TID_BYTE+1] = {0};
	*(unsigned int*)writeinfo = IID;
	*(unsigned short*)(writeinfo + IID_BYTE)= TID;
	char ch = AID;
	if(fixed) ch |= 0x80;
	writeinfo[IID_BYTE+TID_BYTE] = ch;
	Append("indices.bin",writeinfo,size);
}

char GetAID(int TID, char* name){ // ȷ��TID��ȷ�� 
	char* bufin = (char*)malloc(4096);
	int line = 0;
	int unit = 1024;
	int size;
	do{
		size = Read("tables.bin",line,bufin);
		int i;
		int howmany = size/unit;
		char* collector = bufin;
		for(i=0;i<howmany;i++){
			short test = 0;
			test |= (*collector)&0xf0;
			test<<=4;
			test |= (*(collector+ATTR_UNIT))&0xf0;
			test<<=4;
			test |= (*(collector+(ATTR_UNIT<<1)))&0xf0;
			test |= (((*(collector+(ATTR_UNIT*3)))&0xf0)>>4);

			if(test == TID){
				collector += 2;
				char j;
				for(j=0;j<32;j++){
					if(strcmp(name,collector)){
						collector += 32;
						continue;
					}else{
						if((*(char*)(collector-2)) & 0x08) j = (j|0x80);
						if((*(char*)(collector-2)) & 0x04) j = (j|0x40);
						free(bufin);
						return j;
					}
				}
						free(bufin);
				return -1;
			}
			else{
				collector += unit; 
			}
//		line+=unit * howmany; д�������﷢������ֵĴ�debug��5��Сʱ����֪Ϊ��readBuf[1]�е����ݣ���TID==210ʱ�����ڴ˺����ڱ��ı䡣����˵Ӧ�ò���
// �����Ƿ���ReadbuFFER��ʱ��û�б����������Ի��ǻ����������п���������˵ĵط���Ҫ�ٸ��ټ��һ�¡� 
		}
		line+=unit * howmany;
	}while(size == BUFFER_SIZE);
					free(bufin);

} 

int DeleteIMeta(int IID){ // ��indices.bin��ɾ������IID��Ӧ��������Ϣ 
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int line = 0;
	int fs = GetFileSize11("indices.bin");
	int unit = IID_BYTE+TID_BYTE+1;
	while(line<fs){
		int size = Read("indices.bin",line,bufin);	
		int i;
		int canbreak = 0;
		char* checkIID = bufin;
		for(i=0;i<size/unit;i++){
			if(IID == *(int*)checkIID){
				if((checkIID[6] & 0x80) == 1){
					printf("Index dropping fails.  This index is on primary key.\n"); // ֻ������ͼɾ��_PK_��ͷ��ʱ�����ʾ��� 
					free(bufin);
					return -1;
				}
				DeleteLine("indices.bin",line+unit*i,unit,1*unit);
				canbreak = 1;
				break;
			}
			checkIID+=unit;
		}
		if(canbreak)break;
		line+=unit*i;
	}
	free(bufin);
	return 1;
}

void DeleteIIDMeta(int IID){ // ��indexID.bin��ɾ������IID��Ӧ������������-IDӳ�� 
	char* bufin = (char*)(malloc(BUFFER_SIZE));
	int line = 0;
	int unit = I_LENGTH;
	int size;
	do{
		size = Read("indexID.bin",line,bufin);
		int i;
		int canbreak = 0;
		char* checkIID = bufin;
		for(i=0;i<size/unit;i++){
			if(IID == *(int*)checkIID){
				DeleteLine("indexID.bin",line+unit*i,unit,1*unit);
				canbreak = 1;
				break;
			}
			checkIID+=unit;
		}
		if(canbreak)break;
		line+=unit*i;
	}while(size==BUFFER_SIZE);
	free(bufin);
}

void DropIndex(char* in){ // in = Index Name
	int IID = GetIID(in);
	if(IID==-1){
		printf("Index dropping fails.  Does not exist.\n");
		return;
	}
	if(DeleteIMeta(IID)==-1)return;
	DeleteIIDMeta(IID);
	char in2[20] = "index";
	sprintf(in2+5,"%d",IID);
	strcat(in2,".bin");
	remove(in2);
	printf("Index dropping succeeds!\n");
}

int ifexist(char* in, char *tn, char* an, char force){
		if((!force) && strlen(in)>=4)
		if(in[0]=='_' && in[1]=='P' && in[2]=='K' && in[3]=='_'){
			printf("Index creation fails.  Do not begin with '_PK_'.\n");
			return -1;
		}
		
	int IID = GetIID(in);
	if(IID!=-1){
		printf("Index creation fails. Name used.\n");
		return -1;
	}
	
	int AID;
	char ifpk;
	char ifunique;
	char res = GetAID(TID,an);
	
	AID = res & 0x1f;
	ifpk = res>>7;
	ifunique = (res>>6) & 1;
	
	if(res&0x20){
		printf("Index creation fails.  No such attribute. \n");
		return -1;
	}
	if(!ifunique){
		printf("Index creation fails.  The attribute shall be unique.\n");
		return -1;
	}
	int ex = FindIID(TID,AID);
	if(ex!=-1){
		printf("Index creation fails.  Index on that attribute has been created.\n");
		return -1; 
	}
	return 1;
}
// suspect: GetAID 
void CreateIndex(char* in, char* tn, char* an, char force){

	int IID = GetIID(in);
	int TID = GetTID(tn);// ��֤���ڴ˱����д��ڴ����ԣ�������Ϊunique
	int AID;
	char ifpk;
	char ifunique;
	char res = GetAID(TID,an);
	
	AID = res & 0x1f;
	ifpk = res>>7;
	ifunique = (res>>6) & 1;
	
    test=ifexist(in, tn, an, force);
	if(test==-1){
		printf("Alredy exist or no such attribute");
		return;
	}

	IID = AllocateIID(in);
	WriteIMeta(IID,TID,AID,ifpk&&force);
	char in2[20] = "index";
	sprintf(in2+5,"%d",IID);
	strcat(in2,".bin");

	char tbin[20] = "table";
	sprintf(tbin+5,"%d",TID);
	strcat(tbin,".bin");

    if(!force){ // �û�Ҫ������������ʱ���б�Ԫ��Ϣ
	ReWrite(in2,in,0);
	int line = 0;
	char* table = (char*)malloc(1024);	
	GetTable(TID,table);
	int type = (table[AID<<5] & 3);
	int asize = (table[(AID<<5)+1]);
	char end0 = ((table[AID<<5])&2) ? 1 : 0 ;
	PVoidList alldata = 0;
	IntList lines = 0;
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int unit = 0;
	int beginpoint = 0;
	int j;
	for(j=0;j<32;j++){
		if(!table[(j<<5)+1])break;
		unit += (unsigned char)table[(j<<5) + 1];
		if(j<AID){
			beginpoint += (unsigned char)table[(j<<5)+1];
		}
	}
	int size;
	do{
		size = Read(tbin,line,bufin);
		int howmany = size / unit;
		int i;
		char* check = bufin;
		for(i = 0;i<howmany;i++){
			lines = IntListInsert(lines,line + i * unit);
			alldata = PVoidListInsert(check+beginpoint,asize,alldata,end0);
			check += unit;
		}
		line += unit * howmany;
	}while(size == BUFFER_SIZE); 	


#if BP_OK
	BpBuild(IID,type,asize,lines,alldata);
#endif
	lines = IntListDestroy(lines);
	alldata= PVoidListDestroy(alldata);
	free(table);
	free(bufin);
    }
    else{
        ReWrite(in2,in,0);
    }
	printf("Index creation succeeds!  IID = %d.\n",IID);

}

int FindIID(int TID, int AID){ // �����ID������ID���жϴ������Ƿ���������������ڷ�������ID�����򷵻�-1�� 
	int line = 0;
	int size;
	do{
		char bufin[BUFFER_SIZE];
		size = Read("indices.bin",line,bufin);
		char* check = bufin+IID_BYTE;
		int i;
		int unit = IID_BYTE+TID_BYTE+1;
		for(i=0;i<size/unit;i++){
			if(*(short*)check!=TID){
				check += unit;
				continue;
			}
			if(((*(check+TID_BYTE))&0x7f) != AID){
				check += unit;
				continue;
			}
			return *(int*)(check-IID_BYTE);
		}
		line += unit*i;
	}while(size!=-1);
	return -1;
}

int GetTID(char* name){
	int size;
	int line = 0;
	char* bufin = (char*)malloc(BUFFER_SIZE);
	
	do{
	#if DETAIL
printf("GetTID: begin Read()");
printf("\n");
#endif

		size = Read("tableID.bin",line,bufin);	
		char* checkname = bufin+TID_BYTE;
		int unit = TID_BYTE + MAX_TABLE_NAME;
		int i;
		int howmany = size/unit;
		for(i=0;i<howmany;i++){
			if(strcmp(checkname, name)){

				checkname += unit;
				continue;
			}
			short resl = *(short*)(checkname - TID_BYTE);
			free(bufin);
			return resl;
		}
		line += howmany * unit;
	}while(size == BUFFER_SIZE);
	
	free(bufin);
	return -1;
}

int AllocateTID(char* name){

//	printf("in ALLOCTID, fp = %d\n",fp);
	int fs = GetFileSize11("tableID.bin"); // ����-1������ 
//	printf("While assigning TID, fs = %d\n",fs);
//	printf("fs(tableID.bin)=%d\n",fs);
	char* mark = (char*)malloc(fs+1);
	memset(mark,1,fs+1);
	int line = 0;
	int unit = MAX_TABLE_NAME + TID_BYTE;
	int size;
	do{
		char bufin[BUFFER_SIZE];
		char* ptn = bufin;
		size = Read("tableID.bin",line,bufin);
		int i = 0;
		int howmany = size/unit;
		for(i=0;i<howmany;i++){
			short thisIID = *(short*)ptn;
			if(thisIID < fs+1){
				mark[thisIID] = 0;
			}
			ptn += unit;
		}
		line += unit * howmany;
	}while(size==BUFFER_SIZE);
		
	
	short i;
	for(i=0;i<fs+1;i++){
		if(mark[i]){
			char writeinfo[MAX_TABLE_NAME+TID_BYTE+1] = {0};
			*(short*)writeinfo = i;
			strcpy(writeinfo+TID_BYTE,name);
			Append("tableID.bin",writeinfo,unit);
			if(mark)free(mark);
			return i;
		}
	}
	if(mark)free(mark);
}

void WriteTMeta(short TID, MetaTable T){ // ǰ�ĸ�attr:4bit TID�� 2bit:pk?unique?, 2bit: type; 1B: size; 30B: name; 
	char* writeinfo = (char*)malloc(1024);
	memset(writeinfo,0,1024);
	int unit = ATTR_UNIT;
	writeinfo[0] |= ((TID&0xf000)>>8);
	writeinfo[unit] |= ((TID&0x0f00)>>4);
	writeinfo[unit<<1] |= ((TID&0x00f0));
	writeinfo[(unit<<1) + unit] |= ((TID&0x000f)<<4);
	int howmany = T->valid;
	int i;
	for(i = 0; i<howmany; i++){
		if(T->a[i]->pk){
			writeinfo[unit*i] |= 0x08;
		}
		if(T->a[i]->unique){
			writeinfo[unit*i] |= 0x04;
		}
		writeinfo[unit*i] |= T->a[i]->type;
		writeinfo[unit*i+1] |= T->a[i]->size;
		int j;
		for(j=0;j<strlen(T->a[i]->name);j++){
			writeinfo[unit*i+2+j] = T->a[i]->name[j];
		}
	}
	Append("tables.bin",writeinfo,1024);
	free(writeinfo);
} 

void CreateTable(MetaTable T){	
	int TID = GetTID(T->tn);
	if(TID!=-1){
		printf("Table creation fails.  Table name used.\n");
		return;
	}
	TID = AllocateTID(T->tn);
#if DETAIL
	printf("CreateTable: Allocated TID = %d",TID);
	printf("\n");
#endif

	WriteTMeta(TID,T);
	// �������������� 
	int i;
	for(i=0;i<T->valid;i++){

		if(T->a[i]->pk){

			char pkn[MAX_ATTR_NAME + 1];
			strcpy(pkn,T->a[i]->name);
			char in[MAX_INDEX_NAME + 1] = "_PK_";
			sprintf(in+4,"%d",TID);
			printf("tn:%s\n",T->tn);
			CreateIndex(in,T->tn,pkn,1);
			break;
		}
	}
#if DETAIL
	printf("CreateTable: Index on primary key created.");
	printf("\n");
#endif

	char tablebin[20] = "table";
	sprintf(tablebin+5,"%d",TID);
	strcat(tablebin,".bin");
	FILE* fp = fopen(tablebin,"wb");
	fclose(fp);
	printf("Table creation succeeds!  TID = %d.\n",TID); 
}

void DeleteTIDMeta(short TID){
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int line = 0;
	int unit = MAX_TABLE_NAME + TID_BYTE;
	int size;
	do{
		size = Read("tableID.bin",line,bufin);
		int i;
		int canbreak = 0;
		int howmany = size / unit;
		char* checkTID = bufin;
		for(i=0;i<howmany;i++){
			if(*(short*)checkTID == TID){
				DeleteLine("tableID.bin",line+i*unit,unit,1*unit);
				free(bufin);
				return;
			}
			checkTID += unit;
		}
		line += size;
	}while(size==BUFFER_SIZE);
	free(bufin);
}

void DeleteTMeta(short TID){
	char bufin[BUFFER_SIZE];
	int line = 0;
	int unit = MAX_ATTR * ATTR_UNIT;
	int size;
	do{
		size = Read("tables.bin",line,bufin);
		int i;
		int howmany = size/unit;
		short test = 0;
		char* collector = bufin;
		for(i=0;i<howmany;i++){
			test = 0;
			test |= (*collector)&0xf0;
			test<<=4;
			test |= (*(collector+ATTR_UNIT))&0xf0;
			test<<=4;
			test |= (*(collector+(ATTR_UNIT<<1)))&0xf0;
			test |= ((*(collector+(ATTR_UNIT*3)))&0xf0)>>4;
			
			if(test == TID){
				DeleteLine("tables.bin",line + unit * i,unit,unit*1);
				return;
			}
			collector += unit;
		}
		line += size;
	}while(size==BUFFER_SIZE);
}

void DroptheTable(char* tn,int TID){
	IntList il4ILine = 0;
	IntList il4IID = 0; 
	IntList il4IIDLine = 0;
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int line = 0;
	int size;
	int unit = IID_BYTE+TID_BYTE+1;
	do{
		size = Read("indices.bin",line,bufin);
		int howmany = size/unit;
		int i;
		char* checkTID = bufin+IID_BYTE;
		for(i = 0 ; i<howmany;i++){
			if(TID == *(short*)checkTID){
				il4IID=IntListInsert(il4IID,*(int*)(checkTID - IID_BYTE)); // ��intlist��װ�����б�ɾ����������ID 
				il4ILine=IntListInsert(il4ILine,line+i*unit);
				//DeleteLine("indices.bin",line+i*unit,unit,unit*1);
			}
			checkTID += unit;
		}
		line+=size;
	}while(size == BUFFER_SIZE);
//	PPIL(il4ILine);
	DeleteLines("indices.bin",il4ILine,unit);

	line = 0;
	unit = I_LENGTH;
	do{
		size = Read("indexID.bin",line,bufin);
		int howmany = size/unit;
		char* checkIID = bufin;
		int i;
		for(i = 0; i<howmany; i++){
			IntList checker = il4IID;
			while(checker){
				if(checker->data == *(int*)checkIID){
					il4IIDLine = IntListInsert(il4IIDLine,line+i*unit);
					break;
				}
				checker = checker->next;
			}
			checkIID += unit;
		}
		line+=size; 
	}while(size == BUFFER_SIZE);

	DeleteLines("indexID.bin",il4IIDLine,unit);
	
	// �˴���Ӧ��ɾ������index?.bin������?����il4IID 
	IntList killer = il4IID;
	while(killer){
		char ibin[20] = "index";
		sprintf(ibin+5,"%d",killer->data);
		strcat(ibin,".bin");
		remove(ibin); 
		killer = killer->next;
	}
	
	// ���TID��Ϣ 
	il4ILine = IntListDestroy(il4ILine);
	il4IID = IntListDestroy(il4IID);
	il4IIDLine = IntListDestroy(il4IIDLine);
	
	char tbin[20] = "table";
	sprintf(tbin+5,"%d",TID);
	strcat(tbin,".bin");
	remove(tbin);

	free(bufin);
	printf("Table dropping succeeds!\n");
}

void DropTable(char* tn){
#if DETAIL
	printf("DropTable(): enter, tn = %s",tn);
	printf("\n");
#endif
	int TID = GetTID(tn);
	if(TID==-1){
		printf("Table dropping fails.  No such table.\n");
		return;
	}
	DeleteTMeta(TID);
	DeleteTIDMeta(TID);
#if DETAIL
	printf("DropTable(): Meta deletion fullfills. TID = %d",TID);
	printf("\n");
#endif
    
    DroptheTable(tn,TID);
	
}

void GetTable(short TID, char* table){
	char* bufin = (char*) malloc(BUFFER_SIZE);
	int line = 0;
	int unit = 1024;
	int size;
	do{
		size = Read("tables.bin",line,bufin);
		int howmany = size / unit;
		int i;
		char* check = bufin;
		for(i = 0;i<howmany;i++){
			short test = 0;
			test |= check[0] & 0xf0;
			test <<= 4;
			test |= check[32] &0xf0;
			test <<= 4;
			test |= check[64] & 0xf0;
			test |= ((check[96]&0xf0) >> 4);
			if(test == TID){
				int k;
				for(k=0;k<1024;k++){
					table[k] = check[k];
				}
				free(bufin);
				return ;
			}
			check += unit;
		}
		line += unit * howmany;
	}while(size == BUFFER_SIZE);
	free(bufin);
}

