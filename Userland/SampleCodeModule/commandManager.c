#include "commandManager.h"
#include "include/api.h"
//#include "../../Kernel/asm/libasm.asm"


#define MAX_PARAMS 20
#define BUFFER_SPACE 256 //Toma 1 caracter menos, para poder poner el \n
#define NUMBER_REGS 16
#define NUMBER_MEM 5




static void echo();
static void help();
static void printTime();
static void printMem();
static void div(); 
 void printRegs();
static void intToStr(long long, char*);
static int strToInt(char*);
static int addToBuffer(char key);
static int strcmp(const char *str1, const char *str2);
static int bufferPartition();
static void errorCentral(int errorCode);
static int isDecimal(char*);
static int isHex(char*);
static long hexToInt(char*);
static int uintToBase(long long value, char * buffer, int base);
static int twosComplement(long long, char*);

typedef struct buffer {
	char storage[BUFFER_SPACE];
	int length;
} buffer;

static buffer buffers[] = { {.length=0 }, { .length=0 } };
static int bufferNumber;//numero del buffer actual. 

//static long long regs[15];

typedef struct command {
    char * commandName;//nombre del comando 
    int commandParanNum;//cantidad de parametros que recibe 
    void (*runnable)(void); //puntero a la funcion que ejecuta. 
} command; 

static char *param[MAX_PARAMS];
static int paramNumber; //Indica que cantidad de parametros se encontraron

static command commands[] = { {"echo", -1, echo } , {"time", 0, printTime} , {"help", 0, help}, {"inforeg", 0, regis}, {"printmem", 1, printMem}, {"div", 2, div}}; 
static int qCommands = 6; //Si se pone un numero mayor que la cantidad real al buscar un comando inexistente tirara seg fault

static char * errorMsg[5]={"\nCommand not found","\nInvalid number of parameters","\nInvalid argument","\nBuffer overflow", "Invalid Parameter"};

/*
|---------------------------------------------------------------------------|
|                          Funciones de la Shell                            |
|---------------------------------------------------------------------------|
*/


void initializeShells(){
  print("?>");
  changeScreen();
  print("?>");
  changeScreen();
  while(1){
    getCommand();
  }
}


//Se encarga de pedir caracteres hasta que encuentre un '\n'
//En tal caso llama a run para continuar con la ejecucion
void getCommand(){
  char key; 
   do {
	 key = getChar();
    int val = addToBuffer(key); 
   switch(val){
      case 1:
        putChar(key);
        break;
      case -2:
        errorCentral(3);
        break;
       case -1:
          print("\b \b");
          break;
      case 2:
        changeScreen();
        bufferNumber = (bufferNumber == 0 ? 1 : 0);
        break;
      case 3:
        if ( !bufferPartition() ){
            runCurrentCommand(); 
        }
        for(int i = 0 ; i < buffers[bufferNumber].length ; i++)
          buffers[bufferNumber].storage[i]=0;
        buffers[bufferNumber].length=0;
        putChar('\n');
        print("?>");
    }
  } while(key!='\n'); 
}


//Se encarga de que cada tecla sea agregada correctamente al teclado
//Esto puede llegar a implicar cierto comportamiento de cada tecla
static int addToBuffer(char key){
    if(key==14) return 2;//Se toco shift, se pasa de pantalla
    if(key=='\n'){
        buffers[bufferNumber].storage[buffers[bufferNumber].length++]=0; //No estoy seguro si hay que incrementar el largo
        return 3;//Se termino la linea, se analizara el commando
    }
  if(key == 8 && buffers[bufferNumber].length){
    buffers[bufferNumber].length--;
        return -1;//Se paso un baskspace, se eliminara el ultimo caracter del buffer
    }
    if(buffers[bufferNumber].length>=BUFFER_SPACE-1) {
    return -2;//Se esta excediendo el maximo del buffer, se tira error
  }
    if(32 <= key && 126 >= key){
        buffers[bufferNumber].storage[buffers[bufferNumber].length++] = key;
        return 1;//Se paso un simbolo, se agrega a buffer
    }
    return 0;//Se paso una tecla "muda"
}


//Esta funcion se encarga de dividir al buffer en null terminated string asignando un puntero a cada uno de estos
//Puede dividirlo en como maximo MAX_PARAMS strings 
static int bufferPartition(){
	paramNumber=0;
	for(int i=0; i < buffers[bufferNumber].length ; i++){
		if(buffers[bufferNumber].storage[i]==0 || buffers[bufferNumber].storage[i]==' ')
			buffers[bufferNumber].storage[i]=0;
		else if(i==0 || (i>0 && buffers[bufferNumber].storage[i]!=' ' && buffers[bufferNumber].storage[i-1]==0)){
			if(paramNumber == MAX_PARAMS) {
        errorCentral(1);
        return 1;
      }
			else 
        param[paramNumber++]=buffers[bufferNumber].storage + i;
		}
	}			
  return 0; 	
}


//Va a ser llamada por getCommand() una vez que este encontro un '\n'
//Va a sacar el comando y sus parametros a traves del array de parametros param
//El primer parametro siempre debera ser el comando y el resto sus parametros
void runCurrentCommand() {
	int index=-1;
	for(int i=0; i<qCommands; i++){
		if(strcmp(commands[i].commandName, param[0])){
      index=i; //Encuentra el comando en la tabla, guarda el indice
      break;
		}
	}
  if(index==-1){
    errorCentral(0);
    return; //Si el comando no existe tiro ese error
  }
  if(commands[index].commandParanNum!=-1 && commands[index].commandParanNum != paramNumber-1){
    errorCentral(1);
    return; //Si la cantidad de parametros es erronea tiro ese error
  }
  putChar('\n');
  commands[index].runnable();
}


//0:comando desconocido , terminal 
//1:cantidad de argumentos incorrecto , terminal
//2:algun argumento inesperado , terminal 
//3:buffer lleno , no terminal. 
//4:parametro invalido
static void errorCentral(int errorCode){
    print(errorMsg[errorCode]);
    if(errorCode!=3){
      buffers[bufferNumber].length=0;
    }
    else { //manejo del buffer lleno.
      putChar('\n');
      print("?>"); // paso a la siguiente linea 
      //imprimo el contenido del buffer.
      for(int i = 0 ; i < buffers[bufferNumber].length ; i++){
        putChar(buffers[bufferNumber].storage[i]);
      } 
    }
}

/*
|---------------------------------------------------------------------------|
|                                  Comandos                                 |
|---------------------------------------------------------------------------|
*/


static void echo() {
    for(int i=1; i<paramNumber;i++){ print(param[i]);
    putChar(' ');
    }
}

static void div() {
  if ( !isDecimal(param[1]) || !isDecimal(param[2])) {
    errorCentral(4); 
    return;
  }
  int i = strToInt(param[1]); 
  int j = strToInt(param[2]);
  char buffer [100] ;
  uintToBase(i/j, buffer, 10); 
  print(buffer); 
}

static void printTime(){
  char * days[]={"sun","mon","tues","wednes","thurs","friday","saturday"};
  char buffer[3];
  

  int hour = getHour();
  int day = getDay();
  
  if(hour >= 21 ){
    if(day==1)
      print(days[0]);
    else if(day==0 )
      print(days[6]);
    else 
      print(days[day-2]);
   }else{
     print(days[day-1]);
  }


  print(" : ");
  uintToBase(hour, buffer, 10);
  print(buffer);
  putChar(' ');
  putChar(':');
  intToStr(getMinute(), buffer);
  print(buffer);
  putChar(' ');
  putChar(':');
  intToStr(getSecond(), buffer);
  print(buffer);
  putChar(' ');
}

static void help(){
  for ( int i=0; i<qCommands; i++) {
    print(commands[i].commandName); 
    if ( i != qCommands -1 ) putChar('\n');
  }
}

static void printMem(){
	if(!isHex(param[1])){
			errorCentral(4);
			return;
	}
  long * dir = (long *)hexToInt(param[1]);
  int mem[NUMBER_MEM];
  readMem(mem, dir);
  char buffer[10];
  for(int i=0;i<NUMBER_MEM; i++){
    twosComplement(mem[i], buffer);
    print(buffer);
    if(i<NUMBER_MEM-1) putChar('\n');
  }
}

//Va a faltar rsp que se me escapo
  void printRegs(long long * regs){
   char * names[]={"r15","r14","r13","r12","r11","r10","r9","r8","rsi",
    "rdi","rbp","rdx","rcx","rbx","rax", "rsp"};  
  char buffer[65];
  for(int i=0; i<NUMBER_REGS; i++){
    twosComplement(regs[i], buffer);
    print(names[i]);
    print(" : ");
    print(buffer);
    if(i<NUMBER_REGS-1) putChar('\n');
  }
}

/*
|---------------------------------------------------------------------------|
|                          Funciones Auxiliares                             |
|---------------------------------------------------------------------------|
*/


//Compara strings caracter a caracter, si estos son iguales devuelve 1 y si no 0
static int strcmp( const char * str1, const char * str2 ) {
    while (*str1) {
        if (*str1 != *str2)  return 0;
        str1++;
        str2++;
    }
    return *str1 == *str2; 
}

static int uintToBase(long long value, char * buffer, int base)
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


static void intToStr(long long n, char* buffer){
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

//Se asume que el string ingresado representa un entero
static int strToInt(char* str){
  int n=0;
  int i=0, sign=1;
  if(str[i]=='-'){
    sign=-1;
    i++;
  }
  char c=str[i++];
  while(c!=0){
    n*=10;
    n+=c-'0';
    c=str[i++];
  }
  return sign*n;
}

long hexToInt(char* s){
  int i=0, n=0;
  char c=s[i++];
  while(c){
    n*=16;
    if('0' <= c && c <= '9'){
      n+=c-'0';
    }
    if('A' <= c && c <= 'F' ){
      n+=c-'A'+10;
    }
    c=s[i++];
  }
  return n;
}

static int isDecimal(char* str){

  for (int i=0; str[i]; i++)
   if (str[i]<'0' || str[i]>'9')
    return 0;
	return 1;
}

static int isHex(char* str){
	int i=0;
	while(str[i]){
		if( !('0' <= str[i] && str[i] <= '9') && !('a' <= str[i] && str[i] <= 'f'))
		       return 0;
		i++;
	}
	return 1;
}	

static int twosComplement(long long n, char* buffer){
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
