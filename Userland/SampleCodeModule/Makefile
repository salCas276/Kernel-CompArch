include ../Makefile.inc

## Loader tiene que ser tratado de forma particular, por esto se declara el nombre aca arriba y no se trabaja junto con los demas .c
LOADER=_loader
LOADER_OBJ=$(LOADER).o

MODULE_BIN=../0000-sampleCodeModule.bin
MODULE_ELF=../0000-sampleCodeModule.elf

## Path donde se van a buscar los include. 
INCLUDE=-I./include

## Sources va a buscar todos los archivos c para compilar. Los busca de forma recursiva en todas las carpetas. 
SOURCES=$(shell find -type f -name "*.c" ! -name "$(LOADER).c")

## Sources_asm va a buscar todos los archivos asm para compilar. Los busca de forma recursiva en todas las carpetas. 
SOURCES_ASM=$(shell find -type f -name "*.asm")

## Estas variables van a tener todos los nombres de los archivos objetos asociados a los .c y .asm. Se vana  compialr de forma automatica.
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)


##################################################################################################################
#Intentar no tocar abajo de esta linea
##################################################################################################################
CFLAGS += $(INCLUDE)
ALL_OBJECTS= $(OBJECTS) $(OBJECTS_ASM)

all: $(MODULE_BIN)

$(MODULE_BIN): $(MODULE_ELF)
	objcopy -O binary $< $@

$(MODULE_ELF): $(ALL_OBJECTS) $(LOADER_OBJ) 
	$(CC) $(CFLAGS) -T sampleCodeModule.ld -static -Wl,--oformat=elf64-x86-64,--build-id=none -o $@ $(ALL_OBJECTS)

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	$(RM) $(ALL_OBJECTS) $(LOADER_OBJ) $(MODULE_BIN) $(MODULE_ELF)

.PHONY: all clean
