#include <stdio.h>
#include <stdlib.h>
#include "funcionesSWAP.h"

int main(void) {

	tipoConfigSWAP* configuracion = cargarArchivoDeConfiguracionDeSWAP("/home/utnso/Escritorio/cfgSWAP");
	
	//LPM negro me cago en tus pruebas forronan



	destruirConfigSWAP(configuracion);

	return EXIT_SUCCESS;
}
