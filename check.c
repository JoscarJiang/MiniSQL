#include "structs.h"
#include"filemanip.h"
#include"check.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
function:check the containment of indexID.bin
*/
void PPI(){ // 用于查看indexID.bin中存储的内容。建议结合二进制文件察看器，避免0x0d 
	FILE* fp = fopen("indexID.bin","rb");
	int sh;
	char name[100];
	while(ftell(fp) != GetFileSize11(fp)){
		fread(&sh,IID_BYTE,1,fp);
		memset(name,0,100);
		fread(name,MAX_INDEX_NAME,1,fp);
		printf("%d %s\n",sh,name);
	}
	fclose(fp);
}

void PPGB(){ // 用于察看当前读寄存器的状态 
	struct buf* pb = GlobalBuffer->head;
	int i;
	for(i = 0; i < 16; i++){
		printf("%5d",i);
	}
//	printf("\n");
	for(i = 0; i < 16; i++){
		printf("%5d",(pb->data - readBuf[0]) / BUFFER_SIZE);
		pb = pb->next;
	}
//	printf("\n");
	pb = GlobalBuffer->head;
	for(i = 0 ; i<16; i++){
		printf("  %c%c%c",pb->fileName[3],pb->fileName[4],pb->fileName[5]);
		pb = pb->next;
	}
//	printf("\n");
	pb = GlobalBuffer->head;
	for(i = 0 ; i<16; i++){
//		printf("%5d",pb->line);
		pb = pb->next;
	}
//	printf("\n");
	pb = GlobalBuffer->head;
	for(i = 0 ; i<16; i++){
		printf("%5d",pb->size);
		pb = pb->next;
	}
//	printf("\n");
	pb = GlobalBuffer->head;
	for(i = 0 ; i<16; i++){
		printf("%5d",pb->valid);
		pb = pb->next;
	}
	printf("\n");
}

void PPIL(IntList il){
	if(!il)return;
	IntList top = il;
	while(il){
		printf("%d ",il->data);
		il = il->next;
	}
	printf("\n");
}

void PPBINSIZE(){
	FILE* f1;	f1 = fopen("indexID.bin","rb");
	printf("indexID.bin:%d\n",GetFileSize11(f1));
	fclose(f1);

	FILE* f2;	f2 = fopen("indices.bin","rb");
	printf("indices.bin:%d\n",GetFileSize11(f2));
		fclose(f2);

	FILE* f3;	f3 = fopen("tableID.bin","rb");
	printf("tableID.bin:%d\n",GetFileSize11(f3));
			fclose(f3);

	FILE* f4;	f4 = fopen("tables.bin","rb");
	printf("tables.bin:%d\n",GetFileSize11(f4));
				fclose(f4);

//	FILE* f5 = fopen("table0.bin","rb");	
//	printf("table0.bin:%d\n",GetFileSize11(f5));
//	fclose(f5);
}

void PPCOND(Cond c){
	int count = 0;
	int i;
	Cond check = c;
	while(check){
		check = check->next;
		count++;
	}
	printf("%8s","Num");
	for(i=0;i<count;i++){
		printf("%4d",i);
	}
	printf("\n");
	check = c;
	printf("%8s","op");
	for(i=0;i<count;i++){
		printf("%4d",check->op);
		check = check->next;
	}
	printf("\n");
	check = c;
	printf("%8s","an");
	for(i=0;i<count;i++){
		printf("%4s",check->an);
		check = check->next;
	}
	printf("\n");
	check = c;
	printf("%8s","type");
	for(i=0;i<count;i++){
		printf("%4d",check->value->type);
		check = check->next;
	}
	printf("\n");
}

void PPPVL(PVoidList pvl){
	
	PVoidList p = pvl;
	while(p){
		int i;
		for(i=0;i<4;i++){
			printf("%02X ",(((char*)p->data)[i]));
		}
		printf("||");
		p = p->next;
	}
	printf("\n");
	
}

void PPMT(MetaTable T){
	printf("    valid:%d\n",T->valid);
	int i;
	for(i=0;i<T->valid;i++){
		printf("    a[%d]: name:%s,pk%d,size%d,type%d,unique%d\n",i,T->a[i]->name,T->a[i]->pk,T->a[i]->size,T->a[i]->type,T->a[i]->unique); 
	}
}

void PPV(Values vv){
	Values x = vv;
	int count = 0;
	while(x){
		count++;
		x=x->next;
	}
	int i;
	
	printf("%10s","Num");
	for(i=0;i<count;i++){
		printf("%4d",i);
	}
	printf("\n");
	
	printf("%10s","type");
	x = vv;
	for(i=0;i<count;i++){
		printf("%4d",x->type);
		x = x->next;
	}
	printf("\n");
	
	printf("%10s","size");
	x = vv;
	for(i=0;i<count;i++){
		printf("%4d",x->size);
		x = x->next;
	}
	printf("\n");
}

