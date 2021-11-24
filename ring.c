#include <stdio.h> 
#include <mpi.h> 
#include <string.h>
#include <stdlib.h>
#define INIT MPI_Init(&argc, &argv);
#define END MPI_Finalize();
int main(int argc, char *argv[]){
    int right, left, buffer[2] = {0,0}, np, rank, tag, cnt=0;
    INIT
    double start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Status status[2];
    MPI_Comm ring;
    MPI_Request request[2];
    const int periods[1] = {1};
    MPI_Cart_create(MPI_COMM_WORLD, 1, &np, periods, 1, &ring);
    MPI_Cart_shift(ring, 0, 1, &left, &right);
    MPI_Comm_rank(ring, &rank);
    tag = rank; 
    while(cnt < np){
        buffer[0] += rank; 
        buffer[1] -= rank; 
        MPI_Isend(&buffer[0], 1, MPI_INT, right, tag, ring, &request[0]); // MPI_Send(&var_to_send, times , type, ricever, tag, communicator);  
        MPI_Isend(&buffer[1], 1, MPI_INT, left, tag, ring, &request[1]); 
        MPI_Recv(&buffer[0], 1, MPI_INT, left, left, ring, &status[0]);   // MPI_Recv(&var_to_recv, times, type, rank_reciver, tag, communicator, &status );
        MPI_Recv(&buffer[1], 1, MPI_INT, right, right, ring, &status[1]);
        MPI_Wait(&request[0], &status[0]);
        MPI_Wait(&request[1], &status[1]);
        cnt++; 
    }   
    double time = MPI_Wtime() - start;
    printf("I am process %d and i have received %d messages. My final messages have tag %d and value %d, %d\nThe execution time is %lf ms\n",
    rank, 2*cnt, tag, buffer[0], buffer[1], time*1000);
    END
    return 0;
}