/*
 * funcionesFilesystem.h
 *
 *  Created on: 16/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESFILESYSTEM_H_
#define FUNCIONESFILESYSTEM_H_

#include <commons/config.h>
#include "estructurasFilesystem.h"

tipoConfigFilesystem* crearTipoConfigFilesystem();

void destruirTipoConfigFilesystem(tipoConfigFilesystem* camposDeConfiguracion);

int cargarArchivoDeConfiguracionDelFS(t_config* archivoDeConfiguracionDelFS,tipoConfigFilesystem* camposDeConfiguracionDelFS);

#endif /* FUNCIONESFILESYSTEM_H_ */
