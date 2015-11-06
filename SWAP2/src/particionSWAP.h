/*
 * particionSWAP.h
 *
 *  Created on: 14/9/2015
 *      Author: utnso
 */

#ifndef SWAP2_SRC_PARTICIONSWAP_H_
#define SWAP2_SRC_PARTICIONSWAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>

#define CARACTER_VACIO '\0'


//////////////FUNCIONES PARA EL ARCHIVO DE PARTICION///////////////////////

FILE* inicializarParticion(char* nombreDeParticion,int tamanioDePagina,int cantidadDePaginas);

void escribirBloqueMapeado(char* archivo,char* contenidoAEscribir,int numDeBloque, int tamanioDeBloque);

char* leerBloqueMapeado(char* archivo,int numDeBloque, int tamanioDeBloque);



#endif /* SWAP2_SRC_PARTICIONSWAP_H_ */
