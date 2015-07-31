#include "consolaFileSystem.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <commons/string.h>
#include "comandosFileSystem.h"


void ejecutaComando(int comando,t_list* tablaDirectorios){
	int parametro =-1;
	char *id_file[100];
	char *id_bloque;
	char *id_nodo;
	char *id_f;
	char *id_n;

	//t_list* tablaDirectorios = list_create();

	//creo variables para que no tiren error
	char id_fileNuevo[100];
	char id_fileRuta[100];
	char* id_direct[100] ;//= string_new();
	char id_directNuevo[100];
	char id_directRuta[100];
	char*id_fileMDFS;
	char*id_fileMD5;
	int socketFS;
	char teclaReturn;

	t_list* tablaNodos = list_create();//variable para q compile prob. se tiene q pasar
                                       //desde hilo principal
	switch(comando){

	case 1:
		system("clear");
		printf("Inicia formateo de MDFS\n");
		formateoMDFS();
	break;

	case 2:
		system("clear");
		printf("********OPERACIONES SOBRE ARCHIVOS*********\n");
		printf("1:Eliminar archivo\n");
		printf("2:Renombrar archivo\n");
		printf("3:Mover archivo\n");
		//Operacion - pedir tipo de operacion y parametros
		parametro = -1;
		scanf("%d",&parametro);

		while(parametro !=1 && parametro !=2 && parametro !=3) {
			printf("Parámetro inválido");
		}
		switch(parametro){
			case 1:
				//eliminar archivo
				printf("Ingrese nombre de archivo... Se eliminará dicho archivo\n");
				scanf("%s",id_file);
				eliminar_archivo(tablaDirectorios,id_file);
			break;
			case 2:
				//renombrar archivo
				printf("Ingrese nombre de archivo\n");
				scanf("%s",id_file);
				printf("Ingrese  nuevo nombre de archivo\n");
				scanf("%s",id_fileNuevo);
			break;
			case 3:
				//mover archivo
				printf("Ingrese nombre de archivo\n");
				scanf("%s",id_file);
				printf("Ingrese  nueva ruta de archivo\n");
				scanf("%s",id_fileRuta);
				mover_archivo(tablaDirectorios,id_file, id_fileRuta);
			break;
		break;
		}
	break;

	case 3:
		system("clear");
		printf("========OPERACIONES SOBRE DIRECTORIOS=========\n");
		printf("1:Crear directorio\n");
		printf("2:Eliminar directorio\n");
		printf("3:Renombrar directorio\n");
		printf("4:Mover directorio\n");
		//Operacion - pedir tipo de operacion y parametros
		parametro = -1;
		scanf("%d",&parametro);

		while(parametro !=1 && parametro !=2 && parametro !=3 && parametro !=4) {
			printf("Parámetro inválido");
		}
		switch(parametro){
			case 1:
				//crear directorio
				printf("Ingrese nuevo directorio... Se creará un directorio nuevo\n");
				scanf("%s",id_direct);
				crear_directorio(tablaDirectorios,id_direct);
			break;
			case 2:
				//eliminar directorio
				printf("Ingrese nombre de directorio... Se eliminará dicho directorio\n");
				scanf("%s",id_direct);
				eliminar_directorio(tablaDirectorios,id_direct);
			break;
			case 3:
				//renombrar directorio
				printf("Ingrese nombre de directorio\n");
				scanf("%s",id_direct);
				printf("Ingrese nuevo nombre de directorio\n");
				scanf("%s",id_directNuevo);
				renombrar_directorio(tablaDirectorios,id_direct, id_directNuevo);
			break;
			case 4:
				//mover directorio
				printf("Ingrese nombre de directorio\n");
				scanf("%s",id_direct);
				printf("Ingrese nueva ruta de directorio\n");
				scanf("%s",id_directRuta);
				mover_directorio(tablaDirectorios,id_direct, id_directRuta);
			break;
		break;
		}
	break;

	case 4:
		system("clear");
		printf("Ingrese un archivo local\n");
		scanf("%s",id_file);
		printf("Inicia copiado de archivo local a MDFS");
		copiadoLocal_MDFS(id_file);
	break;

	case 5:
		system("clear");
		printf("Ingrese un archivo del MDFS\n");
		scanf("%s",id_fileMDFS);
		printf("Inicia copiado de archivo MDFS al filesystem local\n");
		crear_directorio(id_direct);
		copiadoMDFS_Local(id_fileMDFS);
	break;

	case 6:
		system("clear");
		printf("Ingrese el MD5 del archivo MDFS\n");
		crear_directorio(id_direct);
		scanf("%s",id_fileMD5);
		solicitaMD5(id_fileMD5);
	break;

	case 7:
		system("clear");
		printf("*********OPERACIONES SOBRE BLOQUES*********\n");
		printf("1:Ver bloques\n");
		printf("2:Borrar bloques\n");
		printf("3:Copiar bloques\n");
		//Operacion - pedir tipo de operacion y parametros
		parametro = -1;
		scanf("%d",&parametro);

		while(parametro !=1 && parametro !=2 && parametro !=3 && parametro !=4)	{
			printf("Parámetro inválido");
		}
		switch(parametro){
			case 1:
				//ver bloques
				printf("Ingrese archivo... Se mostrarán los bloques del mismo\n");
				scanf("%d",id_file);
				ver_bloques(id_file);
			break;
			case 2:
				//borrar bloques
				printf("Ingrese archivo... Se borrarán los bloques del mismo\n");
				scanf("%d",id_file);
				borrar_bloques(id_file);
			break;
			case 3:
				//copiar bloques
				printf("Ingrese archivo... Se copiarán los bloques del mismo\n");
				scanf("%d",id_file);
				copiar_bloques(id_file);
			break;
		}
		break;

	case 8:
		system("clear");
		printf("Inicia solicitud de agregar un nuevo nodo\n");
		//scanf("%d",id_nodo);
		agregarNodo(socketFS);//id_nodo);Supongo que deberia enviar socketFS
	break;

	case 9:
		system("clear");
		printf("Inicia solicitud de eliminar un nodo\n");
		scanf("%d",id_nodo);
		eliminarNodo(id_nodo,tablaNodos);
	break;

	case 10:
		system("clear");
		imprimirTablaDirectorio(tablaDirectorios);
		getchar();
		//sleep(10);
        scanf("%c",&teclaReturn);
		system("clear");
		break;

	}

	//consola();
}

void* consola(t_list* tablaDeDirectorios){
	  int operacion = -1;

	  while(operacion != 0){
		  system("clear");
		  puts("Bienvenido a la Consola del FILE SYSTEM\n"
			   "=======================================\n"
				"\n"
				"Ingrese una opción válida:\n"
				"\n"
				"1 -Formatear el MDFS\n"
				"2 -Eliminar/Renombrar/Mover archivos\n"
				"3 -Crear/Eliminar/Renombrar/Mover directorios\n"
				"4 -Copiar un archivo local al MDFS\n"
				"5 -Copiar un archivo del MDFS al filesystem local\n"
				"6 -Solicitar el MD5 de un archivo en MDFS\n"
				"7 -Ver/Borrar/Copiar los bloques que componen un archivo\n"
				"8 -Agregar un nodo de datos\n"
				"9 -Eliminar un nodo de datos\n"
				"10 -Mostrar tabla FileSystem\n"
				"\n"
				"Para salir presione 0");
		  scanf("%d",&operacion);
		  ejecutaComando(operacion,tablaDeDirectorios);
	  }
	  return 0;
}
