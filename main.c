/*
 * main.c -- Image Processing with Kernel filter in CUDA, main process
 * Copyright (c) 2022
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "functions.h"
#include "img_processing.h"

int main(int argc, char** argv) {
	int fd, width, height, loops;
	char *image;
	color_t imageType;
	
	Uso(argc, argv, &image, &width, &height, &loops, &imageType);	

	uint8_t *src = NULL;
	/*Variable para calcular el tiempo de ejecución*/
	uint64_t execution_time = time_m(); 

	/* Leemos los bytes de la foto */
	if ((fd = open(image, O_RDONLY)) < 0) {
		fprintf(stderr, "No se pudo abrir %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	size_t bytes = (imageType == GREY) ? height * width : height * width*3;	/*Comprobamos si queremos la imagen en Blanco y negro para solo multiplicar anchura*altura o también por 3(RGB)*/
	src = (uint8_t *) calloc(bytes, sizeof(uint8_t));
	read_img(fd, src, bytes);
	close(fd);

	gpuFilter(src, width, height, loops, imageType);

	int fd_out;
	char *outImage = (char*) malloc((strlen(image) + 9) * sizeof(char));
	strcpy(outImage, "blur_");
	strcat(outImage, image);
	if ((fd_out = open(outImage, O_CREAT | O_WRONLY, 0644)) == -1) {
		fprintf(stderr, "No se pudo abrir o crear %s\n", outImage);
		return EXIT_FAILURE;
	}
	write_img(fd_out, src, bytes);
	close(fd_out);
	free(outImage);

	/* Calculamos el tiempo de ejecución */
	execution_time = time_m() - execution_time;
	double million = 1000 * 1000;
	fprintf(stdout, "Tiempo de ejecucion: %.3f sec\n", execution_time / million);

    free(src);
	return EXIT_SUCCESS;
}
