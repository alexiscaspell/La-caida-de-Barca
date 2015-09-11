/*
 ============================================================================
 Name        : Servidor.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commonsDeAsedio/select.h>
#include <string.h>
#include <commons/collections/list.h>

#define puerto 7200
//#define ip "127.0.0.1"

void responder(int socketCliente);

int main(void) {

	printf("Creando socket\n");

	int socket = crearSocket();

	printf("Asociando a puerto...\n");

	asociarAPuerto(socket,puerto);

	printf("Escuchando conexiones...\n");

	escucharConexiones(socket,2);

	int cliente1 = crearSocketParaAceptarSolicitudes(socket);

	printf("Cliente conectado...\n");

	int cliente2 = crearSocketParaAceptarSolicitudes(socket);

	printf("Cliente conectado...\n");

	t_list* listaClientes = list_create();

	agregarElementoALista(listaClientes,&socket);

	agregarElementoALista(listaClientes,&cliente1);

	agregarElementoALista(listaClientes,&cliente2);

	t_list* listaLectura = list_create();
	t_list* listaEscritura = list_create();
	t_list* listaExcepciones = list_create();

	printf("Filtrando listas...\n");

	filtrarListas(listaClientes,listaLectura,listaEscritura,listaExcepciones);

	printf("Listas filtradas\n");

	int var;
	int* socketActual;

	for (var = 0; var < list_size(listaClientes); ++var) {

		socketActual = list_get(listaClientes,var);

		if(perteneceALista(listaLectura,*socketActual))
			responder(*socketActual);

	}

	close(cliente1);

	close(cliente2);

	close(socket);

	return EXIT_SUCCESS;
}

void responder(int socketCliente){

	int puertoCliente;

	recibirMensaje(socketCliente,&puertoCliente,sizeof(int));

	printf("El puerto del cliente %d es %d\n",socketCliente,puertoCliente);

	char* puertoString;

	sprintf(puertoString,"%d",puertoCliente);

	char mensaje[30] = "Hola cliente de puerto ";

	string_append(&mensaje,puertoString);

	printf("Enviando mensaje %s...\n",mensaje);

	enviarMensaje(socketCliente,&mensaje,strlen(mensaje));

	printf("Mensaje enviado");

}
