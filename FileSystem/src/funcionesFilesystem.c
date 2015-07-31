#include "funcionesFilesystem.h"
#include <stdbool.h>
#include <stdlib.h>

#define PUERTO_LISTEN "PUERTO_LISTEN"
#define CANTIDAD_NODOS_MINIMA "CANTIDAD_NODOS_MINIMA"
#define EXITO 0
#define FALLO -1

tipoConfigFilesystem* crearTipoConfigFilesystem(){
	tipoConfigFilesystem* aux = malloc(sizeof(tipoConfigFilesystem));
	return aux;
}

void destruirTipoConfigFilesystem(tipoConfigFilesystem* camposDeConfiguracion){
	free(camposDeConfiguracion);
}


int cargarArchivoDeConfiguracionDelFS(t_config* archivoDeConfiguracionDelFS,tipoConfigFilesystem* camposDeConfiguracionDelFS){
	if (config_has_property(archivoDeConfiguracionDelFS,PUERTO_LISTEN) && config_has_property(archivoDeConfiguracionDelFS,CANTIDAD_NODOS_MINIMA)) {

		camposDeConfiguracionDelFS->puertoListen = config_get_int_value(archivoDeConfiguracionDelFS,PUERTO_LISTEN);
		camposDeConfiguracionDelFS->cantidadDeNodosConectadosMinima = config_get_int_value(archivoDeConfiguracionDelFS,CANTIDAD_NODOS_MINIMA);

		return EXITO;
	}
	else {
		return FALLO;
	}
}
