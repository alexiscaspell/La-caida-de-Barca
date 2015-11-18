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



void imprimirRespuesta(tipoRespuesta* respuesta);
void imprimirInstruccion(tipoInstruccion* instruccion);
tipoInstruccion* iniciarHard();
tipoInstruccion* leerHard();
tipoInstruccion* escribirHard();
tipoInstruccion* finalizarHard();

int pidHard;
int paginasHard;
int paginaEscrituraLectura;


int main(void) {


	tipoConfigMemoria* cfg = cargarArchivoDeConfiguracionDeMemoria("cfgMemoria");
	tipoRespuesta* respuesta;
	tipoInstruccion* instruccion;

	int socketSWAP = crearSocket();
	conectarAServidor(socketSWAP,cfg->ipSWAP,cfg->puertoSWAP);

//	pidHard = 5;
//	paginasHard = 3;
//	instruccion = iniciarHard();
//
//	paginaEscrituraLectura = 1;
//	escribirHard();
//
//

	pidHard = 5;
	paginasHard = 10;
	paginaEscrituraLectura = 2;

	int i = 0;
	while(i<4){

		//instruccion = generarInstruccion();
		if (i==0) {
			instruccion = iniciarHard();
		}
		if(i==1){
			instruccion = escribirHard();
		}
		if(i==2){

			instruccion = leerHard();
		}
		if(i==3) {
			instruccion = finalizarHard();
		}

		imprimirInstruccion(instruccion);

		enviarInstruccion(socketSWAP,instruccion);

		printf("Instruccion enviada numero: %d\n",i);
		i++;
		sleep(2);

		respuesta = recibirRespuesta(socketSWAP);

		imprimirRespuesta(respuesta);

	}

	return EXIT_SUCCESS;
}

void imprimirInstruccion(tipoInstruccion* instruccion){
	printf("PID: %d  Instruccion: %c  Paginas: %d  Texto:  %s",instruccion->pid,instruccion->instruccion,instruccion->nroPagina,instruccion->texto);
}


void imprimirRespuesta(tipoRespuesta* respuesta){
	printf("Resultado: %c \nTexto: %s\n\n\n",respuesta->respuesta,respuesta->informacion);
}


tipoInstruccion* iniciarHard(){
	//pidHard++;
	//paginasHard++;
	return crearTipoInstruccion(pidHard,INICIAR,paginasHard,"");
}

tipoInstruccion* leerHard(){
	tipoInstruccion* instruccion = crearTipoInstruccion(pidHard,LEER,paginaEscrituraLectura,"");

	//pidHard++;
	return instruccion;
}

tipoInstruccion* escribirHard(){
	tipoInstruccion* instruccion = crearTipoInstruccion(pidHard,ESCRIBIR,paginaEscrituraLectura,"tuvieja");
	//pidHard++;

	return instruccion;
}


tipoInstruccion* finalizarHard(){
	return crearTipoInstruccion(pidHard,FINALIZAR,0,"");
}
