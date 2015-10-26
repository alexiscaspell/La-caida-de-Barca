/*
 * Tests.c
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */
#include "Tests.h"

void setearMemoriaParTests(tipoEstructuraMemoria* datos){datosMemoria = datos;}
void inicializarTest();
void borrarRastrosDeTest();

void correrTests(){

	context (complete_example) {

	    describe("Tests de prueba") {


	        before {
	        	inicializarTest();

	        } end

	        after {
	        	borrarRastrosDeTest();

	        } end

			it("Test Funciona cspec"){

	        	should_bool(true) be truthy;

	        }end

	    }end

	}
	printf("\n\n");

}

void inicializarTest(){

	datosMemoria->listaAccesosAPaginasRAM = list_create();
	datosMemoria->listaRAM = list_create();
	datosMemoria->listaTablaPaginas = list_create();
	if(estaHabilitadaLaTLB())
	datosMemoria->listaTLB = list_create();
}

void borrarRastrosDeTest(){

	if(estaHabilitadaLaTLB())
			limpiarTLB();

	limpiarRam();

	limpiarTabla();

	limpiarListaAccesos();
}

