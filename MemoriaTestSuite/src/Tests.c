/*
 * Tests.c
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */
#include "Tests.h"

#define rutaConfig "/home/utnso/workspace/MemoriaTestSuite/Debug/cfgMemoria"//*/"cfgMemoria"

void setearMemoriaParTests(tipoEstructuraMemoria* datos){datosMemoria = datos;}
void inicializarTest();
void borrarRastrosDeTest();
void cargarCosas();

void correrTests(){

	context (complete_example) {

	    describe("Tests de prueba") {

	    	cargarCosas();

	        before {
	        	inicializarTest();

	        } end

	        after {
	        	borrarRastrosDeTest();

	        } end

			it("Test escribir pagina inicial"){

	        	tipoInstruccion instruccionDeInicio;

	        	instruccionDeInicio.instruccion = INICIAR;

	        	instruccionDeInicio.nroPagina = 3;

	        	instruccionDeInicio.pid  = 10;

	        	instruccionDeInicio.texto = "";

	        	reservarMemoriaParaProceso(instruccionDeInicio);

	        	tipoInstruccion instruccion;

	        	instruccion.instruccion = ESCRIBIR;

	        	instruccion.nroPagina = 0;

	        	instruccion.pid  = 10;

	        	instruccion.texto = "Pagina de prueba hecha con negrada y finas hierbas";

	        	escribirPagina(instruccion);

	        	int dondeEstaTabla = dondeEstaEnTabla(instruccion.nroPagina,instruccion.pid);

	        	tipoTablaPaginas* instanciaTabla = list_get(datosMemoria->listaTablaPaginas,dondeEstaTabla);

	        	should_bool((list_size(datosMemoria->listaRAM)==1)&&(list_size(instanciaTabla->frames)==1)) be truthy;

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

void funcionParaEscribir(){

}

void cargarCosas(){

	tipoConfigMemoria* configuracion = cargarArchivoDeConfiguracionDeMemoria(rutaConfig);

	bool memoriaActiva = true;

	tipoEstructuraMemoria* datosMemoria = malloc(sizeof(tipoEstructuraMemoria));

	datosMemoria->configuracion = configuracion;

	datosMemoria->memoriaActiva = &memoriaActiva;

	setearEstructuraMemoria(datosMemoria);

	setearMemoriaParTests(datosMemoria);

}

