/*
 * funcionesNodo.c
 *
 *  Created on: 16/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESNODO_C_
#define FUNCIONESNODO_C_

#include "funcionesNodo.h"

/*************FUNCIONES PARA ARCHIVO DE CONFIGURACION*********************/
tipoConfigNodo* crearTipoConfigNodo(){
	tipoConfigNodo* cfg = malloc(sizeof(tipoConfigNodo));
	cfg->ipFS = string_new();
	cfg->espacioDeDatos = string_new();
	cfg->esNodoNuevo = string_new();
	cfg->ipNodo = string_new();
	cfg->directorioTemporal = string_new();
	cfg->tablaDeBloquesNodo = string_new();
	return cfg;
}

void destruirTipoConfigNodo (tipoConfigNodo* cfg){
	free(cfg->ipFS);
	free(cfg->espacioDeDatos);
	free(cfg->esNodoNuevo);
	free(cfg->ipNodo);
	free(cfg->directorioTemporal);
	free(cfg->tablaDeBloquesNodo);
	free(cfg);
}

int cargarArchivoDeConfiguracionDelNodo(t_config* archivoDeConfiguracionDelNodo,tipoConfigNodo* cfg){

	if(config_has_property(archivoDeConfiguracionDelNodo,PUERTO_FS) && config_has_property(archivoDeConfiguracionDelNodo,IP_FS) && config_has_property(archivoDeConfiguracionDelNodo,ESPACIO_DE_DATOS) && config_has_property(archivoDeConfiguracionDelNodo,DIRECTORIO_TEMPORAL) && config_has_property(archivoDeConfiguracionDelNodo,ES_NODO_NUEVO) && config_has_property(archivoDeConfiguracionDelNodo,IP_NODO) && config_has_property(archivoDeConfiguracionDelNodo,PUERTO_FS)){

		cfg->puertoNodo = config_get_int_value(archivoDeConfiguracionDelNodo,PUERTO_NODO);
		cfg->puertoFS = config_get_int_value(archivoDeConfiguracionDelNodo,PUERTO_FS);
		string_append(&(cfg->ipFS),config_get_string_value(archivoDeConfiguracionDelNodo,IP_FS));
		string_append(&(cfg->espacioDeDatos),config_get_string_value(archivoDeConfiguracionDelNodo,ESPACIO_DE_DATOS));
		string_append(&(cfg->esNodoNuevo),config_get_string_value(archivoDeConfiguracionDelNodo,ES_NODO_NUEVO));
		string_append(&(cfg->ipNodo),config_get_string_value(archivoDeConfiguracionDelNodo,IP_NODO));
		string_append(&(cfg->directorioTemporal),config_get_string_value(archivoDeConfiguracionDelNodo,DIRECTORIO_TEMPORAL));
		string_append(&(cfg->tablaDeBloquesNodo),config_get_string_value(archivoDeConfiguracionDelNodo,TABLA_BLOQUES_NODO));


		return EXITO;
	}
	else{
		return FALLO;
	}
}
/***************FIN DE FUNCIONES PARA ARCHIVO DE CONFIGURACION*********************/


/*************INTERFAZ CON FS Y OTROS NODOS**************/
void guardarBloque(int numeroBloqueFS,int numeroArchivoFS,char* datosAGuardar,t_list* tablaDeBloquesNodo,FILE* espacioDeDatos,FILE* archivoTabla){

	int numeroBloqueNodo;
	tipoBloqueNodo* bloqueNodo = crearBloqueNodo(numeroBloqueFS,numeroArchivoFS);
	void* mapeo;

	list_add(tablaDeBloquesNodo,bloqueNodo);
	numeroBloqueNodo = list_size(tablaDeBloquesNodo)-1;

	fprintf(archivoTabla,"%d %d %d\n",numeroBloqueNodo,numeroBloqueFS,numeroArchivoFS);

	mapeo = mapearBloqueDeArchivoAMemoria(espacioDeDatos,numeroBloqueNodo);
	sprintf((char*)mapeo,"%s",datosAGuardar);
	liberarMemoriaDeBloqueDeArchivoMapeado(mapeo);

}

///////////////////////////////////////////////////////////////////

char* obtenerBloque(int numeroBloqueFS,int numeroArchivoFS,t_list* tablaDeBloquesNodo,FILE* espacioDeDatos){
	char* resultado;
	int numeroDeBloqueNodo;
	void* mapeo;

	numeroDeBloqueNodo = indiceDeBloqueNodo(tablaDeBloquesNodo,numeroBloqueFS,numeroArchivoFS);

	mapeo = mapearBloqueDeArchivoAMemoria(espacioDeDatos,numeroDeBloqueNodo);

	resultado = string_duplicate((char*)mapeo);

	liberarMemoriaDeBloqueDeArchivoMapeado(mapeo);

	return resultado;
}



char* obtenerContenidoDeArchivoTemporalDelNodo(FILE* archivoTemporal){
	char* aux;
	void* mapeo = mapearArchivoCompleto(archivoTemporal);

	aux = string_duplicate((char*)mapeo);

	liberarMemoriaDeArchivoCompletoMapeado(archivoTemporal,mapeo);

	return aux;
}




/*************FIN INTERFAZ CON FS Y OTROS NODOS**************/


/**************TABLA DE BLOQUES*************/

///////////////////////////////////////////////////////////////////

tipoBloqueNodo* crearBloqueNodo(int numBloqueFS, int archivo){
	tipoBloqueNodo* aux = malloc(sizeof(tipoBloqueNodo));

	aux->numeroDeBloqueFS = numBloqueFS;
	aux->archivoDuenio = archivo;

	return aux;
}

///////////////////////////////////////////////////////////////////

void destruirBloqueNodo(tipoBloqueNodo* bloqueNodo){
	free(bloqueNodo);
}

///////////////////////////////////////////////////////////////////

int indiceDeBloqueNodo(t_list* tablaDeBloques,int bloqueFS, int archivo){
	int i;
	tipoBloqueNodo* aux;

	for (i = 0; i < list_size(tablaDeBloques); ++i) {
		aux = (tipoBloqueNodo*)list_get(tablaDeBloques,i);
		if (aux->numeroDeBloqueFS==bloqueFS && aux->archivoDuenio==archivo) {
			destruirBloqueNodo(aux);
			return i;
		}
	}

	return -1;
}

void cargarTabla(t_list* tablaDeBloques,FILE* archivoTabla){
	int numeroBloqueNodo,numeroBloqueFS,numeroArchivo;

	rewind(archivoTabla);
	while(fscanf(archivoTabla,"%d %d %d",&numeroBloqueNodo,&numeroBloqueFS,&numeroArchivo) == 3){
		list_add(tablaDeBloques,crearBloqueNodo(numeroBloqueFS,numeroArchivo));
	}
}


#endif /* FUNCIONESNODO_C_ */
