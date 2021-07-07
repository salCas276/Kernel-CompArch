#ifndef GRAPHICVIDEODRIVER_H
#define GRAPHICVIDEODRIVER_H
#include <stdint.h>
#include <stddef.h>

// Definimos algunos colores conocidos 
#define BLUE 0xFF0000
#define RED  0x0000FF
#define GREEN 0x00FF00
#define WHITE 0xFFFFFF
#define BLACK 0x000000

void putPixel( uint16_t x, uint16_t y, int color); 
int videoWidth();
int videoHeight();
void emptyScreen();
void emptyLine( int line, int from, int to);  
void copyPixel( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); 

#endif


