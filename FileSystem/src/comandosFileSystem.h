#ifndef COMANDOSFILESYSTEM_H_
#define COMANDOSFILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <string.h>
#include <unistd.h>
#include <commonsDeAsedio/cliente-servidor.h>
#include "comandosFileSystem.h"
#include "tablasFileSystem.h"

void formateoMDFS ();

void copiadoLocal_MDFS(int id_file);

void copiadoMDFS_Local(int id_file);

void agregarNodo(int id_nodo);

void eliminarNodo(int id_nodo,t_list* listaDeNodos);

void eliminar_archivo(t_list* tabla,char *id_file);

void mover_archivo(t_list* tabla,char *id_file,char * id_fileRuta);

void ejecutarAccionDirOArch(char* comando,char* directorio);

int recibirNodo(int socket,int* puerto,char* ip);

#endif /* COMANDOSFILESYSTEM_H_ */
