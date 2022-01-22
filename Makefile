img_processing_cuda: main.o functions.o img_processing.o
	nvcc -o img_processing_cuda main.o functions.o img_processing.o -O2

main.o: main.c 
	gcc -c main.c -O2

functions.o: functions.c
	gcc -c functions.c -O2

img_processing.o: img_processing.cu
	nvcc -c img_processing.cu

clean:
	\rm -f *.o img_processing_cuda
