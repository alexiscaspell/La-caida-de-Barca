/*
 * instruccionesSWAP.h
 *
 *  Created on: 2/10/2015
 *      Author: utnso
 */

#ifndef SWAP2_SRC_INSTRUCCIONESSWAP_H_
#define SWAP2_SRC_INSTRUCCIONESSWAP_H_

#include <stdio.h>
#include <stdlib.h>

#include "huecosSWAP.h"
#include "particionSWAP.h"


#include <commonsDeAsedio/estructuras.h>


tipoRespuesta* ejecutarInstruccion(tipoInstruccion* instruccion,t_list* listaDeHuecosOcupados, t_list* listaDeHuecosLibres, tipoConfigSWAP* configuracion);

#endif /* SWAP2_SRC_INSTRUCCIONESSWAP_H_ */
