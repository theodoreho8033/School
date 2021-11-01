UCSC CSE 101 Lab Fall 2021, Theodore Ho  

All .c files were entirely coded by Theodore Ho
All .h files and program specs were authored by UCSC CSE101 Fall 2021 Staff 

LINKED LIST AGENDA MANAGER

 Description: Manage up to two agendas implemented as a linked list. 
 
 List.c, List.h: Linked list library
 agendaManager.c: I/O, implements List.c/List.h to manage agendas
 
 Input: stdin string commands to maniuplate/create a list 
   
   Commands: 
          
          ADD "<AGENDA_NAME>" "<EVENT_NAME>" <START_TIME>
          Adds an event starting at the specified time to the agenda
          
          DEL "<AGENDA_NAME>" "<EVENT_NAME>"
          Deletes an event from the specified agenda

          SWAP "<AGENDA_NAME>" "<EVENT_NAME_1>" "<EVENT_NAME_2>"
          Swaps the start times of 2 events in an agenda

          PRINT "<AGENDA_NAME>"
          Prints the agenda to stdout

          MERGE "<AGENDA_NAME_1>" "<AGENDA_NAME2>"
          Combines the two agendas, storing the result at <AGENDA_NAME_1>
          
 Output: stdout prints and errors
