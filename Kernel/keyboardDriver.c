#include "./include/keyboardDriver.h"
void getRegs(long*);
int getcharacterC();

/*transform the data that was received by keyboard to ascii character*/
static const int keyTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0 , 0,		
	8, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, '+',		
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, '{', '|',		
	14, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ',	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		
	0, '9', '-', 0, '5', 0, '+', '1', 0, '3', 0, 0, 0, 0,			
	0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define DIM 50
#define REGS 15
int dim = 0 ; // slot donde debe guardarse la tecla recibida.
char buffer[DIM];
long regBuffer[REGS]; //buffer para guardar los valores de los registros

void loadRegs(long* auxBuffer){
	for(int i=0; i<REGS; i++) auxBuffer[i] = regBuffer[i];
}

char getCharFromKeyDown() {
    int character = getcharacterC(); // implementado en libasm 
	if( character < 79 && keyTable[character] )
        return keyTable[character]; 
	else 
		return 0; 
}

//agrega en la ultima posicion del buffer si es que es una letra.
void updateBuffer(){
	char character = getCharFromKeyDown();
	if(character == 9){
		getRegs(regBuffer); //Falta agregar el 9 a la matriz de teclado, este representa la tecla "tab orizontal"
		return;
		}
	if(dim == DIM ) //si se llena el buffer , pierdo la tecla.
		return;
	if(character == 0 )
		return;
	else 
		buffer[dim++]=character;
}

//devuelve el primer elemento y mueve todos una posicion a la izquierda. 
char getCharInt(){
	if(dim==0)
		return 0; //si no tengo nada en el buffer.
	char character = buffer[0];
	dim--;
	for(int i = 0 ; i < dim ; i++){
		buffer[i]=buffer[i+1];
	}
	buffer[dim+1]=0;//necesario para no recuperar una letra que no fue tecleada. 
	return character;
}


