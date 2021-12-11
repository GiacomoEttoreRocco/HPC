#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

#define INIT MPI_Init(&argc, &argv);
#define END MPI_Finalize();

int indice(int col, int row_dim, int n_row, int col_dim, int n_matr){
    int res = col + col_dim*n_row + col_dim*row_dim*n_matr;
    return res;
}

int main(int argc, char *argv[]){

    INIT

    int col_dim = atoi(argv[1]);
    int row_dim = atoi(argv[2]);
    int matr_dim = atoi(argv[3]);

    //printf("-> %d \n", col_dim);
    //printf("-> %d \n", row_dim);
    //printf("-> %d \n", matr_dim);

    int n_elementi = row_dim * matr_dim * col_dim;

    //printf("-> %d \n", n_elementi);

    int np, rank;

    int seed = 11345;
    srand48(seed);

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *tens0, *tens1, *res;

    if(rank == 0){
        tens0 = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);
        tens1 = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);
        res = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);
        for(int k = 0; k < matr_dim; k++){
            for(int j = 0; j <row_dim; j++){
                for(int i = 0; i <col_dim; i++){
                    tens0[indice(i, row_dim, j, col_dim, k)] = k;
                    tens1[indice(i, row_dim, j, col_dim, k)] = 2*k;
                }
            }
        }
    }
    // MPI_Scatter(sndbuff, sndcount, sndtype, rcvbuff, rcvcount, rcvtype, root, comm, ierr);
    int sndcount, rcvcount;
    sndcount = (int)(n_elementi/np);
    rcvcount = (int)(n_elementi/np);
    double start = MPI_Wtime();
    double* local_matrix0 = (double*)malloc(__SIZEOF_DOUBLE__*(int)(n_elementi/np));
    double* local_matrix1 = (double*)malloc(__SIZEOF_DOUBLE__*(int)(n_elementi/np));
    double* local_res = (double*)malloc(__SIZEOF_DOUBLE__*(int)(n_elementi/np));

    MPI_Scatter(tens0, sndcount, MPI_DOUBLE, local_matrix0, rcvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(tens1, sndcount, MPI_DOUBLE, local_matrix1, rcvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for(int p = 0; p < sndcount; p++){
        local_res[p] = local_matrix0[p] + local_matrix1[p];
    }
    MPI_Gather(local_res, sndcount, MPI_DOUBLE, res, rcvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //printf("----------------\n");
    /*
    if(rank == 0){
        for(int i = 0; i < row_dim*col_dim*matr_dim; i++){
            if(i%row_dim == 0){
                printf("\n");
            }
            if(i%(row_dim*col_dim) == 0){
                printf("\n");
            }
            printf(" %0.1lf ", res[i]);
        }
        printf("\n");
    }
    */
    /*
    free(local_matrix0);
    free(local_matrix1);
    free(local_res);
    */
    double time = (MPI_Wtime() - start)*1000;
    MPI_Barrier(MPI_COMM_WORLD);
    //printf("The process %d took %0.3lfms, the size of the matrices is: %dx%dx%d\n", rank, time, col_dim, row_dim,matr_dim);
    //printf("The last matrix is (or should be) composed by: %d-1+%d-1+%d-1\n", col_dim, row_dim, matr_dim);  
    double sum = 0, mean;
    MPI_Reduce(&time, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    mean = sum/np;
    if(rank == 0) {
        printf("The mean of all processes is %0.3lfms.\n", mean);
    }

    END
}
