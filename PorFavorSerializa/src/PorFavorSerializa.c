/*
 ============================================================================
 Name        : PorFavorSerializa.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/estructuras.h>
#include <commonsDeAsedio/cliente-servidor.h>

#define ip "127.0.0.1"
#define puerto 9000

void imprimirPCB(tipoPCB pcb);
void probarPCB();
void probarEstructura();

int main(void) {

probarEstructura();

//probarPCB();

	return EXIT_SUCCESS;
}


void imprimirPCB(tipoPCB pcb){

	printf("\nESTADO: %c\n",pcb.estado);
	printf("IP: %d\n",pcb.insPointer);
	printf("PID: %d\n",pcb.pid);
	printf("RUTA: %s\n",pcb.ruta);
}



void probarEstructura(){

tipoEstructura estructura;

estructura.caracter = 'a';

estructura.entero = 20;

estructura.cadena = "Lpm funcionaaa!!";

size_t tamanioBloque;

imprimirEstructura(estructura);

void* buffer = serializarEstructura(estructura,&tamanioBloque);

tipoEstructura otraEstructura = deserializarEstructura(buffer,tamanioBloque);

imprimirEstructura(otraEstructura);

}

void probarPCB(){

	tipoPCB pcb;

	pcb.estado = 'a';

	pcb.insPointer = 20;

	pcb.pid = 10;

	pcb.ruta = "home/negro";

	imprimirPCB(pcb);

	size_t tamanioBloque;

	void* bloque = serializarPCB(pcb,&tamanioBloque);

	void* buffer = bloque+sizeof(size_t);

	tamanioBloque-=sizeof(size_t);

	tipoPCB otroPcb = deserializarPCB(tamanioBloque,buffer);

	imprimirPCB(otroPcb);


}
