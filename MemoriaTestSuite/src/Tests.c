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
	        	printf("Test crear espacio para pagina\n");

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
	        	printf("Test escribir pagina inicial\n");

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
	        	printf("Test lectura de unica pagina\n");

	        	tipoInstruccion instruccionDeInicio;

	        	instruccionDeInicio.instruccion = INICIAR;

	        	instruccionDeInicio.nroPagina = 3;

	        	instruccionDeInicio.pid  = 10;

	        	instruccionDeInicio.texto = "";

	        	reservarMemoriaParaProceso(instruccionDeInicio);

	        	tipoInstruccion instruccionEscritura;

	        	instruccionEscritura.instruccion = ESCRIBIR;

	        	instruccionEscritura.nroPagina = 0;

	        	instruccionEscritura.pid  = 10;

	        	instruccionEscritura.texto = "Pagina de prueba hecha con negrada y finas hierbas";

	        	escribirPagina(instruccionEscritura);

	        	printf("Ahora hay que leer lap pagina...\n");

	        	tipoInstruccion instruccionDeLectura;

	        	instruccionDeLectura.instruccion=LEER;

	        	instruccionDeLectura.nroPagina = 0;

	        	instruccionDeLectura.pid = 10;

	        	instruccionDeLectura.texto="";

	        	enviarPaginaPedidaACpu(instruccionDeLectura,0);

	        	bool condicionFinal = (string_equals_ignore_case(datosMemoria->respuestaEnviadaACpu->informacion,instruccionEscritura.texto))&&(datosMemoria->respuestaEnviadaACpu->respuesta);

	        	should_bool(condicionFinal) be truthy;
	        }end


			it("Test modificar pagina"){

			printf("Test modificar pagina\n");

				//inicio pagina

				tipoInstruccion instruccionIniciar;
					instruccionIniciar.instruccion = INICIAR;
					instruccionIniciar.nroPagina = 3;
					instruccionIniciar.pid  = 1;
					instruccionIniciar.texto = "";

				reservarMemoriaParaProceso(instruccionIniciar);

				int posicionEnTablaPaginas = buscarTabla(instruccionIniciar.pid);
				tipoTablaPaginas* instruccionEnTablaPaginas = list_get(datosMemoria->listaTablaPaginas, posicionEnTablaPaginas);

				bool creePagina = (posicionEnTablaPaginas != -1);

				//escribo la pagina por primera vez

				tipoInstruccion instruccionEcribir;
					instruccionEcribir.instruccion = ESCRIBIR;
					instruccionEcribir.nroPagina = 1;
					instruccionEcribir.pid  = 1;
					instruccionEcribir.texto = "pagina que NO tiene finas hiervas";

				escribirPagina(instruccionEcribir);

				printf("Pagina antes:\n%s\n",list_get(datosMemoria->listaRAM,0));


				tipoPagina* paginaAModificar = list_get(instruccionEnTablaPaginas->frames , 0);
				char* textoDePaginaAntes = list_get(datosMemoria->listaRAM, paginaAModificar->posicionEnRAM);

				bool escribiPagina = ( list_size(instruccionEnTablaPaginas->frames) > 0 );

				//modifico la pagina que tengo cargada

				tipoInstruccion instruccionModificar;
					instruccionModificar.instruccion = ESCRIBIR;
					instruccionModificar.nroPagina = 1;
					instruccionModificar.pid  = 1;
					instruccionModificar.texto = "ahora tiene finas hiervas y D'FIESTA";

				escribirPagina(instruccionModificar);

				char* textoDePaginaDespues = list_get(datosMemoria->listaRAM, paginaAModificar->posicionEnRAM );

				bool modifiquePagina = (
					(paginaAModificar->modificado) &&
					(!string_equals_ignore_case(textoDePaginaAntes, textoDePaginaDespues))
				);

				printf("Pagina despues:\n%s\n",list_get(datosMemoria->listaRAM,0));

				should_bool(creePagina && escribiPagina && modifiquePagina) be truthy;

		    }end

			it("Test sacar pagina de TLB por FIFO"){

		    	printf("Test sacar pagina de TLB por FIFO\n");

		    	datosMemoria->configuracion->entradasDeTLB = 2;

		    	datosMemoria->tipoDeAlgoritmoTLB = FIFO;

		    	iniciarProceso(3,1);

		    	escribirPaginaHard(0,1,"pagina que NO tiene finas hiervas");

		    	bool estaPaginaEnTLB = dondeEstaEnTLB(0,1)>=0;

		    	escribirPaginaHard(1,1,"pagina negra con d'fiesta blanca de ojos azules");

		    	escribirPaginaHard(2,1,"nueva pagina");

		    	bool paginaYaNoEstaEnTLB = dondeEstaEnTLB(0,1)<0;

		    	should_bool(estaPaginaEnTLB&&paginaYaNoEstaEnTLB) be truthy;

		    }end

			it("Test sacar pagina de TLB por LRU"){

					    	printf("Test sacar pagina de TLB por LRU\n");

					    	datosMemoria->configuracion->entradasDeTLB = 2;

					    	datosMemoria->tipoDeAlgoritmoTLB = LRU;

					    	iniciarProceso(3,1);

					    	escribirPaginaHard(0,1,"pagina que NO tiene finas hiervas");

					    	escribirPaginaHard(1,1,"pagina negra con d'fiesta blanca de ojos azules");

					    	bool estaPaginaEnTLB = dondeEstaEnTLB(1,1)>=0;

					    	leerPaginaHard(0,1);

					    	escribirPaginaHard(2,1,"nueva pagina");

					    	bool paginaYaNoEstaEnTLB = dondeEstaEnTLB(1,1)<0;

					    	should_bool(estaPaginaEnTLB&&paginaYaNoEstaEnTLB) be truthy;

					    }end

			it("Test sacar pagina de Ram por LRU"){

		    	printf("Test sacar pagina de Ram por LRU\n");

		    	datosMemoria->configuracion->cantidadDeMarcos = 2;

		    	datosMemoria->tipoDeAlgoritmoTLB = LRU;

		    	iniciarProceso(3,1);

		    	escribirPaginaHard(0,1,"pagina que NO tiene finas hiervas");

		    	bool estaPaginaEnRAM = dondeEstaEnTabla(0,1)>=0;

		    	escribirPaginaHard(1,1,"pagina negra con d'fiesta blanca de ojos azules");



		    	escribirPaginaHard(2,1,"nueva pagina");

		    	bool paginaYaNoEstaEnRAM = dondeEstaEnTabla(0,1)<0;

		    	should_bool(estaPaginaEnRAM&&paginaYaNoEstaEnRAM) be truthy;

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

void leerPaginaHard(int nroPagina,int pid){

	tipoInstruccion instruccionLectura;

		instruccionLectura.instruccion = LEER;

		instruccionLectura.nroPagina = nroPagina;

		instruccionLectura.pid  = pid;

		enviarPaginaPedidaACpu(instruccionLectura,0);
}


void escribirPaginaHard(int nroPagina,int pid,char* pagina){

	tipoInstruccion instruccionEscribir;

		instruccionEscribir.instruccion = ESCRIBIR;

		instruccionEscribir.nroPagina = nroPagina;

		instruccionEscribir.pid  = pid;

		instruccionEscribir.texto = pagina;

		escribirPagina(instruccionEscribir);

}

void iniciarProceso(int cantPaginas,int pid){

	tipoInstruccion instruccionDeInicio;

	instruccionDeInicio.instruccion = INICIAR;

	instruccionDeInicio.nroPagina = cantPaginas;

	instruccionDeInicio.pid  = pid;

	instruccionDeInicio.texto = "";

	reservarMemoriaParaProceso(instruccionDeInicio);
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

