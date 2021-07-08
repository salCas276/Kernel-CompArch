#include <stdint.h>
#include "../include/graphicScreenManager.h"
#include "../include/keyboardDriver.h"
#include "../include/clockDriver.h"
#define LENGTH 4

int writeSys(int fd ,  void * string ) ;
int readSys(int count , void * buffer  ); 
int getFromRtcSys(int selector,void *); 
int readRegsSys(int, void*);
void loadRegs(long*);

int (*routines[LENGTH])(int, void*)={writeSys,readSys,getFromRtcSys, readRegsSys};


// 0 -> write 
// 1 -> read 
// 2 -> getFromRtc
// 3 -> readRegs
int SyscallsManage(int id ,int fd ,  char * string){
    return routines[id](fd,string);
}



int getFromRtcSys(int selector,void * aux){
    setRtcBinary();
    int retorno =  getFromRTC(selector);
    if(selector == 4){
	    if(retorno < 3) retorno = 21 + retorno;
	    else retorno -=3;
    }
    return retorno ; 
}


//imprime en la pantalla string 
int writeSys(int count ,  void * auxString ){ 
	char* string = (char*) auxString; 
    for(int i = 0 ; i < count && string[i]; i++){
        printChar(string[i]);
    }
    return count; 
}

//toma del buffer del teclado count letras.
int readSys(int count ,void * auxBuffer ){
	char* buffer = (char*) auxBuffer;
    int i = 0 ; 
    char character ; 
    while ( i < count && ( character=getCharInt() ) != 0 ) {
        buffer[i++]=character;
    }
    buffer[i]=0;
    return i ; // si el buffer estaba vacio nunca entra al while entonces vale cero , sino es distinto de cero 
}

int readRegsSys(int counter, void* auxBuffer){
	long* buffer = (long*) auxBuffer;
	loadRegs(buffer);
	return 0;

}
