/*
 ============================================================================
 Name        : MemoriaTuvieja.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <commonsDeAsedio/estructuras.h>
#include "funciones.h"


tipoInstruccion* generarInstruccion();
void imprimirRespuesta(tipoRespuesta* respuesta);

int main(void) {


	tipoConfigMemoria* cfg = cargarArchivoDeConfiguracionDeMemoria("cfgMemoria");
	tipoRespuesta* respuesta;
	tipoInstruccion* instruccion;

	int socketSWAP = crearSocket();
	conectarAServidor(socketSWAP,cfg->ipSWAP,cfg->puertoSWAP);


	while(1){

		instruccion = generarInstruccion();

		enviarInstruccion(socketSWAP,instruccion);

		respuesta = recibirRespuesta(socketSWAP);

		imprimirRespuesta(respuesta);

	}

	return EXIT_SUCCESS;
}


tipoInstruccion* generarInstruccion(){

	char ins;
	int pid, nroPag;
	char* texto = string_new();

	printf("Ingrese instruccion, pid, nroPag, texto:\n");
	scanf("%c %d %d %s",&ins,&pid,&nroPag,texto);
	fflush(stdin);

	return crearTipoInstruccion(pid,ins,nroPag,texto);
}

void imprimirRespuesta(tipoRespuesta* respuesta){
	printf("\nResultado: %c \nTexto: %s\n\n",respuesta->respuesta,respuesta->informacion);
}
