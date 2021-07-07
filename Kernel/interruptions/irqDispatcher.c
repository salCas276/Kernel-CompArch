#include <time.h>
#include <stdint.h>
#include "../include/lib.h"
#include "../include/keyboardDriver.h"

static void int_20(void);
static void int_21(void);
void (*func_type[])(void)={int_20,int_21};

//llama al codigo a ejecutar 
void irqDispatcher(uint64_t irq) {
	func_type[irq]();
}

void int_20(void) {
	timer_handler();
}


//interrupcion del teclado.
void int_21(void){
	updateBuffer();
}		









