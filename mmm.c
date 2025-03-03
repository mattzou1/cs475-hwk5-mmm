#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"

int **input1;
int **input2;
int **output;
int **tempoutput;
/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random integers from 0 to 99
 */
void mmm_init() {
	// TODO
	//intialize first input array
   	input1 = (int**) malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		input1[i] = (int*) malloc(sizeof(int) * size);
	}
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			input1[i][j] = rand() % 100;
		}
	}
	//intialize second input array
	input2 = (int**) malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		input2[i] = (int*) malloc(sizeof(int) * size);
	}
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			input2[i][j] = rand() % 100;
		}
	}
	//intialize output array
	output = (int**) malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		output[i] = (int*) malloc(sizeof(int) * size);
	}
	//intialize tempoutput array
	tempoutput = (int**) malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		tempoutput[i] = (int*) malloc(sizeof(int) * size);
	}
}

/**
 * Reset a given matrix to zeroes
 * @param matrix pointer to a 2D array
 */
void mmm_reset(int **matrix) {
	// TODO
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			matrix[i][j] = 0;
		}
	}
}

/**
 * Free up memory allocated to all matrices
 */
void mmm_freeup() {
	// TODO
	// free input1
	for (int i = 0; i < size; i++) {
		free(input1[i]);
		input1[i] = NULL;  // remove dangling pointer
	}
	free(input1);
	input1 = NULL;  //remove dangling pointer
	// free input2
	for (int i = 0; i < size; i++) {
		free(input2[i]);
		input2[i] = NULL;  // remove dangling pointer
	}
	free(input2);
	input2 = NULL;  //remove dangling pointer
	// free output
	for (int i = 0; i < size; i++) {
		free(output[i]);
		output[i] = NULL;  // remove dangling pointer
	}
	free(output);
	output = NULL;  //remove dangling pointer
	// free tempoutput
	for (int i = 0; i < size; i++) {
		free(tempoutput[i]);
		tempoutput[i] = NULL;  // remove dangling pointer
	}
	free(tempoutput);
	tempoutput = NULL;  //remove dangling pointer
}

/**
 * Sequential MMM
 */
void mmm_seq() {
	// TODO - code to perform sequential MMM
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			for(int k = 0; k < size; k++){
				output[i][j] += input1[i][k] * input2[k][j];
			}
		}
	}
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args) {
	// TODO - code to perform parallel MMM
	struct thread_args *t_args = (struct thread_args *) args;
	int start_row = t_args->start_row;
    int end_row = t_args->end_row;
	for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                output[i][j] += input1[i][k] * input2[k][j];
            }
        }
    }
	return NULL;
}

/**
 * Verifies the correctness between the matrices generated by
 * the sequential run and the parallel run.
 *
 * @return the largest error between two corresponding elements
 * in the result matrices
 */
double mmm_verify() {
	// TODO
	double error = 0.0; 
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(abs(output[i][j] - tempoutput[i][j]) > error){
				error = abs(output[i][j] - tempoutput[i][j]);
			}
		}
	}
	return error;
}

//prints a given matrix
void printMatrix(int **matrix){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

//copies output matrix to the tempout matrix
void copyOutput(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			tempoutput[i][j] = output[i][j];
		}
	}
}
