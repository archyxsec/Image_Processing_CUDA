/*
 * functions.h -- Headers of necessary functions
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

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include "functions.h"

/*CUDA Safe Call define, para controlar errores*/

#define CUDA_SAFE_CALL(call) {                                    		\
    cudaError err = call;												\
    if( cudaSuccess != err) {                                           \
        fprintf(stderr, "Cuda error en fichero '%s' en linea %i : %s.\n",   \
                __FILE__, __LINE__, cudaGetErrorString( err));         \
        exit(EXIT_FAILURE);                                             \
    } }

#define FRACTION_CEILING(numerator, denominator) ((numerator+denominator-1)/denominator)

/*Estructura para el tipo de fotografía, en gris o color*/
typedef enum {RGB, GREY} color_t;

int write_img(int, uint8_t *, int);
int read_img(int, uint8_t *, int);
void Uso(int, char **, char **, int *, int *, int *, color_t *);
uint64_t time_m(void);

#endif
