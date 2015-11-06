#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include <commonsDeAsedio/cliente-servidor.h>
#include <commonsDeAsedio/estructuras.h>

#include "estructurasSWAP.h"
#include "funcionesSWAP.h"
#include "huecosSWAP.h"
#include "particionSWAP.h"
#include "instruccionesSWAP.h"


#define MAXIMAS_CONEXIONES_ENTRANTES 1
#define INFINITO 1


tipoInstruccion* crearInstruccion(char ins,int pid,int pag,char* testo){
	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion));

	aux->instruccion = ins;
	aux->pid = pid;
	aux->nroPagina = pag;
	aux->texto = strdup(testo);

	return aux;
}

int main(void) {

	tipoConfigSWAP* configuracion = cargarArchivoDeConfiguracionDeSWAP("cfgSWAP");
	t_list* listaDeHuecosUtilizados = inicializarListaDeHuecosUtilizados();

	FILE* particion = inicializarParticion(configuracion->nombreDeSWAP,configuracion->tamanioDePagina,configuracion->cantidadDePaginas);

	tipoInstruccion* instruccionAEjecutar;
	tipoRespuesta* respuestaParaMemoria;

	//FILE* particion = fopen("swap","r+");

/*************Sockets***************/
	int socketParaAdministrador = crearSocket();
	asociarAPuerto(socketParaAdministrador,configuracion->puertoDeEscucha);
	escucharConexiones(socketParaAdministrador,MAXIMAS_CONEXIONES_ENTRANTES);
	int socketParaRecibirInstrucciones = crearSocketParaAceptarSolicitudes(socketParaAdministrador);



/***********Recibir instrucciones y ejecutarlas***********/
	while(INFINITO){

		instruccionAEjecutar = recibirInstruccion(socketParaRecibirInstrucciones);

		respuestaParaMemoria = ejecutarInstruccion(instruccionAEjecutar,listaDeHuecosUtilizados,configuracion);

		enviarRespuesta(socketParaRecibirInstrucciones,respuestaParaMemoria);

		destruirTipoRespuesta(respuestaParaMemoria);
		destruirTipoInstruccion(instruccionAEjecutar);
	}

/********Liberar memoria*************/

	liberarSocket(socketParaRecibirInstrucciones);
	liberarSocket(socketParaAdministrador);

	list_destroy_and_destroy_elements(listaDeHuecosUtilizados,(void*)destruirHuecoUtilizado);
	destruirConfigSWAP(configuracion);
	fclose(particion);


	return EXIT_SUCCESS;
}

