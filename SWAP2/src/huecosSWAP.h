/*
 * huecosSWAP.h
 *
 *  Created on: 13/9/2015
 *      Author: utnso
 */

#ifndef SWAP2_SRC_HUECOSSWAP_H_
#define SWAP2_SRC_HUECOSSWAP_H_

#include "estructurasSWAP.h"
#include "particionSWAP.h"

#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <commonsDeAsedio/mmap.h>
#include <commonsDeAsedio/estructuras.h>

#include <commons/string.h>


#define ERROR_NO_HAY_ESPACIO_EN_SWAP "No se pudo asignar la pagina. No hay espacio disponible en SWAP"
#define OK_ESPACIO_RESERVADO ""
#define OK_ESPACIO_LIBERADO ""
#define OK_PAGINA_ESCRITA ""
#define FALLO -1

bool baseMenor(tipoHuecoUtilizado* h1, tipoHuecoUtilizado* h2);


/********Funciones principales de SWAP**************/

tipoRespuesta* reservarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProcesoNuevo, int cantPaginasSolicitadas,int cantDePaginasDeSWAP,int tamanioDePagina, char* particion);
tipoRespuesta* liberarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProceso);
tipoRespuesta* leerPagina(t_list* listaDeHuecosUtilizados,int pidProceso,int dirLogicaDePagina,int tamanioDePagina,char* particion);
tipoRespuesta* escribirPagina(t_list* listaDeHuecosUtilizados,int pidProceso,char* contenidoAEscribir,int dirLogicaDePagina,int tamanioDePagina, char* particion);

/********Fin de funciones principales de SWAP**************/


tipoHuecoUtilizado* crearHuecoUtilizado(int pidProceso,int inicio,int cantidadDePaginas);
void destruirHuecoUtilizado(tipoHuecoUtilizado* huecoUtilizado);
tipoHuecoLibre* crearHuecoLibre(int base, int cantidadDePaginasQueOcupa);
void destruirHuecoLibre(tipoHuecoLibre* huecoLibre);

t_list* inicializarListaDeHuecosUtilizados();
t_list* inicializarListaDeHuecosLibres(int cantidadDePaginasDeSwap);





int cantidadDePaginasDisponibles(t_list* listaDeHuecosLibres);
int baseParaMProcSiTengoEspacioContiguo(t_list* listaDeHuecosLibres, int cantidadDePaginasSolicitadas);


void asignarEspacio(t_list* listaDeHuecosUtilizados,t_list* listaDeHuecosLibres,int pidProceso,int cantDePaginasSolicitadas, int base);

void actualizarListaDeLibres(t_list* listaDeHuecosLibres, int baseDeHuecoLiberado, int cantidadDePaginasLiberadas);

bool buscarHuecoContiguoA(tipoHuecoLibre* huecoLibre, int* indiceDeHuecoAnterior, int* indiceDeHuecoPosterior, t_list* listaDeHuecoLibres);

void compactacionAlpha(t_list* listaDeHuecosUtilizados, t_list* listaDeHuecosLibres, char* particion,int tamanioDePaginaDeSwap);

void moverHueco(tipoHuecoUtilizado* hueco,char* particion, int ultimaPaginaEscrita,int tamanioDePagina);
void moverPagina(char* particion, int dirFisVieja, int dirFisNueva,int tamanioDePagina);



int traducirDireccionLogicaAFisica(tipoHuecoUtilizado* hueco,int dirLogicaDePagina);
int espacioEntreDosHuecosUtilizados(tipoHuecoUtilizado* h1, tipoHuecoUtilizado* h2);



////////FUNCIONES AUXILIARES///////////
void imprimirHuecoUtilizado(tipoHuecoUtilizado* huecoUtilizado);
void imprimirListaDeHuecosUtilizados(t_list* listaDeHuecosUtilizados);

#endif /* SWAP2_SRC_HUECOSSWAP_H_ */
