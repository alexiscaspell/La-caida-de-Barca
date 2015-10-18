/*
 ============================================================================
 Name        : Server.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commons/string.h>
#include <commonsDeAsedio/estructuras.h>
#include <commonsDeAsedio/select.h>
#include <commons/collections/list.h>


void imprimirPCB(tipoPCB);
void probarPCB(int socketCliente);
void imprimirInstruccion(tipoInstruccion);
void probarInstruccion(int);
void probarRespuesta(int);
void imprimirRespuesta(tipoRespuesta);
void probarSelect(int socketServidor);
void tratarPeticiones(t_list* lista);

int main(void) {

	int socketServidor = crearSocket();

	asociarAPuerto(socketServidor,7200);

	escucharConexiones(socketServidor,10);

	int socketCliente = crearSocketParaAceptarSolicitudes(socketServidor);

	//probarPCB(socketCliente);

	//probarInstruccion(socketCliente);

	probarRespuesta(socketCliente);

	//probarSelect(socketServidor);


	liberarSocket(socketCliente);

	liberarSocket(socketServidor);

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

	tipoPCB* pcbRecibido = recibirPCB(socketCliente);

	imprimirPCB(*pcbRecibido);
}

void probarInstruccion(int socketCliente){

	tipoInstruccion* instruccionRecibida = recibirInstruccion(socketCliente);

	imprimirInstruccion(*instruccionRecibida);

}

void imprimirInstruccion(tipoInstruccion instruccion){

	printf("INSTRUCCION:\n");
	printf("PID: %d\n",instruccion.pid);
	printf("INST: %c\n",instruccion.instruccion);
	printf("PAG: %d\n",instruccion.nroPagina);
	printf("TEXT: %s\n",instruccion.texto);

}

void probarRespuesta(int socketCliente){

	tipoRespuesta* respuestaRecibida;

	respuestaRecibida = recibirRespuesta(socketCliente);

	imprimirRespuesta(*respuestaRecibida);

}

void imprimirRespuesta(tipoRespuesta respuesta){

	printf("RESPUESTA:\n");
		printf("RESPONSE: %c\n",respuesta.respuesta);
		printf("INFO: %s\n",respuesta.informacion);
}

void probarSelect(int socketServidor){

	t_list* listaPrincipal = list_create();
	t_list* listaFiltrada = list_create();
	cargarEnLista(listaPrincipal,socketServidor);

	int nuevoCliente = crearSocketParaAceptarSolicitudes(socketServidor);

	cargarEnLista(listaPrincipal,nuevoCliente);

	bool hayNuevoCliente = false;

	while(1){

		hayNuevoCliente = filtrarListas(listaPrincipal,listaFiltrada);

		if(hayNuevoCliente){

			nuevoCliente = crearSocketParaAceptarSolicitudes(socketServidor);

			cargarEnLista(listaPrincipal,nuevoCliente);

			hayNuevoCliente = false;
		}

		if(!list_is_empty(listaFiltrada))
			tratarPeticiones(listaFiltrada);

	}

}

void tratarPeticiones(t_list* lista){

	int var;

	int* clienteActual;

	for (var = 0; var < list_size(lista); ++var) {

		clienteActual = list_get(lista,var);

		tipoPCB* pcbRecibido = recibirPCB(*clienteActual);

		imprimirPCB(*pcbRecibido);

	}

}
