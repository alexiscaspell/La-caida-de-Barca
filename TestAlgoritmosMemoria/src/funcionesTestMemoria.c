#include "funcionesTestMemoria.h"

/////////////////////////////////
//PARA HACER ESPACIO
/////////////////////////////////

void hacerEspacioEnRAM(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb){
	switch (algoritmo->algoritmoElegido) {
		case FIFO:
			hacerEspacioPorFIFO(algoritmo, ram, tlb);
			break;

		case LRU:
			hacerEspacioPorLRU(algoritmo, ram, tlb);
			break;
	}
}

void hacerEspacioPorFIFO(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb){

	tipoRAM* instruccionASacarDeLaRam = list_get(ram, algoritmo->posicionSacarFIFO);

	//borro de RAM
	list_remove(ram, algoritmo->posicionSacarFIFO);

	//borro de TLB
	int posicionEnTLB = posicionEnTlbPorTipoRAM(instruccionASacarDeLaRam, tlb);
	if(posicionEnTLB != -1) list_remove(tlb, posicionEnTLB);

	//me fijo si esta modificada
	if(instruccionASacarDeLaRam->modificado) enviarInstruccionASwap(instruccionASacarDeLaRam);

	//muevo el puntero del algoritmo
	algoritmo->posicionSacarFIFO ++;

}

void hacerEspacioPorLRU(tipoAlgoritmo* algoritmo, t_list* ram, t_list* tlb){
	tipoRAM* instruccionASacarDeLaRam;
	int posicion = posicionDeInstruccionConMenosUsosEnLRU(algoritmo->usosLRU);

	//borro de RAM
	list_remove(ram, posicion);

	//borro de TLB
	int posicionEnTLB = posicionEnTlbPorTipoRAM(instruccionASacarDeLaRam, tlb);
	if(posicionEnTLB != -1) list_remove(tlb, posicionEnTLB);

	//me fijo si esta modificada
	if(instruccionASacarDeLaRam->modificado) enviarInstruccionASwap(instruccionASacarDeLaRam);
}

int posicionDeInstruccionConMenosUsosEnLRU(t_list* listaUsos){

	int i, minimoUso = 1000, posicion, aux;

	for (i = 0; i < list_size(listaUsos); ++i) {

		aux = *(int*)list_get(listaUsos, i);
		if ( aux < minimoUso){
			minimoUso = aux;
			posicion = i;
		}
	}

	return posicion;
}

void enviarInstruccionASwap(tipoRAM* instruccionASacarDeLaRam){
	//sockets y serializacion
}

int posicionEnTlbPorTipoRAM(tipoRAM* instruccion, t_list* tlb){

	bool encontro = false;

	int i;
	for (i = 0; i < list_size(tlb); ++i) {
		tipoTLB* aux = list_get(tlb,i);
		if (aux->pid == instruccion->pid && aux->numeroDePagina == instruccion->numeroDePagina){
			encontro = true;
			break;
		}
	}

	if(encontro) return i;
	else return -1;
}

int posicionEnRamPorTipoRAM(tipoRAM* instruccion, t_list* ram){

	bool encontro = false;

	int i;
	for (i = 0; i < list_size(ram); ++i) {
		tipoRAM* aux = list_get(ram,i);
		if (aux->pid == instruccion->pid && aux->numeroDePagina == instruccion->numeroDePagina){
			encontro = true;
			break;
		}
	}

	if(encontro) return i;
	else return -1;
}



/////////////////////////////////
//CUANDO USO UNA PAGINA
/////////////////////////////////
/*esto se tiene que usar cuando voy a leer o escribir una pagina para marcarla com usada dependiendo del algoritmo*/

void usarPaginaPorAlegoritmo(tipoRAM* instruccion, tipoAlgoritmo* algoritmo, t_list* ram){

	if (algoritmo->algoritmoElegido == LRU){
		int posicion = posicionEnRamPorTipoRAM(instruccion, ram);

		int modificarUso = *(int*)list_get(algoritmo->usosLRU, posicion) + 1;
		list_replace(algoritmo->usosLRU, posicion, modificarUso);
	}

	if(algoritmo->algoritmoElegido == FIFO){
		//no se hace nada
	}
}













