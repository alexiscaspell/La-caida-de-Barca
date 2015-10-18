/*
 ============================================================================
 Name        : CPUHard.c
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
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/error.h>
#include <commonsDeAsedio/estructuras.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <pthread.h>

/************Archivo de Config de CPU************/
#define IP_PLANIFICADOR "IP_PLANIFICADOR"
#define PUERTO_PLANIFICADOR "PUERTO_PLANIFICADOR"
#define IP_MEMORIA "IP_MEMORIA"
#define PUERTO_MEMORIA "PUERTO_MEMORIA"
#define CANTIDAD_HILOS "CANTIDAD_HILOS"
#define RETARDO "RETARDO"

#define PUERTOMEMORIA 7200
#define IPMEMORIA "127.0.0.1"

typedef struct{
	char* ipPlanificador;
	int puertoPlanificador;
	char* ipMemoria;
	int puertoMemoria;
	int cantidadDeHilos;
	int retardo;
}tipoConfigCPU;

tipoConfigCPU* cargarArchivoDeConfiguracionDeCPU(char* rutaDelArchivoDeConfiguracionDelCPU);

tipoConfigCPU* crearConfigCPU();

void destruirConfigCPU(tipoConfigCPU* estructuraDeConfiguracion);

void getRespuesta(int socket,tipoRespuesta* respuesta){	respuesta = recibirRespuesta(socket);}


int main(void) {

	tipoConfigCPU* configuracion = cargarArchivoDeConfiguracionDeCPU("cfgCPU");

	int socketCpu = crearSocket();

	conectarAServidor(socketCpu,configuracion->ipMemoria,configuracion->puertoMemoria);

	tipoInstruccion instruccionAEnviar;

	instruccionAEnviar.nroPagina =0;

	instruccionAEnviar.instruccion = INICIAR;

	instruccionAEnviar.texto = "";

	instruccionAEnviar.pid = 19;

	enviarInstruccion(socketCpu,instruccionAEnviar);

	tipoRespuesta respuesta;

	getRespuesta(socketCpu,&respuesta);

	printf("el estado de respuesta es %c\n",respuesta.respuesta);

	printf("La info de respuesta es: %s\n",respuesta.informacion);

	if(respuesta.respuesta==PERFECTO)
	printf("Ya se pudo iniciar el proceso...\n");

	else
		printf("Error de inicio de proceso..\n");

	liberarSocket(socketCpu);

	destruirConfigCPU(configuracion);

	return EXIT_SUCCESS;
}




tipoConfigCPU* crearConfigCPU(){
	tipoConfigCPU* cfg = malloc(sizeof(tipoConfigCPU));
	cfg->ipPlanificador = string_new();
	cfg->ipMemoria = string_new();

	return cfg;
}

void destruirConfigCPU(tipoConfigCPU* cfg){
	free(cfg->ipPlanificador);
	free(cfg->ipMemoria);
	free(cfg);
}

tipoConfigCPU* cargarArchivoDeConfiguracionDeCPU(char* rutaDelArchivoDeConfiguracionDelCPU){

	t_config* archivoCfg = config_create(rutaDelArchivoDeConfiguracionDelCPU);
	tipoConfigCPU* cfg = crearConfigCPU();

	validarErrorYAbortar(config_has_property(archivoCfg,IP_PLANIFICADOR)
			&& config_has_property(archivoCfg,PUERTO_PLANIFICADOR)
			&& config_has_property(archivoCfg,IP_MEMORIA)
			&& config_has_property(archivoCfg,PUERTO_MEMORIA)
			&& config_has_property(archivoCfg,CANTIDAD_HILOS)
			&& config_has_property(archivoCfg,RETARDO),
			"Las claves del archivo de configuracion no coinciden con las que requiere el CPU");


	cfg->ipPlanificador = string_duplicate(config_get_string_value(archivoCfg,IP_PLANIFICADOR));
	cfg->puertoPlanificador = config_get_int_value(archivoCfg,PUERTO_PLANIFICADOR);
	cfg->ipMemoria = string_duplicate(config_get_string_value(archivoCfg,IP_MEMORIA));
	cfg->puertoMemoria = config_get_int_value(archivoCfg,PUERTO_MEMORIA);
	cfg->cantidadDeHilos = config_get_int_value(archivoCfg,CANTIDAD_HILOS);
	cfg->retardo = config_get_int_value(archivoCfg,RETARDO);

	config_destroy(archivoCfg);

	return cfg;
}
