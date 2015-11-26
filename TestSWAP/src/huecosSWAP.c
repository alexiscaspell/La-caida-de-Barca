/*
 * huecosSWAP.c
 *
 *  Created on: 13/9/2015
 *      Author: utnso
 */



#include "huecosSWAP.h"


///////////////FUNCIONES PRIVADAS//////////////

void imprimirHuecoUtilizado(tipoHuecoUtilizado* hueco);
void imprimirHuecoLibre(tipoHuecoLibre* hueco);



tipoHuecoUtilizado* buscarHuecoUtilizadoPorPID(t_list* listaDeHuecosUtilizados,int pidProcesoBuscado){
	tipoHuecoUtilizado* aux;
	int i;

	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
		aux = list_get(listaDeHuecosUtilizados,i);
		if(aux->pid == pidProcesoBuscado)
			break;
	}

	return aux;
}


int traducirDireccionLogicaAFisica(tipoHuecoUtilizado* hueco,int dirLogicaDePagina){
	int direccionFisica;

	direccionFisica = hueco->baseDeMProc + dirLogicaDePagina;

	return direccionFisica;
}

void imprimirListaDeHuecosUtilizados(t_list* lista){
	int i;

	for (i = 0; i < list_size(lista); ++i) {
		imprimirHuecoUtilizado(list_get(lista,i));
	}

	puts("\n");
}

void imprimirHuecoUtilizado(tipoHuecoUtilizado* hueco){

	printf("PID: %d   Base: %d CantidadDePaginas: %d\n",hueco->pid,hueco->baseDeMProc,hueco->cantidadDePaginasQueOcupa);

}

void imprimirHuecoLibre(tipoHuecoLibre* hueco){
	printf("Base: %d CantidadDePaginas: %d\n",hueco->base,hueco->cantidadDePaginasQueOcupa);
}

void imprimirListaDeHuecosLibres(t_list* lista){
	list_iterate(lista,(void*)imprimirHuecoLibre);

	puts("\n");
}

bool baseMenor(tipoHuecoUtilizado* h1, tipoHuecoUtilizado* h2){
	return h1->baseDeMProc < h2->baseDeMProc;
}

bool baseMenorDeHuecoLibre(tipoHuecoLibre* unHueco, tipoHuecoLibre* otroHueco){
	return unHueco->base < otroHueco->base;
}

/////////////////////LISTAS DE HUECOS///////////////////////
tipoHuecoUtilizado* crearHuecoUtilizado(int pidProceso,int inicio,int cantidadDePaginas){

	tipoHuecoUtilizado* aux = malloc(sizeof(tipoHuecoUtilizado));
	aux->pid = pidProceso;
	aux->baseDeMProc = inicio;
	aux->cantidadDePaginasQueOcupa = cantidadDePaginas;

	return aux;
}

void destruirHuecoUtilizado(tipoHuecoUtilizado* huecoUtilizado){
	free(huecoUtilizado);
}

t_list* inicializarListaDeHuecosUtilizados(){

	t_list*	listaDeHuecosUtilizados = list_create();
	return listaDeHuecosUtilizados;
}

int ultimaPaginaEscrita(t_list* listaDeHuecosUtilizados){
	tipoHuecoUtilizado* aux = list_get(listaDeHuecosUtilizados,list_size(listaDeHuecosUtilizados)-1);
	return traducirDireccionLogicaAFisica(aux,aux->cantidadDePaginasQueOcupa);
}

//----------------Huecos libres---------------

tipoHuecoLibre* crearHuecoLibre(int base, int cantidadDePaginas){
	tipoHuecoLibre* huecoLibre = malloc(sizeof(tipoHuecoLibre));

	huecoLibre->base = base;
	huecoLibre->cantidadDePaginasQueOcupa = cantidadDePaginas;

	return huecoLibre;
}

void destruirHuecoLibre(tipoHuecoLibre* huecoLibre){
	free(huecoLibre);
}

t_list* inicializarListaDeHuecosLibres(int cantidadDePaginas){
	t_list* listaDeHuecosLibres = list_create();

	list_add(listaDeHuecosLibres,crearHuecoLibre(0,cantidadDePaginas));

	return listaDeHuecosLibres;
}

int cantidadDePaginasDisponibles(t_list* listaDeHuecosLibres){
	int paginasLibres = 0;

	if (list_size(listaDeHuecosLibres) != 0) {
		void contarPaginasLibres(tipoHuecoLibre* huecoLibre){
			paginasLibres += huecoLibre->cantidadDePaginasQueOcupa;
		}
		list_iterate(listaDeHuecosLibres,(void*)contarPaginasLibres);
	}

	return paginasLibres;

}

bool tengoEspacioContiguo(t_list* listaDeHuecosLibres, int cantidadDePaginasSolicitadas, int* baseParaNuevoProceso){
	tipoHuecoLibre* aux;

	int puedoAlmacenarProceso(tipoHuecoLibre* huecoLibre){
		return cantidadDePaginasSolicitadas <= huecoLibre->cantidadDePaginasQueOcupa;
	}

	aux = list_find(listaDeHuecosLibres,(void*)puedoAlmacenarProceso);

	if (aux != NULL) {
		*baseParaNuevoProceso = aux->base;
		return true;
	}
	else {
		return false;
	}
}

void ocuparEspacioLibre(t_list* listaDeHuecosLibres, int baseOcupada, int cantidadDePaginasSolicitadas){

	tipoHuecoLibre* huecoLibre;

	int var;
	for (var = 0; var < list_size(listaDeHuecosLibres); ++var) {
		huecoLibre = list_get(listaDeHuecosLibres,var);

		if (huecoLibre->cantidadDePaginasQueOcupa == cantidadDePaginasSolicitadas) {
			list_remove_and_destroy_element(listaDeHuecosLibres,var,(void*)destruirHuecoLibre);
		}
		else {
			huecoLibre->base = baseOcupada + cantidadDePaginasSolicitadas;
			huecoLibre->cantidadDePaginasQueOcupa -= cantidadDePaginasSolicitadas;
		}
	}

}


//------------Funciones auxiliares para reservar espacio------------//
void asignarEspacio(t_list* listaDeHuecosUtilizados, t_list* listaDeHuecosLibres, int pidProceso,int base,int cantDePaginasSolicitadas){

	tipoHuecoUtilizado* huecoNuevo = crearHuecoUtilizado(pidProceso,base,cantDePaginasSolicitadas);
	list_add(listaDeHuecosUtilizados,huecoNuevo);
	list_sort(listaDeHuecosUtilizados,(void*)baseMenor);

	ocuparEspacioLibre(listaDeHuecosLibres,base,cantDePaginasSolicitadas);
}

void compactacionAlpha(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres, char* particion, int tamanioDePagina, int cantidadDePaginasSWAP, int retardoDeCompactacion, t_log* logger){
	tipoHuecoUtilizado* hueco;
	int ultimaPaginaEscrita = 0;

	logearSeguimiento("Compactacion iniciada por fragmentacion externa",logger);

	sleep(retardoDeCompactacion);

	int i;
	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
		hueco = list_get(listaDeHuecosUtilizados,i);
		moverHueco(hueco,particion,ultimaPaginaEscrita,tamanioDePagina);
		ultimaPaginaEscrita = traducirDireccionLogicaAFisica(hueco,hueco->cantidadDePaginasQueOcupa);
	}

	list_destroy_and_destroy_elements(listaDeHuecosLibres,(void*)destruirHuecoLibre);
	list_add(listaDeHuecosLibres,crearHuecoLibre(ultimaPaginaEscrita + 1, cantidadDePaginasSWAP - ultimaPaginaEscrita - 1));

	logearSeguimiento("Compactacion finalizada",logger);

}

void moverHueco(tipoHuecoUtilizado* hueco,char* particion, int ultimaPaginaEscrita,int tamanioDePagina){
	int direccionFisicaBase = traducirDireccionLogicaAFisica(hueco,hueco->baseDeMProc);
	hueco->baseDeMProc = ultimaPaginaEscrita;

	int dirFisicaVieja;
	int dirFisicaNueva;

	int i;
	for (i = 0; i < hueco->cantidadDePaginasQueOcupa; ++i) {


		dirFisicaVieja = direccionFisicaBase + i;
		dirFisicaNueva = traducirDireccionLogicaAFisica(hueco,i);

		moverPagina(particion,dirFisicaVieja,dirFisicaNueva,tamanioDePagina);
	}

	hueco->baseDeMProc = ultimaPaginaEscrita;


}

void moverPagina(char* particion, int dirFisVieja, int dirFisNueva,int tamanioDePagina){

	char* pagina = leerBloqueMapeado(particion,dirFisVieja,tamanioDePagina);
	escribirBloqueMapeado(particion,pagina,dirFisNueva,tamanioDePagina);

}


//------------Funciones auxiliares para liberar espacio----------//

void liberarProcesoDeListaDeHuecosUtilizadosYDeParticion(t_list* listaDeHuecosUtilizados,char* rutaDeParticion, int tamanioDePagina, int pidALiberar, int* base, int* cantidadDePaginas){
	tipoHuecoUtilizado* aux;
	int i;
	for (i = 0; i < list_size(listaDeHuecosUtilizados); ++i) {
		aux = list_get(listaDeHuecosUtilizados,i);
		if (aux->pid == pidALiberar) {

			*base = aux->baseDeMProc;
			*cantidadDePaginas = aux->cantidadDePaginasQueOcupa;

			borrarMProcDeParticion(rutaDeParticion,*base,*cantidadDePaginas,tamanioDePagina);
			list_remove_and_destroy_element(listaDeHuecosUtilizados,i,(void*)destruirHuecoUtilizado);
		}
	}
}

void actualizarListaDeHuecosLibres(t_list* listaDeHuecosLibres, int base, int cantidadDePaginas){

	tipoHuecoLibre* huecoLibreNuevo = crearHuecoLibre(base,cantidadDePaginas);
	tipoHuecoLibre* aux;

	int indiceAnterior = -1;
	int indicePosterior = -1;

	int var;
	for (var = 0; var < list_size(listaDeHuecosLibres); ++var) {
		aux = list_get(listaDeHuecosLibres,var);

		if (sonContiguosAIzquierda(huecoLibreNuevo,aux)) {
			unirHuecosLibresAIzquierda(huecoLibreNuevo,aux);
			indiceAnterior = var;
		}


		if(sonContiguosADerecha(huecoLibreNuevo,aux)){
			unirHuecosLibresADerecha(huecoLibreNuevo,aux);
			indicePosterior = var;
		}
	}

	if (indiceAnterior != -1) {
		list_remove_and_destroy_element(listaDeHuecosLibres,indiceAnterior,(void*)destruirHuecoLibre);
	}
	if(indicePosterior != -1){
		list_remove_and_destroy_element(listaDeHuecosLibres,indicePosterior,(void*)destruirHuecoLibre);
	}

	list_add(listaDeHuecosLibres,huecoLibreNuevo);
	list_sort(listaDeHuecosLibres,(void*)baseMenorDeHuecoLibre);

}


int ultimaPaginaDeHuecoLibre(tipoHuecoLibre* unHueco){
	return unHueco->base + unHueco->cantidadDePaginasQueOcupa - 1;
}

bool sonContiguosAIzquierda(tipoHuecoLibre* unHueco, tipoHuecoLibre* otroHueco){
	return ultimaPaginaDeHuecoLibre(otroHueco) + 1 == unHueco->base;
}

bool sonContiguosADerecha(tipoHuecoLibre* unHueco, tipoHuecoLibre* otroHueco){
	return ultimaPaginaDeHuecoLibre(unHueco) + 1 == otroHueco->base;
}


void unirHuecosLibresAIzquierda(tipoHuecoLibre* unHueco, tipoHuecoLibre* otroHueco){
	unHueco->base = otroHueco->base;
	unHueco->cantidadDePaginasQueOcupa += otroHueco->cantidadDePaginasQueOcupa;
}

void unirHuecosLibresADerecha(tipoHuecoLibre* unHueco, tipoHuecoLibre* otroHueco){
	unHueco->cantidadDePaginasQueOcupa += otroHueco->cantidadDePaginasQueOcupa;
}

