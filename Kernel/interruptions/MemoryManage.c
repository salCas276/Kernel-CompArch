#include <stdint.h>
#include "../include/memory.h"
#include "../include/screenManager.h"


void MemoryManage(int * reg,long * dir){
    getMem(reg,dir);
}


void nextScreenInt(){
    nextScreen();
}
