#include "./include/videoDriver.h"
#include <stdint.h>
#include <stddef.h>

/*Ubicacion de la placa de video en el mapa I/o*/
static uint8_t * const video = (uint8_t*)0xB8000;

#define MATW 160 // Matrix Width
#define MATH 25  // Matrix Height

static int isValid(int i, int j) {
	 return ! ( i < 0 || i >= MATH || j < 0 || j>=MATW );  
}

void setStyle( int i, int j, char style) {
	if ( ! isValid(i, j) ) return;  	
	video[ i * (MATW) + 2*j + 1 ] = style;
} 

void setChar( int i, int j, char c) {
	if ( ! isValid(i, j) ) return;  
	video[ i * (MATW) + 2*j ] = c;
} 

char getChar( int i, int j) {
	if ( ! isValid(i, j) ) return;  
	return video[ i * (MATW) + 2*j ];
}

void clearChar( int i, int j) { 
    setChar(i, j, ' '); 
}