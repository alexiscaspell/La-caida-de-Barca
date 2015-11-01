/*
 * Tests.h
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */

#ifndef TESTS_H_
#define TESTS_H_
#include "funcionesMemoria.h"
#include <cspecs/cspec.h>
#include "stdbool.h"
#include <commons/string.h>

void correrTests();

tipoEstructuraMemoria* datosMemoria;

void setearMemoriaParTests(tipoEstructuraMemoria*);

void iniciarProceso(int cantPaginas,int pid);

void escribirPaginaHard(int nroPagina,int pid,char* pagina);

void leerPaginaHard(int nroPagina,int pid);


#endif /* TESTS_H_ */
