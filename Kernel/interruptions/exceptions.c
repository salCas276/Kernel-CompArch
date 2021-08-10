
#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#include "../include/keyboardDriver.h"
void printRegister(long long * reg);
int twosComplement(long long, char*);
int uintToBase(long long, char*, int);

void zero_division();
void exceptionDispatcher(int exception);
void opcode();



void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
  else if (exception == OPCODE_EXCEPTION_ID)
    opcode();
}


void opcode(){
    printString("Invalid Opcode");
	  printChar('\n');
}

void zero_division() {
	printString("Div 0 ");
	printChar('\n');
}

void intToStr(long long n, char* buffer);

void printRegister(long long * regs){
   char * names[]={"r15","r14","r13","r12","r11","r10","r9","r8","rsi",
    "rdi","rbp","rdx","rcx","rbx","rax","rip"};
  char buffer[65];
  for(int i=0; i<16; i++){
    twosComplement(regs[i], buffer);
    printString(names[i]);
    printString(" : ");
    printString(buffer);
    printChar('\n');
  }
}

int twosComplement(long long n, char* buffer){
  int sgn = (0x80000000 & n) ? 1 : 0;
  if(sgn){
        int aux = (n & 0xf0000000) >> 28; //
        n = 0x7fffffff & ( 0x10000000 | n);
        uintToBase(n, buffer, 16);
        char s[2];
        uintToBase(aux, s, 16);
        buffer[0]=s[0];
  }
  else{
        uintToBase(n, buffer, 16);
  }
  return sgn;
}


int uintToBase(long long value, char * buffer, int base)
{
        char *p = buffer;
        int digits=0;
        char *p1, *p2;

        //Calculate characters for each digit
        do
        {
                int remainder = value % base;
                *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
                digits++;
        }
        while (value /= base);

        // Terminate string in buffer.
        *p = 0;

        //Reverse string in buffer.
        p1 = buffer;
        p2 = p - 1;
        while (p1 < p2)
        {
                char tmp = *p1;
                *p1 = *p2;
                *p2 = tmp;
                p1++;
                p2--;
        }

        return digits;
}


/*

void printRegister(long long * reg){
	char buffer[100];
	for(int i = 0 ; i < 14 ; i++){
		intToStr(reg[i],buffer);
		printString(buffer);
		printChar('\n');
	}
}




 void intToStr(long long n, char* buffer){
  //Primero invierto el numero y luego lo paso al buffer
  int inverted=0;
  int zeroCounter=n==0 ? 1 : 0;
  while(n!=0){
    inverted*=10;
    if(n%10==0 && inverted==0) zeroCounter++;
    inverted+=n%10;
    n/=10;
  }
  int i=0;
  while(inverted!=0){
    buffer[i++]=inverted%10+'0';
    inverted/=10;
  }
  zeroCounter=i+zeroCounter;
  while(i<zeroCounter) buffer[i++]='0';
  buffer[i]=0;
}
*/
