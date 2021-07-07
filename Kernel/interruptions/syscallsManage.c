#include <stdint.h>
#include "../include/graphicScreenManager.h"
#include "../include/keyboardDriver.h"
#include "../include/clockDriver.h"
#define LENGTH 3

int writeSys(int fd ,  char * string ) ;
int readSys(int count , char * buffer  ); 
int getFromRtcSys(int selector,char *); 

int (*routines[LENGTH])(int, char*)={writeSys,readSys,getFromRtcSys};


// 0 -> write 
// 1 -> read 
// 2 -> getFromRtc
int SyscallsManage(int id ,int fd ,  char * string){
    return routines[id](fd,string);
}



int getFromRtcSys(int selector,char * aux){
    setRtcBinary();
    int retorno =  getFromRTC(selector);
    if(selector == 4){
	    if(retorno < 3) retorno = 21 + retorno;
	    else retorno -=3;
    }
    return retorno ; 
}


//imprime en la pantalla string 
int writeSys(int count ,  char * string ){ 
    for(int i = 0 ; i < count && string[i]; i++){
        printChar(string[i]);
    }
    return count; 
}

//toma del buffer del teclado count letras.
int readSys(int count ,char * buffer ){
    int i = 0 ; 
    char character ; 
    while ( i < count && ( character=getCharInt() ) != 0 ) {
        buffer[i++]=character;
    }
    buffer[i]=0;
    return i ; // si el buffer estaba vacio nunca entra al while entonces vale cero , sino es distinto de cero 
}
