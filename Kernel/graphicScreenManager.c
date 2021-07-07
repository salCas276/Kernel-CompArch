#include "include/graphicScreenManager.h"

// static int bitWidth = 8; 
// static int bitHeight = 11; 

// La implementación no utiliza una matriz explicita, a diferencia del modo texto
// Como cada pantalla no necesariamente tiene los mismos tamaño de fuente, entonces debemos trabajarlos como 
// matrices diferentes. Sin embargo, como el tamaño de la matriz puede cambiar, entonces no me sirve asignar espacio en memoria, no es eficiente. 
// Maximizamos para obtener posiciones (i,j) de una matriz implicita, que no existe concretamente sino como resultado matematico
// El metodo recalculateCharDensity() debe ser invocado cada vez que se cambie el tamaño de fuente. El mismo recalcula los bounds de esta matriz virtual.  

// Fontsizes para cada pantalla 
static int fontsize[] = { 1, 1}; 

// Dado el tamaño de letra de cada pantalla, cuántos chars contendría una linea? 
static int maxCharsPerLine[] = { 0, 0}; 

// Dado el tamaño de letra de cada pantalla, cuántos chars contendría una columna? 
static int maxCharsPerColumn[] = { 0, 0};

// Punteros (i,j) para cada pantalla 
static int currentColumn[] = {0, 0}; 
static int currentRow[] = { 0, 0}; 

// Que pantalla está activa? La 0 o la 1? 
static int activeScreen = 0; 

// Que colores de fondo y fuente estamos usando para cada pantalla? 
static int bcolor[] = { 0x000000, 0x00FF00 }; 
static int fcolor[] = { 0xFFFFFF, 0xFF00FF};

static void clearScreen( int screen ); 
static void scroll(int screen); 

static void nextLineAtScreen( int screen ) {
    currentColumn[screen] = 0; 

    if ( currentRow[screen] == maxCharsPerColumn[screen] - 1) {
        scroll(screen); 
    } else {
        currentRow[screen] ++; 
    }
}

void recalculateCharDensity() {

	for (int i=0; i<2; i++) {
		maxCharsPerLine[i] = videoWidth()/(2*getBitmapWidth()*fontsize[i]); 
		maxCharsPerColumn[i] = videoHeight()/(getBitmapHeight()*fontsize[i]); 
	}
}

// Escribe un char en la posicion (i,j) de la pantalla elegida. Plotea pixeles de la esquina superior izquierda para abajo
static void printCharAt( int screen, int row, int column, int bcolor, int fcolor, char c ) {

	// Me muevo con step del tamaño requerido hasta la punta superior izquierda
	int xPosition = getBitmapWidth()*column*fontsize[screen]+ ( screen ? videoWidth()/2 : 0);
	int yPosition = getBitmapHeight()*row*fontsize[screen]; 

	int color = bcolor; 
	unsigned char * bitmap = getBitmapFor(c); 
	if ( bitmap == 0 ) return; 
	// Voy a iterar de a bits (considerando glifos descriptos de a bits)
	for (int i = 0; i<getBitmapHeight(); i++) 
		for ( int j=0; j<getBitmapWidth(); j++) {
			// Por cada bit del glifo, pinto casilleros cuadrados ( que no necesariamente son de un pixel )
			// Primero me guardo el color del bit segun el bitmap
			color = ( ( bitmap[j] >> i) & 1  ? fcolor : bcolor ); 
			// Ahora voy a recorrer un cuadrado, de nxn pixeles, que es representado por un bit en el bitmap
			for (int k1=0; k1<fontsize[screen]; k1++)
				for(int k2=0; k2<fontsize[screen]; k2 ++)
					putPixel(xPosition+j*fontsize[screen]+k1, yPosition+fontsize[screen]*i+k2, color); 
		}

}

static void printCharAtScreen( int screenID, char c) {
	//si es un bakspace
	if(c == 8){
		deleteChar();
		return;
	}



	// Si es un '/n', salto de linea
	if ( c == '\n' ) {	
		nextLineAtScreen(screenID); 
		return; 
	}

	// Imprimimos el caracter en la pantalla que corresponde, donde marca el punterito
	printCharAt(screenID, currentRow[screenID], currentColumn[screenID], bcolor[screenID], fcolor[screenID], c);
	
	// Actualizamos los valores de currentRow y currentColumn
	// Incremento en uno la columna. Si me pase, es circular y vuelve al principio
	currentColumn[screenID] =  ( currentColumn[screenID] + 1 ) % maxCharsPerLine[screenID]; 

	if ( ! currentColumn[screenID] ) {
		currentRow[screenID] =  ( currentRow[screenID] + 1) % maxCharsPerColumn[screenID]; 
		if ( ! currentRow[screenID] ) {
			currentRow[screenID] = maxCharsPerColumn[screenID]-1; 
			scroll(screenID); 
		}
	}	
}


static void scroll(int screen) {
	int offset = ( screen ? videoWidth()/2 : 0); 

	// Hacemos el scroll 
	for (int j=offset; j<videoWidth()/2+offset; j++)
		for (int i = 0; i< (maxCharsPerColumn[screen]-1)*getBitmapHeight()*fontsize[screen] ; i++ ) 
			copyPixel( j, i+(getBitmapHeight()*fontsize[screen]), j, i); 

	// Liberamos el ultimo renglon 
	for (int j=0; j<maxCharsPerLine[screen]; j++)
			printCharAt(screen, maxCharsPerColumn[screen]-1, j, bcolor[screen], fcolor[screen], ' ');
	
}

//limpia todas  las  pantallas 
void clearAllScreens() {
	recalculateCharDensity(); 
	for (int i=0; i<2; i++)
		clearScreen(i); 
}

//borra la i-esima pantalla 
static void clearScreen( int screen ) {
	currentRow[screen]=0;
	currentColumn[screen]=0;
	for (int i=0; i<videoHeight(); i++) {
		int j; 
		for (j= ( screen ? videoWidth()/2 : 0); j< videoWidth() - ( !screen ? videoWidth()/2 : 0); j++)
			putPixel(j, i, bcolor[screen]); 
	}
}

//imprime un caracter en la pantalla activa. 
void printChar( char c ) {
	printCharAtScreen(activeScreen, c); 
}

//cambia a la siguiente pantalla
int nextScreen() {
	activeScreen =  !activeScreen; 
	return activeScreen; 
}

//Imprime un string en la pantalla activa.
void printString( char * str ) {
	for (int i=0; str[i]; i++)
		printChar(str[i]); 
}


//Agranda la letra de la pantalla activa
void fontUp() {
	if (fontsize[activeScreen] < 5) {
		clearScreen(activeScreen); // vacia la pantalla, no es estetico dos tamaños de fuente en simultaneo
		fontsize[activeScreen] += 1;  // agranda la letra
		recalculateCharDensity();  // recalcula las dimensiones de cada pantalla en terminos de chars mostrables
	}
}

//Achica la letra de la pantalla activa
void fontDown() {
	if (fontsize[activeScreen] > 1) {
		clearScreen(activeScreen); // vacia la pantalla, no es estetico dos tamaños de fuente en simultaneo
		fontsize[activeScreen] -= 1; // achica la letra
		recalculateCharDensity(); // recalcula las dimensiones de cada pantalla en terminos de chars mostrables
	}
}

//Setea el estilo de la pantalla activa 
void resetStyle(int fontColor, int backgroundColor) {
	fcolor[activeScreen] = fontColor; 
	bcolor[activeScreen] = backgroundColor; 

	clearScreen(activeScreen); // no es estetico tener ambos colores, reinicio la pantalla
	currentColumn[activeScreen] = 0; 
	currentRow[activeScreen] = 0; 
}


void deleteChar() {
    if ( !currentColumn[activeScreen] && !currentRow[activeScreen] ) return;
    currentColumn[activeScreen] = currentColumn[activeScreen] - 1; 
    // Si me quede sin espacio para atras, vuelvo al final del renglon y cambio la fila 
    if (currentColumn[activeScreen] < 0) {
        currentColumn[activeScreen] = maxCharsPerLine[activeScreen]-1; 
        currentRow[activeScreen] = currentRow[activeScreen]-1; 
    }
    printCharAt( activeScreen, currentRow[activeScreen], currentColumn[activeScreen], bcolor[activeScreen], bcolor[activeScreen], ' '); 
}


