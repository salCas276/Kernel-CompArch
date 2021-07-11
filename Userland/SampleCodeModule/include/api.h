#include <kernelAccesor.h>

//toma un caracter individual del teclado. 0 si no tiene nada el buffer del teclado.  
char getChar();

//Imprime una cadena individual en pantalla 
void putChar(char c);

//Imprime una cadena null terminated 
void print(char * str);


int getHour();

int getMinute();

int getSecond();

int getDay();

void BackupReg(long long * reg);

void readMem(int * mem, long * dir);

void deleteChar();

void changeScreen();

void getRegs(long*);


