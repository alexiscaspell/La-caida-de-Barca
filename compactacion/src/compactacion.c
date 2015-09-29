/*
 ============================================================================
 Name        : compactacion.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commonsDeAsedio/mmap.h>
#include <commons/string.h>

int main(void) {

	FILE* particion = fopen("particion.txt","r+");

//	escribirBloqueMapeado(particion,"tuviejaTUVIEJA",4,32);
//	escribirBloqueMapeado(particion,"tuviejaDAS",10,32);
//	escribirBloqueMapeado(particion,"tuviejaZZZZZ",1,32);

//	escribirBloqueMapeado(particion,"tuviejaZZZZZ",0,32);
//	escribirBloqueMapeado(particion,"sale Borderlands",15,32);
//	escribirBloqueMapeado(particion,"gorenLPM",18,32);

	//fseek(particion,64,SEEK_SET);
	char* asd = string_new();
	fread(asd,96,1,particion);

	//char* bloque = string_duplicate(leerBloqueMapeado(particion,3,32));

	printf("Cadena: %s\n",asd);

	fseek(particion,32*30,SEEK_SET);

	fwrite(asd,32*3,1,particion);








	fclose(particion);
	puts(""); /* prints  */
	return EXIT_SUCCESS;
}
