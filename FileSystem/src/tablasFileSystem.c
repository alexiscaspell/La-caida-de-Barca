/*
 * tablasFileSystem.c
 *
 *  Created on: 28/6/2015
 *      Author: utnso
 */

#include "tablasFileSystem.h"

int nombreArchivoInt(char* archivo, t_list* tablaArchivos){
	int i;
	for(i=0; i<list_size(tablaArchivos); i++){
		if(strcmp(archivo, list_get(tablaArchivos, i)) == 0){
			break;
		}
	}
	return i;
}

void imprimirListaNodosDeTipoBloqueFS(t_list* listaNodos){
	int i;
	for(i=0; i<list_size(listaNodos); i++){
		printf("%d ", (int)list_get(listaNodos, i));
	}
}

void verBloquesQueComponenUnArchivo(char* archivo, t_list* tablaArchivos){
	int nombreArchivo = nombreArchivoInt(archivo, tablaArchivos);
	tipoBloquesFS* bloqueFS;

	int i;
	for(i=0; i<list_size(tablaArchivos); i++){
		bloqueFS = list_get(tablaArchivos, i);

		if(bloqueFS->nombreArchivo == nombreArchivo){
			printf("nº bloque: %d	Nodos: ", bloqueFS->bloqueFS);
			imprimirListaNodosDeTipoBloqueFS(bloqueFS->listaNodos);
			printf("\n");
		}
	}
}



void eliminarUnNodoDeDatos(int nombreNodo, t_list* tablaInfoNodos){
	tipoInfoNodos* infoNodo;

	int i;
	for(i=0; i<list_size(tablaInfoNodos); i++){
		infoNodo = list_get(tablaInfoNodos, i);
		if(infoNodo->socket == nombreNodo){
			infoNodo->estado = 'n';
			list_replace(tablaInfoNodos, i, infoNodo);
		}
	}
}



bool equalsTipoBloqueFS(tipoBloquesFS bloque1,tipoBloquesFS bloque2){

	return (bloque1.bloqueFS==bloque2.bloqueFS)&&(bloque1.nombreArchivo==bloque2.nombreArchivo);
}

int dondeEstaTipoBloqueFS(t_list* tablaBloquesFS,tipoBloquesFS bloque){

	int i=-1;

	tipoBloquesFS* otroBloque;

	for(i=0;i<list_size(tablaBloquesFS);i++){

		otroBloque = list_get(tablaBloquesFS,i);

		if(equalsTipoBloqueFS(bloque,*otroBloque))
			break;
	}
	return i;
}

bool contieneTipoBloqueFS(t_list* tablaBloquesFS,tipoBloquesFS bloque){

return (dondeEstaTipoBloqueFS(tablaBloquesFS,bloque)!=-1);
}

void agregarBloqueYNodo(t_list* tablaBloquesFS,t_list* listaArchivos,char* rutaArchivo,int nroBloque,int nodo){


	int nombreArchivo = nombreArchivoInt(rutaArchivo,listaArchivos);

	tipoBloquesFS* bloqueBusqueda;

	bloqueBusqueda->bloqueFS = nroBloque;

	bloqueBusqueda->nombreArchivo = rutaArchivo;

	int indiceBloque = dondeEstaTipoBloqueFS(tablaBloquesFS,*bloqueBusqueda);

	if(indiceBloque!=-1){

		tipoBloquesFS* bloqueFS = malloc(sizeof(tipoBloquesFS));

		bloqueFS->bloqueFS = nroBloque;

		bloqueFS->nombreArchivo = rutaArchivo;

		bloqueFS->listaNodos = list_create();

		list_add(tablaBloquesFS,bloqueFS);
	}

	else
		bloqueBusqueda = list_get(tablaBloquesFS,indiceBloque);

	list_add(bloqueBusqueda->listaNodos,&nodo);

}

void borrarTipoBloqueFS(t_list* tablaBloquesFS,char* rutaArchivo,int bloque){

	tipoBloquesFS* bloqueBusqueda;

	bloqueBusqueda->bloqueFS = bloque;

	bloqueBusqueda->nombreArchivo = nombreArchivoInt(rutaArchivo,tablaBloquesFS);

	int indiceBloque = dondeEstaTipoBloqueFS(tablaBloquesFS,*bloqueBusqueda);

	bloqueBusqueda = list_get(tablaBloquesFS,indiceBloque);

	int i,nodoActual;

	for(i=0;i<list_size(bloqueBusqueda->listaNodos);i++){

		nodoActual = list_get(bloqueBusqueda->listaNodos,i);

		//avisarANodoQueBorreBloque(nodoActual,bloqueBusqueda->bloqueFS);

	}

	free(bloqueBusqueda->nombreArchivo);

	list_destroy_and_destroy_elements(tablaBloquesFS,free);

	free(bloqueBusqueda);

}

int minimoEntre(int x,int y){

	if(x<y)
		return x;

	return y;
}

void borrarBloques(t_list* tablaBloques,char* directorio){

	int i,nombreArchivo = nombreArchivoInt(directorio,tablaBloques);

	tipoBloquesFS bloqueBusqueda;

	tipoBloquesFS* bloqueActual;

	bloqueBusqueda.nombreArchivo = nombreArchivo;

	for(i=0;i<list_size(tablaBloques);i++){

		bloqueBusqueda.bloqueFS = i;

		bloqueActual = list_get(tablaBloques,i);

		if(equalsTipoBloqueFS(bloqueBusqueda,*bloqueActual))

			borrarTipoBloqueFS(tablaBloques,directorio,i);
	}

}

/************************Funciones tabla de direcciones*********************************/
bool equals(tipoTablaDirectorio fila1,tipoTablaDirectorio fila2){

	return (strcmp(fila1.directorio,fila2.directorio)==0)&&(fila1.padre==fila2.padre);
}

void agregarALista(t_list* lista,char* directorio,int padre){

	tipoTablaDirectorio* proximaTabla = malloc(sizeof(tipoTablaDirectorio));

	proximaTabla->directorio = directorio;

	proximaTabla->padre = padre;

	list_add(lista,proximaTabla);

}

void cargarDirectorioATabla(t_list* tablaDirectorios, char* direccionDirectorio){

	char** listaCarpetas = string_split(direccionDirectorio, "/");

	int i,padre=0;

	for(i=0; listaCarpetas[i] != NULL; i++){

		if (!existeDirectorio(tablaDirectorios,listaCarpetas[i],padre)) {

            agregarALista(tablaDirectorios,listaCarpetas[i],padre);
		}

		padre = dondeEstaDirectorio(tablaDirectorios,listaCarpetas[i],padre);

	}

}
int dondeEstaDirectorio(t_list* lista,char* carpeta,int padre){

	int i=0,dondeEsta=-1;

	tipoTablaDirectorio* directorioActual;

	tipoTablaDirectorio directorio;

	directorio.directorio = carpeta;

	directorio.padre = padre;

	for(i=0;i<list_size(lista);i++){

		directorioActual = (tipoTablaDirectorio*)list_get(lista,i);

		if(equals(directorio,*directorioActual)){

			dondeEsta = i;

			break;
		}
	}

	return dondeEsta;
}

bool existeDirectorio(t_list* lista,char* directorio,int padre){

return (dondeEstaDirectorio(lista,directorio,padre)!=-1);

}

void imprimirTablaDirectorio(t_list* tablaDirectorios){

	printf("Index	Directorio	Padre\n");

	int i;
	tipoTablaDirectorio* fila;

	for(i=0; i<list_size(tablaDirectorios); i++){

		fila = list_get(tablaDirectorios,i);

		printf("%d	%s	%d\n", i, fila->directorio, fila->padre);

	}

}

int buscarDirectorio(t_list* lista,char* directorioCompleto){

	char** listaCarpetas = string_split(directorioCompleto, "/");

	int i,padre=0;

	for(i=0;listaCarpetas[i+1]!=NULL;i++){

		padre = dondeEstaDirectorio(lista,listaCarpetas[i],padre);
	}

	return dondeEstaDirectorio(lista,listaCarpetas[i],padre);
}

void eliminarDirectorio(t_list* lista,char* directorioCompleto){

	int directorioABorrar = buscarDirectorio(lista,directorioCompleto);

	tipoTablaDirectorio* filaABorrar = list_get(lista,directorioABorrar);

	free(filaABorrar->directorio);

	list_remove(lista,directorioABorrar);

	free(filaABorrar);

}

void moverDirectorio(t_list* lista,char* directorioViejo,char*directorioNuevo){

	eliminarDirectorio(lista,directorioViejo);

	cargarDirectorioATabla(lista,directorioNuevo);
}

/****************************************************************************************/



