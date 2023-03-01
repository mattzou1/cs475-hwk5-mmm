#ifndef MMM_H_
#define MMM_H_

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., extern double **A, **B, **C),
// as well as the size of the matrices, etc.
extern int **input1;
extern int **input2;
extern int **output;
extern int **tempoutput;
extern int size;

struct thread_args {
    int start_row; 
    int end_row; 
};

void mmm_init();
void mmm_reset(int **);
void mmm_freeup();
void mmm_seq();
void *mmm_par(void *args);
double mmm_verify();
void printMatrix(int **matrix);
void copyOutput();

#endif /* MMM_H_ */
