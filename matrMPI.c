#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

#define INIT MPI_Init(&argc, &argv);
#define END MPI_Finalize();

int indice(int col, int row_dim, int n_row, int matr_dim, int n_matr){
    int res = col + row_dim*n_row + matr_dim*n_matr;
    return res;
}

void print_matrix(double matrix[], int dim_row, int dim_col, int n_matrix){
    for(int i = 0; i < dim_row; i++){
        for(int j = 0; j < dim_col; j++){
            printf("id: %d ", i*dim_row+j + n_matrix*dim_row*dim_col);
            printf("val: %0.1lf     ", matrix[i*dim_row+j+n_matrix*dim_row*dim_col]);
        }
        printf("\n");
    }
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
                    tens0[indice(i, row_dim, j, row_dim*col_dim, k)] = k;
                    tens1[indice(i, row_dim, j, row_dim*col_dim, k)] = 2*k;
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
    }
    */
    /*
    free(local_matrix0);
    free(local_matrix1);
    free(local_res);
    */
    double time = (MPI_Wtime() - start)*1000;
    
    printf("Sono il processo %d e ci ho messo %0.3lf, dimensione %dx%dx%d\n", rank, time, col_dim, row_dim,matr_dim);

    END
}
