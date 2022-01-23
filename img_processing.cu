/*
 * img_processing.cu -- Image Processing with Kernel filter in CUDA
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
#include "functions.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

/*GPU kernel functions*/
__global__ void kernel_filter_grey(uint8_t *src, uint8_t *dst, int width, int height) {
	int i, j, k, l;
	/* kernel filter */
	int h[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

	size_t x = blockIdx.x*blockDim.x + threadIdx.x;
	size_t y = blockIdx.y*blockDim.y + threadIdx.y;

	/* Aplicamos el filtro */
	if (0 < x && x < height-1 && 0 < y && y < width-1) {
		float val = 0;
		for (i = x-1, k = 0 ; i <= x+1 ; i++, k++)
			for (j = y-1, l = 0 ; j <= y+1 ; j++, l++)
				val += src[width * i + j] * h[k][l] / 16.0;
		dst[width * x + y] = val;
	}
}

__global__ void kernel_filter_rgb(uint8_t *src, uint8_t *dst, int width, int height) {
	int i, j, k, l;
	/* kernel filter */
	int h[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

	size_t x = blockIdx.x*blockDim.x + threadIdx.x;
	size_t y = blockIdx.y*blockDim.y + threadIdx.y;

	/* Aplicamos el filtro */
	if (0 < x && x < height-1 && 0 < y && y < 3*width-3) {
		float redval = 0, greenval = 0, blueval = 0;
		for (i = x-1, k = 0 ; i <= x+1 ; i++, k++) {
			for (j = (y*3)-3, l = 0 ; j <= (y*3)+3 ; j+=3, l++) {
				redval += src[(width*3) * i + j]* h[k][l] /16.0;
				greenval += src[(width*3) * i + j+1] * h[k][l] /16.0;
				blueval += src[(width*3) * i + j+2] * h[k][l] /16.0;
			}
		}
		dst[width*3 * x + (y*3)] = redval;
		dst[width*3 * x + (y*3)+1] = greenval;
		dst[width*3 * x + (y*3)+2] = blueval;
	}
}

extern "C" void gpuFilter(uint8_t *src, int width, int height, int loops, color_t imageType)
{
	uint8_t *d_src, *d_dst, *tmp;
	size_t bytes = (imageType == GREY) ? height * width : height * width*3;

    CUDA_SAFE_CALL( cudaMalloc(&d_src, bytes * sizeof(uint8_t)) );
    CUDA_SAFE_CALL( cudaMalloc(&d_dst, bytes * sizeof(uint8_t)) );
 
    CUDA_SAFE_CALL( cudaMemcpy(d_src, src, bytes, cudaMemcpyHostToDevice) );
    CUDA_SAFE_CALL( cudaMemset(d_dst, 0, bytes) );

    int t;
	const int blockSize = 16;
	/*Repetimos el bucle loops veces (que indica el usuario)*/
	for (t = 0 ; t < loops ; t++) {
		
		if (imageType == GREY) {
			int gridX = FRACTION_CEILING(height, blockSize);
			int gridY = FRACTION_CEILING(width, blockSize);
			dim3 block(blockSize, blockSize);
			dim3 grid(gridX, gridY);
			kernel_filter_grey<<<grid, block>>>(d_src, d_dst, width, height);
		} else if (imageType == RGB) {
			int gridX = FRACTION_CEILING(height, blockSize);
			int gridY = FRACTION_CEILING(width*3, blockSize);
			dim3 block(blockSize, blockSize);
			dim3 grid(gridX, gridY);
			kernel_filter_rgb<<<grid, block>>>(d_src, d_dst, width, height);
		}

		/* intercambiamos los arrays */
		tmp = d_src;
	    d_src = d_dst;
	    d_dst = tmp;
	}

	CUDA_SAFE_CALL( cudaGetLastError() );
	CUDA_SAFE_CALL( cudaThreadSynchronize() );
    
    /* Copiamos el array de vuelta al host */
    if (loops%2 == 0) {
    	CUDA_SAFE_CALL( cudaMemcpy(src, d_src, bytes, cudaMemcpyDeviceToHost) );
    } else {
   		CUDA_SAFE_CALL( cudaMemcpy(src, d_dst, bytes, cudaMemcpyDeviceToHost) );
   	}

    CUDA_SAFE_CALL( cudaFree(d_src) );
    CUDA_SAFE_CALL( cudaFree(d_dst) );
}
