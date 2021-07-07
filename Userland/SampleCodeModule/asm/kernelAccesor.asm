GLOBAL write 
GLOBAL read 
GLOBAL getFromRtc
GLOBAL regis
GLOBAL readMemAsm
GLOBAL changeScreenAsm 
    	
;;write con dos parametros : numero de salida , cadena a imprimir 
;;debe hacer la int 80 para ejecutar la interrupcion
write : 
	mov rdx , rsi 
	mov rsi ,rdi
	mov rdi , 0
	int 80h
	ret

;; read debe tener dos parametros: puntero al buffer donde se quiere copiar y cantidad de bytes a copiar.
;; read (int count , char * buffer )
read : 
	mov rdx , rsi 
	mov rsi , rdi 
	mov rdi , 1 
	int 80h 
	ret 
	
getFromRtc :
	mov rsi , rdi
	mov rdi , 2 
	int 80h
	ret



;;rdi puntero donde debo guardar , rsi direccion 
readMemAsm:
	int 79h
	ret 


changeScreenAsm : 
	int 78h
	ret
