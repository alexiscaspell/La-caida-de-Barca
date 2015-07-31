#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "map.h"

#define FALLO_OPEN -1



int tamanio_archivo(int fileDescriptor){
	struct stat buf;
	fstat(fileDescriptor, &buf);
	return buf.st_size;
}

void mapArchivo(char *rutaArchivoAMapear){


	int fd_mapper, tamanioDeArchivoMapeado;
	char *mapeo;

	if(( fd_mapper = open (rutaArchivoAMapear, O_RDONLY) ) == FALLO_OPEN){
		//Si no se pudo abrir, imprimir el error y abortar;
		fprintf(stderr, "Error al abrir el archivo '%s': %s\n", rutaArchivoAMapear, strerror(errno));
		abort();
	}

	tamanioDeArchivoMapeado = tamanio_archivo(fd_mapper);
	if( (mapeo = mmap( NULL, tamanioDeArchivoMapeado, PROT_READ, MAP_SHARED, fd_mapper, 0 )) == MAP_FAILED){
		//Si no se pudo ejecutar el mmap, imprimir el error y abortar;
		fprintf(stderr, "Error al ejecutar MMAP del archivo '%s': %s\n", rutaArchivoAMapear,strerror(errno));
		abort();
	}

	printf("\nResultado del map:\n%s",mapeo);
	munmap( mapeo, tamanioDeArchivoMapeado);
	close(fd_mapper);


}
