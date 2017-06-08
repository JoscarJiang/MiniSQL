#if 0
Bplus树的结构自行设计。index0.bin的存储格式自行设计。可完成两者的转化即可。 
建议：固定每个node大小4096B，每个node存15个value（因为每个数据项最大可能的字节是255），每个指针需要附加信息指明：是否有效、指向另一个节点还是文件。 

要求提供如下函数： 

// type约定: 00 int, 01 float, 11 char(n) 
void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, IntList values);
void BpDestroy(BpNode Bptree, int IID, unsigned char type, unsigned char size);
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value);
// 不需要删除操作 
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
int BpEqual(int IID, unsigned char type, unsigned char size, void* value);
// <>操作不使用索引，直接遍历文件，不需要提供 
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il);
 
其中涉及到的结构：
struct intList{
	int data;
	struct intList* next;
};
typedef struct intList* IntList;

函数功能说明：
(以下均假设传入IID = 6)
void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, IntList values);
	建立Bplus树，其中lines和values的数据一一对应，表示<指针（行号），数据值>
	将此树存为index6.bin 
void BpDestroy(int IID, unsigned char type, unsigned char size);
	写入bin文件后，回收此Bplus树，防止占用存储空间。若建树过程中不使用malloc，此函数可不写 
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value);
	把index6.bin读为一棵Bplus树，插入一个<指针（行号），数据值>，再存回index6.bin 
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il);
	将index6.bin读出的Bplus树中，所有数据值小于value的对应的行号存入整数链表il中，返回il。
	以下函数均类似，除了BpEqual只需要返回一个行号即可。 
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
int BpEqual(int IID, unsigned char type, unsigned char size, void* value);
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il);

所有文件操作，需要使用以下函数：
void ReWrite(char* fn, char* data, int size); // 新建fn文件，写入size个字节的数据，数据来自data 
int Read(char* fn, int line, char* bufin); // 在fn文件第line行开始读数据，读到bufin中，返回值是读了多少字节，最多BUFFER_SIZE，若-1表示已经无内容 
向IntList中插入值的操作，需要使用以下函数：
IntList IntListInsert(IntList il, int x);

一个经典的使用Read的模版： 
	char bufin[BUFFER_SIZE];
	int line = 0;
	int unit = 1024; // 对于B+树，此应为BUFFER_SIZE 
	int size;
	do{
		size = Read("tables.bin",line,bufin);
		int howmany = size / unit;
		int i;
		char* check = bufin;
		for(i = 0;i<howmany;i++){
			// 根据实际需要修改以下内容 
//			short test = 0;
//			test |= check[0] & 0xf0;
//			test <<= 4;
//			test |= check[32] &0xf0;
//			test <<= 4;
//			test |= check[64] & 0xf0;
//			test |= ((check[96]&0xf0) >> 4);
//			if(test == TID){
//				int k;
//				for(k=0;k<1024;k++){
//					table[k] = check[k];
//				}
//				return ;
//			}
			// 根据实际需要修改以上内容 
			check += unit;
		}
		line += unit * howmany;
	}while(size == BUFFER_SIZE); 
#endif

#include "Bp.h"

#if 0
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value){
}

IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il){
	return 0;
}
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il){
	return 0;
}
int BpEqual(int IID, unsigned char type, unsigned char size, void* value){
	return -1;
}
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il){
	return 0;
}
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il){
	il=IntListInsert(il,0);
	il=IntListInsert(il,264);
	il=IntListInsert(il,528);
	il=IntListInsert(il,528+264);
	il=IntListInsert(il,528+528);
	il=IntListInsert(il,1056+264);
	il=IntListInsert(il,1056+528);
	return il;
}

void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, PVoidList values){
	static int c = 0;
//	printf ("%d:%d %d %d\n",c,IID,type,size);
//	PPIL(lines);
	PVoidList prtr = values;
	while(prtr){
//		int j;
//		for(j=0; j<4;j++){
//			printf("%02X ",((char*)prtr->data)[j]);
//		}
//		printf("\n");
		prtr = prtr->next;
	}
};
#endif

#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 自己设计的函数 
BpNode insert_into_index(BpNode BpTree, unsigned char type, void* K, int lines);
BpNode find(BpNode Bpt, unsigned char type, void* V, BpNode parent[]);//找节点
BpNode insert_in_leaf(BpNode L, unsigned char type, void* K, int lines);
//把root也传进去
BpNode insert_in_parent(BpNode root, unsigned char type, BpNode N, void* K, BpNode NN, BpNode parent[]);//返回root节点

void print(IntList List);

BpNode find(BpNode Bpt, unsigned char type, void* V, BpNode parent[])//已经修改
{
	BpNode C = Bpt;//C是一个B+树节点
	int i;
	int counter = 0;
	parent[counter++] = C;
	/* C不是叶子节点 */
	while (C->flag == 0)
	{
		for (i = 0; i + 1 <= 14 && C->Node_P[i + 1]->vaild != 0; i++)//可以限制BNpointer的vaild 
		{
			// i是满足 V<=C.Ki 的最小整数 
			//if (V > C->K[i]) {}
			//else if (V <= C->K[i]) break;
			if (type == 0){
				if (*((int*)V) <= *((int*)(C->K[i]))) break;
			}else
			if (type == 1){
				if (*((float*)V) <= *((float*)(C->K[i]))) break;
			}else
			if (type == 3){
				if (strcmp((char*)V, (char*)(C->K[i])) <= 0) break;
			}
			
		}
		//如果没有这样的i
		if (i + 1 > 14)
		{
			C = C->Node_P[14]->next_node;
			parent[counter++] = C;
		}
		else if (C->Node_P[i + 1]->vaild == 0)
		{
			C = C->Node_P[i]->next_node;
			parent[counter++] = C;
		}
		else if (V == C->K[i])
		{
			C = C->Node_P[i + 1]->next_node;
			parent[counter++] = C;
		}
		else//V < C->K[i]
		{
			C = C->Node_P[i]->next_node;
			parent[counter++] = C;
		}
	}//end of while

	return C;
}


// 把K和指向文件的指针插入到B+树中 
BpNode insert_into_index(BpNode BpTree, unsigned char type, void* K, int lines)//传入K的地址
{
	// 记着返回root  
	int i;
	if (BpTree == NULL)//B+树空 
	{
		// create an empty leaf node L, which is also the root
		BpTree = (BpNode)malloc(4096/*sizeof(struct Bplus_Node)*/);
		BpTree->flag = 1;//leaf
		BpTree->isroot = 1;//root
		for (i = 0; i<14; i++)
		{
			BpTree->File_P[i] = (BFpointer)malloc(sizeof(struct Bplus_File_Pointer));
			BpTree->File_P[i]->vaild = 0;//先都设为无效
		}
		//插入
		BpTree->K[0] = K;
		BpTree->File_P[0]->vaild = 1;
		BpTree->File_P[0]->lines = lines;
		//最后一个节点指针 现在还是NULL
		for (i = 0; i < 14; i++)
			BpTree->Node_P[i] = NULL;
		BpTree->Node_P[14] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));
		BpTree->Node_P[14]->next_node = NULL;
		return BpTree;
	}
	else
	{
		BpNode L;
		//在find的时候应该生成一个链表， 记录parent节点
		/*          =====   记录parent节点   =====          */
		//都是指针，操作起来容易
		BpNode parent[6];//树高6就可以应付20多万条记录了
		/*================================================*/

		L = find(BpTree, type, K, parent);//找到K应该插进去的那个叶子节点
		//if (L == BpTree) printf("yeah\n");

		//L还没有满
		if (L->File_P[13]->vaild == 0)
		{
			L = insert_in_leaf(L, type, K, lines);
			//printf("1\n");
		}
		else//L已经满了 split it
		{
			//printf("2\n");
			BpNode LL;//L'
			LL = (BpNode)malloc(4096);//新建的节点  注意变量的修改
			LL->flag = 1;
			LL->isroot = 0;
			for (i = 0; i < 14; i++)
			{
				LL->File_P[i] = (BFpointer)malloc(sizeof(struct Bplus_File_Pointer));
				LL->File_P[i]->vaild = 0;
			}

			//临时变量 保存L中有用的信息
			void* temp_K[15];
			int temp_lines[15];
			int middle = 16 / 2;//n/2=8
			
			for (i = 0; i < 14; i++)
			{
				temp_K[i] = L->K[i];//void*之间的赋值
				temp_lines[i] = L->File_P[i]->lines;
			}

			for (i = 0; i < 14; i++)
			{
				if (type == 0){
					if (*((int*)temp_K[i]) > *((int*)K)) break;
				}else
				if (type == 1){
					if (*((float*)temp_K[i]) > *((float*)K)) break;
				}else
				if (type == 3){
					if (strcmp((char*)temp_K[i], (char*)K) > 0) break;
				}
			}
			//if (temp_K[i]>K) break;
			

			int j;
			for (j = 14; j > i; j--)
			{
				temp_K[j] = temp_K[j - 1];
				temp_lines[j] = temp_lines[j - 1];
			}
			temp_K[i] = K;
			temp_lines[i] = lines;

			for (i = 0; i < 14; i++)
				LL->Node_P[i] = NULL;
			LL->Node_P[14] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));//最后一个节点

			LL->Node_P[14]->next_node = L->Node_P[14]->next_node;
			L->Node_P[14]->next_node = LL;

			// 把L中后半部分的值擦去   注意边界的选取////////////////////////////////////////////////////////
			for (i = 8; i < 14; i++)
				L->File_P[i]->vaild = 0;
			// 修改L中前半部分
			for (i = 0; i <= 7; i++)
			{
				L->K[i] = temp_K[i];
				L->File_P[i]->lines = temp_lines[i];
				//L->File_P[i]->vaild = 1;
			}
			// 修改LL中前半部分
			for (i = 0; i <= 6; i++)
			{
				LL->K[i] = temp_K[i + 8];
				LL->File_P[i]->lines = temp_lines[i + 8];
				LL->File_P[i]->vaild = 1;
			}
			// 选出LL中最小的值 KK 的地址
			void* KK = LL->K[0];

			/*if (L->isroot == 1)
			{
				BpTree = insert_in_parent(L, KK, LL, parent);
			}
			else
			{
				BpNode L_parent;
				for (i = 0; i + 1 < 6; i++)
				if (parent[i + 1] == L)break;
				L_parent = parent[i];
				L_parent = insert_in_parent(L, KK, LL, parent);
			}*/
			BpTree = insert_in_parent(BpTree, type, L, KK, LL, parent);
		}
		
	}
	return BpTree;
}

BpNode insert_in_leaf(BpNode L, unsigned char type, void* K, int lines)
{
	int i, j;

	if (type == 0){
		if (*((int*)K) < *((int*)(L->K[0])))
		{
			//insert P,K into L just before L->P[0]
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0) break;

			L->File_P[i]->vaild = 1;
			//L->K[i] = (void*)malloc(4);

			for (j = i; j>0; j--)
			{
				//*((int*)(L->K[j])) = *((int*)(L->K[j - 1]));
				L->K[j] = L->K[j - 1];
				L->File_P[j]->lines = L->File_P[j - 1]->lines;/////////////////
				//L->File_P[j] = L->File_P[j - 1];
			}
			//*((int*)L->K[0]) = *(int*)K;//void*的赋值
			L->K[0] = K;
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || *((int*)(L->K[i])) > *((int*)K)) break;

			if (L->File_P[i]->vaild == 0)//K最大
			{
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
				//*((int*)L->K[i]) = *(int*)K;
				L->K[i] = K;
			}
			else//L->K[i]>K
			{
				int k;
				for (k = i; k < 14; k++)
				if (L->File_P[k]->vaild == 0) break;

				L->File_P[k]->vaild = 1;

				for (j = k; j>i; j--)
				{
					//*((int*)(L->K[j])) = *((int*)(L->K[j - 1]));
					L->K[j] = L->K[j - 1];
					L->File_P[j]->lines = L->File_P[j - 1]->lines;///////////////////////////
					//L->File_P[j] = L->File_P[j - 1];
				}
				//*((int*)(L->K[i])) = *(int*)K;
				L->K[i] = K;
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
			}
		}
	}else
	if (type == 1){
		if (*((float*)K) < *((float*)(L->K[0])))
		{
			//insert P,K into L just before L->P[0]
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0) break;

			L->File_P[i]->vaild = 1;

			for (j = i; j>0; j--)
			{
				L->K[j] = L->K[j - 1];
				L->File_P[j]->lines = L->File_P[j - 1]->lines;/////////////////
				//L->File_P[j] = L->File_P[j - 1];
			}
			L->K[0] = K;//void*的赋值
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || *((float*)(L->K[i])) > *((float*)K)) break;

			if (L->File_P[i]->vaild == 0)//K最大
			{
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
				L->K[i] = K;
			}
			else//L->K[i]>K
			{
				int k;
				for (k = i; k < 14; k++)
				if (L->File_P[k]->vaild == 0) break;

				L->File_P[k]->vaild = 1;

				for (j = k; j>i; j--)
				{
					L->K[j] = L->K[j - 1];
					L->File_P[j]->lines = L->File_P[j - 1]->lines;///////////////////////////
					//L->File_P[j] = L->File_P[j - 1];
				}
				L->K[i] = K;
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
			}
		}
	}else
	if (type == 3){
		if (strcmp((char*)K, (char*)(L->K[0])) < 0)
		{
			//insert P,K into L just before L->P[0]
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0) break;

			L->File_P[i]->vaild = 1;

			for (j = i; j>0; j--)
			{
				L->K[j] = L->K[j - 1];
				L->File_P[j]->lines = L->File_P[j - 1]->lines;/////////////////
				//L->File_P[j] = L->File_P[j - 1];
			}
			L->K[0] = K;//void*的赋值
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || strcmp((char*)(L->K[i]), (char*)K) > 0) break;

			if (L->File_P[i]->vaild == 0)//K最大
			{
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
				L->K[i] = K;
			}
			else//L->K[i]>K
			{
				int k;
				for (k = i; k < 14; k++)
				if (L->File_P[k]->vaild == 0) break;

				L->File_P[k]->vaild = 1;

				for (j = k; j>i; j--)
				{
					L->K[j] = L->K[j - 1];
					L->File_P[j]->lines = L->File_P[j - 1]->lines;///////////////////////////
					//L->File_P[j] = L->File_P[j - 1];
				}
				L->K[i] = K;
				L->File_P[i]->vaild = 1;
				L->File_P[i]->lines = lines;
			}
		}
	}
	return L;
}

BpNode insert_in_parent(BpNode root, unsigned char type, BpNode N, void* K, BpNode NN, BpNode parent[])
{
	int i, j;
	//N 是 root
	if (N->isroot == 1)
	{
		N->isroot = 0;
		BpNode R;
		R = (BpNode)malloc(4096);//新建一个节点 作为root
		// containing N K NN
		R->flag = 0;
		R->isroot = 1;
		R->K[0] = K;
		for (i = 0; i < 14; i++)
			R->File_P[i] = NULL;//指向文件的节点永远不会用到
		for (i = 0; i < 15; i++)
		{
			R->Node_P[i] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));
			R->Node_P[i]->vaild = 0;
		}
		R->Node_P[0]->vaild = 1;
		R->Node_P[0]->next_node = N;
		R->Node_P[1]->vaild = 1;
		R->Node_P[1]->next_node = NN;
		// OK
		//////////////////////////////////////////////////////////////////////////
		return R;
	}
	//N 不是 root
	BpNode P;
	for (i = 0; i + 1 < 6; i++)
	if (parent[i + 1] == N) break;
	P = parent[i];//P = parent[N]

	//P还没有满/////////////////////////			此处有BUG！！！
	if (P->Node_P[14]->vaild == 0)
	{
		//printf("hello1\n");
		// insert after N
		for (i = 0; i < 14; i++)
		if (P->Node_P[i]->next_node == N) break;//N对应的索引值i


		/**************************************/
		int j;
		for (j = i; j < 15; j++)
		{
			if (P->Node_P[j]->vaild == 0) break;
		}//j-1还等于1

		P->Node_P[j]->vaild = 1;

		int k;
		for ( k = j - 1; k>i; k--)//这里没进去！！！
		{
			//P->Node_P[k]->next_node指向前面的节点   ？
			P->Node_P[k+1]->next_node = P->Node_P[k]->next_node;
			P->K[k] = P->K[k - 1];
		}
		/**************************************/


		// 修改i+1的节点
		//i = i + 1;
		P->K[i] = K;
		P->Node_P[i + 1]->next_node = NN;
		P->Node_P[i + 1]->vaild = 1;
		///////////////////////////////////////////////////////////////////////////
		return root;
	}
	else//P满了 split P
	{
		//printf("hello\n");
		//临时变量 保存P中有用的信息
		void* temp_K[15];
		BpNode temp_pointer[16];
		for (i = 0; i < 14; i++)
		{
			temp_K[i] = P->K[i];
			temp_pointer[i] = P->Node_P[i]->next_node;
		}
		temp_pointer[14] = P->Node_P[14]->next_node;

		// insert ... into T just after N
		for (i = 0; i < 15; i++)
		if (P->Node_P[i]->next_node == N) break;
		// 把N之后的值和指针向后移动
		
		for (j = 14; j>i; j--)
		{
			temp_pointer[j + 1] = temp_pointer[j];
			temp_K[j] = temp_K[j - 1];
		}
		temp_pointer[i + 1] = NN;
		temp_K[i] = K;

		// 创建新的非叶子节点
		BpNode PP;
		PP = (BpNode)malloc(4096);
		PP->flag = 0;
		PP->isroot = 0;
		for (i = 0; i < 14; i++)
			PP->File_P[i] = NULL;
		for (i = 0; i < 15; i++)
		{
			PP->Node_P[i] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));
			PP->Node_P[i]->vaild = 0;
		}

		// 把P中后半部分擦去
		for (i = 8; i <= 14; i++)
			P->Node_P[i]->vaild = 0;
		// 修改P中前半部分
		for (i = 0; i <= 6; i++)
		{
			P->K[i] = temp_K[i];
			P->Node_P[i]->next_node = temp_pointer[i];
			P->Node_P[i]->vaild = 1;
		}
		P->Node_P[7]->next_node = temp_pointer[7];
		P->Node_P[7]->vaild = 1;

		//K[7]被扔掉！
		// 修改PP中前半部分 两边数量一致
		for (i = 0; i <= 6; i++)
		{
			PP->K[i] = temp_K[i + 8];
			PP->Node_P[i]->next_node = temp_pointer[i + 8];
			PP->Node_P[i]->vaild = 1;
		}
		PP->Node_P[7]->next_node = temp_pointer[15];
		PP->Node_P[7]->vaild = 1;

		//insert_in_parent
		root = insert_in_parent(root, type, P, temp_K[7], PP, parent);
		return root;
	}
}

void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, PVoidList values)

{
	BpNode BpTree = NULL;

	//size是干什么的
	//建B+树 体积应该是比较庞大的
	if (lines == NULL || values == NULL){ 
	
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	ReWrite(filename,0,0);
	free(filename);
	return; }

	while (lines != NULL && values != NULL)
	{
		BpTree = insert_into_index(BpTree, type, values->data, lines->data);/*把value的地址放进去*/
		values = values->next;
		lines = lines->next;
	}

	//怎么观察生成的B+树

	//将此B+树存为.bin文件		B+树的存储形式，在内存中和磁盘中分别怎么存储？
	//全部存储在硬盘里   遍历两遍B+树
	//层次遍历 计算文件头信息   queue里放B+树节点
	BpNode queue[3000];//100000合适吗？ 应该足够了
	int head = 0;
	int rear = -1;

	queue[++rear] = BpTree;//rear=0
	// root就是叶子节点 基本不可能
	if (queue[0]->flag == 1) {}

	else
	while (head <= rear)//not empty
	{
		if (queue[head]->flag == 1)
		{
			head++;
			continue;
		}
		int i = 0;
		while (i <= 14 && queue[head]->Node_P[i]->vaild == 1)//有效的子节点
		{
			//enqueue
			//非叶子节点 & 叶子节点
			queue[++rear] = queue[head]->Node_P[i]->next_node;

			queue[head]->offset[i] = rear - head;//子节点与自己间隔的block数
			i++;
		}
		//把无效的offset设为-1
		int j;
		for (j = i; j <= 14; j++)
			queue[head]->offset[j] = -1;
		//dequeue
		head++;
	}

	//
	//("test_rear: %d\n", rear);
	//

	//这里处理的太粗糙了 要重新写过
	//const char* filename = "index0.bin";
	//FILE* index_f;
	//index_f = fopen(filename, "wb");//写二进制文件
	//if (index_f == 0) exit(0);

	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* index_f;
	index_f = fopen(filename, "wb");
	if (index_f == 0) {
		printf("Cannot find %s.\n",filename);
		exit(0);	
	}


	//第二次层次遍历 读入文件
	head = 0;
	rear = -1;
	queue[++rear] = BpTree;
	while (head <= rear)
	{
		int i = 0;
		if (queue[head]->flag == 1)
		{
			//ReWrite(filename, queue[head], 4096);
			//fwrite(queue[head], 4096, 1, index_f);//向索引文件中写一个block

			/*******************************************************************************************/
			block newblock;//要写入文件的一个块
			newblock = (block)malloc(4096);
			//叶子节点不用管offset
			//for (int k = 0; k < 16; k++)
				//newblock->offset[k] = queue[head]->offset[k];
			newblock->flag = 1;
			newblock->isroot = queue[head]->isroot;
			if (type == 0){
				newblock->type = 0;
				int k;
				for (k = 0; k < 14 && queue[head]->File_P[k]->vaild == 1; k++)
				{
					newblock->value_int[k] = *((int*)(queue[head]->K[k]));
					newblock->file_vaild[k] = queue[head]->File_P[k]->vaild;
					newblock->lines[k] = queue[head]->File_P[k]->lines;
				}
			}else
			if (type == 1){
				newblock->type = 1;
				int k;
				for (k = 0; k < 14&& queue[head]->File_P[k]->vaild == 1; k++)
				{
					newblock->value_float[k] = *((float*)(queue[head]->K[k]));
					newblock->file_vaild[k] = queue[head]->File_P[k]->vaild;
					newblock->lines[k] = queue[head]->File_P[k]->lines;
				}
			}else
			if (type == 3){
				newblock->type = 3;
				int k;
				for (k = 0; k < 14&& queue[head]->File_P[k]->vaild == 1; k++)
				{
					strcpy(newblock->value_ch[k], (char*)(queue[head]->K[k]));//对吗？
					newblock->file_vaild[k] = queue[head]->File_P[k]->vaild;
					newblock->lines[k] = queue[head]->File_P[k]->lines;
				}
			}
			else{
				printf("type error in BpBuild!\n");
			}
			//不管node_vaild
			
			fwrite(newblock, 4096, 1, index_f);

			free(newblock);
			newblock = NULL;
			/*******************************************************************************************/
			head++;
			continue;
		}
		//ReWrite(filename, queue[head], 4096);
		//fwrite(queue[head], 4096, 1, index_f);//向索引文件中写一个block
		/*******************************************************************************************/
		block newblock;//要写入文件的一个块
		newblock = (block)malloc(4096);
		int k;
		for (k = 0; k < 15; k++)
		{
			newblock->offset[k] = queue[head]->offset[k];
			newblock->node_vaild[k] = queue[head]->Node_P[k]->vaild;
		}
			
		newblock->flag = queue[head]->flag;
		newblock->isroot = queue[head]->isroot;
		if (type == 0){
			newblock->type = 0;
			//int k;
			for (k = 0; k < 14 && queue[head]->Node_P[k + 1]->vaild == 1; k++)
			{
				newblock->value_int[k] = *((int*)(queue[head]->K[k]));
				//newblock->file_vaild[k] = queue[head]->File_P[k]->vaild;
				//newblock->lines[k] = queue[head]->File_P[k]->lines;
			}
		}else
		if (type == 1){
			newblock->type = 1;
			for (k = 0; k < 14&& queue[head]->Node_P[k + 1]->vaild == 1; k++)
				newblock->value_float[k] = *((float*)(queue[head]->K[k]));
		}else
		if (type == 3){
			newblock->type = 3;
			for (k = 0; k < 14&& queue[head]->Node_P[k + 1]->vaild == 1; k++)
				strcpy(newblock->value_ch[k], (char*)(queue[head]->K[k]));//对吗？
		}
		else{ printf("error in BpBuild!\n"); }

		fwrite(newblock, 4096, 1, index_f);

		free(newblock);
		newblock = NULL;


		/*******************************************************************************************/
		while (i <= 14 && queue[head]->Node_P[i]->vaild == 1)
		{
			queue[++rear] = queue[head]->Node_P[i]->next_node;
			i++;
		}
		head++;
	}

	fclose(index_f);
	// OK
	// 回收B+树
	BpDestroy(BpTree, 0, 0, 0);//有段错误了？？？
}

void BpDestroy(BpNode BpTree, int IID, unsigned char type, unsigned char size)
{
	// 回收Bplus树
	/* 释放了节点中的指针，保留节点 */
	BpNode queue[3000];//100000合适吗？ 应该足够了
	int head = 0;
	int rear = -1;

	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->lines);
	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->vaild);

	queue[++rear] = BpTree;//rear=0
	// root就是叶子节点 基本不可能
	if (queue[0]->flag == 1) {}
	// 复制层次遍历的代码
	else
	while (head <= rear)//not empty
	{
		if (queue[head]->flag == 1)
		{
			//free(queue[head]);
			//queue[head] = NULL;
			head++;
			continue;
		}
		int i = 0;
		while (i <= 14 && queue[head]->Node_P[i]->vaild == 1)//有效的子节点
		{
			//enqueue
			queue[++rear] = queue[head]->Node_P[i]->next_node;
			i++;
		}
		//dequeue
		//free(queue[head]);//释放内存
		//queue[head] = NULL;
		head++;
	}

	//
	//printf("rear: %d\n", rear);
	//

	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->lines);
	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->vaild);

	//从后向前回收
	int i, j;
	for (i = rear; i >= 0; i--)
	{
		//先回收内部的指针
		if (queue[i]->flag == 1)//叶子节点
		{
			free(queue[i]->Node_P[14]);//Node_P[15]
			for (j = 0; j < 14; j++)
				free(queue[i]->File_P[j]);//////////////////
		}
		else//非叶子节点
		{
			for (j = 0; j < 15; j++)
				free(queue[i]->Node_P[j]);
		}
		//回收整个节点
		free(queue[i]);
	}
}


//向IntList中插入数据 插到链表头
IntList insert_il(IntList List, int X)
{
	IntList ptr;
	ptr = (IntList)malloc(sizeof(struct intList));
	ptr->data = X;
	ptr->next = List;
	return ptr;
}

PVoidList insert_vl(PVoidList List, void* X)
{
	PVoidList ptr;
	ptr = (PVoidList)malloc(sizeof(struct pVoidList));
	ptr->data = X;
	ptr->next = List;
	return ptr;
}

void print(IntList List)
{
	if (List == NULL) printf("NULL List!\n");
	while (List != NULL)
	{
		printf("%d ", List->data);
		List = List->next;
	}
}




int BpEqual(int IID, unsigned char type, unsigned char size, void* value)//value 改为 ElementType ， 本来是void*
{
	//读取IID对应的索引文件
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");//读二进制文件
	if (fp == 0) exit(0);

	block buffer;
	buffer = (block)malloc(4096);
	if (fread(buffer, 4096, 1, fp) == 0){ return -1; }

	// 先判断type是否正确
	if (buffer->type != type){ printf("type error in BpEqual!\n"); return -1; }
	fread(buffer,4096,1,fp);

	//读到的block不是叶子节点
	while (buffer->flag == 0)
	{
		int i;
		//第一个比value大的位置
		for (i = 0; i < 14 && buffer->node_vaild[i + 1] == 1; i++)
		{
			if (type == 0){
				if (buffer->value_int[i] > *((int*)value)) break;
			}else
			if (type == 1){
				if (buffer->value_float[i] > *((float*)value)) break;
			}else
			if (type == 3){
				if (strcmp(buffer->value_ch[i], (char*)value) > 0) break;
			}
		}
		//if (buffer->value[i]>value) break;

		//不管i是多少，都满足这个关系
		fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);//从当前位置读进一个block
	}
	fclose(fp);
	//读到叶子节点
	int i;
	if (type == 0){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_int[i] == *((int*)value)) break;

		if (i == 14) { 
		//printf("error in BpEqual\n"); 
		return -1; }
		else return buffer->lines[i];
	}else
	if (type == 1){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_float[i] == *((float*)value)) break;

		if (i == 14) { 
		//printf("error in BpEqual\n"); 
		return -1; }
		else return buffer->lines[i];
	}else
	if (type == 3){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && strcmp(buffer->value_ch[i], (char*)value) == 0) break;

		if (i == 14) {
		// printf("error in BpEqual\n"); 
		return -1; }
		else return buffer->lines[i];
	}
	else{ printf("type error in BpEqual!\n"); return -1; }
	
}




//将index6.bin读出的Bplus树中，所有数据值小于value的对应的行号存入整数链表il中，返回il。
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il)
{
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");
	if (fp == 0) exit(0);

	block buffer;
	buffer = (block)malloc(4096);
	if (GetFileSize11(filename)==0){ return NULL; }
	//读到第一个叶子节点
		fread(buffer,4096,1,fp);

	while (buffer->flag == 0)
	{
		fseek(fp, 4096 * (buffer->offset[0] - 1), SEEK_CUR);
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);
	}
	// type检查...

	il = NULL;
	while (1)
	{
		int i;
		if (type == 0){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (buffer->value_int[i] < *((int*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}else
		if (type == 1){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (buffer->value_float[i] < *((float*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}else
		if (type == 3){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (strcmp(buffer->value_ch[i], (char*)value) < 0) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else{ printf("type error in BpLess!\n"); return NULL; }
		
		//读下一个block 顺序
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	}
}


IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il)
{
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");
	if (fp == 0) exit(0);

	block buffer;
	buffer = (block)malloc(4096);
	if (GetFileSize11(filename)==0){ return NULL; };
	//读到第一个叶子节点
		fread(buffer,4096,1,fp);

	while (buffer->flag == 0)
	{
		fseek(fp, 4096 * (buffer->offset[0] - 1), SEEK_CUR);
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);
	}

	il = NULL;
	while (1)
	{
		int i;
		if (type == 0){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (buffer->value_int[i] <= *((int*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else
		if (type == 1){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (buffer->value_float[i] <= *((float*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else
		if (type == 3){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//当前buffer  Node   buffer->value[i] < value
				if (strcmp(buffer->value_ch[i], (char*)value) <= 0) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else{ printf("type error in BpLess!\n"); return NULL; }

		//读下一个block 顺序
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	}
}

/*
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, ElementType value, IntList il)
{
	const char* filename = "index0.bin";
	FILE* fp = fopen(filename, "rb");
	if (fp == 0) exit(0);
	//先找到能插入的那一个节点
	BpNode buffer;
	buffer = (BpNode)malloc(4096);
	fread(buffer, 4096, 1, fp);
	while (buffer->flag == 0)//复制BpEqual中的代码
	{
		int i;
		for (i = 0; i < 15 && buffer->Node_P[i + 1]->vaild == 1; i++)
		{
			if (buffer->K[i] > value) break;
		}//第一个比value大的位置
		if (i == 15)
			//value最大
			fseek(fp, 4096 * (buffer->offset[15] - 1), SEEK_CUR);
		else
			fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (BpNode)malloc(4096);
		fread(buffer, 4096, 1, fp);//从当前位置读进一个block
	}

	//顺序向后读取
	int i, j;
	for (i = 0; i < 15; i++)
	if (buffer->File_P[i]->vaild == 1 && buffer->K[i] >= value) break;

	il = NULL;
	for (j = i; j < 15; j++)
	{
		if (buffer->File_P[j]->vaild == 1) il = insert_il(il, buffer->File_P[j]->lines);
	}
	free(buffer);
	buffer = NULL;
	buffer = (BpNode)malloc(4096);

	if (fread(buffer, 4096, 1, fp) == 0) return il;
	else
	while (1)
	{
		for (i = 0; i < 15 && buffer->File_P[i]->vaild == 1; i++)
		{
			//当前buffer  Node
			il = insert_il(il, buffer->File_P[i]->lines);
		}
		//读下一个block 顺序
		free(buffer);
		buffer = NULL;
		buffer = (BpNode)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) return il;
	}
}
*/

IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il)
{
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");
	if (fp == 0) exit(0);

	//先找到能插入的那一个节点
	block buffer;
	buffer = (block)malloc(4096);
	if (GetFileSize11(filename)==0){ return NULL; }
	fread(buffer,4096,1,fp);

	while (buffer->flag == 0)//复制BpEqual中的代码
	{
		int i;
		for (i = 0; i < 14 && buffer->node_vaild[i + 1] == 1; i++)
		{
			if (type == 0){
				if (buffer->value_int[i] >= *((int*)value)) break;
			}else
			if (type == 1){
				if (buffer->value_float[i] >= *((float*)value)) break;
			}else
			if (type == 3){
				if (strcmp(buffer->value_ch[i], (char*)value) >= 0) break;
			}
		}
		fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);//从当前位置读进一个block
	}

	//顺序向后读取
	int i, j;
	if (type == 0){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_int[i] >= *((int*)value)) break;
	}else
	if (type == 1){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_float[i] >= *((float*)value)) break;
	}else
	if (type == 3){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && strcmp(buffer->value_ch[i], (char*)value) >= 0) break;
	}
	//for (i = 0; i < 14; i++)
	//if (buffer->file_vaild[i] == 1 && buffer->value[i] >= value) break;

	il = NULL;
	for (j = i; j < 14; j++)
	{
		if (buffer->file_vaild[j] == 1) il = insert_il(il, buffer->lines[j]);
	}
	free(buffer);
	buffer = NULL;
	buffer = (block)malloc(4096);

	if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	else
	while (1)
	{
		for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
		{
			//当前buffer  Node
			il = insert_il(il, buffer->lines[i]);
		}
		//读下一个block 顺序
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	}
}


IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il)
{
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");
	if (fp == 0) exit(0);

	//先找到能插入的那一个节点
	block buffer;
	buffer = (block)malloc(4096);
	printf("IN bpm, filen = %s\n",filename);
//	if (fread(buffer, 4096, 1, fp) == 0){ return NULL; }
	if(GetFileSize11(filename) == 0 ){return 0;
	}
	fread(buffer,4096,1,fp);

	while (buffer->flag == 0)//复制BpEqual中的代码
	{
		int i;
		

		for (i = 0; i < 14 && buffer->node_vaild[i + 1] == 1; i++)
		{
			if (type == 0){
				if (buffer->value_int[i] > *((int*)value)) break;
			}
			else
			if (type == 1){
				if (buffer->value_float[i] > *((float*)value)) break;
			}
			else
			if (type == 3){
				if (strcmp(buffer->value_ch[i], (char*)value) > 0) break;
			}
		}
		fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);//从当前位置读进一个block
	}


	//顺序向后读取
	int i, j;
	if (type == 0){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_int[i] > *((int*)value)) break;
	}
	else
	if (type == 1){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && buffer->value_float[i] > *((float*)value)) break;
	}
	else
	if (type == 3){
		for (i = 0; i < 14; i++)
		if (buffer->file_vaild[i] == 1 && (strcmp(buffer->value_ch[i], (char*)value) > 0)) break;
	}
	//for (i = 0; i < 14; i++)
	//if (buffer->file_vaild[i] == 1 && buffer->value[i] >= value) break;
	il = NULL;
	for (j = i; j < 14; j++)
	{
		if (buffer->file_vaild[j] == 1) {
			il = insert_il(il, buffer->lines[j]);
		}
	}
	free(buffer);
	buffer = NULL;
	buffer = (block)malloc(4096);


	if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp);  return il; }
	else
	while (1)
	{
		for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
		{
			//当前buffer  Node
			il = insert_il(il, buffer->lines[i]);
		}
		//读下一个block 顺序
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	}


}


void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* values)
{
	printf("Enter BPI with IID %d, type %d, size %d, line %d, values %d\n",IID,type,size,line,*(int*)values);
	// 把index6.bin读为一棵Bplus树
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");//只读
	if (fp == 0) exit(0);
	// 读到第一个叶子节点
	block buffer[1000];

	int counter = 0;
	buffer[0] = (block)malloc(4096);

//	printf("in bpi, %s %d\n",filename,GetFileSize11(filename));
	if (GetFileSize11(filename) == 0){
		printf("in BpI, 0 size: %s %d\n",filename,GetFileSize11(filename));
		//空文件
		IntList oldlines = NULL;
		PVoidList oldvalues = NULL;
		oldlines = insert_il(oldlines, line);
		oldvalues = insert_vl(oldvalues, values);
		BpBuild(IID, type, size, oldlines, oldvalues);
		free(buffer[0]);
		return;
	}
	fread(buffer[0],4096,1,fp);
	//读到第一个叶子节点
	while (buffer[counter]->flag == 0)
	{
		//printf("hi");
//		printf("in bpi, ofs = %d\n",buffer[counter]->offset[0]);
		fseek(fp, 4096 * (buffer[counter]->offset[0] - 1), SEEK_CUR);
		free(buffer[counter]);
		buffer[counter] = NULL;
		//buffer = (block)malloc(4096);
		buffer[++counter] = (block)malloc(4096);
//		printf("T5\n");
//		printf("ftell = %d, fs = %d\n",ftell(fp),GetFileSize11(filename));
		fread(buffer[counter], 4096, 1, fp);
//		printf("T6\n");
	}

	
	// 把value和lines拷贝到链表中
	IntList oldlines;
	PVoidList oldvalues;
	oldlines = NULL;
	oldvalues = NULL;
	//int counter = 0;
	
	while (1)
	{
		int i;
		if (type == 0){
			for (i = 0; i < 14 && buffer[counter]->file_vaild[i] == 1; i++)
			{
				
				oldvalues = insert_vl(oldvalues, &(buffer[counter]->value_int[i]));
				
				oldlines = insert_il(oldlines, buffer[counter]->lines[i]);
			}
		}else
		if (type == 1){
			for (i = 0; i < 14 && buffer[counter]->file_vaild[i] == 1; i++)
			{
				oldvalues = insert_vl(oldvalues, &(buffer[counter]->value_float[i]));
				oldlines = insert_il(oldlines, buffer[counter]->lines[i]);
			}
		}else
		if (type == 3){
			for (i = 0; i < 14 && buffer[counter]->file_vaild[i] == 1; i++)
			{
				oldvalues = insert_vl(oldvalues, buffer[counter]->value_ch[i]);
				oldlines = insert_il(oldlines, buffer[counter]->lines[i]);
			}
		}
		else{ printf("error in insert!\n"); }
		
		//free(buffer);
		//buffer = NULL;
		buffer[++counter] = (block)malloc(4096);
		if (fread(buffer[counter], 4096, 1, fp) == 0) break;

		//counter++;
	}
	fclose(fp);


	// 插入一个<指针（行号），数据值>，再存回index6.bin
	oldvalues = insert_vl(oldvalues, values);
	oldlines = insert_il(oldlines, line);


	/*while (oldvalues){
		printf("res:   %d\n", *(int*)(oldvalues->value));
		oldvalues = oldvalues->Next;
	}*/
	//print(oldlines);

	// 建B+树
	// 注意文件名字的修改
	BpBuild(IID, type, size, oldlines, oldvalues);	

	int i;
	for (i = 0; i <= counter; i++)
		free(buffer[i]);
		

}

// size
// type -- ElementType void*
// filename sprintf

// 新增了一个结构体 voidList

