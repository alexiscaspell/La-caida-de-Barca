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

typedef struct {
	char* ipPlanificador;
	int puertoPlanificador;
	char* ipMemoria;
	int puertoMemoria;
	int cantidadDeHilos;
	int retardo;
} tipoConfigCPU;
/*
 tipoConfigCPU* cargarArchivoDeConfiguracionDeCPU(char* rutaDelArchivoDeConfiguracionDelCPU);
 tipoConfigCPU* crearConfigCPU();
 void destruirConfigCPU(tipoConfigCPU* estructuraDeConfiguracion);
 void getRespuesta(int socket,tipoRespuesta* respuesta){	respuesta = recibirRespuesta(socket);}
 */

/********************Variables globales*********************/
int pid = 0, nroPagina = 0;
char instruccion;

/********************Funciones*********************/
void imprimirRespuesta(tipoRespuesta* respuesta) {
	printf("Respuesta de la Iniciacion: \n");
	printf("Respuesta: %c \n", respuesta->respuesta);
	printf("Informacion: %s\n", respuesta->informacion);
}

void crearInstruccion(int socketCPU) {
	fflush(0);
	printf("pid: "); scanf("%d", &pid);
	printf("nroPagina: "); scanf("%d", &nroPagina);

	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion));
		aux->instruccion = INICIAR;
		aux->nroPagina = nroPagina;
		aux->pid = pid;
		aux->texto = "";
	enviarInstruccion(socketCPU, aux);
	free(aux);

	tipoRespuesta* respuesta = recibirRespuesta(socketCPU);
	imprimirRespuesta(respuesta);
}

void EscribirInstruccion(int socketCPU) {
	char* texto = malloc(50);

	fflush(0);
	printf("pid: "); scanf("%d", &pid);
	printf("nroPagina: "); scanf("%d", &nroPagina);
	printf("Texto: "); scanf("%s", texto);

	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion) + strlen(texto));
		aux->instruccion = ESCRIBIR;
		aux->nroPagina = nroPagina;
		aux->pid = pid;
		aux->texto = texto;
	enviarInstruccion(socketCPU, aux);
	free(aux);

	strcpy(texto,"");

	tipoRespuesta* respuesta = recibirRespuesta(socketCPU);
	imprimirRespuesta(respuesta);
}

void LeerInstruccion(int socketCPU) {
	fflush(0);
	printf("pid: "); scanf("%d", &pid);
	printf("nroPagina: "); scanf("%d", &nroPagina);

	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion));
		aux->instruccion = LEER;
		aux->nroPagina = nroPagina;
		aux->pid = pid;
		aux->texto = ""; //no importa
	enviarInstruccion(socketCPU, aux);
	free(aux);

	tipoRespuesta* respuesta = recibirRespuesta(socketCPU);
	imprimirRespuesta(respuesta);
}

void FinalizarInstruccion(int socketCPU) {
	fflush(0);
	printf("pid: "); scanf("%d", &pid);

	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion));
		aux->instruccion = FINALIZAR;
		aux->nroPagina = 0;//nroPagina;//Esto es lo que no importa!!
		aux->pid = pid; //no importa//Si importa pelotudo!!
		aux->texto = ""; //no importa
	enviarInstruccion(socketCPU, aux);

	tipoRespuesta* respuesta = recibirRespuesta(socketCPU);
	imprimirRespuesta(respuesta);
	free(respuesta);

}

void ejecutarAccionMenu(int opcion, int socketCPU) {
	switch (opcion) {
	case 1:
		crearInstruccion(socketCPU);
		break;
	case 2:
		EscribirInstruccion(socketCPU);
		break;
	case 3:
		LeerInstruccion(socketCPU);
		break;
	case 4:
		FinalizarInstruccion(socketCPU);
		break;
	}
}

tipoConfigCPU* crearConfigCPU() {
	tipoConfigCPU* cfg = malloc(sizeof(tipoConfigCPU));
	cfg->ipPlanificador = string_new();
	cfg->ipMemoria = string_new();

	return cfg;
}

void destruirConfigCPU(tipoConfigCPU* cfg) {
	free(cfg->ipPlanificador);
	free(cfg->ipMemoria);
	free(cfg);
}

tipoConfigCPU* cargarArchivoDeConfiguracionDeCPU(
		char* rutaDelArchivoDeConfiguracionDelCPU) {

	t_config* archivoCfg = config_create(rutaDelArchivoDeConfiguracionDelCPU);
	tipoConfigCPU* cfg = crearConfigCPU();

	validarErrorYAbortar(
			config_has_property(archivoCfg, IP_PLANIFICADOR)
					&& config_has_property(archivoCfg, PUERTO_PLANIFICADOR)
					&& config_has_property(archivoCfg, IP_MEMORIA)
					&& config_has_property(archivoCfg, PUERTO_MEMORIA)
					&& config_has_property(archivoCfg, CANTIDAD_HILOS)
					&& config_has_property(archivoCfg, RETARDO),
			"Las claves del archivo de configuracion no coinciden con las que requiere el CPU");

	cfg->ipPlanificador = string_duplicate(
			config_get_string_value(archivoCfg, IP_PLANIFICADOR));
	cfg->puertoPlanificador = config_get_int_value(archivoCfg,
			PUERTO_PLANIFICADOR);
	cfg->ipMemoria = string_duplicate(
			config_get_string_value(archivoCfg, IP_MEMORIA));
	cfg->puertoMemoria = config_get_int_value(archivoCfg, PUERTO_MEMORIA);
	cfg->cantidadDeHilos = config_get_int_value(archivoCfg, CANTIDAD_HILOS);
	cfg->retardo = config_get_int_value(archivoCfg, RETARDO);

	config_destroy(archivoCfg);

	return cfg;
}

int main(void) {

	tipoConfigCPU* configuracion = cargarArchivoDeConfiguracionDeCPU("cfgCPU");

	int socketCpu = crearSocket();
	conectarAServidor(socketCpu, configuracion->ipMemoria,
			configuracion->puertoMemoria);

	bool interruptor = true;
	int opcion;

	while (interruptor) {
		printf("\n");
		printf("1) Crear Instruccion\n");
		printf("2) Escribir Instruccion\n");
		printf("3) Leer Instruccion\n");
		printf("4) Finalizar Instruccion\n");
		printf("\n");
		scanf("%d", &opcion);

		ejecutarAccionMenu(opcion, socketCpu);
	}

	liberarSocket(socketCpu);
	destruirConfigCPU(configuracion);

	return EXIT_SUCCESS;
}
