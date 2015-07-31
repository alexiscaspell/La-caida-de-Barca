#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include "map.h"

int main(void) {

	char *ruta=string_new();

	puts("Ingrese la ruta del archivo a mapear: ");
	scanf("%s",ruta);

	mapArchivo(ruta);

	return EXIT_SUCCESS;
}
