#define _CRT_SECURE_NO_WARNINGS 1

#include<stdlib.h>
#include<string.h>
#include"unistd.h"

#include"macro.h"
#include"structs.h"
#include"filemanip.h"
#include"check.h"
#include"buffermanager.h"
#include"intlistmanip.h"
#include"catalogmanager.h"
#include"recordmanager.h"
#include"Bp.h"

// 注意该用unsigned 的地方别省略了。 


////////////////////// GZX


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#define N 400
#define M 2500

int starlight(char* in)
{
  int i,sum;
  i=0;
  sum=0;
  while(in[i]!='\0')
  {
    sum=sum*10;
    sum+=(in[i]-'0');
    i++;
  }
  return sum;
}

float tobefloat(char* a)
{
  int n,num,power;
  float m = 0.0;
  n=strlen(a);
  power=0;
  for(num=0;num<n;num++)
  {
  if(a[num]=='.')
  power=n-num-1;
  else
  m=m*10+(a[num]-'0');
  }
//  printf("%f  ", m);

  for(num=0;num<power;num++)
    m=m/10;

    return m;
}
#if 0
int main(){ // To Test BpBuild
	IntList lines = 0;
	PVoidList values = 0;
	int IID = 0, type = 0, size = 0;
	lines = IntListInsert(lines, 0);
	lines = IntListInsert(lines, 12);
	char src[8] = {5,0,0,0,6,0,0,0};
	values = PVoidListInsert(src,4,values,0);
	values = PVoidListInsert(src+4,4,values,0);
	
	printf("Hey1!\n");
//	BpBuild(IID,type,size,lines,values); 
	BpBuild(IID,type,size,0,0); 
	printf("Hey2!\n");
	printf("line = %d\n",BpEqual(IID,type,size,src));
}
#endif
#if 1

int main()
{
	FILE* fope = fopen("tableID.bin","wb");fclose(fope);
	fope = fopen("tables.bin","wb");fclose(fope);
	BufsInit();
	FILE* fpp = fopen("indexID.bin","wb");
	fclose(fpp);
	fpp = fopen("indices.bin","wb");
	fclose(fpp);
	fpp = fopen("table0.bin","wb");
	fclose(fpp);
    char* query;
    char* query1;
    char* text;
    char* in;
    char* tn;
    char* an;
    char sentence[M][N];
    char word[M][N];
    char word1[2][N];
    char unword[N]={0};
    

    MetaTable T;
    Values v,d1,d2;
    Cond cond,p1,p2;
    int n,j,num,i,k=0,len,wordnumber,re,cmp,metanum,condnum,valusnum,isfloat;
    FILE *fp;
    
    
    while(1){
    memset(word1,0,(2)*(N));
    memset(sentence,0,(M)*(N));
    memset(word,0,(M)*(N));
    query = (char *)malloc(300*(sizeof(char)));
    //text = (char *)malloc(200*(sizeof(char)));
    query1 = (char *)malloc(300*(sizeof(char)));
    
	printf("what do you want to do?\n");
	gets(query1);
	n=strlen(query1);
	k=0;
	j=0;
	for(i=0;i<n;i++)
	{
		if(query1[i]==' '){
			j++;
		   k=0;	
		}   
		else{
		   word1[j][k]=query1[i];
			k++;			
		}
	}
    printf("w0 %s\n",word1[0]);
    if(strcmp("execfile",word1[0])==0)
    {printf("w1 %s\n",word1[1]);
    fp = fopen(word1[1] , "r");
    fseek( fp , 0 , SEEK_END );
    int file_size;
    file_size = ftell( fp );
    fseek( fp , 0 , SEEK_SET);
    text =  (char *)malloc( file_size * sizeof( char ) + 1);
    memset(text,0,file_size * sizeof( char ) + 1);
    fread( text , file_size , sizeof(char) , fp);
	fclose(fp);
	
	
    wordnumber=0;
    j=0;
	num=0;
	k=0;
    //printf("insert your text \n");
    //gets(text);
    n=strlen(text);

    for(i=0;i<n;i++)
	{
	    if(text[i]==';')
		{
			num++;
			j++;
			k=0;
			i++;
		}
	    else
		{
			sentence[j][k]=text[i];
			k++;
		}

	}
	
    for(i=0;i<num;i++) // <= to < 
    {
    	printf("%d %d i\n",i,num);
       n=strlen(sentence[i]); // n为a[N]的长度
       j=0;
       k=0;
       for(wordnumber=0;wordnumber<n;wordnumber++)
	{
		if((sentence[i][wordnumber]>64&&sentence[i][wordnumber]<91)||(sentence[i][wordnumber]>96&&sentence[i][wordnumber]<123)||sentence[i][wordnumber]=='='||sentence[i][wordnumber]=='>'||sentence[i][wordnumber]=='<'||sentence[i][wordnumber]=='*'||sentence[i][wordnumber]=='.'||(sentence[i][wordnumber]>='0'&&sentence[i][wordnumber]<='9')||sentence[i][wordnumber]=='-')
		{
			word[j][k]=sentence[i][wordnumber];
			k++;
		}
		else
		{
//			printf("word[%d] %s\n",j,word[j]);
			j++;
			k=0;
		}
	}
    if(strcmp("create",word[0])==0)
      {
          if(strcmp("index",word[1])==0)
          {
                 in = (char *)malloc(200*(sizeof(char)));
                 tn = (char *)malloc(200*(sizeof(char)));
                 an = (char *)malloc(200*(sizeof(char)));
                /*i= strcmp("index",word[1]);
                  if(i!=0)
                printf("query failed!\n");*/
                strcpy(in ,word[2]);

                /*i= strcmp("on",word[3]);
                if(i!=0)
                printf("query failed!\n");*/

                strcpy(tn,word[4]);


                strcpy(an,word[6]);
                printf("【Enter CreateIndex(): in:%s,tn:%s,an:%s\n",in,tn,an);
            CreateIndex(in, tn, an,0);
                free(in);
                free(an);
                free(tn);
                in=an=tn=0;
          }

         if(strcmp("table",word[1])==0)
          {
		tn = (char*) malloc(200);
        T=(struct metaTable*)malloc(sizeof(struct metaTable));
        T->valid=0;
     for(wordnumber=0;wordnumber<n;wordnumber++)
	{
		if(sentence[i][wordnumber]==',')
            T->valid++;
	}

	strcpy(tn,word[2]);


k=4;
//printf("%d",T->valid);
   for(metanum=0;metanum<T->valid;metanum++)
   {
       T->a[metanum]=(struct attr*)malloc(sizeof(struct attr));
       printf("For attr[%d], name is %s\n",metanum, word[k]);
       strcpy(T->a[metanum]->name,word[k]);//要申请空间
       k++;
       //strcpy(T->a[i]->type,word[4+k]);
       cmp=strcmp("float",word[k]);
     //  printf("%s %d\n",word[4+k],cmp);
     if(cmp<0) cmp=-1;
     if(cmp>0) cmp=1;
       switch(cmp){
    case(-1): T->a[metanum]->type = 0;break;
     case 0:  T->a[metanum]->type = 1;break;
     case 1:  T->a[metanum]->type = 3;break;
   }
      k++;
//   printf("%d",T->a[metanum]->type);
       if(cmp!=1)
      T->a[metanum]->size=4;
       else
       {
        T->a[metanum]->size=starlight(word[k]);
         k++;
		 k++; 
		 }
		 if(strcmp("unique",word[k])==0)
		 {
		 	T->a[metanum]->unique=1;
		 	k++;
		 }
		 else
		 T->a[metanum]->unique=0;
      k++;
   }
   k+=2;
   printf("pk: w[k] = %s\n",word[k]);
    for(metanum=0;metanum<T->valid;metanum++)  //输入必须是 primary key（id_of_key）\n   的形式，不然不能运行
    {
        if(strcmp(word[k],T->a[metanum]->name)==0){
        	T->a[metanum]->pk=1;
            T->a[metanum]->unique=1;
		}
                    else{
            T->a[metanum]->pk=0;
           	
					}
             }
 /*  k+=4;
   for(metanum=0;metanum<T->valid;metanum++)  //输入必须是 unique （id_of_key）\n   的形式，不然不能运行
    {
        if(strcmp(word[k],T->a[metanum]->name)==0)
            T->a[metanum]->unique=1;
        else{
        	if(!T->a[metanum]->pk)T->a[metanum]->unique=0;
		}
            
    }*/
strcpy(T->tn, tn);
free(tn);
tn=0;
printf("【Enter CreateTable(): tn:%s\n",T->tn,an);
PPMT(T);
printf("】\n"); 

CreateTable(T);

free(T);
          }
          
      }


     if(strcmp("drop",word[0])==0)
      {
          if(strcmp("index",word[1])==0)
      {
            in = (char *)malloc(200*(sizeof(char)));
             strcpy(in,word[2]);
             
printf("【Enter DropIndex(): in:%s\n",in);
             DropIndex( in);
             free(in);
             in=0;
      }
          if(strcmp("table",word[1])==0)
      {
            tn = (char *)malloc(200*(sizeof(char)));
             strcpy(tn,word[2]);
 printf("【Enter DropTable(): tn:%s\n",tn);
            DropTable(tn);
             free(tn);
      }
      }

    if(strcmp("select",word[0])==0)//select *\nfrom dara\nwhere  x <= n
     {
          tn = (char *)malloc(200*(sizeof(char)));
        // /  cond=(struct cond*)malloc(sizeof(struct cond));
       // /   p2 =(struct cond*)malloc(sizeof(struct cond));
       p1 = p2= cond =0;
          strcpy(tn,word[3]);
          if(*word[4]=='\0')
          {
          	printf("【Enter SelectFW(): tn:%s\n",tn);
PPCOND(cond);
printf("】\n");
         SelectFromWhere(tn,cond);
         free(tn);
         goto next;
		  }
          
          condnum=1;
        for(k=0;k<=j;k++)
    {
       if(strcmp("and",word[k])==0)
        condnum++;
    }

    k=5;

   // printf("%d  %s",condnum,word[5]);
          for(re=0;re<condnum;re++){
                p1=(struct cond*)malloc(sizeof(struct cond));
              isfloat=0;
              strcpy(p1->an,word[k]);
              k++;
              if(strcmp("=",word[k])==0)
                p1->op= 0;
              if(strcmp("<=",word[k])==0)
                p1->op= 1;
              if(strcmp("<",word[k])==0)
                p1->op= 3;
              if(strcmp(">",word[k])==0)
                p1->op= 4;
              if(strcmp(">=",word[k])==0)
                p1->op= 6;
              if(strcmp("<>",word[k])==0)
                p1->op= 7;
             // printf("%d",p1->op);
                k++;
                p1->value=(struct values*)malloc(sizeof(struct values));
				if(*word[k]=='\0')
                  {k++;
                   p1->value->type = 3;
                   p1->value->data = malloc(strlen(word[k])+1);
                   strcpy((char*)p1->value->data,word[k]);
                   p1->value->size=strlen(word[k]);
                   p1->value->next=NULL;
                   k++;
                  }
                else{
                len=strlen(word[k]);
                for(valusnum=0;valusnum<len;valusnum++)
                    if(word[k][valusnum]=='.')
                        isfloat=1;

                    if(isfloat==1)
                    {
                        p1->value->type = 1;
                        p1->value->size=4;
                        p1->value->next=NULL;
                        p1->value->data = malloc(4);
                        if(word[k][0]=='-'){
                        for(valusnum=1;valusnum<len;valusnum++)
                        {
                        	unword[valusnum-1]=word[k][valusnum];
						}
							*(float*)p1->value->data =tobefloat(unword)*(-1);}
                        else
                       *(float*)p1->value->data =tobefloat(word[k]);
                    }
                    else
                    {
                        p1->value->type = 0;
                        p1->value->size=4;
                        p1->value->next=NULL;
                        p1->value->data = malloc(4);
                        if(word[k][0]=='-'){
                        for(valusnum=1;valusnum<len;valusnum++)
                        {
                        	unword[valusnum-1]=word[k][valusnum];
						}
						*(int*)p1->value->data =starlight(unword)*(-1);}
                        else
                       *(int*)p1->value->data=starlight(word[k]);
                    }
                }
             //这里还有变量名！
			    k+=2;
                if(cond==NULL){
                    cond=p1;
                    p2 = p1;
                    p1->next=NULL;
                }
                else{
                    p2->next=p1;
                    p2 = p2->next;
                    p2->next = NULL;
                }
          }
printf("【Enter SelectFW(): tn:%s\n",tn);
PPCOND(cond);
printf("】\n");
         SelectFromWhere(tn,cond);
        free(tn);
        tn=0;
          // remove the whole linked list
          while(cond){
         	p2 = cond;
    		cond = cond->next;
          	free(p2->value->data);
          	free(p2->value);
          	free(p2);
          	
		  }
    }

     if(strcmp("delete",word[0])==0)
       {
          tn = (char *)malloc(200*(sizeof(char)));
          //cond=(struct cond*)malloc(sizeof(struct cond));
          p1 =(struct cond*)malloc(sizeof(struct cond));
          //p2 =(struct cond*)malloc(sizeof(struct cond));
          p2=cond=0;
		  strcpy(tn,word[2]);
		  
		  if(*word[3]=='\0')
          {
          	printf("【Enter DeleteFW(): tn:%s\n",tn);
PPCOND(cond);
printf("】\n");
          DeleteFromWhere(tn,cond);
         free(tn);
         goto next;
		  }
		  
          condnum=1;
      //    printf("%s",tn);
        for(k=0;k<=j;k++)
    {
       if(strcmp("and",word[k])==0)
        condnum++;
    }
    k=4;

   // printf("%d  %s",condnum,word[5]);
          for(re=0;re<condnum;re++)
          {
              isfloat=0;
              strcpy(p1->an,word[k]);
              k++;
              if(strcmp("=",word[k])==0)
                p1->op= 0;
              if(strcmp("<=",word[k])==0)
                p1->op= 1;
              if(strcmp("<",word[k])==0)
                p1->op= 3;
              if(strcmp(">",word[k])==0)
                p1->op= 4;
              if(strcmp(">=",word[k])==0)
                p1->op= 6;
              if(strcmp("<>",word[k])==0)
                p1->op= 7;
                k++;
                p1->value=(struct values*)malloc(sizeof(struct values));
                if(word[k]=='\0')
                  {k++;
                   p1->value->type = 3;
                    p1->value->data = malloc(strlen(word[k])+1);
                   strcpy((char*)p1->value->data,word[k]);
                   p1->value->size=strlen(word[k]);
                   p1->value->next=NULL;
                  }
                else{
                len=strlen(word[k]);
                for(valusnum=0;valusnum<len;valusnum++)
                    if(word[k][valusnum]=='.')
                        isfloat=1;

                    if(isfloat==1)
                    {
                        p1->value->type = 1;
                        p1->value->size=4;
                        p1->value->next=NULL;
                        p1->value->data = malloc(4);
                        if(word[k][0]=='-'){
                        for(valusnum=1;valusnum<len;valusnum++)
                        {
                        	unword[valusnum-1]=word[k][valusnum];
						}
							*(float*)p1->value->data =tobefloat(unword)*(-1);}
                        else
                        *(float*)p1->value->data=tobefloat(word[k]);
                    }
                    else
                    {
                        p1->value->type = 0;
                        p1->value->size=4;
                        p1->value->next=NULL;
                        p1->value->data = malloc(4);
                        if(word[k][0]=='-'){
                        for(valusnum=1;valusnum<len;valusnum++)
                        {
                        	unword[valusnum-1]=word[k][valusnum];
						}
						*(int*)p1->value->data =starlight(unword)*(-1);}
                        else
                        *(int*)p1->value->data=starlight(word[k]);
                    }
                }

             //这里还有变量！
                k++;

                if(cond==NULL){
                    cond=p1;
                    p2 = p1;
                    p1->next=NULL;
                }

                else{
                    p2->next=p1;
                    p2 = p2->next;
                    p2->next = NULL;
                }

                p1=(struct cond*)malloc(sizeof(struct cond));
               // printf("dra");
          }
printf("【Enter DeleteFW(): tn:%s\n",tn);
PPCOND(cond);
printf("】\n");
          DeleteFromWhere(tn,cond);
          free(tn);
		  while(cond){
          	p2 = cond;
          	cond = cond->next;
          	free(p2->value->data);
          	free(p2->value);
			 free(p2);
          	
		  }
          
          free(p1);
     }

	if(strcmp("quit",word[0])==0){
		    free(query);
  		  free(text);
		exit(0);
	}

     if(strcmp("insert",word[0])==0)//insert into *** values (b,c)
     {
     	
         tn = (char *)malloc(200*(sizeof(char)));
         //v=(struct values*)malloc(sizeof(struct values));
         d1=(struct values*)malloc(sizeof(struct values));
         v = d2 = 0;
         //d2=(struct values*)malloc(sizeof(struct values));
         strcpy(tn,word[2]);
         k=5;
         valusnum=1;
        for(wordnumber=0;wordnumber<n;wordnumber++)
	{
		if(sentence[i][wordnumber]==',')
            valusnum++;
	}
         for(re=0;re<valusnum;re++){
			isfloat = 0;
         if(*word[k]=='\0')
                  {k++;
                   d1->type = 3;
                   d1->data = malloc(strlen(word[k])+1);
                   strcpy((char*)d1->data,word[k]);
                   d1->size=strlen(word[k]);
                   k++;
                  }
                else{
                len=strlen(word[k]);
                int z;
                for(z=0;z<len;z++)
                    if(word[k][z]=='.')
                        isfloat=1;

                    if(isfloat==1)
                    {
                        d1->type = 1;
                        d1->size=4;
                        d1->data = malloc(4);
                         if(word[k][0]=='-'){
                         	int y = 1;
                        for(y=1;y<len;y++)
                        {
                        	unword[y-1]=word[k][y];
						}
							*(float*)d1->data =tobefloat(unword)*(-1);}
                        else
                        *(float*)d1->data=tobefloat(word[k]);
                    }
                    else
                    {
                        d1->type = 0;
                        d1->size=4;
                        d1->data = malloc(4);
                         if(word[k][0]=='-'){
                         	int y = 1;
                        for(y=1;y<len;y++)
                        {
                        	unword[y-1]=word[k][y];
						}
							*(int*)d1->data =starlight(unword)*(-1);}
                        else
                        *(int*)d1->data=starlight(word[k]);
                    }
                }

                if(v==NULL){         	
                    v=d1;
                    d2 = d1;
                    d1->next=NULL;
                }

                else{         	
                    d2->next=d1;
                    d2 = d2->next;
                    d2->next = NULL;
                }

d1 =0 ;
                d1=(struct values*)malloc(sizeof(struct values));
              k++;
         }

printf("【Enter InsertIV(): tn:%s\n",tn);
PPV(v);
printf("】\n");
		InsertIntoValues(tn,v);
         //printf("%s",word[k]);
         if(d1){
         	free(d1);
         	d1 = 0;
		 }
		 while(v){
		 	d2 = v;
		 	v = v->next;
		 	free(d2->data);
		 	free(d2);
		 	d2 = 0;
		 }
		 free(tn);
		 tn=0;
     }


next:
    //printf("%d",j);
    for(k=0;k<=j;k++)
    {
       // printf("%s",word[k]);
        n=strlen(word[k]);
        for(re=0;re<n;re++){
        	word[k][re] = '\0';
		}
    }

    }
    free(query);
    free(text);
}
    
  //  printf("strcmp %d\n",strcmp("quit",word1[0]));
    	if(strcmp("quit",word1[0])==0){
  		  free(query1);
		exit(0);
	}


for(k=0;k<=1;k++)
    {
       // printf("%s",word[k]);
        n=strlen(word1[k]);
        for(re=0;re<n;re++){
        	word[k][re] = '\0';
		}
    }

}
    
    
}
#endif
////////////////////// end of GZX

#if 0
int main(){	
	FILE* fope = fopen("tableID.bin","wb");fclose(fope);
	fope = fopen("tables.bin","wb");fclose(fope);
	BufsInit();
	FILE* fp = fopen("indexID.bin","wb");
	fclose(fp);
	fp = fopen("indices.bin","wb");
	fclose(fp);
	fp = fopen("table0.bin","wb");
	fclose(fp);
	
	int i,j,k ;
	MetaTable T = (MetaTable)malloc(sizeof(struct metaTable));
	
	strcpy(T->tn,"012345678911234567892123456789312345678941234567895");
	T->valid = 8;
	int rand[120]={5,7,9,3,6,4,8,5,4,1,22,3,6,15,1,0,2,2,20,15,2,4,8,9,9,6,2,5,4,7,12,1,3,13,24,8,9,9,10,1,5,0,12,4,1,6,12,6,0,8,7,6,5,12,5,15};
	char sizes[8]={4,40,40,40,4,40,4,4};
	char types[8]={0,3 ,3, 3, 0,3 ,1,1};
	char unis[8]= {1,1 ,0, 0, 0,1 ,0,1};
	char names[8][15]={"ID","name","dept","instructor","classNum","kitName","score","encrypt"};

	for(i=0;i<8;i++){
		T->a[i] = (Attr) malloc(sizeof(struct attr));
		Attr ta = T->a[i];
		ta->pk = 0;
		if(i==0){
			ta->pk = 1;
		}
		strcpy(ta->name,names[i]);
		ta->size = sizes[i];
		ta->type = types[i];
		ta->unique = unis[i];
	}
	CreateTable(T);


//	for(i=0;i<140;i++){
////		DropTable(T->tn);
//		T->tn[i/80] = 'A'+i%80;
//		CreateTable(T);
//		CreateIndex(T->tn,T->tn,"encrypt",0);
//	}

		Values v[100][8];
	
	
	for(i=0;i<100;i++){
		for(j=0;j<8;j++){
			v[i][j]=(Values)malloc(sizeof(struct values));
			v[i][j]->size = T->a[j]->size;
			v[i][j]->type = T->a[j]->type;
			if(v[i][j]->type==3){
				v[i][j]->data = malloc(((unsigned char)(v[i][j]->size))+1);
				memset(v[i][j]->data,0,sizeof(v[i][j]->data));
				strcpy(v[i][j]->data,"YouGuess!");
				continue;
			}else if(v[i][j]->type==1){
				v[i][j]->data = malloc(4);
				*(float*)(v[i][j]->data) = -5.05;
			}else{
				v[i][j]->data = malloc(4);
				*(int*)(v[i][j]->data) = -90;
			}
		}
	}

	for(i=0;i<100;i++){
		for(j=0;j<7;j++){
			v[i][j]->next = v[i][j+1];
		}
		v[i][j]->next = 0;
	}

	for(k=0;k<51;k++){
//		DropTable(T->tn);
		T->tn[k/80] = 'A'+k%80;
		CreateTable(T);
		CreateIndex(T->tn,T->tn,"encrypt",0);
	

	for(i=0;i<5;i++){//45
		for(j=0;j<8;j++){
			int t = v[i][j]->type;
			if(t==0){
				*(int*)v[i][j]->data += (((2*i+1)*(j+3)+1)*rand[(i*(j+1))%10]);
			}else if(t==1){
				*(float*)v[i][j]->data += ((float)((i+1)*(i+1)*(i+1)*rand[j])) / ((float)(j+1)*(rand[i%25]+2));
			}else if(t==3){
				((char*)v[i][j]->data)[i%5] = 'A' + rand[i+j] + j +j;
			}
			if(j==6)*(float*)v[i][j]->data = i%2 + 4;
		}
	InsertIntoValues(T->tn,v[i][0]); 
	}
	SelectFromWhere(T->tn,0); 
}
#if 0
	SelectFromWhere(T->tn,0); 
	
	// 6 = 4
	// 1 <= YouGIess!
	// 2 < YouGMess
	// 3 > YVuGuess!
	// 4 >= -2
	// 5 <> YOuGuess!
	// 0 <= 798
	// 7 < 1275
	// 0 > 54
	// 1 >= YHuGuess
	Cond conds[10];
	char ops[10] = {0,1,3,4,6,7,1,3,4,6};
	for(i=0;i<10;i++){
		conds[i] = (Cond)malloc(sizeof(struct cond));
		conds[i]->value = (Values)malloc(sizeof(struct values));
		conds[i]->op = ops[i];
		strcpy(conds[i]->an,T->a[i%8]->name);
	}
	for(i=0;i<9;i++){
		conds[i]->next = conds[i+1];
	}
	conds[i]->next=0;
	strcpy(conds[0]->an,T->a[6]->name);
	strcpy(conds[6]->an,T->a[0]->name);

	conds[0]->value->type = 0;
	conds[0]->value->size = 4;
	conds[0]->value->data = (int*)malloc(4);
	*(int*)conds[0]->value->data  = 4;

	conds[1]->value->type = 3;
	conds[1]->value->size = 9;
	conds[1]->value->data = (char*)malloc(10);
	strcpy(conds[1]->value->data, "YouGIess!");
	
	conds[2]->value->type = 3;
	conds[2]->value->size = 10;
	conds[2]->value->data = (char*)malloc(11);
	strcpy(conds[2]->value->data, "YouGMess!!");

	conds[3]->value->type = 3;
	conds[3]->value->size = 8;
	conds[3]->value->data = (char*)malloc(9);
	strcpy(conds[3]->value->data, "YVuGuess");
	
	conds[4]->value->type = 0;
	conds[4]->value->size = 4;
	conds[4]->value->data = (int*)malloc(4);
	*(int*)conds[4]->value->data  = -2;
	
	conds[5]->value->type = 3;
	conds[5]->value->size = 9;
	conds[5]->value->data = (char*)malloc(10);
	strcpy(conds[5]->value->data, "YOuGuess!");

	conds[6]->value->type = 0;
	conds[6]->value->size = 4;
	conds[6]->value->data = (int*)malloc(4);
	*(int*)conds[6]->value->data  = 798;

	conds[7]->value->type = 0;
	conds[7]->value->size = 4;
	conds[7]->value->data = (int*)malloc(4);
	*(int*)conds[7]->value->data = 1275;
	
	conds[8]->value->type = 0;
	conds[8]->value->size = 4;
	conds[8]->value->data = (int*)malloc(4);
	*(int*)conds[8]->value->data  = 54;
	

	conds[9]->value->type = 3;
	conds[9]->value->size = 9;
	conds[9]->value->data = (char*)malloc(10);
	strcpy(conds[9]->value->data, "YHuGuess!");
	
	
	Cond cond5 = (Cond) malloc(sizeof(struct cond));
	cond5->next = 0;
	cond5->op = 4;
	strcpy(cond5->an,T->a[7]->name);
	cond5->value = (Values)malloc(sizeof(struct values));
	cond5->value->type = 1;
	cond5->value->size = 4;
	cond5->value->data = (float*)malloc(4);
	*(float*)cond5->value->data = 0;
	
	conds[5]->next=0;
	SelectFromWhere(T->tn,conds[0]);
	SelectFromWhere(T->tn,conds[9]);
	DeleteFromWhere(T->tn,conds[9]);
	SelectFromWhere(T->tn,0);
	CreateIndex("i1",T->tn,T->a[5]->name,0);
	CreateIndex("i2",T->tn,T->a[6]->name,0);
	CreateIndex("i3",T->tn,T->a[7]->name,0);
	SelectFromWhere(T->tn,cond5); 
	DropIndex("i3");
	DropTable(T->tn);
	SelectFromWhere(T->tn,0);
	CreateTable(T);
	CreateIndex("i1",T->tn,T->a[5]->name,0);
	
//	fp = fopen("table0.bin","wb");
//	printf("fs=%d\n",GetFileSize11(fp));
//	fclose(fp);
#endif 

//for(i=0;i<100000;i++){

printf("------------------------------\n");
printf("To test fopen(). \n");
FILE* fpt = fopen("indexID.bin","rb");
printf("fp = fopen (indexID.bin , rb):  %d  %p\n",fpt,fpt);
fclose(fpt);
//}
}
#endif

#if 0
int main(){	
	FILE* fope = fopen("tableID.bin","wb");fclose(fope);
	fope = fopen("tables.bin","wb");fclose(fope);
	BufsInit();
	FILE* fp = fopen("indexID.bin","wb");
	fclose(fp);
	fp = fopen("indices.bin","wb");
	fclose(fp);
	fp = fopen("table0.bin","wb");
	fclose(fp);
	
	int i,j,k ;
	MetaTable T = (MetaTable)malloc(sizeof(struct metaTable));
	
	strcpy(T->tn,"student");
	T->valid = 8;
	int rand[]={5,7,9,3,6,4,8,5,4,1,22,3,6,15,1,0,2,2,20,15,2,4,8,9,9,6,2,5,4,7,12,1,3,13,24,8,9,9,10,1,5,0,12,4,1,6,12,6,0,8,7,6,5,12,5,15};
	char sizes[8]={4,40,40,40,4,40,4,4};
	char types[8]={0,3 ,3, 3, 0,3 ,1,1};
	char unis[8]= {1,1 ,0, 0, 0,1 ,0,1};
	char names[8][15]={"ID","name","dept","instructor","classNum","kitName","score","encrypt"};
	
	for(i=0;i<8;i++){
		T->a[i] = (Attr) malloc(sizeof(struct attr));
		Attr ta = T->a[i];
		ta->pk = 0;
		if(i==0){
			ta->pk = 1;
		}
		strcpy(ta->name,names[i]);
		ta->size = sizes[i];
		ta->type = types[i];
		ta->unique = unis[i];
	}
	CreateTable(T);
		Values v[25][8];
	
	
	for(i=0;i<25;i++){
		for(j=0;j<8;j++){
			v[i][j]=(Values)malloc(sizeof(struct values));
			v[i][j]->size = T->a[j]->size;
			v[i][j]->type = T->a[j]->type;
			if(v[i][j]->type==3){
				v[i][j]->data = malloc(((unsigned char)(v[i][j]->size))+1);
				memset(v[i][j]->data,0,sizeof(v[i][j]->data));
				strcpy(v[i][j]->data,"YouGuess!");
				continue;
			}else if(v[i][j]->type==1){
				v[i][j]->data = malloc(4);
				*(float*)(v[i][j]->data) = -5.05;
			}else{
				v[i][j]->data = malloc(4);
				*(int*)(v[i][j]->data) = -90;
			}
		}
	}

	for(i=0;i<25;i++){
		for(j=0;j<7;j++){
			v[i][j]->next = v[i][j+1];
		}
		v[i][j]->next = 0;
	}

	
	for(i=0;i<15;i++){
		for(j=0;j<8;j++){
			int t = v[i][j]->type;
			if(t==0){
				*(int*)v[i][j]->data += (((2*i+1)*(j+3)+1)*rand[(i*(j+1))%10]);
			}else if(t==1){
				*(float*)v[i][j]->data += ((float)((i+1)*(i+1)*(i+1)*rand[j])) / ((float)(j+1)*(rand[i]+2));
			}else if(t==3){
				((char*)v[i][j]->data)[i%5] = 'A' + rand[i+j] + j +j;
			}
			if(j==6)*(float*)v[i][j]->data = i%2 + 4;
		}
	InsertIntoValues(T->tn,v[i][0]); 
	}
	
	SelectFromWhere(T->tn,0); 
	
	// 6 = 4
	// 1 <= YouGIess!
	// 2 < YouGMess
	// 3 > YVuGuess!
	// 4 >= -2
	// 5 <> YOuGuess!
	// 0 <= 798
	// 7 < 1275
	// 0 > 54
	// 1 >= YHuGuess
	Cond conds[10];
	char ops[10] = {0,1,3,4,6,7,1,3,4,6};
	for(i=0;i<10;i++){
		conds[i] = (Cond)malloc(sizeof(struct cond));
		conds[i]->value = (Values)malloc(sizeof(struct values));
		conds[i]->op = ops[i];
		strcpy(conds[i]->an,T->a[i%8]->name);
	}
	for(i=0;i<9;i++){
		conds[i]->next = conds[i+1];
	}
	conds[i]->next=0;
	strcpy(conds[0]->an,T->a[6]->name);
	strcpy(conds[6]->an,T->a[0]->name);

	conds[0]->value->type = 0;
	conds[0]->value->size = 4;
	conds[0]->value->data = (int*)malloc(4);
	*(int*)conds[0]->value->data  = 4;

	conds[1]->value->type = 3;
	conds[1]->value->size = 9;
	conds[1]->value->data = (char*)malloc(10);
	strcpy(conds[1]->value->data, "YouGIess!");
	
	conds[2]->value->type = 3;
	conds[2]->value->size = 10;
	conds[2]->value->data = (char*)malloc(11);
	strcpy(conds[2]->value->data, "YouGMess!!");

	conds[3]->value->type = 3;
	conds[3]->value->size = 8;
	conds[3]->value->data = (char*)malloc(9);
	strcpy(conds[3]->value->data, "YVuGuess");
	
	conds[4]->value->type = 0;
	conds[4]->value->size = 4;
	conds[4]->value->data = (int*)malloc(4);
	*(int*)conds[4]->value->data  = -2;
	
	conds[5]->value->type = 3;
	conds[5]->value->size = 9;
	conds[5]->value->data = (char*)malloc(10);
	strcpy(conds[5]->value->data, "YOuGuess!");

	conds[6]->value->type = 0;
	conds[6]->value->size = 4;
	conds[6]->value->data = (int*)malloc(4);
	*(int*)conds[6]->value->data  = 798;

	conds[7]->value->type = 0;
	conds[7]->value->size = 4;
	conds[7]->value->data = (int*)malloc(4);
	*(int*)conds[7]->value->data = 1275;
	
	conds[8]->value->type = 0;
	conds[8]->value->size = 4;
	conds[8]->value->data = (int*)malloc(4);
	*(int*)conds[8]->value->data  = 54;
	

	conds[9]->value->type = 3;
	conds[9]->value->size = 9;
	conds[9]->value->data = (char*)malloc(10);
	strcpy(conds[9]->value->data, "YHuGuess!");
	
	
	Cond cond5 = (Cond) malloc(sizeof(struct cond));
	cond5->next = 0;
	cond5->op = 4;
	strcpy(cond5->an,T->a[7]->name);
	cond5->value = (Values)malloc(sizeof(struct values));
	cond5->value->type = 1;
	cond5->value->size = 4;
	cond5->value->data = (float*)malloc(4);
	*(float*)cond5->value->data = 0;
	
	conds[5]->next=0;
	SelectFromWhere(T->tn,conds[0]);
	SelectFromWhere(T->tn,conds[9]);
	DeleteFromWhere(T->tn,conds[9]);
	SelectFromWhere(T->tn,0);
	CreateIndex("i1",T->tn,T->a[5]->name,0);
	CreateIndex("i2",T->tn,T->a[6]->name,0);
	CreateIndex("i3",T->tn,T->a[7]->name,0);
	SelectFromWhere(T->tn,cond5); 
	DropIndex("i3");
	DropTable(T->tn);
	SelectFromWhere(T->tn,0);
	CreateTable(T);
	CreateIndex("i1",T->tn,T->a[5]->name,0);
}
#endif 

#if 0
int main(){
		
	FILE* fope = fopen("tableID.bin","wb");fclose(fope);
	fope = fopen("tables.bin","wb");fclose(fope);
	BufsInit();
	FILE* fp = fopen("indexID.bin","wb");

	fclose(fp);
	fp = fopen("indices.bin","wb");
	fclose(fp);
	fp = fopen("table0.bin","wb");
	fclose(fp);
	MetaTable T = (MetaTable)malloc(sizeof(struct metaTable));
	unsigned char sizes[32]={4,4,4,4,4,255,4,4,   255,255,255,255,255,255,255,255,    1,1,10,10,100,100,1,1,        4,4,4,4,4,4,4,4};
	char types[32]=         {1,0,0,0,0,3,  0,0,   3,  3,  3,  3,  3,  3,  3,  3,      3,3,3, 3, 3,  3,  3,3,        1,1,1,1,1,1,1,0};
	char unis[32]=          {1,1,1,1,1,1,  0,0,   1,  1,  1,  1,  0,  0,  0,  0,      0,1,0, 1, 0,  1,  0,1,        0,0,0,1,0,1,1,1};
	char names[32][29]={"0fu","1iu","2iu","3iu","4iu","5c4upk","6i","7i",    "8ch255u","9ch255u","10ch255u","11ch255u","12ch255","13ch255","14ch255","15ch255",\
	"16ch1","17ch1u","18ch10","19ch10u","20ch100","21ch100u","22ch1","23ch1u",         "24f","25f","26f","27fu","28f","29fu","30fu","31iu"};
	int i;
	strcpy(T->tn,"Test56789022345678903234567890423456789052345678906");
	T->valid = 32;
	for(i=0;i<32;i++){
		T->a[i] = (Attr) malloc(sizeof(struct attr));
		Attr ta = T->a[i];
		ta->pk = 0;
		if(i==5){
			ta->pk = 1;
		}
		strcpy(ta->name,names[i]);
		ta->size = sizes[i];
		ta->type = types[i];
		ta->unique = unis[i];
	}
	CreateTable(T);
	
	Values v[100][32];
	
	int j;
	for(i=0;i<100;i++){
		for(j=0;j<32;j++){
			v[i][j]=(Values)malloc(sizeof(struct values));
			v[i][j]->size = T->a[j]->size;
			v[i][j]->type = T->a[j]->type;
			if(v[i][j]->type==3){
				v[i][j]->data = malloc((unsigned char)(v[i][j]->size)+1);
				if(((unsigned char)(v[i][j]->size) == 255)){
//					strcpy((char*)(v[i][j]->data),CH255);
					continue;
				}
			}else{
				v[i][j]->data = malloc(4);
			}
			memset(v[i][j]->data,0,sizeof(v[i][j]->data));
			memset(v[i][j]->data,65,1);
		}
	}
	
	for(i=0;i<100;i++){
		for(j=0;j<31;j++){
			v[i][j]->next = v[i][j+1];
		}
		v[i][j]->next = 0;
	}
	
	for(i=0;i<10;i++){
		InsertIntoValues(T->tn,v[i][0]); 
	}
//	SelectFromWhere(T->tn,0); 
} 
#endif

#if 0
int main(){
	FILE* fope = fopen("tableID.bin","wb");fclose(fope);
	fope = fopen("tables.bin","wb");fclose(fope);
	BufsInit();
	FILE* fp = fopen("indexID.bin","wb");

	fclose(fp);
	fp = fopen("indices.bin","wb");
	fclose(fp);
	fp = fopen("table0.bin","wb");
	fclose(fp);
	MetaTable T = (MetaTable)malloc(sizeof(struct metaTable));
	int i = 0;
	for(i=0;i<32;i++){
		T->a[i] = (Attr)malloc(sizeof (struct attr));
		Attr thisa = T->a[i];
		if(i==1){
			thisa->pk = thisa->unique = 1;
			thisa->type = 0; thisa->size = 4;
			strcpy(thisa->name,"4");
		}
		if(i==0){
			thisa->pk = 0;thisa->unique = 1;
			thisa->type = 1; thisa->size = 4;
			strcpy(thisa->name,"B1");
		}
		if(i==3){
			thisa->pk=0;
			thisa->unique = 1;
			thisa->type = 3;thisa->size = 1;
			strcpy(thisa->name,"A");
		}
		if(i==2){
			thisa->pk=0;
			thisa->unique = 0;
			thisa->type = 3;
			thisa->size = 255;
			strcpy(thisa->name,"AAABBBCCCDDDEEEEEEDDDCCCBBBAA");
		}
	}
	T->valid = 4;
	
	for(i=0;i<8;i++){
		strcpy(T->tn, "ABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAA");
		T->tn[(i/40)]+=i%40+1; 
		printf("%d %s\n",i,T->tn);
		CreateTable(T);
		char in[60] = "ABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCD";
		in[(i/40)] += i%40+1;
	}
	
	for(i=0;i<3;i++){
		char in[60] = "ABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCDAABCD";
		in[(i/40)] += i%40+1;
		CreateIndex(in+i,XX,"B1",0);
	}
	
	char used[64];
	strcpy(used,XX);
	
	Values v = (Values)malloc(sizeof(struct values));
	v->type = 0;
	v->size = 4;
	v->data = (int*)malloc(sizeof(int));
	*(float*)(v->data) = 10*10%30;
	v->next = 0;
	
	Values vv;
	vv = (Values)malloc(sizeof(struct values));
	v->next = vv;
	vv->type = 0;
	vv->size = 4;
	vv->data = (int*)malloc(sizeof(int));
	*(int*)(vv->data) = 5;
	vv->next = 0;
	
	Values vvv = vv->next;
	vvv = (Values)malloc(sizeof(struct values));
	vv->next = vvv;
	vvv->type = 3;
	vvv->size = 4;
	vvv->data = (char*)malloc(5);
	strcpy((char*)vvv->data,"hhii");
	vvv->next = 0;
	
	Values vvvv = (Values)malloc(sizeof(struct values));
	vvv->next = vvvv;
	vvvv->type = 3;
	vvvv->size = 1;
	vvvv->data = (char*)malloc(2);
	strcpy((char*)vvvv->data,"b");
	vvvv->next = 0;
	
	for(i=0;i<12;i++){
		*(int*)(vv->data) = 5+i;		
		*(float*)(v->data) = (i*(i*i%23)%11);
		*(char*)vvvv->data = 'b' + ((i*i)%23+(i*i*i)%137)%24;
		InsertIntoValues(used,v);
	}
	
	Cond cond4 = (Cond) malloc(sizeof(struct cond));
	cond4->next = 0;
	cond4->op = 4;
	strcpy(cond4->an,"4");
	cond4->value = (Values)malloc(sizeof(struct values));
	cond4->value->type = 0;
	cond4->value->size = 4;
	cond4->value->data = (int*)malloc(4);
	*(int*)cond4->value->data = 7;
	Cond cond5 = (Cond) malloc(sizeof(struct cond));
	cond5->next = 0;
	cond5->op = 4;
	strcpy(cond5->an,"A");
	cond5->value = (Values)malloc(sizeof(struct values));
	cond5->value->type = 3;
	cond5->value->size = 1;
	cond5->value->data = (char*)malloc(2);
	*(char*)cond5->value->data = 'd';
	((char*)cond5->value->data)[1] = 0;
///////////////////
	SelectFromWhere(used,0);
	SelectFromWhere(used,cond4);
	DeleteFromWhere(used,cond4);
	SelectFromWhere(used,0);
//printf("bfr ci\n");
	SelectFromWhere(used,cond5);
////	printf("//////1the op of cond is :%d\n",cond5->op);
	CreateIndex("jj",XX,"A",0);
////	printf("//////2the op of cond is :%d\n",cond5->op);
//	DropIndex("jj");
//	printf("iid for jj = %d\n",GetIID("jj"));
//printf("aft ci\n");
//
//printf("Before final cond5:\n");
//	SelectFromWhere(used,cond5);
//printf("After final cond5:\n");
//	strcpy(cond5->an,"4");
//	
////	printf("//////3the op of cond is :%d\n",cond5->op);
	SelectFromWhere(used,cond5);
//////////////////////////////////////////
	///
//	Cond c[6];
//	int ii;
//	for(ii=0;ii<6;ii++){
//		c[ii] = (Cond) malloc(sizeof(struct cond));
//		c[ii]->op = (ii+4)%2;
//	}
//	for(ii=0;ii<5;ii++){
//		c[ii]->next = c[ii+1];
//	}
//	c[ii]->next = 0;
//	PPCOND(c[0]);
//	c[0] = CondOpt(c[0]);
//	PPCOND(c[0]);
	///
	return 0;
}

#endif

