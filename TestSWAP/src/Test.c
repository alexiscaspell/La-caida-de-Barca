/*
 * Test.c
 *
 *  Created on: 6/11/2015
 *      Author: utnso
 */


#include "Test.h"


void correrTests(){

	context(asd){

		describe("Tests de SWAP"){
			tipoConfigSWAP* cfg;
			t_list* listaDeHuecos;
			tipoHuecoUtilizado* hueco;

			before{
				cfg = cargarArchivoDeConfiguracionDeSWAP("cfgSWAP");
				listaDeHuecos = inicializarListaDeHuecosUtilizados();
				hueco = crearHuecoUtilizado(1,4,2);

				list_add(listaDeHuecos,hueco);
			}end

			after{
				list_destroy_and_destroy_elements(listaDeHuecos,(void*)destruirHuecoUtilizado);
				destruirConfigSWAP(cfg);
			}end


			it("Reservar espacio para un proceso"){

				reservarEspacioParaUnProceso(listaDeHuecos,cfg);

			}end

			it("Liberar espacio de un proceso"){

				liberarEspacioDeProceso(listaDeHuecos,1);

			}end


		}end

	}//fin de context

}

void reservarEspacioParaUnProceso(t_list* listaDeHuecos,tipoConfigSWAP* cfg){

	tipoInstruccion* instruccion = crearTipoInstruccion(1,INICIAR,5,"");

	tipoRespuesta* respuesta = ejecutarInstruccion(instruccion,listaDeHuecos,cfg);

	should_bool(respuesta->respuesta==PERFECTO) be truthy;
	should_bool(list_size(listaDeHuecos)==2) be truthy;
}

void liberarEspacioDeProceso(t_list* listaDeHuecos, int pid){
	liberarEspacio(listaDeHuecos,pid);

	should_bool(list_size(listaDeHuecos)==0) be truthy;
}
