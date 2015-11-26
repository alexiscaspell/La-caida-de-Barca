/*
 * Test.c
 *
 *  Created on: 6/11/2015
 *      Author: utnso
 */


#include "Test.h"






			tipoConfigSWAP* cfg;
			t_list* listaDeHuecosUtilizados;
			t_list* listaDeHuecosLibres;
			tipoHuecoUtilizado* hueco;
			t_log* logger;
			bool finalizarProceso;


void correrTests(){

	context(asd){

		describe("Tests de SWAP"){


			before{

				logger = crearLoggerParaSeguimiento("logSWAP","Administrador de SWAP");
				logearSeguimiento("Inicio de ejecucion de proceso SWAP",logger);

				cfg = cargarArchivoDeConfiguracionDeSWAP("cfgSWAP");
				listaDeHuecosUtilizados = inicializarListaDeHuecosUtilizados();
				listaDeHuecosLibres = inicializarListaDeHuecosLibres(cfg->cantidadDePaginas);
				inicializarParticion(cfg->nombreDeSWAP,cfg->tamanioDePagina,cfg->cantidadDePaginas);
				hueco = crearHuecoUtilizado(1,4,2);
				finalizarProceso = false;

				//list_add(listaDeHuecosUtilizados,hueco);
			}end

			after{

				list_destroy_and_destroy_elements(listaDeHuecosUtilizados,(void*)destruirHuecoUtilizado);
				list_destroy_and_destroy_elements(listaDeHuecosLibres,(void*)destruirHuecoLibre);
				destruirConfigSWAP(cfg);
				destruirLogger(logger);

			}end

			it("Listas inicializadas correctamente"){
				listasInicializadas();
			}end

			it("Reservar espacio para un proceso"){
				reservarEspacioParaUnProceso();
			}end

		}end

	}//fin de context

}


tipoRespuesta* ejecutar(tipoInstruccion* instruccion){
	return ejecutarInstruccion(instruccion,listaDeHuecosUtilizados,listaDeHuecosLibres,cfg,logger,&finalizarProceso);
}


void listasInicializadas(){
	tipoHuecoLibre* hueco = list_get(listaDeHuecosLibres,0);

	should_int(list_size(listaDeHuecosUtilizados)) be equal to(0);
	should_int(list_size(listaDeHuecosLibres)) be equal to(1);

	should_int(hueco->base) be equal to(0);
	should_int(hueco->cantidadDePaginasQueOcupa) be equal to(cfg->cantidadDePaginas);

}

void reservarEspacioParaUnProceso(){
	tipoInstruccion* reservar = crearTipoInstruccion(1,INICIAR,4,"");

	tipoRespuesta* resultadoDeReserva = ejecutar(reservar);

	should_bool(resultadoDeReserva->respuesta == PERFECTO) be truthy;

}
//
//void liberarEspacioDeProceso(t_list* listaDeHuecos, int pid){
//
//	should_bool(1==0) be truthy;
//}

