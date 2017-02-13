#include "holes.h"


void placeProcessInfo(process * current, int * track, char ** memory){
    for (int i = 0; i < current->size; i++){
        (*memory)[(*track)+i] = current->name;
    }
    current->start = *track;
    *track = *track + current->size;
}

void removeProcessInfo(process * first, char ** memory){
    for (int i = 0; i < first->size; i++){
        (*memory)[(first->start)+i] = '.';
    }
    first->placedCount++;
}

void removeProcess(int pos, process ** wait, int total){
    for (int i = pos; i < total; i++){
        (*wait)[i] = (*wait)[i+1];
    }

}
void printMem(char * memory){
    for (int i = 0; i < 128; ++i){
        printf("%c", memory[i]); 
    }
}

int holes(char * memory){
    int total = 0;
    for (int i = 0; i < 127; i++){
        if (memory[i] != '.' && memory[i+1] == '.'){
            total++;
        }
    }
    return total;
}

double memUsage(char * memory){
    double total;
    for(int i = 0; i < 128; i++){
        if (memory[i] != '.'){
            total++;
        }
    }
    total = (total/128)*100;
    return total;
}
void nextFit(process * wait, int total, char * memory){
    int track = 0;
    int freeSpace = 0;
    int listPos = 0;
    int oldest = 0;
    int finished = total;
    int loads = 0;
    double useTotal = 0;
    int holesTotal = 0;
    double memCul = 0;
    int all = total;
    while(finished != 0){
        while(memory[track+freeSpace] == '.' && freeSpace < 128){
            freeSpace++;
        }
        if(freeSpace < wait[listPos].size){
            
            while(freeSpace < wait[listPos].size){
                if(track > wait[oldest].start && wait[oldest].start >= 0){
                    track = wait[oldest].start;
                }
                removeProcessInfo(&wait[oldest],&memory);
                if(wait[0].placedCount == 3){
                    removeProcess(0,&wait,total);
                    oldest--;
                    listPos--;
                    total--;
                } else {
                    finished++;
                }
                
                freeSpace = 0;
                while(memory[track+freeSpace] == '.' && freeSpace < 128){
                    freeSpace++;
                }
                if(freeSpace+track == 128){
                    track = 0;
                }
                oldest = (oldest + 1) % total;
            }
        }
        placeProcessInfo(&(wait[listPos]), &track, &memory);
        loads++;
        useTotal += memUsage(memory);
        holesTotal += holes(memory);
        memCul = useTotal/loads;
        printf("pid loaded, #process = %d, #holes = %d, %%memusage = %.0lf, cumulative %%mem = %.0lf\n", wait[listPos].pNum, holes(memory), memUsage(memory), memCul);
        freeSpace = 0;
        listPos++;
        listPos = listPos%total;
        finished--;
    }
    printf("Total loads = %d, average #processes = %.1lf, average #holes = %.1lf, cumulative %%mem = %.0lf\n", loads, (double) loads/all, (double) holesTotal/loads, memCul);
}

void findSmallestBlock(char * memory, int * temptrack, process * wait, int listPos){
    int startBlock = 0;
    int endBlock = 0;
    int tempSpace = 128;
    int freeSpace = 0;

    for(int i = 0; i < 127; i++){
        if((memory[i] == '.' && memory[i-1] != '.') || (i == 1 && memory[i] == '.')){
            startBlock = 1;
            endBlock = 0;      
        } 
        if(startBlock == 1 && endBlock == 0){
            freeSpace++;
        }
        if(memory[i] == '.' && (memory[i+1] != '.' || i == 126)){
            endBlock = 1;
            startBlock = 0;
            if(freeSpace < tempSpace && freeSpace >= wait[listPos].size){
                tempSpace = freeSpace;
                *temptrack = i - freeSpace + 1;
            }
            freeSpace = 0;
        }
    }
    if (startBlock == 1 && endBlock == 0){
        freeSpace = 1;
    }
    if(freeSpace < tempSpace && freeSpace >= wait[listPos].size){
        tempSpace = freeSpace;
        *temptrack = 127;
    }
}

void bestFit(process * wait, int total, char * memory){
    int track = 0;
    int listPos = 0;
    int oldest = 0;
    int finished = total;
    int loads = 0;
    double useTotal = 0;
    int holesTotal = 0;
    double memCul = 0;
    int all = total;

    while (finished != 0){
        findSmallestBlock(memory, &track, wait, listPos);
        while(track == -1){
            removeProcessInfo(&wait[oldest],&memory);
            findSmallestBlock(memory, &track, wait, listPos);
            if(wait[oldest].placedCount == 3){
                removeProcess(oldest,&wait,total);
                oldest--;
                listPos--;
                total--;
            } else {
                finished++;
            }
            oldest = (oldest + 1) % total;
        }
        placeProcessInfo(&(wait[listPos]), &track, &memory);
        loads++;
        useTotal += memUsage(memory);
        holesTotal += holes(memory);
        memCul = useTotal/loads;
        printf("pid loaded, #process = %d, #holes = %d, %%memusage = %.0lf, cumulative %%mem = %.0lf\n", wait[listPos].pNum, holes(memory), memUsage(memory), memCul);
        listPos++;
        listPos = listPos%total;
        finished--;
        track = -1;
    }
    printf("Total loads = %d, average #processes = %.1lf, average #holes = %.1lf, cumulative %%mem = %.0lf\n", loads, (double) loads/all, (double) holesTotal/loads, memCul);

}
void findBlock(char * memory, int * temptrack, process * wait, int listPos){
    int startBlock = 0;
    int endBlock = 0;
    int freeSpace = 0;

    for(int i = 0; i < 127; i++){
        if((memory[i] == '.' && memory[i-1] != '.') || (i == 1 && memory[i] == '.')){
            startBlock = 1;
            endBlock = 0;      
        } 
        if(startBlock == 1 && endBlock == 0){
            freeSpace++;
        }
        if(memory[i] == '.' && (memory[i+1] != '.' || i == 126)){
            endBlock = 1;
            startBlock = 0;
            if(freeSpace >= wait[listPos].size){
                *temptrack = i - freeSpace + 1;
                return;
            }
            freeSpace = 0;
        }
    }
    if (startBlock == 1 && endBlock == 0){
        freeSpace = 1;
    }
    if(freeSpace >= wait[listPos].size){
        *temptrack = 127;
    }
}
void firstFit(process * wait, int total, char * memory){
    int track = 0;
    int listPos = 0;
    int oldest = 0;
    int finished = total;
    int loads = 0;
    double useTotal = 0;
    int holesTotal = 0;
    double memCul = 0;
    int all = total;

    while (finished != 0){
        findBlock(memory, &track, wait, listPos);
        while(track == -1){
            removeProcessInfo(&wait[oldest],&memory);
            findBlock(memory, &track, wait, listPos);
            if(wait[oldest].placedCount == 3){
                removeProcess(oldest,&wait,total);
                oldest--;
                listPos--;
                total--;
            } else {
                finished++;
            }
            oldest = (oldest + 1) % total;
        }
        placeProcessInfo(&(wait[listPos]), &track, &memory);
        loads++;
        useTotal += memUsage(memory);
        holesTotal += holes(memory);
        memCul = useTotal/loads;
        printf("pid loaded, #process = %d, #holes = %d, %%memusage = %.0lf, cumulative %%mem = %.0lf\n", wait[listPos].pNum, holes(memory), memUsage(memory), memCul);
        listPos++;
        listPos = listPos%total;
        finished--;
        track = -1;
    }
    printf("Total loads = %d, average #processes = %.1lf, average #holes = %.1lf, cumulative %%mem = %.0lf\n", loads, (double) loads/all, (double) holesTotal/loads, memCul);
}

void findLargestBlock(char * memory, int * temptrack, process * wait, int listPos){
    int startBlock = 0;
    int endBlock = 0;
    int tempSpace = 0;
    int freeSpace = 0;

    for(int i = 0; i < 127; i++){
        if((memory[i] == '.' && memory[i-1] != '.') || (i == 1 && memory[i] == '.')){
            startBlock = 1;
            endBlock = 0;      
        } 
        if(startBlock == 1 && endBlock == 0){
            freeSpace++;
        }
        if(memory[i] == '.' && (memory[i+1] != '.' || i == 126)){
            endBlock = 1;
            startBlock = 0;
            if(freeSpace > tempSpace && freeSpace >= wait[listPos].size){
                tempSpace = freeSpace;
                *temptrack = i - freeSpace + 1;
            }
            freeSpace = 0;
        }
    }
    if (startBlock == 1 && endBlock == 0){
        freeSpace = 1;
    }
    if(freeSpace < tempSpace && freeSpace >= wait[listPos].size){
        tempSpace = freeSpace;
        *temptrack = 127;
    }
}

void worstFit(process * wait, int total, char * memory){
    int track = 0;
    int listPos = 0;
    int oldest = 0;
    int finished = total;
    int loads = 0;
    double useTotal = 0;
    int holesTotal = 0;
    double memCul = 0;
    int all = total;

    while (finished != 0){
        findLargestBlock(memory, &track, wait, listPos);
        while(track == -1){
            removeProcessInfo(&wait[oldest],&memory);
            findLargestBlock(memory, &track, wait, listPos);
            if(wait[oldest].placedCount == 3){
                removeProcess(oldest,&wait,total);
                oldest--;
                listPos--;
                total--;
            } else {
                finished++;
            }
            oldest = (oldest + 1) % total;
        }
        placeProcessInfo(&(wait[listPos]), &track, &memory);
        loads++;
        useTotal += memUsage(memory);
        holesTotal += holes(memory);
        memCul = useTotal/loads;
        printf("pid loaded, #process = %d, #holes = %d, %%memusage = %.0lf, cumulative %%mem = %.0lf\n", wait[listPos].pNum, holes(memory), memUsage(memory), memCul);
        listPos++;
        listPos = listPos%total;
        finished--;
        track = -1;
    }
    printf("Total loads = %d, average #processes = %.1lf, average #holes = %.1lf, cumulative %%mem = %.0lf\n", loads, (double) loads/all, (double) holesTotal/loads, memCul);

}