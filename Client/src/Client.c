/*
 ============================================================================
 Name        : Client.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <string.h>
#include <commonsDeAsedio/estructuras.h>


#define puerto 7200
#define ip "127.0.0.1"


void imprimirPCB(tipoPCB pcb);
void probarPCB(int);
void imprimirInstruccion(tipoInstruccion);
void probarInstruccion(int);
void probarRespuesta(int);
void imprimirRespuesta(tipoRespuesta);

int main(void) {

	int socketCliente = crearSocket();

	conectarAServidor(socketCliente,ip,puerto);

	//probarPCB(socketCliente);

	//probarInstruccion(socketCliente);

	probarRespuesta(socketCliente);

	liberarSocket(socketCliente);

	return EXIT_SUCCESS;
}

void imprimirPCB(tipoPCB pcb){

	printf("PCB:\n");
	printf("PID: %d\n",pcb.pid);
	printf("IP: %d\n",pcb.insPointer);
	printf("STATE: %c\n",pcb.estado);
	printf("DIR: %s\n",pcb.ruta);

}

void probarPCB(int socketCliente){

	tipoPCB pcb;

	pcb.estado = 'b';

	pcb.insPointer = 1;

	pcb.pid = 8;

	pcb.ruta = "home";

	printf("ANTES: \n");

	imprimirPCB(pcb);

	enviarPCB(socketCliente,pcb);

	void* mensaje = serializarPCB(pcb);

	tipoPCB otroPCB;

	void* buffer = mensaje+sizeof(size_t);


	deserializarPCB(buffer,&otroPCB);

	printf("\n\nDESPUES: \n");

	imprimirPCB(otroPCB);

	free(mensaje);

}

void probarInstruccion(int socketCliente){

	tipoInstruccion instruccion;

	instruccion.pid = 2;

	instruccion.instruccion = 'b';

	instruccion.nroPagina = 19;

	instruccion.texto = "todo mal";

	printf("ANTES: \n");

	imprimirInstruccion(instruccion);

	enviarInstruccion(socketCliente,instruccion);

	void* mensaje = serializarInstruccion(instruccion);

	tipoInstruccion otraInstruccion;

	void* buffer = mensaje+sizeof(size_t);

	deserializarInstruccion(buffer,&otraInstruccion);

	printf("\n\nDESPUES: \n");

	imprimirInstruccion(otraInstruccion);

	free(mensaje);

}

void imprimirInstruccion(tipoInstruccion instruccion){

	printf("INSTRUCCION:\n");
	printf("PID: %d\n",instruccion.pid);
	printf("INST: %c\n",instruccion.instruccion);
	printf("PAG: %d\n",instruccion.nroPagina);
	printf("TEXT: %s\n",instruccion.texto);

}

void probarRespuesta(int socketCliente){

	tipoRespuesta respuesta;

	respuesta.respuesta = 'm';

	respuesta.informacion = "todo manqueado";

	printf("ANTES: \n");

	imprimirRespuesta(respuesta);

	enviarRespuesta(socketCliente,respuesta);

	void* mensaje = serializarRespuesta(respuesta);

	tipoRespuesta* otraRespuesta;

	void* buffer = mensaje+sizeof(size_t);

	deserializarRespuesta(buffer,otraRespuesta);

	printf("\n\nDESPUES: \n");

	imprimirRespuesta(*otraRespuesta);

	printf("ya imprimi respuesta");

	free(mensaje);

	printf("hice el free de respuesta\n");
}

void imprimirRespuesta(tipoRespuesta respuesta){

	printf("RESPUESTA:\n");
		printf("RESPONSE: %c\n",respuesta.respuesta);
		printf("INFO: %s\n",respuesta.informacion);
}

