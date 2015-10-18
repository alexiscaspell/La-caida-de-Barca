/*
 * funcionesMemoria.h
 *
 *  Created on: 17/10/2015
 *      Author: utnso
 */

#ifndef FUNCIONESTESTMEMORIA_H_
#define FUNCIONESTESTMEMORIA_H_

#include <commons/collections/list.h>
#include <commonsDeAsedio/estructuras.h>
#include <stdbool.h>

#define FIFO '1'
#define LRU '2'

/////////////////////////////////
//ESTRUCTURAS
/////////////////////////////////
typedef struct{
	int posicionSacarFIFO;
	t_list* usosLRU;
	char algoritmoElegido;
}tipoAlgoritmo;


typedef struct{
	int pid;
	int numeroDePagina;
	bool modificado;
	char* contenido;
}tipoRAM;


typedef struct{
	int pid;
	int numeroDePagina;
	int posicionEnRAM;
}tipoTLB;

/////////////////////////////////
//FUNCIONES
/////////////////////////////////

/////////////////////////////////
//PARA HACER ESPACIO
/////////////////////////////////
void hacerEspacioEnRAM(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb);

void hacerEspacioPorFIFO(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb);

void hacerEspacioPorLRU(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb);

int posicionDeInstruccionConMenosUsosEnLRU(t_list* listaUsos);

void enviarInstruccionASwap(tipoRAM* instruccionASacarDeLaRam);

int posicionEnTlbPorTipoRAM(tipoRAM* instruccion, t_list* tlb);

int posicionEnRamPorTipoRAM(tipoRAM* instruccion, t_list* ram);

void usarPaginaPorAlegoritmo(tipoRAM* instruccion, tipoAlgoritmo* algoritmo, t_list* ram);

#endif /* FUNCIONESTESTMEMORIA_H_ */
