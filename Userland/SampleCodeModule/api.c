#include <api.h>

char getChar(){
    char buffer = 0;
    while ( !buffer )
        read(1, &buffer);
    return buffer;
}

//Imprime un caracter individual en pantalla 
void putChar(char c){
    char buffer[2];
    buffer[0]=c;
    buffer[1]=0;
    write(1,buffer); //el uno no significa cantidad de caracteres 
}

//Imprime una cadena null terminated 
void print(char * str){
    int i = 0 ;
    while(str[i]){
        i++;
    }
    write(i,str);
}

void getRegs(long * buffer){
	readRegs(buffer);
}

int getHour(){
    return getFromRtc(0x04);
}

int getMinute(){
    return getFromRtc(0x02);
}

int getSecond(){
    return getFromRtc(0);
}

int getDay(){
    return getFromRtc(0x06);
}


void readMem(long * mem , long * dir){
    readMemAsm(mem,dir);
}



void deleteChar(){
    putChar(8);
}


void changeScreen(){
    changeScreenAsm();
}
