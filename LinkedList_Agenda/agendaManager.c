#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
Theodore Ho (theho) 
CSE101 f21 

prog1: This is the application of List.c. Given an input of agenda objects and operations output the proper agenda. 
	Uses a linked list data structure defined in List.h to organize event objects (EventObj) into agendas. 
	Calls operations in defined in List.h to manipulate and print created agendas from a given input. 



*/

//The event object definition with pointer. 
typedef struct EventObj{
    char *name;
    int time;
} EventObj;
typedef EventObj* EventPtr;


//Error enums to make error handeling a bit cleaner in code. 
enum errors {NONE, AGENDA_NOT_FOUND, EVENT_NOT_FOUND, EVENT_SAME_TIME,
		 EVENT_SAME_NAME, MERGE_SAME, MERGE_CONFLICT};

/*
void printError(int error): prints given error code 
	inputs:
		int error: an error code specifying a type of error
	functionality:
		prints an error message based on the error code
		does not print if anything if error == NONE

*/
void printError(int error){
	if(error == AGENDA_NOT_FOUND){
		printf("ERROR: Agenda not found\n");
	}else if(error == EVENT_NOT_FOUND){
		
		printf("ERROR: Event not found\n");
	}else if(error ==  EVENT_SAME_TIME){
		
		printf("ERROR: Already an event at that time\n");
	}else if(error == EVENT_SAME_NAME){
		printf("ERROR: Already an event with that name\n");
	
	}else if(error == MERGE_SAME){
		printf("ERROR: Cannot merge same agenda\n");
	}else if (error == MERGE_CONFLICT){
		printf("ERROR: Cannot merge: Conflict found.\n");
	}	

}


//given print format function 
void print(ListPtr l, char *agenda_name) {
    printf("======== %s ========\n", agenda_name);
    printList(l); 

    printf("===== END OF %s =====\n", agenda_name);
}


/*
bool dataEQ(void * event1v, void * event2v): checks if events contain same variables 
	inputs:
		void * event1v, event2v: pointer to an EventObj. 
					  must be defined as void to match definition in List.h 
	return: true if AT LEAST ONE of the event variables matches
		false otherwise 
*/
bool dataEq(void * event1v, void * event2v){
	
// 	cast parameter as EventPtr
	EventPtr event1 = (EventPtr)event1v;
	EventPtr event2 = (EventPtr)event2v;

//	compare time and name variables 
	if (event1->time == event2->time 
		|| strcmp(event1->name, event2->name) == 0){	
			return true; 	
		}else{
		return false; 
		}
}
/*
void dataPrint(void * eventv): prints event 
	inputs:
		void * eventv: pointer to eventObj.
				must be defined as void to match definition in List.h
	return: void, prints event data 
*/

void dataPrint(void * eventv){

//	cast parameter as EventPtr
	EventPtr event = (EventPtr)eventv;

//	parse event variables 
	int time = event->time; 
	char timeStr[10];

//	format string and print	
	sprintf(timeStr, "%02d:00", time);
	printf("%s %s", timeStr, event->name);

//	free time str
	 
}

/*
void freeDt(void * eventv): frees an event 
	inputs: 
		void * eventv: pointer to eventObj
				must be defined as void to match definition in List.h
	return: null, frees event
*/

void freeDt(void * eventv){

//	cast parameter as EventPtr
	EventPtr event = (EventPtr)eventv;
	free(event->name);
	event->name = NULL;
//	free event
	free(event);
	event = NULL; 
}
/*
EventPtr newEventCreate(char* name, int time): creates a new event 
	inputs: 
		char * name: name variable in event
		int time: time variable in event
	return: EventPtr, creates a new event pointer with given variables
*/

EventPtr newEventCreate(char* name, int time){

//	allocate new event pointer 
	EventPtr new = (EventPtr)malloc(sizeof(EventObj));
	
//	set variables and return 
	new->name = name;
	new->time = time; 
	return new; 
}

/*
int checkName(char * event, ListPtr agendaList): checks if event with given name exists in agenda list 
	inputs: 
		char * event: name varible in particular EventPtr in agendaList
		ListPtr agendaList: pointer to agenda to search for event 
	return: int, index of event with char * event as name in agendaList
		-1 if not found
*/
int checkName(char * event, ListPtr agendaList){

//	create new event to compare with findElement
//	make time invalid so it only checks for name 
	EventPtr eventSearch = newEventCreate(event, -912); 
	
//	get index, free eventSearch and return
	int index = findElement(agendaList, eventSearch);
	free(eventSearch); 
	eventSearch = NULL; 
	return index;
}

/*
int checkTime(int time, ListPtr agendaList): checks if event with given time exists in agenda list
	inputs: 
		int time: time varible in particular EventPtr in agendaList
		ListPtr agendaList: pointer to agenda to search for event 
	return: int, index of event with int time as time in agendaList
		-1 if not found
*/
int checkTime(int time, ListPtr agendaList){

//	create new event to compare with findElement
//	make name invalid so it only checks for time 
	EventPtr eventSearch = newEventCreate("", time); 
	int index = findElement(agendaList, eventSearch);

//	get index, free eventSearch and return
	free(eventSearch); 
	eventSearch = NULL;
	return index;
}

/*
int delEvent(char * event, ListPtr agendaList): deletes event from agenda list
	inputs: 
		char * event: event name variable to identify event to be deleted 
		ListPtr agendaList: pointer to agenda list to delete event from 
	return: int, error code 
*/
int delEvent(char * event, ListPtr agendaList){

//	get index of event
	int index = checkName(event, agendaList);

// 	return error if not found 
	if (index == -1){
		return EVENT_NOT_FOUND;

// 	if found, delete element at index and return NONE error
	}else{
	EventPtr evDel = delElement(agendaList, index);
	freeDt(evDel);
	evDel = NULL; 
		return NONE;  
	}
}

/*
int addAgenda(char * event, int time, ListPtr agendaList): creates new event and adds to given agenda 
	inputs: 
		char * event: name variable of new event to be added
		int time: time variable of new event to be added
		List Ptr agendaList: agenda list to add new event
	return: int, error code	
*/

int addAgenda(char * event, int time, ListPtr agendaList){

//	create new event with given variables 
	EventPtr newEvent = newEventCreate(event, time); 

// 	if event doesnt already exist add to agenda list and return NONE error
	if (findElement(agendaList, newEvent) == -1){
		//printf("\nher");
		appendList(agendaList, newEvent); 
		return NONE; 
		
//	else determine if name or time match and return appropiate error
	}else if (checkName(event, agendaList) != -1){
		free(newEvent);
		newEvent = NULL;
	 	return EVENT_SAME_NAME;
	}else{
		free(newEvent);
		newEvent = NULL;
		return EVENT_SAME_TIME; 
	}
}

/*
int swapEvent(char * event1, char * event2, ListPtr agendaList): swaps two events indices on list 
	inputs: 
		char * event1: name variable of event to swap 
		char * event2: name variable of event to swap
		ListPtr agendaList: agenda list containing events to swap 
	return: int, error code 
*/

int swapEvent(char * event1, char * event2, ListPtr agendaList){

//	get indices of events with given name variables 
	int index1 = checkName(event1, agendaList);
	int index2 = checkName(event2, agendaList);

//	return error if invalid indices 	
	if (index1 == -1 || index2 == -1){
		return EVENT_NOT_FOUND; 

//	else delete and store events. index2-1 because length(agendaList) is -1 after first delete	
	}else{
	EventPtr eventSW1 = delElement(agendaList, index1);
	EventPtr eventSW2 = delElement(agendaList, index2-1);

//	determine which event should be added first and then insert events at their swappers index
	if (index1 > index2){
		bool ret1 = insertElement(agendaList, index2, eventSW1);
		bool ret2 = insertElement(agendaList, index1, eventSW2);		
	}else{		
		bool ret1 = insertElement(agendaList, index1, eventSW2);
		bool ret2 = insertElement(agendaList, index2, eventSW1);	
	}
	return NONE; 
	}

}

/*
int swapEventTime(char * event1, char*event2, ListPtr agendaList): swaps events times (not indices) from given agenda list 
	inputs: 
		char * event1: name variable of event to swap 
		char * event2: name variable of event to swap
		ListPtr agendaList: agenda list containing events to swap 
	return: int, error code 
*/

int swapEventTimes(char * event1, char * event2, ListPtr agendaList){

//	get indices of events with given name variables 
	int index1 = checkName(event1, agendaList);
	int index2 = checkName(event2, agendaList);
	
	//printf("\n event1: %i, event2:%i\n", index1, index2);
//	return error if invalid indices
	if (index1 == -1 || index2 == -1){
		return EVENT_NOT_FOUND; 	
	}else if(index1 == index2){
		return NONE; 
	}
	
//	else begin swapping times 

	else{

//		we might need to decremement index2 if we are deleting before it 	
		if (index1 < index2){
		index2 = index2 -1;
		}
	
//		first delete and store event from list 
		EventPtr eventSW1 = delElement(agendaList, index1);
		EventPtr eventSW2 = delElement(agendaList, index2);
		int time1 = eventSW1->time;
		int time2 = eventSW2->time;

//		swap time variables 
		eventSW1->time = time2; 
		eventSW2->time = time1;

//		we can just append because list will be sorted before printing 		
		appendList(agendaList, eventSW1);
		appendList(agendaList, eventSW2); 
		return NONE; 
	}
}

/*
void sortList(ListPtr agenda): performs a basic bubble sort on an agenda list based on time variable
	inputs:
		ListPtr agenda: list to be sorted 
	return: void, sorts list based on time varible in event 
*/

void sortList(ListPtr agenda){	

//	bubble sort alg 
	int size = length(agenda); 
	for (int index0 = 0; index0 < size - 1; ++index0){
		for(int index1 = 0; index1 < size - index0 - 1; ++index1){
		EventPtr event1= getElement(agenda, index1);
		EventPtr event2 = getElement(agenda, index1 +1); 
			if (event1->time > event2->time){
				int err = swapEvent(event1->name, event2->name, agenda); 
				}				
		}	 
	}	
}

/*
int mergeAgenda(ListPtr * ptrAgendaList1, ListPtr * prtAgendaList2): merges *ptrAgendaList2 into *ptrAgendaList1
	inputs: 
		ListPtr * ptrAgendaList1: list pointer to merge agenda to
		ListPtr * ptrAgendaList2: list pointer to merge agenda from 
	return: int, error code 
*/

int mergeAgenda(ListPtr  * ptrAgendaList1, ListPtr * ptrAgendaList2){

//	get ListPtrs to search 
	ListPtr agendaList2 = * ptrAgendaList2;
	ListPtr agendaList1 = * ptrAgendaList1;

// 	check if valid 
	if (agendaList2 == NULL || agendaList1 == NULL){
	printf("ag1: %p | ag2: %p", agendaList1, agendaList2);
		return AGENDA_NOT_FOUND; 
	}

//	we must first verify there are no merge conflicts before altering one of the lists 	
	int length2 = length(agendaList2) ; 
	int index = 0; 	
	while(index < length2  ){
		EventPtr event = getElement(agendaList2, index);

		if (findElement(agendaList1, event) != -1){			
			return MERGE_CONFLICT;
		}	
		index++;
	}
	
//	if no conflicts are detected begin merge 		
	while (length2  > 0){

//	for every element in the list to merge from, delete it and append it to the other list 
		EventPtr event = delElement(agendaList2, 0);
		length2 = length2 -1; 			
		appendList(agendaList1, event);			
	}

//	free the second list 
	freeList(ptrAgendaList2, false);	
	return NONE; 	
}


int main(int argc, char **argv) {

//	declare agenda lists 
	ListPtr agendaList1 = newList(&dataEq, &dataPrint, &freeDt);
	ListPtr agendaList2 = newList(&dataEq, &dataPrint, &freeDt);

//	keep track of agenda names 
	char * agendaName1 = (char*)malloc(sizeof(char)*100);
	char * agendaName2 = (char*)malloc(sizeof(char)*100); 
	strcpy(agendaName1, "");
	strcpy(agendaName2, "");
// 	keep track of error code
	int error = NONE; 

//	keep track if the requested agenda from input is a new agenda 
	bool newAgenda; 

//	keep track of amount of agendas
	int agendaCount = 0; 

//	parse vars
	char * p; 
	char * input = NULL; 
	size_t len =0;
	char * tk; 
	
	
	//getline(&input, &len, stdin) != -1
	while(getline(&input, &len, stdin) != -1){
		
//		var for function input type
		tk = strtok(input, " "); 
		const char * func = tk; 

//		var for agenda name in input 
		char agendaInName[100];
	
//		parameters of input (could be merge from agenda, event, time, ect.)
		const char * param1; 
		const char * param2;
		
//		copy agenda names of current agenda 
		char * cname1 = agendaName1; 
		char * cname2 = agendaName2; 

//		vars for current working agenda name and adress 
		char * curAgendaName; 
		ListPtr curAgendaList;

//		parse agenda input name 		
		tk = strtok(NULL, "\""); 
		strcpy(agendaInName,tk);

//		reset newAgenda bool and error code 	
		newAgenda = false; 
		error = NONE; 
		if (agendaCount > 2){
			agendaCount = 2;
		}
		
			
//		if the input agenda name matches one of the current agendas
//		set the currrent working agenda and agenda name accordingly 		
		if(strcmp(cname1, agendaInName) == 0){	
			curAgendaList = agendaList1; 
			curAgendaName = agendaName1;	
		}else if (strcmp(cname2, agendaInName) == 0){
			curAgendaList = agendaList2; 	
			curAgendaName = agendaName2;
		}else{
			
//			otherwise we are dealing with a new agenda so set
//			newAgenda bool to true and currrent working agenda name to input name 
			newAgenda = true; 	
			curAgendaName = agendaInName;
//			if we have space for new agenda increment agenda count
//			set current working agenda to the free agenda and set the agenda name 		
			agendaCount++;	
			if(agendaCount <= 2 && strcmp(func, "ADD") == 0){				
				if(strcmp(cname1, "") == 0){

				curAgendaList = agendaList1;
				strcpy(agendaName1, curAgendaName);
				}else{
				curAgendaList = agendaList2; 
				strcpy(agendaName2, curAgendaName);
				}
//			decrement if not an ADD because only ADD can add an agenda 
			}else if(strcmp(func, "ADD") != 0){
			agendaCount--;
			}		
		}
		
				
// 		if we are printing, we dont need to check for additional parameters so we do not need to 
//		continue to parse. so check for print first. 		
		if (strcmp(func, "PRINT") == 0){

//			we cannot print an agenda if its a new agenda	 	
			if (newAgenda == true){
				error = AGENDA_NOT_FOUND; 

//			otherwise sort and print the agenda 				
			}else{ 
				sortList(curAgendaList);	  
				print(curAgendaList, curAgendaName); 				
			}

//		if its not print, continue to parse the additional input parameters 		
		}else{		
			tk = strtok(NULL, "\"");
			tk = strtok(NULL, "\""); 
			param1 = tk; 	
			if (strcmp(func, "DEL") != 0 
				|| strcmp(func, "MERGE") != 0) {			
				if (strcmp(func, "ADD") != 0){
					tk = strtok(NULL, "\""); 
					tk = strtok(NULL, "\""); 	
				}else{
					tk = strtok(NULL, " ");
				}						
				param2 = tk; 							
			}
		}

//		if we are adding get the event and time from input 	
		if (strcmp(func, "ADD") == 0){
			
			int time = strtod(param2, &p); 
			char * event = strdup(param1); 

//		if its not a new agenda, or we have less than max agendas, add the agenda 
			if (newAgenda == false || agendaCount <= 2){
			error = addAgenda(event, time, curAgendaList); 
			

//		otherwise set the error
			}else{		
			error = AGENDA_NOT_FOUND; 
			}
			if (error != NONE){
				free(event);
				event = NULL; 
			}
			
		}
		
//		if we are deleting get the event name to be deleted 
		else if (strcmp(func,"DEL") == 0){
			char * event = strdup(param1); 

//			if its a new agenda we cannot delete so set error
			if (newAgenda == true){
				error = AGENDA_NOT_FOUND;

//			otherwise delete event from agenda 		
			}else{
				error = delEvent(event, curAgendaList);
			 			
			}
			free(event);
			event =  NULL;
		
//		if its swap get the event names from input		
		}else if (strcmp(func,"SWAP") == 0){
			char * event1 = strdup(param1); 
			char * event2 = strdup(param2); 

//			we cannot swap from a new agenda so print error if new agenda 
			if (newAgenda == true){
				error = AGENDA_NOT_FOUND; 

//			otherwise swap 
			}else{
				error = swapEventTimes(event1, event2, curAgendaList); 		
			}
			free(event1);
			free(event2);
			event1 = NULL;
			event2 = NULL;
		

//		code for merge input 		
		}else if (strcmp(func, "MERGE") == 0) {
				//printf("her");	
//			if first agenda is a new agenda, or second agenda is a new agenda, set error 			
			if(newAgenda == true || (strcmp(cname1, param1) != 0 && strcmp(cname2, param1) !=0)){
				error = AGENDA_NOT_FOUND;
				

//			else if they are the same agenda set error  
			}else if(strcmp(param1, agendaInName) == 0){
				error = MERGE_SAME;
					
			}

//			either merge agendaList2->agendaList1 or agendaList1->agendaList2
//			merge then reset agenda name and decrement agenda count 		
			else if(curAgendaList == agendaList1){
				error = mergeAgenda(&agendaList1, &agendaList2); 
					
			}else{
					
				error = mergeAgenda(&agendaList2, &agendaList1);	 
			}		
		}
		
//		if we deleted an agenda due to a merge, we must redeclare it
		if (agendaList2 == NULL){
			agendaList2 = newList(&dataEq, &dataPrint, &freeDt);
			strcpy(agendaName2,"");
			agendaCount--;
		}
		if (agendaList1 == NULL){
			agendaList1 = newList(&dataEq, &dataPrint, &freeDt); 
			strcpy(agendaName1,"");
			agendaCount--;	
		}
		
//		print error message	
		printError(error); 		
	}
	
//	release memory at the end 

	freeList(&agendaList1, true);
	freeList(&agendaList2, true);
	free(input);
	input = NULL; 
	free(agendaName1);
	agendaName1 = NULL;
	free(agendaName2);
	agendaName2 = NULL;
}
