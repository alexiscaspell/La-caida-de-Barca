/*
 * cliente-servidor.h
 *
 *  Created on: 1/6/2015
 *      Author: utnso
 */

#include <sys/select.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <commons/string.h>

#ifndef CLIENTE_SERVIDOR_H_
#define CLIENTE_SERVIDOR_H_


void validarError(int validar, char* mensaje);

/**
* @NAME: crearSocket
* @DESC: crea un socket y retorna su descriptor
* @PARAMS:
*/
int crearSocket();

/**
* @NAME: crearSocket
* @DESC: asocia un puerto con un socket
* @PARAMS:
* 	socket que se quiere asociar
* 	puerto al que se lo va a asociar
*/
void asociarAPuerto(int socket, int puerto);


/**
* @NAME: liberarSocket
* @DESC: crea un socket y retorna su descriptor
* @PARAMS:
*/
void liberarSocket(int socket);


/**
* @NAME: obviarErrorDeIpEnUso
* @DESC: No toma en cuenta el error que sale cuando se uso un socket anteriormente
* @PARAMS:
*/

void obviarErrorDeIpEnUso(int socket);

/**
* @NAME: conectarAServidor
* @DESC: conecta un socket cliente a un socket servidor
* @PARAMS:
*/

void conectarAServidor(int socket,char* direccionIp,int puerto);

/**
* @NAME: enviarMensaje
* @DESC: envia el mensaje "buffer" de un tamanio "tamanioBuffer"
* @PARAMS:
*/
int enviarMensaje(int socketCliente, void* buffer,int tamanioBuffer);

/**
* @NAME: recibirMensaje
* @DESC: recibe el mensaje "buffer" de un tamanio "tamanioBuffer"
* @PARAMS:
*/
int recibirMensaje(int socketCliente, void* buffer,int tamanioBuffer);

/**
* @NAME: recibirMensajeCompleto
* @DESC: se bloquea hasta que recibe la cantidad de bytes de "tamanioBuffer"
* @PARAMS:
*/
int recibirMensajeCompleto(int socketCliente, void* buffer,int tamanioBuffer);

/**
* @NAME: escucharConexiones
* @DESC: bloquea el proceso hasta que haya una solicitud de conexion
* @PARAMS:
*/
void escucharConexiones(int socket, int maximasConexionesEntrantes);

/**
* @NAME: crearSocketParaAceptarSolicitudes
* @DESC: retorna el descriptor del respectivo socket que se conecto al servidor
* @PARAMS:
*/
int crearSocketParaAceptarSolicitudes(int socketServidor);

/**
* @NAME: recibirBloque
* @DESC: recibe un bloque completo de datos de tamaño tamanioBloque
* @PARAMS:
*/
void* recibirBloque(size_t* tamanioBloque,int socketEnviador);

/**
* @NAME: serializarIntYCadena
* @DESC: guarda en el bloque "bloque" el int "entero" y la cadena "cadena"
* @PARAMS:
*/
void serializarIntYCadena(int entero,char* cadena,void* bloque);

/**
* @NAME: deserializarIntYCadena
* @DESC: guarda una cadena y un int recibidos en un bloque de memoria
* @PARAMS:
*/
void deserializarIntYCadena(int* entero,char* cadena,int tamanioBloque,void* bloque);

#endif /* CLIENTE_SERVIDOR_H_ */
