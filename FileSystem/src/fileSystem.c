#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consolaFileSystem.h"
#include <commons/config.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commonsDeAsedio/thread.h>
#include <commons/collections/list.h>
#include "comandosFileSystem.h"
#include "tablasFileSystem.h"

#define MAXNODO 2

void cargarArchivoDeConfiguracion(t_config *archivoDeConfiguracion, char* puerto, char* cantidadDeNodos);

int main(void) {

	t_list* tablaDirectorios = list_create();

	t_list *nombreNodo = list_create(),
		   *bloquesNodo = list_create();

	/*************HILOS******************/
	//HILOS
	pthread_t hiloConsola;
	pthread_t hiloFunciones;


	/**********CARGAR ARCHIVO DE CONFIGURACION************/
	//VARIABLES
	char* puerto;
	char* cantidadMinimaDeNodos;

	t_config *archivoDeConfiguracion = config_create("/home/utnso/Escritorio/archivoConfig.cfg");

	cargarArchivoDeConfiguracion(archivoDeConfiguracion, puerto,cantidadMinimaDeNodos);

	printf("Archivo de configuracion cargado\n");

	config_destroy(archivoDeConfiguracion);

	int cantidadDeNodos = atoi(cantidadMinimaDeNodos);

	/*****************CARGAR ARCHIVO DE DATOS**********************/
	FILE* archivoDeDatos = fopen("/home/utnso/Escritorio/datos.txt","r");

	void* mapeo = mapearArchivoCompleto(archivoDeDatos);

	/**************************CREO HILOS******************************/
	int respuestaHiloConsola = crearThread(&hiloConsola, consola, tablaDirectorios);
	destruirThread(hiloConsola);



	/***********CARGANDO NODOS****************/
	//VARIABLES SOCKETS
	int socketsNodos[cantidadDeNodos-1];
	int i;
	for(i=0; i<cantidadDeNodos; i++){
		socketsNodos[i] = crearSocket();
	}

	int socketFileSystem = crearSocket();
	asociarAPuerto(socketFileSystem, atoi(puerto));
	escucharConexiones(socketFileSystem, cantidadDeNodos);

	//ESPERO SOLICITUDES
	printf("Esperando conexiones de nodos\n");

	int puertoNodo;

	char* ipNodo = string_new();

	socketsNodos[0] = recibirNodo(socketFileSystem,&puerto,ipNodo);

	printf("se acaba de conectar el nodo %d, que tiene ip: %s y espera en el puerto: %d\n",socketsNodos[0],ipNodo,puertoNodo);

	/*size_t longitudMensaje = strlen((char*)mapeo);

	printf("longitud de mensaje: %d\n", longitudMensaje);

	int numeroBloque=0;

	void* buffer = malloc(longitudMensaje + sizeof(size_t)+sizeof(int));

    memcpy(buffer,&numeroBloque,sizeof(int));
	memcpy(buffer+sizeof(int), &longitudMensaje, sizeof(longitudMensaje));
	memcpy(buffer+sizeof(int)+ sizeof(longitudMensaje), (char*)mapeo, longitudMensaje);

	enviarMensaje(socketsNodos[0], buffer, longitudMensaje + sizeof(longitudMensaje));*/

	printf("Nodo conectado\n");


	/***************LIBERO MEMORIA**************************/
	liberarSocket(socketFileSystem);
	liberarMemoriaDeArchivoCompletoMapeado(archivoDeDatos, mapeo);
	fclose(archivoDeDatos);

	return EXIT_SUCCESS;
}

void enviarBloquesANodos(t_list* nodos,t_list* archivosCargados,t_list* listaBloquesFS,char* archivoMapeado,char* rutaArchivo){

	int i,bytesEnviados=0,socketNodoActual,longitudBloque,inicioDeLectura;

	size_t tamanioBase = sizeof(int)+sizeof(size_t),tamanioTotal;

	void* bloque;

	tipoInfoNodos* nodoActual;

	tipoBloquesFS* bloqueActual;

	list_add(archivosCargados,rutaArchivo);

	bloqueActual->nombreArchivo = list_size(archivosCargados)-1;

	while(inicioDeLectura<=strlen(archivoMapeado)){

	for(i=0;i<list_size(nodos);i++){

		nodoActual = list_get(nodos,i);

		socketNodoActual = nodoActual->socket;

		longitudBloque = proximoBloque(archivoMapeado,inicioDeLectura);

		tamanioTotal = tamanioBase+longitudBloque;

		bloque = malloc(tamanioTotal);

		memcpy(bloque,&tamanioBase+longitudBloque,sizeof(size_t));

		serializarIntYCadena(i+bytesEnviados,string_substring(archivoMapeado,inicioDeLectura,longitudBloque),bloque+sizeof(size_t));

		enviarMensaje(socketNodoActual,bloque,tamanioTotal);

		bloqueActual->bloqueFS = list_size(listaBloquesFS)-1;

		bloqueActual->listaNodos = list_create();

		list_add(bloqueActual->listaNodos,&socketNodoActual);

		inicioDeLectura = longitudBloque;

	}

	}

}

int proximoBloque(char* archivoMapeado,int inicio){

	int i,fin=strlen(archivoMapeado),tamanioBloque=minimoEntre(20*1024*1024,fin);

		for(i=tamanioBloque;i<=inicio;i--){

			if(archivoMapeado[i]=='\n'){

				return i;
			}
		}

		return tamanioBloque;

}




void cargarArchivoDeConfiguracion(t_config *archivoDeConfiguracion, char* puerto, char* cantidadDeNodos){
	if(obtenerCampoDeConfiguracion(&puerto,"puerto", archivoDeConfiguracion) != 0 && obtenerCampoDeConfiguracion(&cantidadDeNodos,"nodos_minimos", archivoDeConfiguracion) !=0){
		perror("Error al leer archivo de configuracion\n");
		exit(-1);
	}
}

