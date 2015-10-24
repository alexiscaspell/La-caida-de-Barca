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


int main(void) {

	tipoConfigSWAP* configuracion = cargarArchivoDeConfiguracionDeSWAP("cfgSWAP");

	int socketSwap = crearSocket();

	asociarAPuerto(socketSwap,configuracion->puertoDeEscucha);

	escucharConexiones(socketSwap,1);

	int socketMemoria = crearSocketParaAceptarSolicitudes(socketSwap);

	printf("pase :'(\n");

	char* negro = "negro";

	tipoRespuesta respuesta;

	respuesta.respuesta = PERFECTO;

	respuesta.informacion = negro;

	printf("pase :'(\n");


	while(true){

	tipoInstruccion* instruccionRecibida = recibirInstruccion(socketMemoria);

	if(instruccionRecibida->instruccion==INICIAR){

		printf("Recibi una instruccion del proceso %d de reserva de %d pagina/s\n",instruccionRecibida->pid,instruccionRecibida->nroPagina);

		enviarRespuesta(socketMemoria,&respuesta);
		}
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

