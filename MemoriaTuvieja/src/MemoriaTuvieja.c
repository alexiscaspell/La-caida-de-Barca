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
tipoInstruccion* generarInstruccionHard();

int pidHard;
int paginasHard;


int main(void) {
	pidHard = 0;
	paginasHard = 0;

	tipoConfigMemoria* cfg = cargarArchivoDeConfiguracionDeMemoria("cfgMemoria");
	tipoRespuesta* respuesta;
	tipoInstruccion* instruccion;

	int socketSWAP = crearSocket();
	conectarAServidor(socketSWAP,cfg->ipSWAP,cfg->puertoSWAP);

	int i = 1;
	while(i<10){

		//instruccion = generarInstruccion();
		instruccion = generarInstruccionHard();

		enviarInstruccion(socketSWAP,instruccion);

		printf("Instruccion enviada numero: %d\n",i);
		i++;
		//sleep(5);

		respuesta = recibirRespuesta(socketSWAP);

		imprimirRespuesta(respuesta);

	}

	return EXIT_SUCCESS;
}


tipoInstruccion* generarInstruccion(){

	char ins;
	int pid, nroPag;
	char* texto = string_new();

	printf("Ingrese instruccion, pid, nroPag:\n");
	scanf("%c %d %d",&ins,&pid,&nroPag);
	fflush(stdin);

	return crearTipoInstruccion(pid,ins,nroPag,"asd");
}

void imprimirRespuesta(tipoRespuesta* respuesta){
	printf("\nResultado: %c \nTexto: %s\n\n",respuesta->respuesta,respuesta->informacion);
}


tipoInstruccion* generarInstruccionHard(){
	pidHard++;
	paginasHard++;
	return crearTipoInstruccion(pidHard,INICIAR,paginasHard,"asd");
}
