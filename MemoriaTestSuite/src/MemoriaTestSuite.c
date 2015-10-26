/*
 ============================================================================
 Name        : MemoriaTestSuite.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcionesMemoria.h"
#include "Tests.h"


int main(void) {

	tipoConfigMemoria* configuracion = cargarArchivoDeConfiguracionDeMemoria("cfgMemoria");

	bool memoriaActiva = true;

	tipoEstructuraMemoria* datosMemoria = malloc(sizeof(tipoEstructuraMemoria));

	datosMemoria->configuracion = configuracion;

	datosMemoria->memoriaActiva = &memoriaActiva;

	setearEstructuraMemoria(datosMemoria);

	correrTests();

	return EXIT_SUCCESS;
}
