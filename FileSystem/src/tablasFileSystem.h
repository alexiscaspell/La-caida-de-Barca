/*
 * tablasFileSystem.h
 *
 *  Created on: 28/6/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include <commons/collections/list.h>

#ifndef TABLASFILESYSTEM_H_
#define TABLASFILESYSTEM_H_

typedef struct tipoTablaDirectorio{
	char* directorio;
	int padre;
}tipoTablaDirectorio;

typedef struct{
	int nombreArchivo;
	int bloqueFS;
	t_list* listaNodos;
}tipoBloquesFS;

typedef struct{
	int socket;
	char* ip;
	int puerto;
	char estado;
}tipoInfoNodos;

/*typedef struct{
	char* rutaArchivo;
	int nombre;
}tipoInfoArchivo;*/


int nombreArchivoInt(char* archivo, t_list* tablaArchivos);

void imprimirListaNodosDeTipoBloqueFS(t_list* listaNodos);

void verBloquesQueComponenUnArchivo(char* archivo, t_list* tablaArchivos);

void eliminarUnNodoDeDatos(int nombreNodo, t_list* tablaInfoNodos);

bool equalsTipoBloqueFS(tipoBloquesFS bloque1,tipoBloquesFS bloque2);

int dondeEstaTipoBloqueFS(t_list* tablaBloquesFS,tipoBloquesFS bloque);

bool contieneTipoBloqueFS(t_list* tablaBloquesFS,tipoBloquesFS bloque);

void agregarBloqueYNodo(t_list* tablaBloquesFS,t_list* listaArchivos,char* rutaArchivo,int nroBloque,int nodo);

void borrarTipoBloqueFS(t_list* tablaBloquesFS,char* rutaArchivo,int bloque);

int minimoEntre(int x,int y);

void borrarBloques(t_list* tablaBloques,char* directorio);



/************************Funciones tabla de direcciones*********************************/

bool equals(tipoTablaDirectorio fila1,tipoTablaDirectorio fila2);

bool existeDirectorio(t_list* lista,char* directorio,int padre);

int dondeEstaDirectorio(t_list* lista,char* directorio,int padre);

void cargarDirectorioATabla(t_list* tablaDirectorios, char* direccionDirectorio);

void agregarALista(t_list* lista,char* directorio,int padre);

void imprimirTablaDirectorio(t_list* tablaDirectorios);

int buscarDirectorio(t_list* lista,char* directorioCompleto);

void eliminarDirectorio(t_list* lista,char* directorioCompleto);

void moverDirectorio(t_list* lista,char* directorioViejo,char*directorioNuevo);

/****************************************************************************************/


#endif /* TABLASFILESYSTEM_H_ */
