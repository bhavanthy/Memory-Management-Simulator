#ifndef _BMODCHAL_holes_
#define _BMODCHAL_holes_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Process{
    char name;
    int pNum;
    int size;
    int start;
    int placedCount;
} process;

int holes(char * memory);
double memUsage(char * memory);
void placeProcessInfo(process * current, int * track, char ** memory);
void removeProcessInfo(process * first, char ** memory);
void removeProcess(int pos, process ** wait, int total);
void firstFit(process * wait, int total, char * memory);
void bestFit(process * wait, int total, char * memory);
void printMem(char * memory);
void findSmallestBlock(char * memory, int * temptrack, process * wait, int listPos);
void findBlock(char * memory, int * temptrack, process * wait, int listPos);
void nextFit(process * wait, int total, char * memory);
void findLargestBlock(char * memory, int * temptrack, process * wait, int listPos);
void worstFit(process * wait, int total, char * memory);
#endif
