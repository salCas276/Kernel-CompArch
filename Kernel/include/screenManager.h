#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "../include/videoDriver.h"

//limpia todas  las  pantallas 
void clearAllScreens();

//borra la i-esima pantalla 
void clearScreen( int screenID );

//setea la cantidad de pantallas que va a haber(divisor de 80)
//recibe un vector con los estilos de las pantallas 
void setManyScreens( int q, int * styles);

//imprime un caracter en la pantalla activa. 
void printChar( char c );

//cambia a la siguiente pantalla
void nextScreen(); 

//Imprime un string en la pantalla activa.
void printString( char * str ); 

#endif