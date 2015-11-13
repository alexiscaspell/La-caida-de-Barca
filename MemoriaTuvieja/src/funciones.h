/*
 * funciones.h
 *
 *  Created on: 7/11/2015
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_


#include <commons/string.h>
#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/error.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commonsDeAsedio/estructuras.h>
#include <commons/collections/list.h>
#include <commonsDeAsedio/select.h>


#define PUERTO_ESCUCHA "PUERTO_ESCUCHA"
#define IP_SWAP "IP_SWAP"
#define PUERTO_SWAP "PUERTO_SWAP"
#define MAXIMO_MARCOS_POR_PROCESO "MAXIMO_MARCOS_POR_PROCESO"
#define CANTIDAD_MARCOS "CANTIDAD_MARCOS"
#define TAMANIO_MARCO "TAMANIO_MARCO"
#define ENTRADAS_TLB "ENTRADAS_TLB"
#define TLB_HABILITADA "TLB_HABILITADA"
#define RETARDO_MEMORIA "RETARDO_MEMORIA"

/*******************ESTRUCTURAS*************************/

typedef struct{
	int puertoDeEscucha;
	char* ipSWAP;
	int puertoSWAP;
	int maximoDeMarcosPorProceso;
	int cantidadDeMarcos;
	int tamanioDeMarco;
	int entradasDeTLB;
	char* TLBHabilitada;
	int retardoDeMemoria;
}tipoConfigMemoria;


tipoConfigMemoria* cargarArchivoDeConfiguracionDeMemoria(char* rutaDelArchivoDeConfiguracion);

void destruirConfigMemoria(tipoConfigMemoria* estructuraDeConfiguracion);

tipoConfigMemoria* crearConfigMemoria();


#endif /* FUNCIONES_H_ */
