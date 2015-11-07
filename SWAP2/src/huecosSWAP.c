/*
 * huecosSWAP.c
 *
 *  Created on: 13/9/2015
 *      Author: utnso
 */



#include "huecosSWAP.h"


/*****************Funciones privadas************************/
int espacioEntreDosHuecosUtilizados(tipoHuecoUtilizado* h1, tipoHuecoUtilizado* h2){
	return (h2->base - (h1->base + h1->cantidadDePaginasQueOcupa));
}

tipoHuecoUtilizado* buscarHuecoPorPID(t_list* listaDeHuecosUtilizados,int pidProcesoBuscado){
	tipoHuecoUtilizado* aux;

	int pidBuscada(tipoHuecoUtilizado* huecoUtilizado){
		return pidProcesoBuscado == huecoUtilizado->pid;
	}

	aux = list_find(listaDeHuecosUtilizados,pidBuscada);

	return aux;
}

int traducirDireccionLogicaAFisica(tipoHuecoUtilizado* hueco,int dirLogicaDePagina){
	int direccionFisica;

	direccionFisica = hueco->base + dirLogicaDePagina;

	return direccionFisica;
}

void imprimirListaDeHuecosUtilizados(t_list* lista){
	int i;

//	for (i = 0; i < list_size(lista); ++i) {
//		imprimirHuecoUtilizado(list_get(lista,i));
//	}

	list_iterate(lista,(void*)imprimirHuecoUtilizado);

	puts("\n");
}

void imprimirHuecoUtilizado(tipoHuecoUtilizado* hueco){

	printf("PID: %d   base: %d cantidadDePaginas: %d\n",hueco->pid,hueco->base,hueco->cantidadDePaginasQueOcupa);

}

int cantidadDePaginasOcupadas(t_list* listaDeHuecosOcupados){
	int paginasOcupadas = 0;
	tipoHuecoUtilizado* huecoAux;
	int i;

	if (!list_is_empty(listaDeHuecosOcupados)) {
		for (i = 0; i < list_size(listaDeHuecosOcupados); ++i) {
			huecoAux = list_get(listaDeHuecosOcupados,i);
			paginasOcupadas += huecoAux->cantidadDePaginasQueOcupa;
		}
	}

	return paginasOcupadas;
}

//int indiceDeHuecoAnterior(t_list* listaDeHuecosUtilizados, int baseNuevo){
//	tipoHuecoUtilizado* aux;
//	int i;
//
//	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
//		aux = list_get(listaDeHuecosUtilizados,i);
//		if (baseNuevo == traducirDireccionLogicaAFisica(aux,aux->cantidadDePaginasQueOcupa)) {
//			return i;
//		}
//		else {
//			break;
//		}
//	}
//
//	return -1;
//
//}

bool baseMenor(tipoHuecoUtilizado* hueco1, tipoHuecoUtilizado* hueco2){
	return hueco1->base < hueco2->base;
}



/********************Funciones basicas para la gestion de huecos**********************************/

tipoHuecoUtilizado* crearHuecoUtilizado(int pidProceso,int baseDeProceso,int cantidadDePaginasQueOcupa){

	tipoHuecoUtilizado* aux = malloc(sizeof(tipoHuecoUtilizado));
	aux->pid = pidProceso;
	aux->base = baseDeProceso;
	aux->cantidadDePaginasQueOcupa = cantidadDePaginasQueOcupa;

	return aux;
}

void destruirHuecoUtilizado(tipoHuecoUtilizado* huecoUtilizado){
	free(huecoUtilizado);
}

tipoHuecoLibre* crearHuecoLibre(int base, int cantidadDePaginasQueOcupa){
	tipoHuecoLibre* huecoLibre = malloc(sizeof(tipoHuecoLibre));

	huecoLibre->base = base;
	huecoLibre->cantidadDePaginasQueOcupa = cantidadDePaginasQueOcupa;

	return huecoLibre;
}

void destruirHuecoLibre(tipoHuecoLibre* huecoLibre){
	free(huecoLibre);
}

t_list* inicializarListaDeHuecosUtilizados(){

	t_list*	listaDeHuecosUtilizados = list_create();
	return listaDeHuecosUtilizados;
}

t_list* inicializarListaDeHuecosLibres(int cantidadDePaginasDeSwap){

	t_list* listaDeHuecoLibres = list_create();

	list_add(listaDeHuecoLibres,crearHuecoLibre(0,cantidadDePaginasDeSwap));

	return listaDeHuecoLibres;
}


/*******************Funciones principales que atienden las instrucciones del lenguaje mansisop***************************************/

tipoRespuesta* reservarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProcesoNuevo, int cantPaginasSolicitadas,int cantDePaginasDeSWAP,int tamanioDePagina, char* particion){

	tipoRespuesta* respuestaASolicitudDeReserva;
	int baseParaNuevoPID;


	if (cantidadDePaginasDisponibles(listaDeHuecosLibres) >= cantPaginasSolicitadas) {//puedo reservar la cantidad de paginas que el mproc necesita?

		baseParaNuevoPID = baseParaMProcSiTengoEspacioContiguo(listaDeHuecosLibres,cantPaginasSolicitadas);
		if(baseParaNuevoPID == FALLO){//tengo espacio contiguo para almacenar las paginas que el mprc necesita?

			compactacionAlpha(listaDeHuecosUtilizados,listaDeHuecosLibres,particion,tamanioDePagina);
			tipoHuecoUtilizado* aux = list_get(listaDeHuecosUtilizados,list_size(listaDeHuecosUtilizados)-1);
			baseParaNuevoPID = traducirDireccionLogicaAFisica(aux,aux->cantidadDePaginasQueOcupa);
		}

		asignarEspacio(listaDeHuecosUtilizados,listaDeHuecosLibres,pidProcesoNuevo,cantPaginasSolicitadas,baseParaNuevoPID);

		respuestaASolicitudDeReserva = crearTipoRespuesta(PERFECTO,OK_ESPACIO_RESERVADO);

	}
	else {
		//enviar error a memoria
		//printf("Error al asignar pid: %d, no hay espacio contiguo suficiente para almacenar %d paginas\n",pidProcesoNuevo,cantPaginasSolicitadas);
		respuestaASolicitudDeReserva = crearTipoRespuesta(MANQUEADO,ERROR_NO_HAY_ESPACIO_EN_SWAP);
	}

	return respuestaASolicitudDeReserva;
}

tipoRespuesta* liberarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProceso){

	tipoRespuesta* respuestaASolicitudDeLiberacion;

	int baseDeHuecoLiberado;
	int cantidadDePaginasLiberadas;

	//buscarHuecoDePIDyBorrarHuecoDeLista
	tipoHuecoUtilizado* aux;
	int i;

	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
		aux = list_get(listaDeHuecosUtilizados,i);

		if (aux->pid == pidProceso) {
			baseDeHuecoLiberado = aux->base;
			cantidadDePaginasLiberadas = aux->cantidadDePaginasQueOcupa;
			list_remove_and_destroy_element(listaDeHuecosUtilizados,i,(void*)destruirHuecoUtilizado);

			break;
		}
	}

	actualizarListaDeLibres(listaDeHuecosLibres,baseDeHuecoLiberado,cantidadDePaginasLiberadas);

	respuestaASolicitudDeLiberacion = crearTipoRespuesta(PERFECTO,OK_ESPACIO_LIBERADO);

	return respuestaASolicitudDeLiberacion;
}

tipoRespuesta* leerPagina(t_list* listaDeHuecosUtilizados,int pidProceso,int direccionLogicaDePagina,int tamanioDePaginaDeSwap,char* particion){
	tipoRespuesta* respuestaASolicitudDeLectura;

	char* contenidoDePagina;

	tipoHuecoUtilizado* huecoDelProceso = buscarHuecoPorPID(listaDeHuecosUtilizados,pidProceso);

	int direccionFisicaEnParticion = traducirDireccionLogicaAFisica(huecoDelProceso,direccionLogicaDePagina);

	contenidoDePagina = string_duplicate(leerBloqueMapeado(particion,direccionFisicaEnParticion,tamanioDePaginaDeSwap));

	respuestaASolicitudDeLectura = crearTipoRespuesta(PERFECTO,contenidoDePagina);

	return respuestaASolicitudDeLectura;
}

tipoRespuesta* escribirPagina(t_list* listaDeHuecosUtilizados,int pidProceso,char* contenidoAEscribir,int direccionLogicaDePagina,int tamanioDePaginaDeSwap, char* particion){

	tipoRespuesta* respuestaASolicitudDeEscritura;

	tipoHuecoUtilizado* huecoDelProceso = buscarHuecoPorPID(listaDeHuecosUtilizados,pidProceso);

	int direccionFisicaEnParticion = traducirDireccionLogicaAFisica(huecoDelProceso,direccionLogicaDePagina);

	escribirBloqueMapeado(particion,contenidoAEscribir,direccionFisicaEnParticion,tamanioDePaginaDeSwap);

	respuestaASolicitudDeEscritura = crearTipoRespuesta(PERFECTO,OK_PAGINA_ESCRITA);

	return respuestaASolicitudDeEscritura;
}




/*****************Funciones secundarias*********************************/
void asignarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProceso,int cantidadDePaginasSolicitadas,int base){

	tipoHuecoUtilizado* huecoNuevo = crearHuecoUtilizado(pidProceso,base,cantidadDePaginasSolicitadas);
	tipoHuecoLibre* huecoLibre;

	int buscarHuecoPorBase (tipoHuecoLibre* aux){
		return aux->base == base;
	}
	huecoLibre = list_find(listaDeHuecosLibres,buscarHuecoPorBase);

	huecoLibre->base = base + cantidadDePaginasSolicitadas;
	huecoLibre->cantidadDePaginasQueOcupa -= cantidadDePaginasSolicitadas;

	list_add(listaDeHuecosUtilizados,huecoNuevo);
	list_sort(listaDeHuecosUtilizados,(void*)baseMenor);
}
//OK

int cantidadDePaginasDisponibles(t_list* listaDeHuecosLibres){
	int paginasLibres = 0;

	void contarPaginasLibres(tipoHuecoLibre* huecoLibre){
		paginasLibres += huecoLibre->cantidadDePaginasQueOcupa;
	}
	list_iterate(listaDeHuecosLibres,(void*)contarPaginasLibres);

	return paginasLibres;
}
//OK

int baseParaMProcSiTengoEspacioContiguo(t_list* listaDeHuecosLibres, int cantidadDePaginasSolicitadas){

	tipoHuecoLibre* aux;

	int puedoAlmacenarMProc(tipoHuecoLibre* huecoLibre){
		return cantidadDePaginasSolicitadas <= huecoLibre->cantidadDePaginasQueOcupa;
	}

	aux = list_find(listaDeHuecosLibres,puedoAlmacenarMProc);

	if (aux != NULL) {
		return aux->base;
	}
	else {
		return FALLO;
	}
}
//OK


int direccionFisicaDeUltimaPaginaDeHuecoLibre(tipoHuecoLibre* huecoLibre){
	return traducirDireccionLogicaAFisica(huecoLibre,huecoLibre->cantidadDePaginasQueOcupa - 1);
}
//OK

bool buscarHuecoContiguoA(tipoHuecoLibre* huecoLibre, int* indiceDeHuecoAnterior, int* indiceDeHuecoPosterior, t_list* listaDeHuecoLibres){

	tipoHuecoLibre* aux;

	int i;
	for (i = 0; i < list_size(listaDeHuecoLibres); ++i) {
		aux = list_get(listaDeHuecoLibres,i);

		if (huecoLibre->base == direccionFisicaDeUltimaPaginaDeHuecoLibre(aux) + 1) {
			*indiceDeHuecoAnterior = i;
		}
		else {
			*indiceDeHuecoAnterior = -1;
		}

		if (aux->base == direccionFisicaDeUltimaPaginaDeHuecoLibre(huecoLibre) + 1) {
			*indiceDeHuecoPosterior = i;
		}
		else {
			*indiceDeHuecoPosterior = -1;
		}
	}

	return (*indiceDeHuecoAnterior || *indiceDeHuecoPosterior);
}
//OK

void unirHuecosLibres(tipoHuecoLibre* huecoNuevo, int indiceAnterior, int indicePosterior, t_list* listaDeHuecosLibres){
	int baseNueva = huecoNuevo->base;
	int cantidadDePaginasLibres = huecoNuevo->cantidadDePaginasQueOcupa;
	tipoHuecoLibre *aux1, *aux2;


	if (indiceAnterior != -1) {
		aux1 = list_get(listaDeHuecosLibres,indiceAnterior);
		baseNueva = aux1->base;
		cantidadDePaginasLibres += aux1->cantidadDePaginasQueOcupa;
		list_remove_and_destroy_element(listaDeHuecosLibres,indiceAnterior,(void*)destruirHuecoLibre);
	}

	if (indicePosterior != -1) {
		aux2 = list_get(listaDeHuecosLibres,indicePosterior);
		cantidadDePaginasLibres += aux2->cantidadDePaginasQueOcupa;
		list_remove_and_destroy_element(listaDeHuecosLibres,indicePosterior,(void*)destruirHuecoLibre);
	}

	huecoNuevo->base = baseNueva;
	huecoNuevo->cantidadDePaginasQueOcupa = cantidadDePaginasLibres;
}
//OK

void actualizarListaDeLibres(t_list* listaDeHuecosLibres, int baseDeHuecoLiberado, int cantidadDePaginasLiberadas){

	bool hayHuecosLibresContiguos;
	int indiceHuecoAnterior,indiceHuecoPosterior;
	tipoHuecoLibre* huecoLibreNuevo = crearHuecoLibre(baseDeHuecoLiberado,cantidadDePaginasLiberadas);

	hayHuecosLibresContiguos = buscarHuecoContiguoA(huecoLibreNuevo,&indiceHuecoAnterior,&indiceHuecoPosterior,listaDeHuecosLibres);
	if (hayHuecosLibresContiguos) {

		unirHuecosLibres(huecoLibreNuevo,indiceHuecoAnterior,indiceHuecoPosterior,listaDeHuecosLibres);
	}

	list_add(listaDeHuecosLibres,huecoLibreNuevo);
	//ordenar
}
//falta ordenar


//void insertarHuecoOrdenado(void* hueco,void* listaDeHuecos,int (*estaEntre) (void*,void*,void*)){
//	int indiceNuevo;
//
//	if (list_is_empty(listaDeHuecos)) {
//		indiceNuevo = 0;
//	}
//	else if(list_size(listaDeHuecos) == 1){
//		indiceNuevo = 1;
//	}
//	else {
//		void *aux1,*aux2;
//
//		int i;
//		for (i = 0; i < list_size(listaDeHuecos) - 1; ++i) {
//			aux1 = list_get(listaDeHuecos,i);
//			aux2 = list_get(listaDeHuecos,i+1);
//
//			if (estaEntre(hueco,aux1,aux2)) {
//				indiceNuevo = i + 1;
//				break;
//			}
//		}
//
//		if (i == list_size(listaDeHuecos) - 1) {
//			indiceNuevo = list_size(listaDeHuecos);
//		}
//	}
//
//	list_add_in_index(listaDeHuecos,indiceNuevo,hueco);
//}

//int huecoLibreEstaEntre(tipoHuecoLibre* hueco,tipoHuecoLibre* posibleHuecoAnterior,tipoHuecoLibre* posibleHuecoPosterior){
//
//}

void compactacionAlpha(t_list* listaDeHuecosUtilizados, t_list* listaDeHuecosLibres, char* particion,int tamanioDePaginaDeSwap){
	tipoHuecoUtilizado* hueco;
	int ultimaPaginaEscrita = 0;


	int i;
	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
		hueco = list_get(listaDeHuecosUtilizados,i);

		moverHueco(hueco,particion,ultimaPaginaEscrita,tamanioDePaginaDeSwap);
		ultimaPaginaEscrita = traducirDireccionLogicaAFisica(hueco,hueco->cantidadDePaginasQueOcupa);
	}
}

void moverHueco(tipoHuecoUtilizado* hueco,char* particion, int ultimaPaginaEscrita,int tamanioDePagina){
	int direccionFisicaBase = traducirDireccionLogicaAFisica(hueco,hueco->base);
	hueco->base = ultimaPaginaEscrita;

	int dirFisicaVieja;
	int dirFisicaNueva;

	int i;
	for (i = 0; i < hueco->cantidadDePaginasQueOcupa; ++i) {


		dirFisicaVieja = direccionFisicaBase + i;
		dirFisicaNueva = traducirDireccionLogicaAFisica(hueco,i);

		moverPagina(particion,dirFisicaVieja,dirFisicaNueva,tamanioDePagina);
	}

	hueco->base = ultimaPaginaEscrita;


}

void moverPagina(char* particion, int dirFisVieja, int dirFisNueva,int tamanioDePagina){

	char* pagina = leerBloqueMapeado(particion,dirFisVieja,tamanioDePagina);
	escribirBloqueMapeado(particion,pagina,dirFisNueva,tamanioDePagina);

}




