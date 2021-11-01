#include <stdio.h>
#include <stdlib.h>
#include "List.h"




ListPtr newList(bool (*dataEqual)(void *obj1, void*obj2), void (*dataPrinter)(void *data), void (*freeData)(void *data)){
	
//	allocate space for new list 
	ListPtr  newListPtr = (ListPtr)malloc(sizeof(ListObj)); 

//	set head to NULL, length to 0, and function pointers to corresponding parameters 
	newListPtr -> head = NULL; 
	newListPtr -> dataEqual = dataEqual;
	newListPtr -> dataPrinter = dataPrinter; 
	newListPtr -> freeData = freeData; 
	newListPtr -> length = 0; 
	return   newListPtr; 	
}

void freeList( ListPtr *pL, bool freeData){
	
//	get list and head 
	ListPtr lptr = *pL; 
	NodePtr nextNodePtr = lptr->head; 
	NodePtr cue = nextNodePtr; 

//	get free function from list 
	void (*freeDataFunc)(void * data) = lptr->freeData; 

//	go through list 		
	while (cue != NULL){
		cue = nextNodePtr->next;

//		if we want to free data free the data
		if (freeData == true){
			(*freeDataFunc)(nextNodePtr -> data);
			}  
//		free the node 
		free(nextNodePtr);
		nextNodePtr = cue;						
	}

//	free the list pointer 	
	free(*pL);
	*pL = NULL; 
	pL = NULL;  	
}

int length( ListPtr L ){

//	if NULL pointer or invalid length return error
	if (L == NULL){
		return -1; 

//	else return length 
	}else if (L->length >= 0){
		return L->length;
	}else{
		return -1; 
	}
}


int findElement( ListPtr L, void *comp ){

//	if length is 0 return error 
	if (length(L) == 0){
		return -1;
	}

//	else get head, set index and get dataEqual function 
	NodePtr curNode = L->head; 
	int index = 0; 
	bool (*dataEqualFunc)(void * obj1, void * obj2) = L->dataEqual; 

//	itterate through list 
	while(curNode != NULL) {
	
//		if data is found return index 
		if((*dataEqualFunc)(curNode->data, comp) == true){
			return index;
		}else{
			index++;
			curNode = curNode->next; 			
		}		
	}

//	if not found return error
	return -1; 	
}

void *getElement( ListPtr L, int i){

//	if invalid length or index return NULL
	if (length(L) == 0 || i > length(L) - 1 || i < 0) {
		return NULL; 

//	else iterate through list	
	}else{
		NodePtr curNode = L->head;
		int index = 0; 
		while(curNode != NULL) {
	
//			if index matches return data 
			if (index == i){
			return curNode->data;
					
			}else{
			index++;
			curNode = curNode->next; 			
			}		
		}	
	}

//	return NULL if index never matched (should never reach this point though)
	return NULL; 

}

void *delElement( ListPtr L, int i ) {

//	initialize return value as NULL
	void * ret = NULL;

//	if invalid length or index return NULL   
	if (length(L) == 0 || i > length(L) - 1 || i < 0) {
		return NULL; 
	}

// 	else if deleting head delete and update head and set return value 
	else if (i == 0){
		ret = L->head->data; 
		NodePtr keep = L->head->next; 
		free (L->head);
		L->head = keep; 
		L->length = length(L) -1; 
	}
	
//	else iterate through lsit 	
	else{
		NodePtr curNode = L->head;
		NodePtr nextNode = curNode->next;  
		int index = 1;
		while(nextNode != NULL){

//			if at index delete element, set node addresses properly, and store data in ret 
			if (index == i){
			ret = nextNode->data;
			curNode->next = nextNode->next; 
			free(nextNode);
			L->length = length(L) -1; 
			break;
			}else{
			curNode = nextNode;
			nextNode = nextNode->next; 
			index++;	
			}
		}
	}

//	return deleted data 
	return ret;
}


bool appendList( ListPtr L, void *data ){

//	if invalid list return error	
	if (L == NULL){
	return false; 
	}
	
//	else create new node 
	NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
	newNode->next = NULL;
	newNode->data = data;
	NodePtr nextNode = L->head; 

//	if list is empty set new node to head 
	if (L->head == NULL){
		L->head = newNode; 

//	else traverse to end of list 
	}else{
		while (nextNode->next != NULL){
			nextNode = nextNode->next; 	
		}
		
//		set new node at end of list 
		nextNode->next = newNode; 
	}

//	increment length and return true 
	L->length++; 
	return true; 
}
bool insertElement(ListPtr L, int pos, void *data){

//	create new node with data to be inserted 
	NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
	newNode->data = data; 

//  	if at head set head to new node 
	NodePtr shiftNode = L->head; 
	if (pos == 0){		
		L->head = newNode; 
		L->head->next = shiftNode; 
		L->length++; 
		return true;

//	else if at tail, simply append the list 
	}else if (pos == length(L)){	
		if (appendList(L, data) == true){
			free(newNode);
			newNode = NULL; 	
			return true; 
		}
	}
	
//	else traverse list to desired index 
	else{
		int index = 1;
		NodePtr prevNode = shiftNode; 
		shiftNode = shiftNode->next;
		
		while(shiftNode!=NULL){

//			if at desired index insert node 
			if (index == pos){
				prevNode->next = newNode; 
				newNode->next = shiftNode;
				L->length++; 	
			return true; 			
			}else{
				index++;
				prevNode = shiftNode; 
				shiftNode = shiftNode->next; 
			}	
		}	
	}
	free(newNode);
	newNode = NULL; 
	return false; 
}

void printList(ListPtr L){

//	get head of list and data printer function 
	void (*printFunc)(void* data) = L->dataPrinter; 
	NodePtr shiftNode = L->head; 

//	print every node data 
	while (shiftNode != NULL){
		(*printFunc)(shiftNode->data);
		printf("\n");
		shiftNode = shiftNode->next; 
	}
}


