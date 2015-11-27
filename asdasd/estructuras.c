/*
 * estructuras.c
 *
 *  Created on: 3/10/2015
 *      Author: utnso
 */
#include "estructuras.h"



tipoPCB* recibirPCB(int socketEnviador){

	size_t tamanioRuta;

	tipoPCB* respuesta = malloc(sizeof(tipoPCB));

	recibirMensajeCompleto(socketEnviador, &(respuesta->pid), sizeof(int));

	recibirMensajeCompleto(socketEnviador, &(respuesta->insPointer), sizeof(int));

	recibirMensajeCompleto(socketEnviador, &(respuesta->estado), sizeof(char));

	recibirMensajeCompleto(socketEnviador, &tamanioRuta, sizeof(size_t));

	respuesta->ruta = malloc(tamanioRuta);

	recibirMensajeCompleto(socketEnviador, respuesta->ruta, tamanioRuta);

	return respuesta;

}

void enviarPCB(int socketCliente, tipoPCB* PCB)
{
		size_t tamanioRuta = strlen(PCB->ruta) + sizeof(char);

		enviarMensaje(socketCliente, &(PCB->pid), sizeof(int));

		enviarMensaje(socketCliente, &(PCB->insPointer), sizeof(int));

		enviarMensaje(socketCliente, &(PCB->estado), sizeof(char));

		enviarMensaje(socketCliente, &tamanioRuta, sizeof(int));

		enviarMensaje(socketCliente, PCB->ruta, tamanioRuta);
}

void imprimirPCB(tipoPCB* PCB)
{
	printf("pID: %i | instructionPointer: %i | estado: %c | ruta: %s\n", PCB->pid, PCB->insPointer, PCB->estado, PCB->ruta);
}

tipoInstruccion* recibirInstruccion(int socketEnviador){

	tipoInstruccion* unaInstruccionRecibida;

	int tamanioDeDatos;
	recibirMensajeCompleto(socketEnviador,&tamanioDeDatos,sizeof(int));

	tipoMensaje* unMensajeRecibido = crearMensaje(tamanioDeDatos);
	recibirMensajeCompleto(socketEnviador,unMensajeRecibido->datos,unMensajeRecibido->tamanio);

	unaInstruccionRecibida = deserializarInstruccionPosta(unMensajeRecibido);

	destruirMensaje(unMensajeRecibido);

	return unaInstruccionRecibida;



//	tipoInstruccion* instruccion = malloc(sizeof(tipoInstruccion));
//
//	size_t tamanioTexto;
//
//	recibirMensajeCompleto(socketEnviador,&(instruccion->pid),sizeof(int));
//	recibirMensajeCompleto(socketEnviador,&(instruccion->instruccion),sizeof(char));
//	recibirMensajeCompleto(socketEnviador,&(instruccion->nroPagina),sizeof(int));
//	recibirMensajeCompleto(socketEnviador,&tamanioTexto,sizeof(size_t));
//	instruccion->texto = malloc(tamanioTexto);
//	recibirMensajeCompleto(socketEnviador,instruccion->texto,tamanioTexto);
//
//	return instruccion;
}

void enviarInstruccion(int socketCliente,tipoInstruccion* instruccion){

	tipoMensaje* unMensajeAEnviar = serializarInstruccion(instruccion);

	enviarMensaje(socketCliente,&unMensajeAEnviar->tamanio,sizeof(int));
	enviarMensaje(socketCliente,unMensajeAEnviar->datos,unMensajeAEnviar->tamanio);

	destruirMensaje(unMensajeAEnviar);

//	size_t tamanioTexto = strlen(instruccion->texto)+sizeof(char);
//	enviarMensaje(socketCliente,&(instruccion->pid),sizeof(int));
//	enviarMensaje(socketCliente,&(instruccion->instruccion),sizeof(char));
//	enviarMensaje(socketCliente,&(instruccion->nroPagina),sizeof(int));
//	enviarMensaje(socketCliente,&tamanioTexto,sizeof(size_t));
//	enviarMensaje(socketCliente,instruccion->texto,tamanioTexto);

}

void enviarRespuesta(int socketCliente,tipoRespuesta* respuesta){

	tipoMensaje* unMensajeAEnviar = serializarRespuesta(respuesta);

	enviarMensaje(socketCliente,&unMensajeAEnviar->tamanio,sizeof(int));
	enviarMensaje(socketCliente,unMensajeAEnviar->datos,unMensajeAEnviar->tamanio);

	destruirMensaje(unMensajeAEnviar);


//	size_t tamanioInfo = strlen(respuesta->informacion)+sizeof(char);
//
//	enviarMensaje(socketCliente,&(respuesta->respuesta),sizeof(char));
//	enviarMensaje(socketCliente,&tamanioInfo,sizeof(size_t));
//	enviarMensaje(socketCliente,respuesta->informacion,tamanioInfo);
}

tipoRespuesta* recibirRespuesta(int socketEnviador){

	tipoRespuesta* unaRespuestaRecibida;
	tipoMensaje* unMensajeRecibido;

	int tamanioDeDatos;
	recibirMensajeCompleto(socketEnviador,&tamanioDeDatos,sizeof(int));

	unMensajeRecibido = crearMensaje(tamanioDeDatos);
	recibirMensajeCompleto(socketEnviador,unMensajeRecibido->datos,unMensajeRecibido->tamanio);

	unaRespuestaRecibida = deserializarRespuesta(unMensajeRecibido);

	return unaRespuestaRecibida;

//	size_t tamanioInfo;
//
//	tipoRespuesta* respuesta = malloc(sizeof(tipoRespuesta));
//
//	recibirMensajeCompleto(socketEnviador,&(respuesta->respuesta),sizeof(char));
//	recibirMensajeCompleto(socketEnviador,&tamanioInfo,sizeof(size_t));
//	respuesta->informacion = malloc(tamanioInfo);
//	recibirMensajeCompleto(socketEnviador,respuesta->informacion,tamanioInfo);
//
//	return respuesta;
}

/*******************Funciones para tipoRespuesta*************************/

tipoRespuesta* crearTipoRespuesta(char respuesta, char* informacion){//esto es al dope, no, yo lo uso en el swap

	tipoRespuesta* aux = malloc(sizeof(tipoRespuesta));
	aux->respuesta = respuesta;
	aux->informacion = string_duplicate(informacion);
	return aux;
}

tipoInstruccion* crearTipoInstruccion(int pid,char instruccion,int nroPagina,char* texto ){//esto es al dope, no, yo lo uso en el swap

	tipoInstruccion* aux = malloc(sizeof(tipoInstruccion));
	aux->instruccion = instruccion;
	aux->nroPagina = nroPagina;
	aux->pid = pid;
	aux->texto = string_duplicate(texto);
	return aux;
}

void destruirTipoRespuesta(tipoRespuesta* respuesta){
	free(respuesta->informacion);
	free(respuesta);
}

void destruirTipoInstruccion(tipoInstruccion* instruccion){
	free(instruccion->texto);
	free(instruccion);
}


