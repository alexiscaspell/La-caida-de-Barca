/*
 * instruccionesSWAP.c
 *
 *  Created on: 2/10/2015
 *      Author: utnso
 */


#include "instruccionesSWAP.h"
#include "funcionesSWAP.h"

#include <commons/collections/list.h>


tipoRespuesta* ejecutarInstruccion(tipoInstruccion* instruccion,t_list* listaDeHuecosOcupados, t_list* listaDeHuecosLibres, tipoConfigSWAP* configuracion){

	tipoRespuesta* respuesta;

	switch (instruccion->instruccion) {
		case INICIAR:
			respuesta = reservarEspacio(listaDeHuecosOcupados,listaDeHuecosLibres,instruccion->pid,instruccion->nroPagina,configuracion->cantidadDePaginas,configuracion->tamanioDePagina,configuracion->nombreDeSWAP);
			break;

		case LEER:
			respuesta = leerPagina(listaDeHuecosOcupados,instruccion->pid,instruccion->nroPagina,configuracion->tamanioDePagina,configuracion->nombreDeSWAP);
			break;

		case ESCRIBIR:
			respuesta = escribirPagina(listaDeHuecosOcupados,instruccion->pid,instruccion->texto,instruccion->nroPagina,configuracion->tamanioDePagina,configuracion->nombreDeSWAP);
			break;

		case FINALIZAR:
			respuesta = liberarEspacio(listaDeHuecosOcupados,listaDeHuecosLibres,instruccion->pid);
			break;
	}

	return respuesta;

}
