/*
 * Test.h
 *
 *  Created on: 6/11/2015
 *      Author: utnso
 */

#ifndef TEST_H_
#define TEST_H_

#include "estructurasSWAP.h"
#include "funcionesSWAP.h"
#include "huecosSWAP.h"
#include "instruccionesSWAP.h"
#include "particionSWAP.h"
#include <cspecs/cspec.h>
#include <commonsDeAsedio/estructuras.h>


void correrTests();

//funciones
void reservarEspacioParaUnProceso(t_list* listaDeHuecos,tipoConfigSWAP* cfg);
void reservarEspacioParaDosProcesos(t_list* listaDeHuecos,tipoConfigSWAP* cfg);
void liberarEspacioDeProceso(t_list* listaDeHuecos, int pid);


#endif /* TEST_H_ */
