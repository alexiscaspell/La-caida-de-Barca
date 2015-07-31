/*la consola saca los comandos de aca, cuando tengamos que llamar las funciones del FileSystem
desde un nodo, marta, etc. los llamamos desde aca*/

#include "comandosFileSystem.h"

void formateoMDFS (t_list* tabla){

	int i;

	tipoTablaDirectorio* fila;

	for(i=0;i<list_size(tabla);i++){

	fila = list_get(tabla,i);

	eliminarDirectorio(tabla,fila->directorio);

	}

	//printf("Llamada a funcion: Con este comando va a formatear el MDFS\n");
	//formatear();
}


void copiadoLocal_MDFS(int id_file){
	printf("Llamada a funcion: Con este comando va a copiar el archivo local %dtipoTablaDirectorio al MDFS\n", id_file);
	//copiar();
}

void copiadoMDFS_Local(int id_file){
	printf("Llamada a funcion: Con este comando va a  copiar el archivo MDFS %d al filesystem local\n", id_file);
	//copiar();
}

void agregarNodo(int socket){
	//printf("Llamada a funcion: Con este comando va a agregar el nodo %d\n", id_nodo);

	int puerto,socketNodo;

	char* ip= string_new();

	socketNodo = recibirNodo(socket,&puerto,ip);

	//guardarEnTabla(socketNodo,puerto,ip);
	//agregar();
}

int recibirNodo(int socket,int* puerto,char* ip){

	size_t tamanioMensaje;

	int socketNodo = crearSocketParaAceptarSolicitudes(socket);

	void* buffer = recibirBloque(&tamanioMensaje,socketNodo);

	deserializarIntYCadena(puerto,ip,tamanioMensaje,buffer);

	free(buffer);

	return socketNodo;

}

void eliminarNodo(int id_nodo,t_list* listaDeNodos){
	printf("Llamada a funcion: Con este comando va a eliminar el nodo %d\n", id_nodo);
	//eliminar();

	eliminarUnNodoDeDatos(id_nodo,listaDeNodos);
}


void eliminar_archivo(t_list* tabla,char* archivo){

	eliminarDirectorio(tabla,archivo);
}


void mover_archivo(t_list* tabla,char* archivoViejo,char * archivoNuevo){

	moverDirectorio(tabla,archivoViejo,archivoNuevo);
}

void crear_directorio(t_list* tabla,char* directorio){

	cargarDirectorioATabla(tabla,directorio);
}

void eliminar_directorio(t_list* tabla,char* directorio){

eliminarDirectorio(tabla,directorio);
}

void borrar_bloques(t_list* tablaBloques,char* directorio){

	borrarBloques(tablaBloques,directorio);
}

void copiar_bloques(char *id_direct){

	//aca se tiene que pasar int bloque y char* directorio y con eso hacer un
	//agregarBloqueYNodo()

}

void ver_bloques(t_list* tablaBloques,char* archivo){

	verBloquesQueComponenUnArchivo(archivo,tablaBloques);
}


void solicitaMD5(char *id_fileMD5){

}

void renombrar_directorio(t_list* tabla,char* directorioViejo, char* directorioNuevo){

moverDirectorio(tabla,directorioViejo,directorioNuevo);
}

void mover_directorio(t_list* tabla,char* directorioViejo, char* directorioNuevo){
//es lo mismo que renombrarlo!!!

	moverDirectorio(tabla,directorioViejo,directorioNuevo);
}



