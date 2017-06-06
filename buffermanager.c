#include "buffermanager.h"
#include "intlistmanip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void DoAppend(char* fn){  // ��д���������ļ����ӵ��ļ��� 
	FILE* fp = fopen(fn,"ab");
	fwrite(writeBuf->data,writeBuf->size,1,fp);
	fclose(fp);

	struct buf* pb = GlobalBuffer->head;
	int i;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) 
			pb->valid = 0; // �˴����Ż�����������֮ǰ������ҳ��Ȼ�ǲ���ģ��ɲ�ȡ 
		pb = pb->next;
	}
	return;
} 


// ����������������Ҫ��Ӳ���϶�ȡ����ʱ������������������루�ļ������ӵڼ����ֽڿ�ʼ��������õ�һ�����Ŀ�����ݵĻ�����
// ��������4096B��ʹ��readBuf[����ֵ]���Եõ��������ݵ�char���顣 
int GetReadBuffer(char* FileName, int line){ // �������ݴ����16��readbuffer�е���һ�� �����ѵ��ļ�β�����򷵻�-1 
	int result = -1;
	struct buf* pb; 
	struct buf* ppb;
	ppb = pb = (GlobalBuffer->head);

	// �����Ҫ�����ݣ��Ѿ���ĳ��buffer�� 
	while( pb && !(pb->valid && pb->line==line && !strcmp(pb->fileName,FileName))){ // ���ԸĽ��ĵط����������line500����Ҫ��line600��ʱ��ҲҪ�ж����� 
		ppb = pb;
		pb = pb->next;
	}
	if(pb && 0){
		if(pb == ppb){
			GlobalBuffer->tail->next = pb;
			GlobalBuffer->tail = pb;
			GlobalBuffer->head = pb->next;
			pb->next = 0;
		}else{
			GlobalBuffer->tail->next = pb;
			GlobalBuffer->tail = pb;
			ppb->next = pb->next;
			pb->next = 0;
		}
//		printf("H");
		return (pb->data  - readBuf[0]) / BUFFER_SIZE;
	}else
	
	// ��Ҫ�Ӹ����ļ����к���ȡ���ݡ� 
	{
		

		pb = GlobalBuffer->head; 
		memset(pb->data,0,BUFFER_SIZE);
		FILE *fp = fopen(FileName,"rb");
		

		int fs = GetFileSize11(FileName);
		if(line>=fs){
			fclose(fp);
			return -1;	
		} 
		fseek(fp,line,0); //�ƶ���Ҫ��ʼ��ȡ����  
		int rest = fs - line;
		fread(pb->data,1,BUFFER_SIZE,fp); //һ�ζ�4096B
		fclose(fp);
		pb->size = rest > BUFFER_SIZE ? BUFFER_SIZE : rest;
		pb->valid=1; // ��������Ч 
		strcpy(pb->fileName,FileName); //����˻���������Ϣ
		pb->line =  line;
		GlobalBuffer->tail->next = pb;
		GlobalBuffer->tail = pb;
		GlobalBuffer->head = pb->next;
		pb->next = 0;
//		printf("BelowHere\n");
		return (pb->data - readBuf[0]) / BUFFER_SIZE;
	}
}

int Append(char* fn, char* data, int size){ // ��fn�ļ�ĩβ����data��ʼ����size���ֽ�
	memset(writeBuf->data,0,BUFFER_SIZE);
	int k;
	int round = 0;
	while(size>0){
		int thissize = size>BUFFER_SIZE?BUFFER_SIZE:size;
		for(k = 0; k<thissize;k++){
			writeBuf->data[k] = data[round*BUFFER_SIZE+k];
		}
		writeBuf->size = thissize;
		DoAppend(fn);
		size-=thissize;
		round++;
	}
}

#if 1
int Read(char* fn, int line, char* bufin){ // ��fn�ļ���line�п�ʼ�����ݣ�����bufin�У�����ֵ�Ƕ��˶����ֽڣ����BUFFER_SIZE����-1��ʾ�Ѿ������� 
	int size;
	struct buf* pb = GlobalBuffer->head;

	int rbf = GetReadBuffer(fn,line);

	if(rbf==-1)return -1;//not found
	int i;
	for(i=0;i<16;i++){
		if(rbf == (pb->data - readBuf[0]) / BUFFER_SIZE){
			size = pb->size;
			break;
		} 
		pb = pb->next;
	}
	for(i=0;i<size;i++){
		bufin[i] = readBuf[rbf][i];//rbf��buffer�ı�ţ�i������
	}
	return size;
}
#endif

#if 0
int Read(char* fn, int line, char* bufin){
	FILE* fp = fopen(fn,"rb");
	int fs = GetFileSize11(fn);
	fseek(fp,line,0);
	if(fs<line){
		fclose(fp);
		return-1;
	}if(fs-line>=4096){
		fread(bufin,1,4096,fp);
		fclose(fp);
		fp=0;
		return 4096;
	}else{
		fread(bufin,1,fs-line,fp);
		fclose(fp);
		fp=0;
		return fs-line;		
	}
}
#endif
void BufsInit(){
	writeBuf = (struct writeBufStruct*) malloc(sizeof(struct writeBufStruct));
	GlobalBuffer = (Bufs*) malloc(sizeof(Bufs));
	GlobalBuffer->head = (struct buf*) malloc(sizeof(struct buf) * 16);
	int i;
	for(i =0 ; i<16;i++){
		((GlobalBuffer->head)+i)->next = ((i!=15) ? ((GlobalBuffer->head) + i+1) : 0);
		((GlobalBuffer->head)+i)->valid = 0; // ��Ӧ����Ϊ����0�������� 
		((GlobalBuffer->head)+i)->size = 0;
		((GlobalBuffer->head)+i)->line = 0;
		((GlobalBuffer->head)+i)->data = readBuf[i]; // ����һ��������� 
//		char tmp[5];
//		sprintf(tmp,"%d",i*2);
//		strcpy( ((GlobalBuffer->head)+i)->fileName, tmp);
	}
	GlobalBuffer->tail = (GlobalBuffer->head) + 15;
}

void Write(char* fn,int line, char* data, int size){ // ��Ҫ�ĳɴ���ʽ�ϣ�������write buffer 
	FILE* fp = fopen(fn,"rb+");
	fseek(fp,line,0);
	fwrite(data,size,1,fp);
	int i;
	struct buf* pb = GlobalBuffer->head;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) pb->valid = 0; // �˴����Ż�����������֮ǰ������ҳ��Ȼ�ǲ���ģ��ɲ�ȡ 
		pb = pb->next;
	}
	fclose(fp);
}

void DeleteLine(char* fn, int line, int unit, int howmany){ // �ӵײ��������������� 
	int fs = GetFileSize11(fn);
	char bufin[BUFFER_SIZE];
	char writeinfo[BUFFER_SIZE];
	int thissize = howmany>BUFFER_SIZE?BUFFER_SIZE:howmany;
	thissize = (thissize/unit) * unit;
	Read(fn,fs-thissize,bufin);
	int i,j,k;
	for(i = 0 ,j=0;i<thissize;){
		char* start;
		start = writeinfo+i;
		for(k=0;k<unit;k++){
			start[k] = bufin[thissize-(j+1)*unit + k];
		}
		i+=unit;
		j++;
	}
	Write(fn,line,writeinfo,thissize); 
	truncate(fn,fs-thissize);
	if(howmany>BUFFER_SIZE) DeleteLine(fn,line+BUFFER_SIZE,unit,howmany-thissize);
}

void DeleteLines(char* fn, IntList il, int unit){
	il = DecreaseIntList(il);
	IntList killer = il;
	while(killer){
		int line = killer->data;
		DeleteLine(fn,line,unit,1*unit);
		killer = killer->next;
	}
}

void ReWrite(char* fn, char* data, int size){
	FILE* fp = fopen(fn,"wb");
	int residual = size;
	while(residual){
		int thissize = BUFFER_SIZE>residual?residual:BUFFER_SIZE;
		writeBuf->size = thissize;
		memcpy(writeBuf->data,data,thissize);
		fwrite(writeBuf->data,writeBuf->size,1,fp);	
		residual -= thissize;
	}	int i;

	struct buf* pb = GlobalBuffer->head;
	for(i = 0 ; i<16;i++){
		if(!strcmp(fn,pb->fileName)) pb->valid = 0; // �˴����Ż�����������֮ǰ������ҳ��Ȼ�ǲ���ģ��ɲ�ȡ 
		pb = pb->next;
	}

	fclose(fp);
}
