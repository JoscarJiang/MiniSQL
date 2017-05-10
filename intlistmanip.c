#include "intlistmanip.h"
#include <stdlib.h>
#include <string.h>

IntList IncreaseIntList(IntList il){
	IntList head = il;
	if(!il)return 0;
	int count=1;
	while(head && (head = head->next))count++;

	while(count-1){
		IntList parent = il;
		IntList cmp = il;
		int j = count;
		while(--j){
			if(cmp->data > cmp->next->data){
				if(il != cmp){
					parent->next = cmp->next;
					cmp->next = cmp->next->next;
					parent->next->next = cmp;
					parent = parent->next;
				}else{
					il = il->next;
					parent = il;
					cmp->next = il->next;
					il->next = cmp;
				}
			}else 
			if(cmp->data <= cmp->next->data){
				parent = cmp;
				cmp = cmp->next;
			}

		}
		count--;
	}
	return il;
}

IntList DecreaseIntList(IntList il){
	IntList head = il;
	if(!il)return 0;
	int count=1;
	while(head && (head = head->next))count++;

	while(count-1){
		IntList parent = il;
		IntList cmp = il;
		int j = count;
		while(--j){
			if(cmp->data < cmp->next->data){
				if(il != cmp){
					parent->next = cmp->next;
					cmp->next = cmp->next->next;
					parent->next->next = cmp;
					parent = parent->next;
				}else{
					il = il->next;
					parent = il;
					cmp->next = il->next;
					il->next = cmp;
				}
			}else 
			if(cmp->data >= cmp->next->data){
				parent = cmp;
				cmp = cmp->next;
			}

		}
		count--;
	}
//	PPIL(il);
	return il;
}

IntList IntListInsert(IntList il, int x){
	IntList node = (IntList) malloc(sizeof(struct intList));
	node->data = x;
	node->next = 0;
	if(!il){
		il = (IntList) malloc(sizeof(struct intList));
		il->data = x;
		il->next = 0;
		free(node);
		return il;
	}
	node->next = il;
	il = node;
	return il;
}

IntList IntListDestroy(IntList il){
	IntList killer;
	while(il){
		killer = il;
		il = il->next;
		free(killer);
	}
	return il;
}

IntList IntListDeleteNode(IntList die, IntList il){
	if(die == il){
		il = il->next;
		free(die);
		return il;
	}else if(die->next == 0){
		IntList newtail = il;
		while(newtail->next->next){
			newtail = newtail->next;
		}
		newtail->next = 0;
		free(die);
		return il;
	}else{
		IntList killer = die->next;
		die->data = die->next->data;
		die->next = killer->next;
		free(killer);
		return il;
	}
}

PVoidList PVoidListDestroy(PVoidList pvl){
	PVoidList killer;
	while(pvl){
		killer = pvl;
		pvl = pvl->next;
		if(killer->data)free(killer->data);
		free(killer);
	}
	return pvl;
}

PVoidList PVoidListInsert(char* src, int size, PVoidList pvl, char end0){
if(!end0){
	PVoidList node = (PVoidList) malloc(sizeof(struct pVoidList));
	node->data = malloc(size);
	node->next = 0;
	memcpy(node->data,src,size);
	if(!pvl) return node;
	else{
		node->next = pvl;
		pvl = node;
		return node;
	}	
}else{
	PVoidList node = (PVoidList) malloc(sizeof(struct pVoidList));
	node->data = malloc(size + 1);
	node->next = 0;
	memcpy(node->data,src,size);
	((char*)(node->data))[size] = 0;
	if(!pvl) return node;
	else{
		node->next = pvl;
		pvl = node;
		return node;
	}
}
}

