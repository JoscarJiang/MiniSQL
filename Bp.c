#if 0
Bplus���Ľṹ������ơ�index0.bin�Ĵ洢��ʽ������ơ���������ߵ�ת�����ɡ� 
���飺�̶�ÿ��node��С4096B��ÿ��node��15��value����Ϊÿ�������������ܵ��ֽ���255����ÿ��ָ����Ҫ������Ϣָ�����Ƿ���Ч��ָ����һ���ڵ㻹���ļ��� 

Ҫ���ṩ���º����� 

// typeԼ��: 00 int, 01 float, 11 char(n) 
void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, IntList values);
void BpDestroy(BpNode Bptree, int IID, unsigned char type, unsigned char size);
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value);
// ����Ҫɾ������ 
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
int BpEqual(int IID, unsigned char type, unsigned char size, void* value);
// <>������ʹ��������ֱ�ӱ����ļ�������Ҫ�ṩ 
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il);
 
�����漰���Ľṹ��
struct intList{
	int data;
	struct intList* next;
};
typedef struct intList* IntList;

��������˵����
(���¾����贫��IID = 6)
void BpBuild(int IID, unsigned char type, unsigned char size, IntList lines, IntList values);
	����Bplus��������lines��values������һһ��Ӧ����ʾ<ָ�루�кţ�������ֵ>
	��������Ϊindex6.bin 
void BpDestroy(int IID, unsigned char type, unsigned char size);
	д��bin�ļ��󣬻��մ�Bplus������ֹռ�ô洢�ռ䡣�����������в�ʹ��malloc���˺����ɲ�д 
void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* value);
	��index6.bin��Ϊһ��Bplus��������һ��<ָ�루�кţ�������ֵ>���ٴ��index6.bin 
IntList BpLess(int IID, unsigned char type, unsigned char size, void* value, IntList il);
	��index6.bin������Bplus���У���������ֵС��value�Ķ�Ӧ���кŴ�����������il�У�����il��
	���º��������ƣ�����BpEqualֻ��Ҫ����һ���кż��ɡ� 
IntList BpLessEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
int BpEqual(int IID, unsigned char type, unsigned char size, void* value);
IntList BpMoreEqual(int IID, unsigned char type, unsigned char size, void* value, IntList il);
IntList BpMore(int IID, unsigned char type, unsigned char size, void* value, IntList il);

�����ļ���������Ҫʹ�����º�����
void ReWrite(char* fn, char* data, int size); // �½�fn�ļ���д��size���ֽڵ����ݣ���������data 
int Read(char* fn, int line, char* bufin); // ��fn�ļ���line�п�ʼ�����ݣ�����bufin�У�����ֵ�Ƕ��˶����ֽڣ����BUFFER_SIZE����-1��ʾ�Ѿ������� 
��IntList�в���ֵ�Ĳ�������Ҫʹ�����º�����
IntList IntListInsert(IntList il, int x);

һ�������ʹ��Read��ģ�棺 
	char bufin[BUFFER_SIZE];
	int line = 0;
	int unit = 1024; // ����B+������ӦΪBUFFER_SIZE 
	int size;
	do{
		size = Read("tables.bin",line,bufin);
		int howmany = size / unit;
		int i;
		char* check = bufin;
		for(i = 0;i<howmany;i++){
			// ����ʵ����Ҫ�޸��������� 
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
			// ����ʵ����Ҫ�޸��������� 
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

// �Լ���Ƶĺ��� 
BpNode insert_into_index(BpNode BpTree, unsigned char type, void* K, int lines);
BpNode find(BpNode Bpt, unsigned char type, void* V, BpNode parent[]);//�ҽڵ�
BpNode insert_in_leaf(BpNode L, unsigned char type, void* K, int lines);
//��rootҲ����ȥ
BpNode insert_in_parent(BpNode root, unsigned char type, BpNode N, void* K, BpNode NN, BpNode parent[]);//����root�ڵ�

void print(IntList List);

BpNode find(BpNode Bpt, unsigned char type, void* V, BpNode parent[])//�Ѿ��޸�
{
	BpNode C = Bpt;//C��һ��B+���ڵ�
	int i;
	int counter = 0;
	parent[counter++] = C;
	/* C����Ҷ�ӽڵ� */
	while (C->flag == 0)
	{
		for (i = 0; i + 1 <= 14 && C->Node_P[i + 1]->vaild != 0; i++)//��������BNpointer��vaild 
		{
			// i������ V<=C.Ki ����С���� 
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
		//���û��������i
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


// ��K��ָ���ļ���ָ����뵽B+���� 
BpNode insert_into_index(BpNode BpTree, unsigned char type, void* K, int lines)//����K�ĵ�ַ
{
	// ���ŷ���root  
	int i;
	if (BpTree == NULL)//B+���� 
	{
		// create an empty leaf node L, which is also the root
		BpTree = (BpNode)malloc(4096/*sizeof(struct Bplus_Node)*/);
		BpTree->flag = 1;//leaf
		BpTree->isroot = 1;//root
		for (i = 0; i<14; i++)
		{
			BpTree->File_P[i] = (BFpointer)malloc(sizeof(struct Bplus_File_Pointer));
			BpTree->File_P[i]->vaild = 0;//�ȶ���Ϊ��Ч
		}
		//����
		BpTree->K[0] = K;
		BpTree->File_P[0]->vaild = 1;
		BpTree->File_P[0]->lines = lines;
		//���һ���ڵ�ָ�� ���ڻ���NULL
		for (i = 0; i < 14; i++)
			BpTree->Node_P[i] = NULL;
		BpTree->Node_P[14] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));
		BpTree->Node_P[14]->next_node = NULL;
		return BpTree;
	}
	else
	{
		BpNode L;
		//��find��ʱ��Ӧ������һ������ ��¼parent�ڵ�
		/*          =====   ��¼parent�ڵ�   =====          */
		//����ָ�룬������������
		BpNode parent[6];//����6�Ϳ���Ӧ��20��������¼��
		/*================================================*/

		L = find(BpTree, type, K, parent);//�ҵ�KӦ�ò��ȥ���Ǹ�Ҷ�ӽڵ�
		//if (L == BpTree) printf("yeah\n");

		//L��û����
		if (L->File_P[13]->vaild == 0)
		{
			L = insert_in_leaf(L, type, K, lines);
			//printf("1\n");
		}
		else//L�Ѿ����� split it
		{
			//printf("2\n");
			BpNode LL;//L'
			LL = (BpNode)malloc(4096);//�½��Ľڵ�  ע��������޸�
			LL->flag = 1;
			LL->isroot = 0;
			for (i = 0; i < 14; i++)
			{
				LL->File_P[i] = (BFpointer)malloc(sizeof(struct Bplus_File_Pointer));
				LL->File_P[i]->vaild = 0;
			}

			//��ʱ���� ����L�����õ���Ϣ
			void* temp_K[15];
			int temp_lines[15];
			int middle = 16 / 2;//n/2=8
			
			for (i = 0; i < 14; i++)
			{
				temp_K[i] = L->K[i];//void*֮��ĸ�ֵ
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
			LL->Node_P[14] = (BNpointer)malloc(sizeof(struct Bplus_Node_Pointer));//���һ���ڵ�

			LL->Node_P[14]->next_node = L->Node_P[14]->next_node;
			L->Node_P[14]->next_node = LL;

			// ��L�к�벿�ֵ�ֵ��ȥ   ע��߽��ѡȡ////////////////////////////////////////////////////////
			for (i = 8; i < 14; i++)
				L->File_P[i]->vaild = 0;
			// �޸�L��ǰ�벿��
			for (i = 0; i <= 7; i++)
			{
				L->K[i] = temp_K[i];
				L->File_P[i]->lines = temp_lines[i];
				//L->File_P[i]->vaild = 1;
			}
			// �޸�LL��ǰ�벿��
			for (i = 0; i <= 6; i++)
			{
				LL->K[i] = temp_K[i + 8];
				LL->File_P[i]->lines = temp_lines[i + 8];
				LL->File_P[i]->vaild = 1;
			}
			// ѡ��LL����С��ֵ KK �ĵ�ַ
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
			//*((int*)L->K[0]) = *(int*)K;//void*�ĸ�ֵ
			L->K[0] = K;
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || *((int*)(L->K[i])) > *((int*)K)) break;

			if (L->File_P[i]->vaild == 0)//K���
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
			L->K[0] = K;//void*�ĸ�ֵ
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || *((float*)(L->K[i])) > *((float*)K)) break;

			if (L->File_P[i]->vaild == 0)//K���
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
			L->K[0] = K;//void*�ĸ�ֵ
			L->File_P[0]->vaild = 1;
			L->File_P[0]->lines = lines;
		}
		else
		{
			//Ki be the highest value in L that is less than K
			for (i = 0; i < 14; i++)
			if (L->File_P[i]->vaild == 0 || strcmp((char*)(L->K[i]), (char*)K) > 0) break;

			if (L->File_P[i]->vaild == 0)//K���
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
	//N �� root
	if (N->isroot == 1)
	{
		N->isroot = 0;
		BpNode R;
		R = (BpNode)malloc(4096);//�½�һ���ڵ� ��Ϊroot
		// containing N K NN
		R->flag = 0;
		R->isroot = 1;
		R->K[0] = K;
		for (i = 0; i < 14; i++)
			R->File_P[i] = NULL;//ָ���ļ��Ľڵ���Զ�����õ�
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
	//N ���� root
	BpNode P;
	for (i = 0; i + 1 < 6; i++)
	if (parent[i + 1] == N) break;
	P = parent[i];//P = parent[N]

	//P��û����/////////////////////////			�˴���BUG������
	if (P->Node_P[14]->vaild == 0)
	{
		//printf("hello1\n");
		// insert after N
		for (i = 0; i < 14; i++)
		if (P->Node_P[i]->next_node == N) break;//N��Ӧ������ֵi


		/**************************************/
		int j;
		for (j = i; j < 15; j++)
		{
			if (P->Node_P[j]->vaild == 0) break;
		}//j-1������1

		P->Node_P[j]->vaild = 1;

		int k;
		for ( k = j - 1; k>i; k--)//����û��ȥ������
		{
			//P->Node_P[k]->next_nodeָ��ǰ��Ľڵ�   ��
			P->Node_P[k+1]->next_node = P->Node_P[k]->next_node;
			P->K[k] = P->K[k - 1];
		}
		/**************************************/


		// �޸�i+1�Ľڵ�
		//i = i + 1;
		P->K[i] = K;
		P->Node_P[i + 1]->next_node = NN;
		P->Node_P[i + 1]->vaild = 1;
		///////////////////////////////////////////////////////////////////////////
		return root;
	}
	else//P���� split P
	{
		//printf("hello\n");
		//��ʱ���� ����P�����õ���Ϣ
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
		// ��N֮���ֵ��ָ������ƶ�
		
		for (j = 14; j>i; j--)
		{
			temp_pointer[j + 1] = temp_pointer[j];
			temp_K[j] = temp_K[j - 1];
		}
		temp_pointer[i + 1] = NN;
		temp_K[i] = K;

		// �����µķ�Ҷ�ӽڵ�
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

		// ��P�к�벿�ֲ�ȥ
		for (i = 8; i <= 14; i++)
			P->Node_P[i]->vaild = 0;
		// �޸�P��ǰ�벿��
		for (i = 0; i <= 6; i++)
		{
			P->K[i] = temp_K[i];
			P->Node_P[i]->next_node = temp_pointer[i];
			P->Node_P[i]->vaild = 1;
		}
		P->Node_P[7]->next_node = temp_pointer[7];
		P->Node_P[7]->vaild = 1;

		//K[7]���ӵ���
		// �޸�PP��ǰ�벿�� ��������һ��
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

	//size�Ǹ�ʲô��
	//��B+�� ���Ӧ���ǱȽ��Ӵ��
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
		BpTree = insert_into_index(BpTree, type, values->data, lines->data);/*��value�ĵ�ַ�Ž�ȥ*/
		values = values->next;
		lines = lines->next;
	}

	//��ô�۲����ɵ�B+��

	//����B+����Ϊ.bin�ļ�		B+���Ĵ洢��ʽ�����ڴ��кʹ����зֱ���ô�洢��
	//ȫ���洢��Ӳ����   ��������B+��
	//��α��� �����ļ�ͷ��Ϣ   queue���B+���ڵ�
	BpNode queue[3000];//100000������ Ӧ���㹻��
	int head = 0;
	int rear = -1;

	queue[++rear] = BpTree;//rear=0
	// root����Ҷ�ӽڵ� ����������
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
		while (i <= 14 && queue[head]->Node_P[i]->vaild == 1)//��Ч���ӽڵ�
		{
			//enqueue
			//��Ҷ�ӽڵ� & Ҷ�ӽڵ�
			queue[++rear] = queue[head]->Node_P[i]->next_node;

			queue[head]->offset[i] = rear - head;//�ӽڵ����Լ������block��
			i++;
		}
		//����Ч��offset��Ϊ-1
		int j;
		for (j = i; j <= 14; j++)
			queue[head]->offset[j] = -1;
		//dequeue
		head++;
	}

	//
	//("test_rear: %d\n", rear);
	//

	//���ﴦ���̫�ֲ��� Ҫ����д��
	//const char* filename = "index0.bin";
	//FILE* index_f;
	//index_f = fopen(filename, "wb");//д�������ļ�
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


	//�ڶ��β�α��� �����ļ�
	head = 0;
	rear = -1;
	queue[++rear] = BpTree;
	while (head <= rear)
	{
		int i = 0;
		if (queue[head]->flag == 1)
		{
			//ReWrite(filename, queue[head], 4096);
			//fwrite(queue[head], 4096, 1, index_f);//�������ļ���дһ��block

			/*******************************************************************************************/
			block newblock;//Ҫд���ļ���һ����
			newblock = (block)malloc(4096);
			//Ҷ�ӽڵ㲻�ù�offset
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
					strcpy(newblock->value_ch[k], (char*)(queue[head]->K[k]));//����
					newblock->file_vaild[k] = queue[head]->File_P[k]->vaild;
					newblock->lines[k] = queue[head]->File_P[k]->lines;
				}
			}
			else{
				printf("type error in BpBuild!\n");
			}
			//����node_vaild
			
			fwrite(newblock, 4096, 1, index_f);

			free(newblock);
			newblock = NULL;
			/*******************************************************************************************/
			head++;
			continue;
		}
		//ReWrite(filename, queue[head], 4096);
		//fwrite(queue[head], 4096, 1, index_f);//�������ļ���дһ��block
		/*******************************************************************************************/
		block newblock;//Ҫд���ļ���һ����
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
				strcpy(newblock->value_ch[k], (char*)(queue[head]->K[k]));//����
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
	// ����B+��
	BpDestroy(BpTree, 0, 0, 0);//�жδ����ˣ�����
}

void BpDestroy(BpNode BpTree, int IID, unsigned char type, unsigned char size)
{
	// ����Bplus��
	/* �ͷ��˽ڵ��е�ָ�룬�����ڵ� */
	BpNode queue[3000];//100000������ Ӧ���㹻��
	int head = 0;
	int rear = -1;

	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->lines);
	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->vaild);

	queue[++rear] = BpTree;//rear=0
	// root����Ҷ�ӽڵ� ����������
	if (queue[0]->flag == 1) {}
	// ���Ʋ�α����Ĵ���
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
		while (i <= 14 && queue[head]->Node_P[i]->vaild == 1)//��Ч���ӽڵ�
		{
			//enqueue
			queue[++rear] = queue[head]->Node_P[i]->next_node;
			i++;
		}
		//dequeue
		//free(queue[head]);//�ͷ��ڴ�
		//queue[head] = NULL;
		head++;
	}

	//
	//printf("rear: %d\n", rear);
	//

	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->lines);
	//printf("%d\n", BpTree->Node_P[1]->next_node->File_P[0]->vaild);

	//�Ӻ���ǰ����
	int i, j;
	for (i = rear; i >= 0; i--)
	{
		//�Ȼ����ڲ���ָ��
		if (queue[i]->flag == 1)//Ҷ�ӽڵ�
		{
			free(queue[i]->Node_P[14]);//Node_P[15]
			for (j = 0; j < 14; j++)
				free(queue[i]->File_P[j]);//////////////////
		}
		else//��Ҷ�ӽڵ�
		{
			for (j = 0; j < 15; j++)
				free(queue[i]->Node_P[j]);
		}
		//���������ڵ�
		free(queue[i]);
	}
}


//��IntList�в������� �嵽����ͷ
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




int BpEqual(int IID, unsigned char type, unsigned char size, void* value)//value ��Ϊ ElementType �� ������void*
{
	//��ȡIID��Ӧ�������ļ�
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");//���������ļ�
	if (fp == 0) exit(0);

	block buffer;
	buffer = (block)malloc(4096);
	if (fread(buffer, 4096, 1, fp) == 0){ return -1; }

	// ���ж�type�Ƿ���ȷ
	if (buffer->type != type){ printf("type error in BpEqual!\n"); return -1; }
	fread(buffer,4096,1,fp);

	//������block����Ҷ�ӽڵ�
	while (buffer->flag == 0)
	{
		int i;
		//��һ����value���λ��
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

		//����i�Ƕ��٣������������ϵ
		fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);//�ӵ�ǰλ�ö���һ��block
	}
	fclose(fp);
	//����Ҷ�ӽڵ�
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




//��index6.bin������Bplus���У���������ֵС��value�Ķ�Ӧ���кŴ�����������il�У�����il��
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
	//������һ��Ҷ�ӽڵ�
		fread(buffer,4096,1,fp);

	while (buffer->flag == 0)
	{
		fseek(fp, 4096 * (buffer->offset[0] - 1), SEEK_CUR);
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		fread(buffer, 4096, 1, fp);
	}
	// type���...

	il = NULL;
	while (1)
	{
		int i;
		if (type == 0){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//��ǰbuffer  Node   buffer->value[i] < value
				if (buffer->value_int[i] < *((int*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}else
		if (type == 1){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//��ǰbuffer  Node   buffer->value[i] < value
				if (buffer->value_float[i] < *((float*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}else
		if (type == 3){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//��ǰbuffer  Node   buffer->value[i] < value
				if (strcmp(buffer->value_ch[i], (char*)value) < 0) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else{ printf("type error in BpLess!\n"); return NULL; }
		
		//����һ��block ˳��
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
	//������һ��Ҷ�ӽڵ�
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
				//��ǰbuffer  Node   buffer->value[i] < value
				if (buffer->value_int[i] <= *((int*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else
		if (type == 1){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//��ǰbuffer  Node   buffer->value[i] < value
				if (buffer->value_float[i] <= *((float*)value)) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else
		if (type == 3){
			for (i = 0; i < 14 && buffer->file_vaild[i] == 1; i++)
			{
				//��ǰbuffer  Node   buffer->value[i] < value
				if (strcmp(buffer->value_ch[i], (char*)value) <= 0) il = insert_il(il, buffer->lines[i]);
				else { fclose(fp); return il; }
			}
		}
		else{ printf("type error in BpLess!\n"); return NULL; }

		//����һ��block ˳��
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
	//���ҵ��ܲ������һ���ڵ�
	BpNode buffer;
	buffer = (BpNode)malloc(4096);
	fread(buffer, 4096, 1, fp);
	while (buffer->flag == 0)//����BpEqual�еĴ���
	{
		int i;
		for (i = 0; i < 15 && buffer->Node_P[i + 1]->vaild == 1; i++)
		{
			if (buffer->K[i] > value) break;
		}//��һ����value���λ��
		if (i == 15)
			//value���
			fseek(fp, 4096 * (buffer->offset[15] - 1), SEEK_CUR);
		else
			fseek(fp, 4096 * (buffer->offset[i] - 1), SEEK_CUR);

		free(buffer);
		buffer = NULL;
		buffer = (BpNode)malloc(4096);
		fread(buffer, 4096, 1, fp);//�ӵ�ǰλ�ö���һ��block
	}

	//˳������ȡ
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
			//��ǰbuffer  Node
			il = insert_il(il, buffer->File_P[i]->lines);
		}
		//����һ��block ˳��
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

	//���ҵ��ܲ������һ���ڵ�
	block buffer;
	buffer = (block)malloc(4096);
	if (GetFileSize11(filename)==0){ return NULL; }
	fread(buffer,4096,1,fp);

	while (buffer->flag == 0)//����BpEqual�еĴ���
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
		fread(buffer, 4096, 1, fp);//�ӵ�ǰλ�ö���һ��block
	}

	//˳������ȡ
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
			//��ǰbuffer  Node
			il = insert_il(il, buffer->lines[i]);
		}
		//����һ��block ˳��
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

	//���ҵ��ܲ������һ���ڵ�
	block buffer;
	buffer = (block)malloc(4096);
	printf("IN bpm, filen = %s\n",filename);
//	if (fread(buffer, 4096, 1, fp) == 0){ return NULL; }
	if(GetFileSize11(filename) == 0 ){return 0;
	}
	fread(buffer,4096,1,fp);

	while (buffer->flag == 0)//����BpEqual�еĴ���
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
		fread(buffer, 4096, 1, fp);//�ӵ�ǰλ�ö���һ��block
	}


	//˳������ȡ
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
			//��ǰbuffer  Node
			il = insert_il(il, buffer->lines[i]);
		}
		//����һ��block ˳��
		free(buffer);
		buffer = NULL;
		buffer = (block)malloc(4096);
		if (fread(buffer, 4096, 1, fp) == 0) { fclose(fp); return il; }
	}


}


void BpInsert(int IID, unsigned char type, unsigned char size, int line, void* values)
{
	printf("Enter BPI with IID %d, type %d, size %d, line %d, values %d\n",IID,type,size,line,*(int*)values);
	// ��index6.bin��Ϊһ��Bplus��
	char* filename;
	filename = (char*)malloc(20);
	memcpy(filename, "index", 5);
	sprintf(filename + 5, "%d", IID);
	strcat(filename, ".bin");
	FILE* fp = fopen(filename, "rb");//ֻ��
	if (fp == 0) exit(0);
	// ������һ��Ҷ�ӽڵ�
	block buffer[1000];

	int counter = 0;
	buffer[0] = (block)malloc(4096);

//	printf("in bpi, %s %d\n",filename,GetFileSize11(filename));
	if (GetFileSize11(filename) == 0){
		printf("in BpI, 0 size: %s %d\n",filename,GetFileSize11(filename));
		//���ļ�
		IntList oldlines = NULL;
		PVoidList oldvalues = NULL;
		oldlines = insert_il(oldlines, line);
		oldvalues = insert_vl(oldvalues, values);
		BpBuild(IID, type, size, oldlines, oldvalues);
		free(buffer[0]);
		return;
	}
	fread(buffer[0],4096,1,fp);
	//������һ��Ҷ�ӽڵ�
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

	
	// ��value��lines������������
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


	// ����һ��<ָ�루�кţ�������ֵ>���ٴ��index6.bin
	oldvalues = insert_vl(oldvalues, values);
	oldlines = insert_il(oldlines, line);


	/*while (oldvalues){
		printf("res:   %d\n", *(int*)(oldvalues->value));
		oldvalues = oldvalues->Next;
	}*/
	//print(oldlines);

	// ��B+��
	// ע���ļ����ֵ��޸�
	BpBuild(IID, type, size, oldlines, oldvalues);	

	int i;
	for (i = 0; i <= counter; i++)
		free(buffer[i]);
		

}

// size
// type -- ElementType void*
// filename sprintf

// ������һ���ṹ�� voidList

