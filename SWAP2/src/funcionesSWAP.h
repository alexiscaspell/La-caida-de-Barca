/*
 * funcionesSWAP.h
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#ifndef SWAP2_SRC_FUNCIONESSWAP_H_
#define SWAP2_SRC_FUNCIONESSWAP_H_

#include <commons/string.h>
#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/error.h>
#include <string.h>
#include <commonsDeAsedio/mmap.h>
#include <commons/collections/list.h>

#include "estructurasSWAP.h"

#define PUERTO_ESCUCHA "PUERTO_ESCUCHA"
#define NOMBRE_SWAP "NOMBRE_SWAP"
#define CANTIDAD_PAGINAS "CANTIDAD_PAGINAS"
#define TAMANIO_PAGINA "TAMANIO_PAGINA"
#define RETARDO_COMPACTACION "RETARDO_COMPACTACION"

#define RUTA_DE_ARCHIVO_DE_CONFIGURACION "cfgSwap"


////////////////////FUNCIONES PARA EL MANEJO DE ARCHIVO DE CONFIGURACION///////////////////////

tipoConfigSWAP* crearConfigSWAP();

void destruirConfigSWAP (tipoConfigSWAP* estructuraDeConfiguracion);

tipoConfigSWAP* cargarArchivoDeConfiguracionDeSWAP(char* rutaDelArchivoDeConfiguracion);

tipoConfigSWAP cargarArchivoDeConfiguracionDeSWAPSinPuntero();


#endif /* SWAP2_SRC_FUNCIONESSWAP_H_ */
