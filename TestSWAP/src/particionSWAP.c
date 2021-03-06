/*
 * particionSWAP.c
 *
 *  Created on: 14/9/2015
 *      Author: utnso
 */


#include "particionSWAP.h"


char* recuperarBloque(char* bloqueAVaciar);
char* completarBloque(char* bloqueACompletar, int tamanioDeBloque);
void borrarBloque(char* rutaDeParticion, int numeroDeBloque, int tamanioDeBloque);



//////////////FUNCIONES PARA EL ARCHIVO DE PARTICION///////////////////////


void inicializarParticion(char* nombreDeParticion,int tamanioDePagina,int cantidadDePaginas){

	//FILE* particion;

	char* instruccion = string_new();
	string_append_with_format(&instruccion,"dd if=/dev/zero of=%s bs=%d count=%d > asd",nombreDeParticion,tamanioDePagina,cantidadDePaginas);

	printf("Inicializando particion...\n\n");
	system(instruccion);
	printf("Particion inicializada. \n");

	free(instruccion);

	//particion = fopen(nombreDeParticion,"r+");//modo actualizacion, el archivo debe existir
	//return particion;
}




void escribirBloqueMapeado(char* rutaDeParticion,char* contenidoAEscribir,int numDeBloque, int tamanioDeBloque){

	FILE* archivo = fopen(rutaDeParticion,"r+");

	fseek(archivo,numDeBloque*tamanioDeBloque,SEEK_SET);

	char* aux = completarBloque(contenidoAEscribir,tamanioDeBloque);

	fwrite(aux,tamanioDeBloque,1,archivo);

	//free(aux);

	fclose(archivo);
}

char* leerBloqueMapeado(char* rutaDeParticion,int numDeBloque, int tamanioDeBloque){
	FILE* archivo = fopen(rutaDeParticion,"r+");

	char* leido = string_new();

	fseek(archivo,numDeBloque*tamanioDeBloque,SEEK_SET);

	fread(leido,tamanioDeBloque,1,archivo);

	char* aux = recuperarBloque(leido);

	fclose(archivo);
	free(leido);
	//free(aux);

	return aux;
}


void borrarMProcDeParticion(char* rutaDeParticion,int baseDeMProc,int cantidadDePaginasDeMProc,int tamanioDeBloque){

	int i;
	for (i = 0; i < cantidadDePaginasDeMProc; ++i) {

		borrarBloque(rutaDeParticion,baseDeMProc,tamanioDeBloque);
		baseDeMProc++;
	}
}


/////////AUXILIARES/////////


char* completarBloque(char* bloqueACompletar, int tamanioDeBloque){

	int cantACompletar = tamanioDeBloque - string_length(bloqueACompletar);
	char* aux = string_duplicate(bloqueACompletar);

	string_append(&aux,string_repeat(CARACTER_VACIO,cantACompletar));

	return aux;
}

char* recuperarBloque(char* bloqueAVaciar){
	char* bloque = string_new();
	int i;

	for (i = 0; i < string_length(bloqueAVaciar); ++i) {
		if (bloqueAVaciar[i] == CARACTER_VACIO)
			break;
	}

	bloque = string_substring_until(bloqueAVaciar,i);

	return bloque;
}

void borrarBloque(char* rutaDeParticion, int numeroDeBloque, int tamanioDeBloque){
	escribirBloqueMapeado(rutaDeParticion,"",numeroDeBloque,tamanioDeBloque);
}

