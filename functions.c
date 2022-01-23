/*
 * functions.c -- necessary funtions
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
#include <assert.h>
#include <sys/time.h>
#include "functions.h"

void Uso(int argc, char **argv, char **image, int *width, int *height, int *loops, color_t *imageType) {
	if (argc == 6 && !strcmp(argv[5], "grey")) {
		*image = (char *)malloc((strlen(argv[1])+1) * sizeof(char));
		strcpy(*image, argv[1]);	
		*width = atoi(argv[2]);
		*height = atoi(argv[3]);
		*loops = atoi(argv[4]);
		*imageType = GREY;
	} else if (argc == 6 && !strcmp(argv[5], "rgb")) {
		*image = (char *)malloc((strlen(argv[1])+1) * sizeof(char));
		strcpy(*image, argv[1]);	
		*width = atoi(argv[2]);
		*height = atoi(argv[3]);
		*loops = atoi(argv[4]);
		*imageType = RGB;
	} else {
		fprintf(stderr, "Error de Entrada!\n%s <nombre_imagen> <ancho> <altura> <loops> [rgb/grey].\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

int write_img(int fd , uint8_t* buff , int size) {
	int n, sent;
	for (sent = 0 ; sent < size ; sent += n)
		if ((n = write(fd, buff + sent, size - sent)) == -1)
			return -1;
	return sent;
}

int read_img(int fd , uint8_t* buff , int size) {
	int n, sent;
	for (sent = 0 ; sent < size ; sent += n)
		if ((n = read(fd, buff + sent, size - sent)) == -1)
			return -1;
	return sent;
}

uint64_t time_m(void) {
	struct timeval tv;
	assert(gettimeofday(&tv, NULL) == 0);
	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
