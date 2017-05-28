#include "recordmanager.h"
#include "catalogmanager.h"
#include "buffermanager.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void PPRslt(Rslt res){
	int i;
	for(i=0;i<res->mt->valid;i++){
		printf("%10s", res->mt->a[i]->name);
	}
	printf("\n");
	Tuples tup = res->t;
	
	while(tup){
		for(i=0;i<res->mt->valid;i++){
			int tipi=res->mt->a[i]->type;
			if(tipi==0){
				printf("%10d",*(int*)(tup->data[i]));
			}else
			if(tipi==1){
				printf("%9.3lf",*(float*)(tup->data[i]));
			}else{
				printf("%10s",(char*)(tup->data[i]));
			}
		}
		tup=tup->next;
		printf("\n");
	}
}

MetaTable TranslateMetaTable(MetaTable mt, char* table){
	int i;
	mt->valid = 0;
	for(i=0;i<32;i++){
		if(table[(i<<5)+1]){
			mt->valid ++ ;
		}else{
			return mt;
		}
		mt->a[i]->size = (unsigned char) table[(i<<5)+1];
		mt->a[i]->type = table[(i<<5)] & 3;
		char an[33]={0};
		int j;
		for(j=0;j<30;j++){
			an[j] = table[(i<<5)+2+j];
		}
		strcpy(mt->a[i]->name, an);
	}
	return mt;
}

char CheckCond(char* data, unsigned char* allsizes, unsigned char* alltypes, Cond cond, short* dist){
	Cond checker = cond;
	int i = 0 ;
	while(checker){
		char type = checker->value->type;
		char size = (unsigned char)checker->value->size;
		if(type==0){ // int
			float seen;
			if(alltypes[i] == 0)
				seen = *(int*)(data+dist[i]);
			if(alltypes[i]==1)
				seen = *(float*)(data+dist[i]);
			int given = *(int*)checker->value->data;
			if(checker->op == 0){ // =
				if(seen!=given) return 0;
			}else
			if(checker->op == 1){ // <=
				if(seen>given) return 0;
			}else
			if(checker->op == 3){ // <
				if(seen>=given) return 0;
			}else
			if(checker->op == 4){ // >
				if(seen<=given) return 0;
			}else
			if(checker->op == 6){ // >=
				if(seen<given) return 0;
			}else
			if(checker->op == 7){ // <>
				if(seen==given) return 0;
			}
		}else
		if(type==1){ // float
			float seen;
			if(alltypes[i] == 0)
				seen = *(int*)(data+dist[i]);
			if(alltypes[i]==1)
				seen = *(float*)(data+dist[i]);
			float given = *(float*)checker->value->data;
			if(checker->op == 0){ // =
				if(seen!=given) return 0;
			}else
			if(checker->op == 1){ // <=
				if(seen>given) return 0;
			}else
			if(checker->op == 3){ // <
				if(seen>=given) return 0;
			}else
			if(checker->op == 4){ // >
				if(seen<=given) return 0;
			}else
			if(checker->op == 6){ // >=
				if(seen<given) return 0;
			}else
			if(checker->op == 7){ // <>
				if(seen==given) return 0;
			}
		}else
		if(type==3){ // char(n)
			char* seen = (char*) malloc(allsizes[i] + 1);
			memcpy(seen,data+dist[i], allsizes[i]);
			seen[allsizes[i]] = 0;
			if(checker->op == 0){ // =
				if(strcmp(seen, (char*)checker->value->data)) {free(seen);return 0;}
				free(seen); 
			}else
			if(checker->op == 1){ // <=
				if(strcmp(seen, (char*)checker->value->data) > 0) {free(seen);return 0;}
				free(seen);
			}else
			if(checker->op == 3){ // <
				if(strcmp(seen, (char*)checker->value->data) >= 0) {free(seen);return 0;}
				free(seen);
			}else
			if(checker->op == 4){ // >
				if(strcmp(seen, (char*)checker->value->data) <= 0) {free(seen);return 0;}
				free(seen);
			}else
			if(checker->op == 6){ // >=
				if(strcmp(seen, (char*)checker->value->data) < 0) {free(seen);return 0;}
				free(seen);
			}else
			if(checker->op == 7){ // <>
				if(!strcmp(seen, (char*)checker->value->data)) {free(seen);return 0;}
				free(seen);
			}			
		}
		i++;
		checker = checker->next;
	}
	return 1;
}

void* TranslateAttr(Attr a, void* data, char* source){
	if(a->type == 3){
		data = malloc(a->size+1);
		memset(data,0,a->size+1);
		memcpy(data,source,a->size);
		return data;
	}
	data = malloc(a->size);
	memcpy(data,source,a->size);
	return data;
}

Rslt AddRslt(char* data, Rslt res){
	Tuples tup = (Tuples) malloc(sizeof(struct tuples));
	int i;
	char* check = data;
	for(i=0;i<res->mt->valid;i++){
		tup->data[i] = TranslateAttr(res->mt->a[i], tup->data[i], check);
		check += res->mt->a[i]->size;
	}
	for(;i<32;i++){
		tup->data[i] = 0;
	}
	tup->next = 0;
	Tuples checkt = res->t;
	if(!checkt){
		res->t = tup;
	}else{
		tup->next = res->t;
		res->t = tup;
	}
	return res;
}

void MetaTableDestroy(MetaTable mt){
	int i;
	for(i=0;i<32;i++){
		free(mt->a[i]);
	}
	return;
}

void RsltDestroy(Rslt res){
	int v = res->mt->valid;
	MetaTableDestroy(res->mt);
	free(res->mt);
	Tuples killer = res->t;
	while(res->t){
		int i;
		for(i=0;i<v;i++){
///				free(res->t->data[0]);

			free(res->t->data[i]);
		}
		killer = res->t;
		res->t = res->t->next;
		free(killer);
	}
	free(res);
}

Cond CondOpt(Cond cond){
	if(cond == 0 || cond->next == 0) return cond;
	
	Cond head = cond;
	
	Cond train = head->next;
	Cond father = head;
	while(train){
		if(train->op == 0){
			father->next = train->next;
			train->next = head;
			head = train;
			train = father->next;
		}else{
			father = father->next;
			train = train->next;
		}
	}
	return head;
}

IntList GetIntList(Cond cond,int TID,IntList il,char* found){
	Cond attempt = cond;
	while(attempt){
		if(attempt->op == 7){ // 不等于的情况不用索引 
		}else{
			int AID = GetAID(TID,attempt->an) & 31;
			int IID = FindIID(TID,AID);		
			if(IID == -1) {
				attempt = attempt->next;
				continue; ///////////////////////
			}
			switch(attempt->op){
				case 0:{
printf("bfr Bp=()\n");
					il = IntListInsert(il,BpEqual(IID,attempt->value->type,(unsigned char)attempt->value->size,attempt->value->data));
printf("aft Bp=(), PPIL()\n");
PPIL(il);
					break;
				}
				case 1:{
printf("bfr Bp<=()\n");

					il = BpLessEqual(IID,attempt->value->type,attempt->value->size,attempt->value->data,il);
printf("aft Bp<=(), PPIL()\n");
PPIL(il);
					break;
				}
				case 3:{
printf("bfr Bp<()\n");

					il = BpLess(IID,attempt->value->type,attempt->value->size,attempt->value->data,il);
printf("aft BpM<(), PPIL()\n");
PPIL(il);
					break;
				}
				case 4:{
//printf("befbpm\n");
//printf("%d %d %d %d\n",IID,attempt->value->type,attempt->value->size,*(int*)attempt->value->data);
printf("bfr Bp>()\n");
					il = BpMore(IID,attempt->value->type,attempt->value->size,attempt->value->data,il);
printf("aft Bp>(), PPIL()\n");
PPIL(il);
					break;
				}
				case 6:{
printf("bfr Bp>=()\n");

					il = BpMoreEqual(IID,attempt->value->type,attempt->value->size,attempt->value->data,il);
printf("aft Bp>=(), PPIL()\n");
PPIL(il);
					break;
				}
			}
			*found = 1;
			il=IncreaseIntList(il);
			return il;
		}
		attempt = attempt->next;
	} 
	*found = 0;
	return 0;
}
 
void SelectFromWhere(char* tn, Cond cond){

#if DETAIL
printf("SFW: begin, tn = %s",tn);
printf("\n");
#endif

	int i;
	short TID = GetTID(tn);
	if(TID==-1){
		printf("Selection fails.  No such table.\n");
		return;
	}
	char* table = (char*) malloc (1024);
	GetTable(TID,table);
#if DETAIL
printf("SFW: TID = %d",TID);
printf("\n");
#endif

	Rslt res = (Rslt) malloc(sizeof(struct rslt));
	res->mt = (MetaTable) malloc(sizeof(struct metaTable));
	res->t = 0;
	for(i=0;i<32;i++){
		res->mt->a[i] = (Attr) malloc(sizeof (struct attr));
	}
	res->mt = TranslateMetaTable(res->mt, table);
#if DETAIL
printf("SFW: begin checking cond");
printf("\n");
#endif
	
	// 异常返回记得free掉一些东西 
	// 确认cond符合要求 
	Cond checker = cond;
	while(checker){
		int AID = GetAID(TID,checker->an)&31;
		if(AID == -1){
			printf("Selection fails!  No such attribute: '%s'\n",checker->an);
			free(table);
			RsltDestroy(res); 
			return;
		}
		char t0 = res->mt->a[AID]->type;
		char t1 = checker->value->type;
		if(t0 == 3 && t1 != 3){
			printf("Selection fails!  Expected char(n), but provided int or float.\n");
			free(table);
			RsltDestroy(res); 
			return;
		}
		if(t1 == 3 && t0 != 3){
			printf("Selection fails!  Expected int or float, but provided char(n).\n");
			free(table);
			RsltDestroy(res); 
			return;
		}
		checker = checker->next;
	}
	// 检查cond中的属性存在 
	// 检查cond中的数据类型匹配 
	// 考虑int和float的互化。改变cond中的type 

	int unit = 0 ; // 从 table?.bin 当中需要读出的字节数（一个元组的长度）
	int alloc = res->mt->valid;
	short* endpoint = (short*) malloc(alloc*2); // 忘记乘2导致属性数不能超过4个。可继续往下检查。。。。 
	for(i=0;i<res->mt->valid;i++){
		endpoint[i] = unit;
		unit += res->mt->a[i]->size;
	}
#if DETAIL
printf("SFW: prepare for all[]");
printf("\n");
#endif
	
	// 准备传入CheckCond的参数，需要元信息 
	Cond counter = cond;
	int concon=0;
	while(counter){
		concon++;
		counter = counter->next;
	}
	short* dist = (short*) malloc(concon*2);
	unsigned char* allsizes = (unsigned char*) malloc(concon);
	unsigned char* alltypes = (unsigned char*) malloc(concon);
	counter = cond;
	for(i=0;i<concon;i++){
		dist[i] = endpoint[GetAID(TID,counter->an)&31];
		allsizes[i] = res->mt->a[GetAID(TID,counter->an)&31]->size;
		alltypes[i] = res->mt->a[GetAID(TID,counter->an)&31]->type;
		counter = counter->next;
	}
	char found=0;
	cond = CondOpt(cond);
	// 逐行Check 
	IntList il = 0;
#if BP_OK
	printf("In sfw, bfr gil:\n");
	PPCOND(cond);
	il = GetIntList(cond,TID,il,&found);
	printf("In SFW(), aft GIL(), PPIL():\n");
	PPIL(il);
#endif
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int line = 0;
	int size;
	char* tbin = (char*)malloc(20);
	memcpy(tbin,"table",5);
	sprintf(tbin+5,"%d",TID);
	strcat(tbin,".bin");
if(!found){ // found为0表示没有找到可用索引 
#if DETAIL
printf("SFW: cannot find any index");
printf("\n");
#endif

	do{
		size = Read(tbin,line,bufin);
		int howmany = size/unit;
		int j;
		for(j=0;j<howmany;j++){
			if(CheckCond(bufin+j*unit,allsizes,alltypes,cond,dist)){
				res = AddRslt(bufin+j*unit,res);
			}
		}
		line += howmany * unit;
	}while(size==BUFFER_SIZE);
}else{ // found被置1表示找到了一个索引 
#if DETAIL
printf("SFW: find an index");
printf("\n");
#endif

	IntList cand = il;
	while(cand){
		line = cand->data;
		int currentStartLine = line;
		size = Read(tbin,line,bufin);
		do{
			if(CheckCond(bufin+(line-currentStartLine),allsizes,alltypes,cond,dist)){
				res = AddRslt(bufin+(line-currentStartLine),res);
			}
			cand = cand->next;
			if(!cand) break;
			line = cand->data;
		}while(line + unit <= currentStartLine + size);
	}
}
	// 若可用索引， 
	// 用B+找出所有符合条件的行号，存到intlist
	// 在此intList的数据的基础上，把table?.bin翻译成result 
	// 在result中判断，把不符合条件的整个tuple剔除
	// 若不可用索引，
	// 直接在bufin中判断，把符合条件的整个tuple存入result
	// 再在result中判断，剔除不合条件的
	
#if DETAIL
printf("SFW: begin destroying");
printf("\n");
#endif

	 
	// 判断是否符合条件用一个函数 

	PPRslt(res);
	RsltDestroy(res);
	il = IntListDestroy(il);////

	free(table);
	free(bufin);
	free(tbin);	

	if(endpoint)free(endpoint); 
	if(dist) free(dist);
	if(allsizes) free(allsizes);
	if(alltypes) free(alltypes);

}

void DeleteFromWhere(char* tn, Cond cond){
	// 若可用索引，用B+找出所有符合条件的行号
	// 根据行号，把二进制存到result中，需要记录行号 
	// 再根据之后的条件，把result中不符合条件的删除
	// 把最后剩下的行号更新到intList里面 
	// 调用DeleteLines
	// 若不可用索引，第一回在bufin中存result
	// 之后直接在result中逐个判断、剔除
	// 再更新intList 
	// DeleteLines
	// 重新构造这张表上的所有索引 
	
	unsigned short TID = GetTID(tn);
//	printf("delete tid:%d\n",TID);
	if((short)TID == -1){
		printf("Deletion fails.  No such table.\n");
		return;
	}
	char* table = (char*) malloc(1024);
	GetTable(TID,table);
	MetaTable mt = (MetaTable) malloc(sizeof(struct metaTable));
	int i;
	for(i=0;i<32;i++){
		mt->a[i] = (Attr)malloc(sizeof(struct attr));
	}
	TranslateMetaTable(mt,table);
	Cond checker = cond;
	while(checker){
		int AID = GetAID(TID,checker->an)&31;
		if(AID == -1){
			printf("Deletion fails!  No such attribute: '%s'\n",checker->an);
			free(table);
			MetaTableDestroy(mt);
			free(mt);
			return;
		}
		char t0 = mt->a[AID]->type;
		char t1 = checker->value->type;
		if(t0 == 3 && t1 != 3){
			printf("Deletion fails!  Expected char(n), but provided int or float.\n");
			free(table);
			MetaTableDestroy(mt);
			free(mt);
			return;
		}
		if(t1 == 3 && t0 != 3){
			printf("Deletion fails!  Expected int or float, but provided char(n).\n");
			free(table);
			MetaTableDestroy(mt);
			free(mt);
			return;
		}
		checker = checker->next;
	}
	
	int unit = 0 ; // 从 table?.bin 当中需要读出的字节数（一个元组的长度）
	int alloc = mt->valid;
	short* endpoint = (short*) malloc(alloc*2);
	
	for(i=0;i<mt->valid;i++){
		endpoint[i] = unit;
		unit += mt->a[i]->size;
	}
	
	// 准备传入CheckCond的参数，需要元信息 
	Cond counter = cond;
	int concon=0;
	while(counter){
		concon++;
		counter = counter->next;
	}
	short* dist = (short*) malloc(concon*2);
	unsigned char* allsizes = (unsigned char*) malloc(concon);
	unsigned char* alltypes = (unsigned char*) malloc(concon);
	counter = cond;
	for(i=0;i<concon;i++){
		dist[i] = endpoint[GetAID(TID,counter->an)&31];
		allsizes[i] = mt->a[GetAID(TID,counter->an)&31]->size;
		alltypes[i] = mt->a[GetAID(TID,counter->an)&31]->type;
		counter = counter->next;
	}
	
	char found;
	cond = CondOpt(cond);
	IntList il4deletion = 0;
	il4deletion = GetIntList(cond,TID,il4deletion,&found);
//	PPIL(il4deletion);
	
	// 逐行Check 
	char* bufin = (char*)malloc(BUFFER_SIZE);
	int line = 0;
	int size;
	char* tbin = (char*)malloc(20);
	memcpy(tbin,"table",5);
	sprintf(tbin+5,"%d",TID);
	strcat(tbin,".bin");
if(!found){
	do{
		size = Read(tbin,line,bufin);
		int howmany = size/unit;
		int j;
		for(j=0;j<howmany;j++){
			if(CheckCond(bufin+j*unit,allsizes,alltypes,cond,dist)){
				il4deletion = IntListInsert(il4deletion,line+j*unit);
			}
		}
		line += howmany * unit;
	}while(size==BUFFER_SIZE);
}else{ // found被置1表示找到了一个索引 
	IntList cand = il4deletion;
	while(cand){
		line = cand->data;
		int currentStartLine = line;
		size = Read(tbin,line,bufin);
		do{
			if(!CheckCond(bufin+(line-currentStartLine),allsizes,alltypes,cond,dist)){
				il4deletion = IntListDeleteNode(cand,il4deletion);
			}
			cand = cand->next;
			if(!cand) break;
			line = cand->data;
		}while(line + unit <= currentStartLine + size);
	}
}
	printf("In dfw: ppil,  %s fs = %d\n",tbin,GetFileSize11(tbin));
	PPIL(il4deletion);
	DeleteLines(tbin,il4deletion,unit);
	printf("In dfw: aft deletelins  %s fs = %d\n",tbin,GetFileSize11(tbin));

	// 重建索引
	char* need = (char*) malloc (alloc); // 是否存在索引。
	int* IIDs = (int*) malloc(alloc<<2); // 每个属性的索引ID 
//	PVoidList* alldata = (PVoidList*) malloc(alloc * sizeof(struct pVoidList)); /////// Error!
	PVoidList* alldata = (PVoidList*) malloc(alloc * sizeof(PVoidList)); /////// Error!
	
	IntList lines = 0;
	
	for(i=0;i<alloc;i++){
		alldata[i] = 0;
		int IID = FindIID(TID,i);
		if(IID == -1){
			need[i] = 0;
		}else{
			need[i] = 1;
			IIDs[i] = IID;
		}
	} 


	// 把数据弄进alldata 
	line = 0;
	do{
		size = Read(tbin,line,bufin);
		int howmany = size/unit;
		int j;
		for(j=0;j<howmany;j++){
			lines = IntListInsert(lines,line+unit*j);
			int k;
			for(k=0;k<alloc;k++){
				if(need[k]){
//					printf("%d %d\n",j*unit+endpoint[k],(unsigned char)(table[(k<<5)+1]));
					alldata[k]=PVoidListInsert(bufin+j*unit+endpoint[k],(unsigned char)(table[(k<<5)+1]),alldata[k],(unsigned char)((table[k<<5])&2)?1:0);
//					printf("alldata[k]=%02X\n",*(char*)(alldata[k]));
				}
			}
		}
		line += howmany * unit;
	}while(size==BUFFER_SIZE);	
	
	for(i=0;i<mt->valid;i++){
		if(need[i]){
//					printf("alldata[i]=%02X\n",*(char*)(alldata[i]));
			BpBuild(IIDs[i],(unsigned char)(table[i<<5]) & 3,(unsigned char)(table[(i<<5)+1]),lines,alldata[i]);
		}
		alldata[i]=PVoidListDestroy(alldata[i]);
	} 
	printf("Deletion succeeds!\n");
	
	if(alldata)free(alldata);
	il4deletion = IntListDestroy(il4deletion);
	lines = IntListDestroy(lines);
	free(IIDs);
	free(need);
	MetaTableDestroy(mt);
	free(mt);
	free(tbin);
	free(bufin);
	if(endpoint) free(endpoint);
	if(dist) free(dist);
	if(allsizes) free(allsizes);
	if(alltypes) free(alltypes); 
	free(table);

	return;
}

void InsertIntoValues(char* tn, Values v){
	short TID;
	TID = GetTID(tn);
	if(TID==-1){
		printf("Insertion fails.  No such table.\n");
		return;
	}
	char* table = (char*)malloc(1024);
	void* inputdata[32]; // 用于查重记录 
	int beginpoint[32] = {0};
	int allsizes[32] = {0};
	char alltypes[32] = {0};
	int k;
	for(k=0;k<32;k++){
		inputdata[k] = 0;
	}
	GetTable(TID,table);
	Values trial = v;
	char* check = table;
	char* checkunique = (char*)malloc(32);
	memset(checkunique,0,32);
	while(trial){
		// 如果有unique还要检查是否重复 
		if(!check[1]){
			printf("Insertion fails.  Too many attributes.\n");
			free(checkunique);
			free(table);
			return;
		}
		char TypeShouldBe = check[0]&3;
		unsigned char SizeShouldBe = (unsigned char)check[1];
		char TypeIndeed = trial->type;
		unsigned char SizeIndeed = (unsigned char)trial->size;
		if(TypeShouldBe != TypeIndeed){
			if(TypeShouldBe == 1 && TypeIndeed == 0){
				float tmp = (float)(*(int*)trial->data);
				free(trial->data);
				trial->data = malloc(sizeof(tmp));
				*(float*)(trial->data) = tmp;
				trial->type = 1;
			}
			else{
				printf("Insertion fails.  Data types do not match.  Should %d, but %d.\n",TypeShouldBe, TypeIndeed);
				free(table);
				free(checkunique);
				return;
			}
		}
		if(SizeShouldBe < SizeIndeed){
			printf("Insertion fails.  The 'char' type is too long.\n");
			free(table);
			free(checkunique);
			return;
		}
		int AID = (check-table)>>5;
		allsizes[AID] = SizeShouldBe;
		alltypes[AID] = TypeShouldBe;
		if(!AID)beginpoint[AID] = 0;
		else{
			beginpoint[AID] = beginpoint[AID-1]+allsizes[AID-1];
		}
		if(check[0] & 4){ // should be unique
			int IID = FindIID(TID,AID);
			if((IID == -1)){ //////////////////////////////
				checkunique[AID] = 1; // 需要后续确认是unique的 
				if(trial->type == 3){
					inputdata[AID] = malloc((unsigned char)trial->size+1);
					memcpy(inputdata[AID],trial->data,(unsigned char)trial->size);		
					((char*)inputdata[AID])[(unsigned char)trial->size] = 0;			
				}else{
					inputdata[AID] = malloc((unsigned char)trial->size);
					memcpy(inputdata[AID],trial->data,(unsigned char)trial->size);	
				}
			}else{
				int line = BpEqual(IID,check[0]&3,check[1],trial->data);
				if(line == -1){ // is unique
				}else{
					printf("Insertion fails.  '%s' should be unique, but the new value is duplicate.\n",check+2); 
					free(table);
					free(checkunique);
					return;
				}
			}
		}
		trial = trial->next;
		check += 32;
	}
	if(((check-table)!=1024) && check[1]){
		printf("Insertion fails.  Too few attributes.\n");
		free(table);
		free(checkunique);
		return;
	}
	
	int valid = ((check - table)>>5); // 计算共有多少个属性 
	int j;
	for(j=0;j<valid;j++){
		if(checkunique[j] == 1)break;
	}
	
	char tbin[16] = "table";
	sprintf(tbin+5,"%d",TID);
	strcat(tbin,".bin");
	int unit = beginpoint[valid-1] + allsizes[valid-1];

	if(j!=valid){ // 需要扫描全文件 
		char bufin[BUFFER_SIZE];
		int line = 0;
		int size;
		do{
			size = Read(tbin,line,bufin);
			int howmany = size / unit;
			int i;
			char* check = bufin;
			for(i = 0;i<howmany;i++){
				int p;
				for(p=0;p<valid;p++){
					if(checkunique[p]){
						if(alltypes[p]==3){
							char given[256]={0};
							if(p==valid-1)
							memcpy(given,check+beginpoint[p],unit-beginpoint[p]);
							else
							memcpy(given,check+beginpoint[p],beginpoint[p+1]-beginpoint[p]);
							if(strlen(given) == strlen(inputdata[p])){
								int q;
								for(q=0;q<strlen(inputdata[p]);q++){
									if(((char*)inputdata[p])[q] != check[beginpoint[p]+q]) break;
								}
								if(q==allsizes[p]){
									printf("Insertion fails.  '%s' should be unique, but the new value is duplicate.\n","char(n)"); 
									free(checkunique);
									free(table);
									return;
								}
							}
						}else{
							int q;
							int same = 0;
							for(q=0;q<4;q++){ // int或float 
								if(((char*)((char*)(inputdata[p])))[q] == ((char*)check)[beginpoint[p]+q]){
									same++;
								}
							}
							if(same&4){
								printf("Insertion fails.  '%s' should be unique, but the new value is duplicate.\n",alltypes[p]==0?"int":"float"); 
									free(checkunique);
									free(table);
									return;
							}
						}
					}
				}
				check += unit;
			}
			line += unit * howmany;
		}while(size == BUFFER_SIZE); 
	}
	
	trial = v;
	char* writeinfo = (char*)malloc(4096);
	int cursor = 0;
	int round = 0;
	while(trial){
		int i;
		for(i=0;i<(unsigned char)table[(round<<5)+1];i++){
			if(i<(unsigned char)trial->size)
				writeinfo[cursor+i] = ((char*)trial->data)[i];
			else{
				writeinfo[cursor+i] = 0;
			}
		}
		cursor += (unsigned char)table[(round<<5)+1]; // 移动到写下一个属性的位置 
		trial = trial->next;
		round++; 
	}
	Append(tbin,writeinfo,cursor);
	
	// 在table?.bin后append
	// 在所有涉及此表的索引中增加一个value 
	trial = v;
	int a = 0;
	int lastline;
	int fs = GetFileSize11(tbin);

	while(trial){
		int IID = FindIID(TID,a++);
		if(IID==-1){
		}else{
#if BP_OK
//			printf("bef bpi: IID = %d, type = %d, size = %d, line = %d, data=%d\n",IID,trial->type,(unsigned char)trial->size,fs-unit,*(int*)trial->data);
			BpInsert(IID,trial->type,(unsigned char)trial->size,fs-unit,trial->data);
//			printf("aft bpi\n");
#endif
		}
		trial = trial->next;
	}
	
	printf("Insertion succeeds!\n");
	free(checkunique);
	free(table);
	free(writeinfo);
}
