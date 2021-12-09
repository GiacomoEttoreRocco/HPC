#include <stdio.h>
#include <stdlib.h>

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
    
    int col_dim = atoi(argv[1]);
    int row_dim = atoi(argv[2]);
    int matr_dim = atoi(argv[3]);
    int const n_elementi = row_dim * matr_dim * col_dim;
    double* tens0 = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);
    double* tens1 = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);

    double* res = (double*)malloc(__SIZEOF_DOUBLE__*n_elementi);

    for(int k = 0; k < matr_dim; k++){
        for(int j = 0; j <row_dim; j++){
            for(int i = 0; i <col_dim; i++){
                //printf("...%d...\n", indice(i, 2, j, 4, k));
                tens0[indice(i, row_dim, j, row_dim*col_dim, k)] = k;
                tens1[indice(i, row_dim, j, row_dim*col_dim, k)] = 2*k;
            }
        }
    }
    for(int p = 0; p < matr_dim*row_dim*col_dim; p++){
        res[p] = tens0[p] + tens1[p];
    }
    //print_matrix(matr, col_dim, row_dim, 0);    
    //print_matrix(matr, col_dim, row_dim, 1);    
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
