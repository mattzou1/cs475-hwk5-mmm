#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int size;

int main(int argc, char *argv[]) {
	double clockstart, clockend;
	//no arg check
	if(argc == 1){
		printf("Usage: ./mmmSol <mode> [num threads] <size>\n");
	}
	else if(strcmp(argv[1], "S") == 0){
		//Arg check
		if(argc == 3){
			//convert arg inputs to int
			sscanf(argv[2], "%d", &size);
			mmm_init();
			printf("========\n");
			printf("mode: sequential\n");
			printf("thread count: 1\n");
			printf("size: %d\n", size);
			printf("========\n");
			double totalTime = 0.0;
			//find total time of 3 sequential mode runs
			for(int i = 0; i < 4; i++){
				mmm_reset(output);
				clockstart = rtclock();
				mmm_seq();	
				clockend = rtclock();
				if(i != 0){
					totalTime += clockend - clockstart;
				}
			}
			printf("Sequential Time (avg of 3 runs): %.6f sec\n", totalTime / 3);
			// printMatrix(output);
			mmm_freeup();
		}
		else{
			printf("Usage: ./mmmSol <mode> [num threads] <size>\n");
		}
	}
	else if(strcmp(argv[1], "P") == 0){
		//arg check
		if(argc == 4){
			int numThreads;
			//convert arg inputs to ints
			sscanf(argv[3], "%d", &size);
			sscanf(argv[2], "%d", &numThreads);
			mmm_init();
			int rows_per_thread = size / numThreads;
			int start_row;
			int end_row;
			printf("========\n");
			printf("mode: parallel\n");
			printf("thread count: %d\n", numThreads);
			printf("size: %d\n", size);
			printf("========\n");
			double stotalTime = 0.0;
			//find total time of 3 sequential mode runs
			for(int i = 0; i < 4; i++){
				mmm_reset(output);
				clockstart = rtclock();
				mmm_seq();	
				clockend = rtclock();
				if(i != 0){
					stotalTime += clockend - clockstart;
				}
			}
			printf("Sequential Time (avg of 3 runs): %.6f sec\n", stotalTime / 3);
			copyOutput();
			double ptotalTime = 0.0;
			//find average of 3 parallel mode runs
			for(int i = 0; i < 3; i++){
				mmm_reset(output);
				start_row = 0; 
				end_row = rows_per_thread - 1; 
				clockstart = rtclock();
				//create thread array and thread_args array
				pthread_t *threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
				struct thread_args *tArgs = (struct thread_args*) malloc(numThreads * sizeof(struct thread_args));
				//create threads and their arguments
				for (int i = 0; i < numThreads; i++) {
					tArgs[i].start_row = start_row;
					tArgs[i].end_row = end_row;
					pthread_create(&threads[i], NULL, mmm_par, &tArgs[i]);
					start_row = end_row + 1;
					end_row += rows_per_thread;
				}
				//join threads
				for (int i = 0; i < numThreads; i++) {
					pthread_join(threads[i], NULL);
				}
				//free threads and thread_Args
				free(threads);
				free(tArgs);
				clockend = rtclock();
				if(i != 0){
					ptotalTime += clockend - clockstart;
				}
			}
			printf("Parallel Time (avg of 3 runs): %.6f sec\n", ptotalTime / 3);
			printf("Speedup: %.6f\n", stotalTime/ptotalTime);
			printf("Verifying... largest error between parallel and sequential matrix: %.6f\n", mmm_verify());
			mmm_freeup();
		}
		else{
			printf("Usage: parallel mode requires [num threads]\n");
		}
	}
	else{
		printf("Usage: ./mmmSol <mode> [num threads] <size>\n");
	}
	
	// clockstart = rtclock(); // start clocking

	// // start: stuff I want to clock

	// // end: stuff I want to clock

	// clockend = rtclock(); // stop clocking
	// printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
