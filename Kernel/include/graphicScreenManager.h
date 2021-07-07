#ifndef GRAPHICSCREENMANAGER_H
#define GRAPHICSCREENMANAGER_H

#include "../include/graphicVideoDriver.h"
#include "../include/bitmapFonts.h"

// Notar como el contrato del screenManager nunca refiere explicitamente a "screen1" o "screen0". 
// Simplemente usa metodos ordinarios de escritura en pantalla e internamente decide dinamicamente donde debe escribir


//Limpia todas  las  pantallas 
void clearAllScreens();

//Cambia a la siguiente pantalla. devuelve el ID de pantalla activa tras el llamado a la funcion
int nextScreen();

//Imprime un caracter en la pantalla activa. 
void printChar( char c );

//Imprime un string en la pantalla activa.
void printString( char * str );

//Agranda la letra de la pantalla activa
void fontUp(); 

//Achica la letra de la pantalla activa
void fontDown(); 

//Setea el estilo de la pantalla activa 
void resetStyle(int fontColor, int backgroundColor); 

//Borra el ultimo caracter de la pantalla activa
void deleteChar(); 






#endif