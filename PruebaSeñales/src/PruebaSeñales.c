#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


/*FERNILANDIA*/
int global = 1;


void paraDeImprimir(){
	printf("\n");
	printf("\n");
	printf("me tiraste la señal\n");
	global = 0;
}

int main(void) {

	sigset_t set; //inicializo
	sigemptyset(&set); // es como un new
	sigaddset(&set, SIGINT); //le asigno el tipo de señal

	signal(SIGINT, paraDeImprimir);

	while(global){
		printf("estoy imprimiendo mucho wiiiii....\n");
	}


	return EXIT_SUCCESS;
}
