/*
 * estructurasSWAP.h
 *
 *  Created on: 12/9/2015
 *      Author: utnso
 */

#ifndef SWAP2_SRC_ESTRUCTURASSWAP_H_
#define SWAP2_SRC_ESTRUCTURASSWAP_H_



//------------CONFIGURACION-------------

typedef struct{
	int puertoDeEscucha;
	char* nombreDeSWAP;
	int cantidadDePaginas;
	int tamanioDePagina;
	int retardoDeCompactacion;
}tipoConfigSWAP;

//------------CONTROL DE HUECOS-------------

typedef struct{
	int pid;
	int base;
	int cantidadDePaginasQueOcupa;
}tipoHuecoUtilizado;

typedef struct{
	int base;
	int cantidadDePaginasQueOcupa;
}tipoHuecoLibre;

#endif /* SWAP2_SRC_ESTRUCTURASSWAP_H_ */
