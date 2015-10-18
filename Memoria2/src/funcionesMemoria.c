#include "funcionesMemoria.h"
#include <commonsDeAsedio/estructuras.h>

void destruirConfigMemoria(tipoConfigMemoria* estructuraDeConfiguracion) {
	free(estructuraDeConfiguracion->ipSWAP);
	free(estructuraDeConfiguracion->TLBHabilitada);
	free(estructuraDeConfiguracion);
}

tipoConfigMemoria* crearConfigMemoria() {
	tipoConfigMemoria* cfg = malloc(sizeof(tipoConfigMemoria));
	cfg->ipSWAP = string_new();
	cfg->TLBHabilitada = string_new();

	return cfg;
}

/*********************ESTRUCTURAS*************************/

tipoConfigMemoria* cargarArchivoDeConfiguracionDeMemoria(
		char* rutaDelArchivoDeConfiguracion) {

	t_config* archivoCfg = config_create(rutaDelArchivoDeConfiguracion);
	tipoConfigMemoria* cfg = crearConfigMemoria();

	validarErrorYAbortar(
			config_has_property(archivoCfg, PUERTO_ESCUCHA)
					&& config_has_property(archivoCfg, IP_SWAP)
					&& config_has_property(archivoCfg, PUERTO_SWAP)
					&& config_has_property(archivoCfg,
					MAXIMO_MARCOS_POR_PROCESO)
					&& config_has_property(archivoCfg, CANTIDAD_MARCOS)
					&& config_has_property(archivoCfg, TAMANIO_MARCO)
					&& config_has_property(archivoCfg, ENTRADAS_TLB)
					&& config_has_property(archivoCfg, TLB_HABILITADA)
					&& config_has_property(archivoCfg, RETARDO_MEMORIA),
			"Las claves del archivo de configuracion no coinciden con las que requiere el Administrador de Memoria.");

	cfg->puertoDeEscucha = config_get_int_value(archivoCfg, PUERTO_ESCUCHA);
	cfg->ipSWAP = string_duplicate(
			config_get_string_value(archivoCfg, IP_SWAP));
	cfg->puertoSWAP = config_get_int_value(archivoCfg, PUERTO_SWAP);
	cfg->maximoDeMarcosPorProceso = config_get_int_value(archivoCfg,
	MAXIMO_MARCOS_POR_PROCESO);
	cfg->cantidadDeMarcos = config_get_int_value(archivoCfg, CANTIDAD_MARCOS);
	cfg->tamanioDeMarco = config_get_int_value(archivoCfg, TAMANIO_MARCO);
	cfg->entradasDeTLB = config_get_int_value(archivoCfg, ENTRADAS_TLB);
	cfg->TLBHabilitada = string_duplicate(
			config_get_string_value(archivoCfg, TLB_HABILITADA));
	cfg->retardoDeMemoria = config_get_int_value(archivoCfg, RETARDO_MEMORIA);

	config_destroy(archivoCfg);

	return cfg;
}

void setearEstructuraMemoria(tipoEstructuraMemoria* datos) {

	datosMemoria = datos;

	datosMemoria->listaTablaPaginas = list_create();

	datosMemoria->listaRAM = list_create();

	datosMemoria->listaTLB = list_create();
}

/************************FUNCIONES********************************/

void tratarPeticion(int cpuAtendida) {

	tipoInstruccion* instruccion = recibirInstruccion(cpuAtendida);

	switch (instruccion->instruccion) {
	case INICIAR:
		reservarMemoriaParaProceso(*instruccion, cpuAtendida);
		break;

	case LEER:
		enviarPaginaPedidaACpu(*instruccion,cpuAtendida);
		break;

	case ESCRIBIR:
			escribirPagina(*instruccion,cpuAtendida);
		break;

	case FINALIZAR:
		quitarProceso(*instruccion, cpuAtendida);
		break;
	}
}

void tratarPeticiones() {

	int var;
	for (var = 1; var <= datosMemoria->maximoSocket; ++var) {
		if (FD_ISSET(var, datosMemoria->cpusATratar)&&var!=datosMemoria->socketCpus)
			tratarPeticion(var);
	}
}

/**************INSTRUCCIONES*******************/

//////////////////
//INICIAR
/////////////////
void reservarMemoriaParaProceso(tipoInstruccion instruccion, int cpuATratar) {

	tipoRespuesta respuesta;// = malloc(sizeof(tipoRespuesta));

	if (puedoReservarEnSWAP(instruccion, &respuesta)) {

		printf("pude reservar en swap!!\n");

		tipoTablaPaginas* tablaDePaginasNueva = malloc(sizeof(tipoTablaPaginas));

		printf("pedi instancia de admin\n");

		tablaDePaginasNueva->frames = list_create();

		tablaDePaginasNueva->paginasAsignadas = 0;

		tablaDePaginasNueva->paginasPedidas = instruccion.nroPagina;

		tablaDePaginasNueva->pid = instruccion.pid;

		printf("cree instancia de administracion..\n");

		list_add(datosMemoria->listaTablaPaginas, tablaDePaginasNueva);

		printf("agregue pagina a admin de paginas\n");
	}

	enviarRespuesta(cpuATratar, respuesta);
}

bool puedoReservarEnSWAP(tipoInstruccion instruccion, tipoRespuesta* respuesta) {

	return instruccionASwapRealizada(instruccion, respuesta);
}

//////////////////
//LEER PAGINA
/////////////////

void enviarPaginaPedidaACpu(tipoInstruccion instruccion, int cpuATratar) {

	tipoRespuesta* respuesta = malloc(sizeof(tipoRespuesta));

	int posicionDePag = -1,dondeEstaTabla = buscarTabla(instruccion.pid);

	bool estaEnTLB = true;

	if(dondeEstaTabla>=0){

		if(estaHabilitadaLaTLB())
			posicionDePag = dondeEstaEnTLB(instruccion.nroPagina,instruccion.pid);


		if (posicionDePag<0) {

		posicionDePag = dondeEstaEnTabla(instruccion.nroPagina,instruccion.pid);

		estaEnTLB = false;
		}

		if(posicionDePag<0) {

		 traerPaginaDesdeSwap(instruccion, respuesta);

		 posicionDePag = dondeEstaEnTabla(instruccion.nroPagina,instruccion.pid);
		}


		if(posicionDePag>=0){

		respuesta->respuesta = PERFECTO;

		respuesta->informacion = malloc(datosMemoria->configuracion->tamanioDeMarco);

		respuesta->informacion = traerPaginaDesdeRam(posicionDePag);

		if(!estaEnTLB){
		//agregar a la tlb :'(
		}

				}
		else{
			tipoInstruccion instruccionDeBorrado;

			instruccionDeBorrado.instruccion = FINALIZAR;

			instruccionDeBorrado.pid = instruccion.pid;

			instruccionDeBorrado.nroPagina = 0;

			instruccionDeBorrado.texto = "";

			tipoRespuesta* respuestaSwap;

			if(instruccionASwapRealizada(instruccionDeBorrado,respuestaSwap))
				destruirProceso(instruccion.pid);

			respuesta->respuesta = MANQUEADO;

			respuesta->informacion = "Pagina no encontrada";
		}
	}

	else{
					tipoInstruccion instruccionDeBorrado;

					instruccionDeBorrado.instruccion = FINALIZAR;

					instruccionDeBorrado.pid = instruccion.pid;

					instruccionDeBorrado.nroPagina = 0;

					instruccionDeBorrado.texto = "";

					tipoRespuesta* respuestaSwap;

					if(instruccionASwapRealizada(instruccionDeBorrado,respuestaSwap))
					destruirProceso(instruccion.pid);

					respuesta->respuesta = MANQUEADO;

					respuesta->informacion = "Tabla de paginas no existente";
				}

	enviarRespuesta(cpuATratar, *respuesta);

	}

char* traerPaginaDesdeRam(int direccion){

	sleep(datosMemoria->configuracion->retardoDeMemoria);

	char* pagina = list_get(datosMemoria->listaRAM,direccion);

	return pagina;

}

int buscarTabla(int pid){

int var,posicionDeTabla = -1;

tipoTablaPaginas* tablaActual;

for (var = 0; var < list_size(datosMemoria->listaTablaPaginas); ++var) {

	tablaActual = list_get(datosMemoria->listaTablaPaginas,var);

	if(tablaActual->pid==pid){

		posicionDeTabla = var;

		break;
	}
}

return posicionDeTabla;

}

int dondeEstaEnTabla(int nroPagina, int pid) {

	sleep(datosMemoria->configuracion->retardoDeMemoria);

	int posicionDePagina = -1,posicionDeTabla = buscarTabla(pid);

	tipoTablaPaginas* tablaActual;

	tipoPagina* paginaActual;

	if(posicionDeTabla>=0){

	tablaActual = list_get(datosMemoria->listaTablaPaginas, posicionDeTabla);

		int i;
		for (i = 0; i < list_size(tablaActual->frames); ++i) {

			paginaActual = list_get(tablaActual->frames,i);

			if(paginaActual->numeroDePagina==nroPagina){

				posicionDePagina = i;

				break;
			}

		}

		}

	return posicionDePagina;

}

int dondeEstaEnTLB(int nroPagina, int pid) {

	int var, posicionDePagina = -1;

	tipoTLB* estructuraTLBActual;

	for (var = 0; var < list_size(datosMemoria->listaTLB); ++var) {

		estructuraTLBActual = list_get(datosMemoria->listaRAM, var);

		if (estructuraTLBActual->numeroDePagina == nroPagina&& estructuraTLBActual->pid == pid) {

			posicionDePagina = estructuraTLBActual->posicionEnRAM;

			break;
		}
	}

	return posicionDePagina;
}

void traerPaginaDesdeSwap(tipoInstruccion instruccion, tipoRespuesta* respuesta) {

	//ejecutar algoritmos locos

}

void quitarPaginasDeTLB(int pid) {

	int var, posicionEnTLB;
	for (var = 0; var < cantidadDePaginasAsignadas(pid); ++var) {

		posicionEnTLB = dondeEstaEnTLB(var, pid);

		if (posicionEnTLB >= 0)
			list_remove(datosMemoria->listaTLB,posicionEnTLB);
	}

}
int cantidadDePaginasAsignadas(int pid){

	int var,cantidadDePaginasAsignadas = 0;

	tipoTablaPaginas* tablaActual;
	/*

	tipoAdministracionPaginas* instanciaAdministracionActual;

	for (var = 0; var < list_size(datosMemoria->administradorPaginas); ++var) {

	instanciaAdministracionActual = list_get(datosMemoria->administradorPaginas,var);

	if(instanciaAdministracionActual->pid==pid){

		cantidadDePaginasAsignadas = instanciaAdministracionActual->paginasAsignadas;

		break;
		}
	}*/
	for (var = 0; var < datosMemoria->listaTablaPaginas; ++var) {

		tablaActual = list_get(datosMemoria->listaTablaPaginas,var);

		if(tablaActual->pid==pid){

			cantidadDePaginasAsignadas = tablaActual->paginasAsignadas;
		}

	}

	return cantidadDePaginasAsignadas;

}

void quitarTabla(int pid) {

	int var,dondeEstaTabla = buscarTabla(pid);

	tipoTablaPaginas* tablaDeProcesoABorrar = list_get(datosMemoria->listaTablaPaginas,dondeEstaTabla);

	tipoPagina* paginaActual;

	for (var = 0; var < cantidadDePaginasAsignadas(pid); ++var) {

		paginaActual = list_get(tablaDeProcesoABorrar->frames,var);

		liberarPaginaDeRAM(dondeEstaEnTabla(paginaActual->numeroDePagina,pid));

		free(paginaActual);
	}

	free(tablaDeProcesoABorrar);

	list_remove(datosMemoria->listaTablaPaginas,dondeEstaTabla);
}

void liberarPaginaDeRAM(int posicionEnRam){

	char* pagina = list_get(datosMemoria->listaRAM,posicionEnRam);

	free(pagina);

	list_remove(datosMemoria->listaRAM,posicionEnRam);
}

//////////////////
//ESCRIBIR PAGINA
/////////////////

bool estaHabilitadaLaTLB(){

	return string_equals_ignore_case(datosMemoria->configuracion->TLBHabilitada,"SI");
}

void escribirPagina(tipoInstruccion instruccion,int cpuATratar){

	tipoRespuesta* respuesta = malloc(sizeof(tipoRespuesta));

	int posicionDePag = -1,dondeEstaTabla = buscarTabla(instruccion.pid);

	bool estaEnTLB = true;

	if(dondeEstaTabla>=0){

		tipoTablaPaginas* tablaDeProceso = list_get(datosMemoria->listaTablaPaginas,dondeEstaTabla);

	if(strlen(instruccion.texto)<datosMemoria->configuracion->tamanioDeMarco&&instruccion.nroPagina<=tablaDeProceso->paginasPedidas){

		if(estaHabilitadaLaTLB())
			posicionDePag = dondeEstaEnTLB(instruccion.nroPagina,instruccion.pid);


		if (posicionDePag<0) {

		posicionDePag = dondeEstaEnTabla(instruccion.nroPagina,instruccion.pid);

		estaEnTLB = false;
		}

		if(posicionDePag<0) {

		 traerPaginaDesdeSwap(instruccion, respuesta);

		 posicionDePag = dondeEstaEnTabla(instruccion.nroPagina,instruccion.pid);
		}

		respuesta->respuesta = PERFECTO;

		respuesta->informacion = malloc(datosMemoria->configuracion->tamanioDeMarco);

		memcpy(respuesta->informacion,instruccion.texto,datosMemoria->configuracion->tamanioDeMarco);

		if(posicionDePag>=0){

		char* paginaAModificar = traerPaginaDesdeRam(posicionDePag);

		free(paginaAModificar);

		paginaAModificar = malloc(datosMemoria->configuracion->tamanioDeMarco);

		memcpy(paginaAModificar,instruccion.texto,datosMemoria->configuracion->tamanioDeMarco);

		agregarPagina(instruccion.nroPagina,instruccion.pid,paginaAModificar);

				}
		else{
				tipoPagina* nuevaPagina = malloc(sizeof(tipoPagina));

				nuevaPagina->modificado = false;

				nuevaPagina->numeroDePagina = instruccion.nroPagina;

				nuevaPagina->posicionEnRAM = agregarPaginaARam(instruccion.texto);

				list_add(tablaDeProceso->frames,nuevaPagina);
			}

		if(!estaEnTLB){

		//agregar a la tlb :'(
			}

		}

	else{

		tipoInstruccion instruccionDeBorrado;

		instruccionDeBorrado.instruccion = FINALIZAR;

		instruccionDeBorrado.pid = instruccion.pid;

		instruccionDeBorrado.nroPagina = 0;

		instruccionDeBorrado.texto = "";

		tipoRespuesta* respuestaSwap;

		if(instruccionASwapRealizada(instruccionDeBorrado,respuestaSwap))
		destruirProceso(instruccion.pid);

		respuesta->respuesta = MANQUEADO;

		if(instruccion.nroPagina>tablaDeProceso->paginasPedidas)
		respuesta->informacion = "Tamaño de pagina demasiado grande";

		else
			respuesta->informacion = "Numero de pagina demasiado grande";

		}
	}

				else{

					tipoInstruccion instruccionDeBorrado;

					instruccionDeBorrado.instruccion = FINALIZAR;

					instruccionDeBorrado.pid = instruccion.pid;

					instruccionDeBorrado.nroPagina = 0;

					instruccionDeBorrado.texto = "";

					tipoRespuesta* respuestaSwap;

					if(instruccionASwapRealizada(instruccionDeBorrado,respuestaSwap))
					destruirProceso(instruccion.pid);

					respuesta->respuesta = MANQUEADO;

					respuesta->informacion = "Tabla de paginas no existente";
				}

	enviarRespuesta(cpuATratar, *respuesta);
	}

void agregarPagina(int nroPagina,int pid,char* pagina){


//acordate de modificar bool a true

}

int agregarPaginaARam(char* pagina){

	return PERFECTO;

}

////////////////////
//FINALIZAR PROCESO
////////////////////

bool instruccionASwapRealizada(tipoInstruccion instruccion,tipoRespuesta* respuesta) {

	enviarInstruccion(datosMemoria->socketSWAP, instruccion);

	respuesta = recibirRespuesta(datosMemoria->socketSWAP);

	printf("recibi respuesta de swap\n");

	printf("el estado de respuesta es %c\n",respuesta->respuesta);

	printf("La info de respuesta es: %s\n",respuesta->informacion);

	if(respuesta->respuesta==NULL)
		printf("No se puede leer estado de respuesta\n");

	return (respuesta->respuesta == PERFECTO);
}

void quitarProceso(tipoInstruccion instruccion, int cpuaATratar) {

	tipoRespuesta* respuesta;

	instruccionASwapRealizada(instruccion, respuesta);

	if (respuesta->respuesta == PERFECTO) {

		destruirProceso(instruccion.pid);
	}

	enviarRespuesta(cpuaATratar, *respuesta);
}

void destruirProceso(int pid) {

	quitarTabla(pid);

	quitarPaginasDeTLB(pid);

	//quitarAdministracionDePaginas(pid);
}


/*void quitarAdministracionDePaginas(int pid){

	int var;

	tipoAdministracionPaginas* instanciaAdministracionPagina;

	for (var = 0; var < list_size(datosMemoria->administradorPaginas); ++var) {

		instanciaAdministracionPagina = list_get(datosMemoria->administradorPaginas,var);

		if(instanciaAdministracionPagina->pid==pid){
			list_remove(datosMemoria->administradorPaginas,var);
			break;
		}
	}
}*/
