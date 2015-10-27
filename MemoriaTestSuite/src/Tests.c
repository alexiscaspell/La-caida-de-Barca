/*
 * Tests.c
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */
#include "Tests.h"

#define rutaConfig /*"/home/utnso/workspace/MemoriaTestSuite/Debug/cfgMemoria"//*/"cfgMemoria"

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

			it("Test crear espacio para pagina"){

	        	tipoInstruccion instruccionDeInicio;

	        	instruccionDeInicio.instruccion = INICIAR;

	        	instruccionDeInicio.nroPagina = 3;

	        	instruccionDeInicio.pid  = 10;

	        	instruccionDeInicio.texto = "";

	        	reservarMemoriaParaProceso(instruccionDeInicio);

	        	int dondeEstaTabla = buscarTabla(instruccionDeInicio.pid);

	        	tipoTablaPaginas* instanciaTabla = list_get(datosMemoria->listaTablaPaginas,dondeEstaTabla);

	        	bool condicionFinal = (instanciaTabla->paginasAsignadas==0)&&(instanciaTabla->paginasPedidas==instruccionDeInicio.nroPagina)&&(instanciaTabla->pid==instruccionDeInicio.pid)&&(list_size(instanciaTabla->frames)==0);

	        	should_bool(condicionFinal) be truthy;

	        }end

			it("Test escribir pagina inicial"){

	        	tipoInstruccion instruccionDeInicio;

	        	instruccionDeInicio.instruccion = INICIAR;

	        	instruccionDeInicio.nroPagina = 3;

	        	instruccionDeInicio.pid  = 10;

	        	instruccionDeInicio.texto = "";

	        	reservarMemoriaParaProceso(instruccionDeInicio);

	        	int dondeEstaTabla = buscarTabla(instruccionDeInicio.pid);

	        	tipoTablaPaginas* instanciaTabla = list_get(datosMemoria->listaTablaPaginas,dondeEstaTabla);

	        	bool condicionInicial = (list_size(datosMemoria->listaRAM)==0)&&(list_size(instanciaTabla->frames)==0);

	        	tipoInstruccion instruccion;

	        	instruccion.instruccion = ESCRIBIR;

	        	instruccion.nroPagina = 0;

	        	instruccion.pid  = 10;

	        	instruccion.texto = "Pagina de prueba hecha con negrada y finas hierbas";

	        	escribirPagina(instruccion);

	        	bool condicionFinal = (list_size(datosMemoria->listaRAM)==1)&&(list_size(instanciaTabla->frames)==1);

	        	should_bool(condicionInicial&&condicionFinal) be truthy;

	        }end

			it("Test lectura de unica pagina"){

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

	        	instruccion.instruccion = LEER;

	        	enviarPaginaPedidaACpu(instruccion,0);

	        	bool condicionFinal = (string_equals_ignore_case(datosMemoria->respuestaEnviadaACpu->informacion,instruccion.texto))&&(datosMemoria->respuestaEnviadaACpu->respuesta);

	        	should_bool(condicionFinal) be truthy;
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

	//limpiarTabla();//tira error

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

