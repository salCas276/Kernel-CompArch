#include "./include/screenManager.h"

#define WIDTH 80
#define HEIGHT 25
#define MAXSCREENS 8

static int qScreens = 1; 
static int getScreenWidth() ;
static void printCharAtScreen( int screenID, char c);
static void nextLineAtScreen( int screenID );
static void deleteChar(); 
static void scroll( int activeScreen); 

// Buffer de valor actual para cada pantalla. Si hay menos de cuatro pantallas activas, se usan los primeros espacios, pero el screenID se mantiene.
static int currentColumn[] = { 0, 0, 0, 0 }; 
static int currentRow[] = { 0, 0, 0, 0 }; 

// Que pantalla esta escrita ahora? 
static int activeScreen = 0; 

void clearAllScreens() { 
	for ( int i = 0; i < HEIGHT; i ++ )
		for ( int j = 0; j < WIDTH; j ++ )
			clearChar(i, j);
}

void clearScreen( int screenID ) {
	for (int i = 0; i < HEIGHT; i ++ ) 
		for ( int j = 0; j < getScreenWidth(); j ++ ) 
			clearChar( i, screenID*getScreenWidth() + j);  	
}

static int getScreenWidth() {
	return WIDTH / qScreens; 
}

void setManyScreens( int q, int * styles ) {
	// 1 2 o 4 pantallas
	if ( q <= 0 || ! (q % 80) || q > MAXSCREENS ) return; 
	qScreens = q; 
	for ( int k=0; k<q; k++)
		for ( int i=0; i<HEIGHT; i++ )
			for (int j=getScreenWidth()*k; j<getScreenWidth()*(1+k); j++) {
				setStyle(i, j, styles[k]);
				setChar(i, j, ' ');
			}
}


static void printCharAtScreen( int screenID, char c) {

	// Si es un '/n', salto de linea
	if ( c == '\n' ) {	
		nextLineAtScreen(screenID); 
		return; 
	}

	// Si es un backspace
	if(c == 8){
		deleteChar();
		return;
	}


	// Imprimimos el caracter en la pantalla que corresponde, donde marca el punterito
	setChar( currentRow[screenID], screenID*getScreenWidth()+currentColumn[screenID], c);
	
	// Actualizamos los valores de currentRow y currentColumn
	currentColumn[screenID] =  ( currentColumn[screenID] + 1 ) % getScreenWidth(); 
	// Si pegué la vuelta, tengo que actualizar la fila 
	if ( ! currentColumn[screenID] ) {
		if ( currentRow[screenID] < HEIGHT - 1 ) 
			currentRow[screenID] = currentRow[screenID] + 1; 
		else scroll(screenID); 
	}
	
}

static void scroll(int screen) {
	for (int i=0; i<HEIGHT-1; i++)
		for (int j=0; j<getScreenWidth(); j++) 
			setChar(i, getScreenWidth()*activeScreen + j, getChar(i+1, j+getScreenWidth()*activeScreen)); 
	
	for (int j=0; j<getScreenWidth(); j++) {
		setChar(HEIGHT-1,getScreenWidth()*activeScreen + j, ' '); 
	}
	
}


static void nextLineAtScreen( int screenID ) {
	currentColumn[screenID] = 0; 
	if ( currentRow[screenID] < HEIGHT - 1 ) 
		currentRow[screenID] = currentRow[screenID] + 1; 
	else scroll(screenID); 
}

void printChar( char c ) {
	printCharAtScreen(activeScreen, c); 
}

void printString( char * str ) {
	for (int i =0; str[i]; i++)
		printCharAtScreen(activeScreen, str[i]); 
}


void nextScreen() {
	activeScreen = ( activeScreen + 1 ) % qScreens; 
}

static void deleteChar() {
    if ( !currentColumn[activeScreen] && !currentRow[activeScreen] ) return;
	printCharAtScreen( activeScreen, ' '); 
    currentColumn[activeScreen] = currentColumn[activeScreen] - 2; 
    // Si me quede sin espacio para atras, vuelvo al final del renglon y cambio la fila 
    if (currentColumn[activeScreen] < 0) {
        currentColumn[activeScreen] = getScreenWidth()/qScreens-1; 
        currentRow[activeScreen] = currentRow[activeScreen]-1; 
    }
}

