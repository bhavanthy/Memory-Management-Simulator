#include "holes.h"
int main (int args, char ** argsv){
    FILE * file;
    char tempName;
    int tempSize;
    file = fopen(argsv[1],"r");
    int total = 0;
    int pNum = 1;
    char * memory;
    process * waitQueue;
    process * waitQueue1;
    process * waitQueue2;
    process * waitQueue3;

    memory = malloc(sizeof(char)*128);
    waitQueue = malloc(sizeof(process) * 128);
    waitQueue1 = malloc(sizeof(process) * 128);
    waitQueue2 = malloc(sizeof(process) * 128);
    waitQueue3 = malloc(sizeof(process) * 128);

    for (int i = 0; i < 128; i++){
        fscanf(file,"%c %d", &tempName, &tempSize);
        if (tempSize != 0 && tempName != '\n'){
            waitQueue[total].name = tempName;
            waitQueue[total].size = tempSize;
            waitQueue[total].placedCount = 0;
            waitQueue[total].start = -1;
            waitQueue[total].pNum = pNum;

            waitQueue1[total].name = tempName;
            waitQueue1[total].size = tempSize;
            waitQueue1[total].placedCount = 0;
            waitQueue1[total].start = -1;
            waitQueue1[total].pNum = pNum;

            waitQueue2[total].name = tempName;
            waitQueue2[total].size = tempSize;
            waitQueue2[total].placedCount = 0;
            waitQueue2[total].start = -1;
            waitQueue2[total].pNum = pNum;

            waitQueue3[total].name = tempName;
            waitQueue3[total].size = tempSize;
            waitQueue3[total].placedCount = 0;
            waitQueue3[total].start = -1;
            waitQueue3[total].pNum = pNum;

            total++;
            pNum++;
        }
    }
    for (int i = 0; i < 128; ++i){
        memory[i] = '.';
    }
    printf("First Fit:\n");
    firstFit(waitQueue, total, memory);

    for (int i = 0; i < 128; ++i){
        memory[i] = '.';
    }
    printf("Best Fit:\n");
    bestFit(waitQueue1, total, memory);

    for (int i = 0; i < 128; ++i){
        memory[i] = '.';
    }
    printf("Next Fit:\n");
    nextFit(waitQueue2, total, memory);
    for (int i = 0; i < 128; ++i){
        memory[i] = '.';
    }
    printf("Worst Fit:\n");
    worstFit(waitQueue3, total, memory);
    return 0;
}
