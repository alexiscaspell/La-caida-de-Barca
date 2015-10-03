/*
 ============================================================================
 Name        : prueba.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <commonsDeAsedio/cliente-servidor.h>
#include <commonsDeAsedio/thread.h>
#include <commonsDeAsedio/semaforos.h>

int var = 0;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
sem_t semaforo;

void* sumarUno(){
	//while(var < 1000){

		decrementarSemaforo(&semaforo);
		//bloquearRecurso(&mutex1);
		var++;
		printf("%d\n",var);

		//liberarRecurso(&mutex1);
		incrementarSemaforo(&semaforo);
	//}

	return NULL;
}

void* sumarDiez(){

	//while(var < 1000){
		decrementarSemaforo(&semaforo);
		//bloquearRecurso(&mutex2);
		var +=10;
		printf("%d\n",var);
		//liberarRecurso(&mutex2);
		incrementarSemaforo(&semaforo);
	//}

	return NULL;
}

int main(void) {

	inicializarMutex(&mutex1);
	inicializarMutex(&mutex2);

	inicializarSemaforo(&semaforo,1);

	pthread_t hebra1;
	pthread_t hebra2;

	while(var < 1000){
		crearThread(&hebra1,sumarUno,NULL);
		crearThread(&hebra2,sumarDiez,NULL);
	}





	destruirThread(hebra1);
	destruirThread(hebra2);
	destruirMutex(&mutex1);
	destruirMutex(&mutex2);
	destruirSemaforo(&semaforo);


	puts(""); /* prints  */
	return EXIT_SUCCESS;
}

