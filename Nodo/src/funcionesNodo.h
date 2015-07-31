/*
 * funcionesNodo.h
 *
 *  Created on: 16/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESNODO_H_
#define FUNCIONESNODO_H_

#include "estructurasNodo.h"
#include <commons/config.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <string.h>
#include <commonsDeAsedio/mmap.h>

#define EXITO 0
#define FALLO -1

#define VEINTE_MIB 15

#define PUERTO_FS "PUERTO_FS"
#define IP_FS "IP_FS"
#define ESPACIO_DE_DATOS "ARCHIVO_BIN"
#define DIRECTORIO_TEMPORAL "DIR_TEMP"
#define ES_NODO_NUEVO "NODO_NUEVO"
#define IP_NODO "IP_NODO"
#define PUERTO_NODO "PUERTO_NODO"
#define TABLA_BLOQUES_NODO "TABLA_BLOQUES_NODO"


tipoConfigNodo* crearTipoConfigNodo();

void destruirTipoConfigNodo (tipoConfigNodo* cfg);

int cargarArchivoDeConfiguracionDelNodo(t_config* archivoDeConfiguracionDelNodo,tipoConfigNodo* cfg);

/**************INTERFAZ CON FILESYSTEM Y OTROS NODOS*******************/

void guardarBloque(int numeroBloqueFS,int numeroArchivoFS,char* datosAGuardar,t_list* tablaDeBloquesNodo,FILE* espacioDeDatos,FILE* archivoTabla);
char* obtenerBloque(int numeroBloqueFS,int numeroArchivoFS,t_list* tablaDeBloquesNodo,FILE* espacioDeDatos);
char* obtenerContenidoDeArchivoTemporalDelNodo(FILE* archivoTemporal);


/************TABLA DE BLOQUES************/

tipoBloqueNodo* crearBloqueNodo(int numBloqueFS, int archivo);
void destruirBloqueNodo(tipoBloqueNodo* bloqueNodo);
int indiceDeBloqueNodo(t_list* tablaDeBloques,int bloqueFS, int archivo);
void cargarTabla(t_list* tablaDeBloques,FILE* archivoTabla);

#endif /* FUNCIONESNODO_H_ */
