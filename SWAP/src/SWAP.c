#include <stdio.h>
#include <stdlib.h>
#include "funcionesSWAP.h"

int main(void) {

	tipoConfigSWAP* configuracion = cargarArchivoDeConfiguracionDeSWAP("/home/utnso/Escritorio/cfgSWAP");

	puts("tuvieja");
	puts("tujavie");
	puts("tujavier");

	destruirConfigSWAP(configuracion);


	puts("No se permiten gorens, solo se permite uno.");

	return EXIT_SUCCESS;
}
