
#include <stdio.h>
#include <stdlib.h>

#include <commonsDeAsedio/estructuras.h>

/*****************FUNCIONES********************/
void imprimirPCB(tipoPCB pcb){
	printf("estado: %c\n", pcb.estado);
	printf("instruccion: %d\n", pcb.insPointer);
	printf("pid: %d\n", pcb.pid);
	printf("ruta: %s\n", pcb.ruta);
	printf("\n");
}
int main(void) {

	tipoPCB pcb;
		pcb.estado = 'm';
		pcb.insPointer = 123;
		pcb.pid = 100;
		pcb.ruta = "tuvieja";

	imprimirPCB(pcb);

	void* buffer = serializarPCB(pcb);

	tipoPCB pcbRecibido;
	deserializarPCB(buffer, &pcbRecibido);

	imprimirPCB(pcbRecibido);

	return EXIT_SUCCESS;
}
