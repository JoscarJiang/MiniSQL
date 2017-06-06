#define _CRT_SECURE_NO_WARNINGS 1
#include"filemanip.h"


int GetFileSize11(char* filename){

    FILE *fp=fopen(filename,"rb");

    if(!fp) {
	    return -1;	
	}  
	    fseek(fp,0L,SEEK_END);  
    int size=ftell(fp);  
		fclose(fp);  
    return size;  
}

