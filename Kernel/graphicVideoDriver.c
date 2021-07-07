#include "./include/graphicVideoDriver.h"

// Fuente: https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
// Explicación: Este struct tiene información sobre el VBE mode activado. Se encuentra cargado en la dirección de memoria 0x5C0
// Se carga un puntero a esta zona de memoria y después podemos acceder a todas las características
struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

// Mode info es dicho puntero al struct de configuraciones
static const struct vbe_mode_info_structure * modeInfo = (struct vbe_mode_info_structure *) 0x5C00;


// La resolución de la pantalla está contenida en el struct, en width y height. 
// Para este proyecto en particular, es 1024*768
// Podemos acceder al pixel (i,j) 0<=i<=767 y 0<=j<=1023
// Devuelve la ddm del primero de los tres espacios reservados por cada pixel
static char * getPixelAddress( int i, int j ) {
	uint64_t aux = modeInfo->framebuffer;
    return ((char *)(aux)+3*(modeInfo->width*i+j)); 
}

// Devuelve la anchura en pixeles de la pantalla 
int videoWidth() {
	return modeInfo->width; 
}

// Devuelve el largo de la pantalla 
int videoHeight() {
	return modeInfo->height; 
}


// La mínima unidad gráfica es el píxel.  
// El color es un hexa de la forma 0xBBGGRR
// Por ejemplo, 0xFFFFFF es el blanco
void putPixel( uint16_t x, uint16_t y, int color) {
        char * pixel = getPixelAddress(y, x); 
        // Hace decalajes para acceder a las distintas componentes cromaticas
		pixel[0] = (char)((color >> 16) & 0xFF);    //Obtiene el azul
    	pixel[1] = (char)((color >> 8) & 0xFF); //Obtiene el verde
		pixel[2] = (char)(color & 0xFF);        //Obtiene el rojo
}

// Vierte el color del pixel (x1, y1) en (x2, y2)
void copyPixel( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
        char * pixelSource = getPixelAddress(y1, x1); 
		char * pixelTarget = getPixelAddress(y2, x2); 

        // Hace decalajes para acceder a las distintas componentes cromaticas
		pixelTarget[0] = pixelSource[0];    //Obtiene el azul
    	pixelTarget[1] = pixelSource[1]; //Obtiene el verde
		pixelTarget[2] = pixelSource[2];      //Obtiene el rojo
}


void emptyScreen() {
	char * pixel = getPixelAddress( 0, 0); 
	for ( int i=0; i<videoWidth()*videoHeight()*3; i++ ) 
		pixel[i] = 0; 
}

void emptyLine( int line, int from, int to) {
	char * pixel = getPixelAddress(line, 0); 
	for (int i=from; i<to;  i++) {
		pixel[3*i+0] = 0; 
		pixel[3*i+1] = 0;
		pixel[3*i+2] = 0;  
	}
}
