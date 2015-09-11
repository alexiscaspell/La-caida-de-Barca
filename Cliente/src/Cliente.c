/*
 ============================================================================
 Name        : Cliente.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/cliente-servidor.h>

#define ip "192.168.2.13"//"127.0.0.1"

int main(void) {

	int puerto;

	printf("Ingrese puerto (distinto de 7200): ");

	scanf("%d",&puerto);

	printf("Creando socket\n");

	int socket = crearSocket();

	printf("Asociando a puerto...\n");

	asociarAPuerto(socket,puerto);

	conectarAServidor(socket,ip,7200);

	printf("Conectado a servidor\n");

	int buffer = puerto;

	char respuesta[30];

	printf("Enviando mensaje...\n");

	enviarMensaje(socket,&buffer,sizeof(buffer));

	printf("Mensaje enviado");

	recibirMensaje(socket,&respuesta,strlen(respuesta));

	printf("\n\nRespuesta: %s",respuesta);

	close(socket);

	getchar();


	return EXIT_SUCCESS;
}
