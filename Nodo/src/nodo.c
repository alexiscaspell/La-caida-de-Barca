#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/string.h>
#include <commons/config.h>
#include "funcionesNodo.h"
#include "estructurasNodo.h"
#include <commonsDeAsedio/cliente-servidor.h>
#include <pthread.h>


int main(void) {

	char* rutaDelArchivoDeConfiguracionDelNodo = "/home/utnso/Escritorio/cfgNodo";//Se tiene que ingresar por scanf, por ahora lo dejo asi porque es mas facil.


	//////////////Cargo archivo de configuracion
	t_config* archivoDeConfiguracionDelNodo = config_create(rutaDelArchivoDeConfiguracionDelNodo);
	tipoConfigNodo* camposDelArchivoDeConfiguracionDelNodo = crearTipoConfigNodo();

	if (cargarArchivoDeConfiguracionDelNodo(archivoDeConfiguracionDelNodo,camposDelArchivoDeConfiguracionDelNodo)==FALLO) {
		perror("Error al cargar el archivo de configuracion del nodo.");
	}

	////////////Abro archivo de configuracion y de tabla de bloques
	FILE* archivoDeEspacioDeDatos,* archivoDeTablaDeBloques;
	archivoDeEspacioDeDatos = fopen(camposDelArchivoDeConfiguracionDelNodo->espacioDeDatos,"r+");
	archivoDeTablaDeBloques = fopen(camposDelArchivoDeConfiguracionDelNodo->tablaDeBloquesNodo,"r+");

	t_list* tablaDeBloques = list_create();
	cargarTabla(tablaDeBloques,archivoDeTablaDeBloques);

	///////////Creo socket y me conecto al filesystem
	int socketParaRecibirSolicitudesDelFS;
	socketParaRecibirSolicitudesDelFS = crearSocket();
	conectarAServidor(socketParaRecibirSolicitudesDelFS,camposDelArchivoDeConfiguracionDelNodo->ipFS,camposDelArchivoDeConfiguracionDelNodo->puertoFS);

	//////////Creo hilo
	pthread_t hiloParaGuardarBloques;
	pthread_t hiloParaLeerBloques;


	/*guardarBloque(3,1,"asd\n",tablaDeBloques,archivoDeEspacioDeDatos,archivoDeTablaDeBloques);
	guardarBloque(20,5,"manaos\n",tablaDeBloques,archivoDeEspacioDeDatos,archivoDeTablaDeBloques);
	guardarBloque(15,2,"tujavimancura\n",tablaDeBloques,archivoDeEspacioDeDatos,archivoDeTablaDeBloques);*/

	//guardarBloque(66,6,"por su pollo\n",tablaDeBloques,archivoDeEspacioDeDatos,archivoDeTablaDeBloques);


	char* b0,*b1,*b2,*b3;

	b0 = obtenerBloque(3,1,tablaDeBloques,archivoDeEspacioDeDatos);
	b1 = obtenerBloque(20,5,tablaDeBloques,archivoDeEspacioDeDatos);
	b2 = obtenerBloque(15,2,tablaDeBloques,archivoDeEspacioDeDatos);
	b3 = obtenerBloque(66,6,tablaDeBloques,archivoDeEspacioDeDatos);


	printf("%s%s%s%s\n\n",b0,b2,b1,b3);



	//////////Cierro archivos y libero memoria
	fclose(archivoDeEspacioDeDatos);
	destruirTipoConfigNodo(camposDelArchivoDeConfiguracionDelNodo);
	config_destroy(archivoDeConfiguracionDelNodo);

	return EXIT_SUCCESS;
}
