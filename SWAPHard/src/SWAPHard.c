/*
 ============================================================================
 Name        : SWAPHard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/estructuras.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commons/string.h>
#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/error.h>
#include <string.h>
#include <commonsDeAsedio/mmap.h>
#include <commons/collections/list.h>

#define PUERTO_ESCUCHA "PUERTO_ESCUCHA"
#define NOMBRE_SWAP "NOMBRE_SWAP"
#define CANTIDAD_PAGINAS "CANTIDAD_PAGINAS"
#define TAMANIO_PAGINA "TAMANIO_PAGINA"
#define RETARDO_COMPACTACION "RETARDO_COMPACTACION"

#define PUERTOSWAP 6000

typedef struct{
	int puertoDeEscucha;
	char* nombreDeSWAP;
	int cantidadDePaginas;
	int tamanioDePagina;
	int retardoDeCompactacion;
}tipoConfigSWAP;


////////////////////FUNCIONES PARA EL MANEJO DE ARCHIVO DE CONFIGURACION///////////////////////

tipoConfigSWAP* crearConfigSWAP();

void destruirConfigSWAP (tipoConfigSWAP* estructuraDeConfiguracion);

tipoConfigSWAP* cargarArchivoDeConfiguracionDeSWAP(char* rutaDelArchivoDeConfiguracion);

typedef struct{

	int pid;

	int nroPagina;

	char* contenido;
}paginaNegra;

bool dondeEstaPagina(int nroPagina,int pid,t_list* listaPaginas,char* pagina);

tipoRespuesta* responderLeer(tipoInstruccion* instruccionRecibida,t_list* listaPaginas);

tipoRespuesta* responderIniciar(tipoInstruccion* instruccionRecibida);

tipoRespuesta* atenderInstruccion(tipoInstruccion* instruccionRecibida,t_list* listaPaginas);


int main(void) {

	tipoConfigSWAP* configuracion = cargarArchivoDeConfiguracionDeSWAP("cfgSWAP");

	int socketSwap = crearSocket();

	t_list* listaPaginas = list_create();

	asociarAPuerto(socketSwap,configuracion->puertoDeEscucha);

	escucharConexiones(socketSwap,1);

	int socketMemoria = crearSocketParaAceptarSolicitudes(socketSwap);

	while(true){

	tipoInstruccion* instruccionRecibida = recibirInstruccion(socketMemoria);

	tipoRespuesta* respuestaAMemoria = atenderInstruccion(instruccionRecibida,listaPaginas);

	enviarRespuesta(socketMemoria,respuestaAMemoria);

	}

	liberarSocket(socketMemoria);

	liberarSocket(socketSwap);

	destruirConfigSWAP(configuracion);

	return EXIT_SUCCESS;
}


tipoConfigSWAP* crearConfigSWAP(){
	tipoConfigSWAP* cfg = malloc(sizeof(tipoConfigSWAP));
	cfg->nombreDeSWAP = string_new();

	return cfg;
}

void destruirConfigSWAP (tipoConfigSWAP* cfg){
	free(cfg->nombreDeSWAP);
	free(cfg);
}

tipoConfigSWAP* cargarArchivoDeConfiguracionDeSWAP(char* rutaDelArchivoDeConfiguracion){

	t_config* archivoCfg = config_create(rutaDelArchivoDeConfiguracion);

	tipoConfigSWAP* cfg = crearConfigSWAP();

	validarErrorYAbortar(config_has_property(archivoCfg,PUERTO_ESCUCHA)
			&& config_has_property(archivoCfg,NOMBRE_SWAP)
			&& config_has_property(archivoCfg,CANTIDAD_PAGINAS)
			&& config_has_property(archivoCfg,TAMANIO_PAGINA)
			&& config_has_property(archivoCfg,RETARDO_COMPACTACION),
			"Las claves del archivo de configuracion no coinciden con las que requiere el SWAP");


	cfg->puertoDeEscucha = config_get_int_value(archivoCfg,PUERTO_ESCUCHA);
	cfg->nombreDeSWAP = string_duplicate(config_get_string_value(archivoCfg,NOMBRE_SWAP));
	cfg->cantidadDePaginas = config_get_int_value(archivoCfg,CANTIDAD_PAGINAS);
	cfg->retardoDeCompactacion = config_get_int_value(archivoCfg,RETARDO_COMPACTACION);
	cfg->tamanioDePagina = config_get_int_value(archivoCfg,TAMANIO_PAGINA);


	config_destroy(archivoCfg);

	return cfg;
}

tipoRespuesta* atenderInstruccion(tipoInstruccion* instruccionRecibida,t_list* listaPaginas){

	tipoRespuesta* respuesta;

	switch (instruccionRecibida->instruccion) {

		case INICIAR:
			printf("Inico de proceso %d de %d paginas..\n",instruccionRecibida->pid,instruccionRecibida->nroPagina);
			respuesta = responderIniciar(instruccionRecibida);
			if(respuesta->respuesta==PERFECTO)
				printf("Inicio de proceso efectivo!!\n");
			else printf("Inicio de proceso fallido :'(\n");
		break;
		case LEER:
			printf("Lectura de proceso %d de pagina %d ..\n",instruccionRecibida->pid,instruccionRecibida->nroPagina);
			respuesta = responderLeer(instruccionRecibida,listaPaginas);
			if(respuesta->respuesta==PERFECTO)
				printf("Lectura de proceso efectiva!!\n");
			else printf("Lectura de proceso fallida :'(\n");
		break;
		case FINALIZAR:
		printf("Finalizacion de proceso %d ..\n",instruccionRecibida->pid);
			respuesta = responderIniciar(instruccionRecibida);
			if(respuesta->respuesta==PERFECTO)
				printf("Finalizacion de proceso efectiva!!\n");
			else printf("Finalizacion de proceso fallida :'(\n");
		break;
	}
	return respuesta;
}

tipoRespuesta* responderIniciar(tipoInstruccion* instruccionRecibida){


	tipoRespuesta* respuesta = crearTipoRespuesta(PERFECTO,"");

	return respuesta;

}

tipoRespuesta* responderLeer(tipoInstruccion* instruccionRecibida,t_list* listaPaginas){

	tipoRespuesta* respuesta;

	char* pagina;

	bool estaPagina = dondeEstaPagina(instruccionRecibida->nroPagina,instruccionRecibida->pid,listaPaginas,pagina);

	if(estaPagina) respuesta = crearTipoRespuesta(PERFECTO,pagina);

	else respuesta = crearTipoRespuesta(MANQUEADO,"pagina no esta en SWAP");

	return respuesta;

	}

bool dondeEstaPagina(int nroPagina,int pid,t_list* listaPaginas,char* pagina){

	bool estaPagina = false;

	paginaNegra* paginaActual;

	int var;
	for (var = 0; var < list_size(listaPaginas); ++var) {

		paginaActual = list_get(listaPaginas,var);

		if(paginaActual->nroPagina==nroPagina&&paginaActual->pid==pid){

			estaPagina = true;

			pagina = paginaActual->contenido;

			break;
		}
	}

	return estaPagina;
	}









