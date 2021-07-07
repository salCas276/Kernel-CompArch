#include <stdint.h>
#include "../include/memory.h"
#include "../include/graphicScreenManager.h"


void MemoryManage(int * reg,long * dir){
    getMem(reg,dir);
}


void nextScreenInt(){
    nextScreen();
}
