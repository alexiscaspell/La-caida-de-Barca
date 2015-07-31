/*
 * estructurasNodo.h
 *
 *  Created on: 16/6/2015
 *      Author: utnso
 */

#ifndef ESTRUCTURASNODO_H_
#define ESTRUCTURASNODO_H_

typedef struct{
	//int numeroDeBloqueNodo;
	int numeroDeBloqueFS;
	int archivoDuenio;
}tipoBloqueNodo;

typedef struct{
	char* ipFS,
		* espacioDeDatos,
		* esNodoNuevo,
		* ipNodo,
		* directorioTemporal,
		* tablaDeBloquesNodo;
	int puertoFS,
		puertoNodo;
}tipoConfigNodo;



#endif /* ESTRUCTURASNODO_H_ */
