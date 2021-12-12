#include <stdio.h> 
#include <mpi.h> 
#include <string.h>
#include <stdlib.h>
#define INIT MPI_Init(&argc, &argv);
#define END MPI_Finalize();
double main(int argc, char *argv[]){
    int right, left, buffer[2] = {0,0}, np, rank, tag, cnt=0, tcnt;
    INIT
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Status status[2];
    MPI_Comm ring;
    MPI_Request request[2];
    const int periods[1] = {1};
    MPI_Cart_create(MPI_COMM_WORLD, 1, &np, periods, 1, &ring);
    MPI_Cart_shift(ring, 0, 1, &left, &right);
    MPI_Comm_rank(ring, &rank);
    tag = rank; 
    double sum, mean, start, time;
    for(int rep = 0; rep < 1000; rep++){
        sum = 0;
        start = MPI_Wtime();
        for(int cycle = 0; cycle < 1000; cycle++){
            buffer[0] = 0; // azzeriamo il buffer, come se iniziassimo un nuovo ciclo completamente nuovo.
            buffer[1] = 0; 
            while(cnt < np){ //while(cnt < np)
                buffer[0] += rank; 
                buffer[1] -= rank; 
                //printf("msg: %d, number of cycle of msgs: %d, rep: %d\n", buffer[0], cycle, rep);
                MPI_Isend(&buffer[0], 1, MPI_INT, right, tag, ring, &request[0]); // MPI_Send(&var_to_send, times , type, ricever, tag, communicator);  
                MPI_Isend(&buffer[1], 1, MPI_INT, left, tag, ring, &request[1]); 
                MPI_Recv(&buffer[0], 1, MPI_INT, left, left, ring, &status[0]);   // MPI_Recv(&var_to_recv, times, type, rank_reciver, tag, communicator, &status );
                MPI_Recv(&buffer[1], 1, MPI_INT, right, right, ring, &status[1]);
                MPI_Wait(&request[0], &status[0]);
                MPI_Wait(&request[1], &status[1]);
                cnt++; 
            }   
            //printf("%d execution made.\n", cycle);
            tcnt = cnt;
            cnt = 0;
        }
        
        //-------------------- CALCOLO MEDIA----------------------
        time = MPI_Wtime() - start;
        MPI_Reduce(&time, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, ring);
        if(rank == 0) {
            //printf("The mean of all processes is %0.3lfms.\n", (sum/np)*1000);
            mean = (sum/(np*1.0)*1000);
            //printf("%d processors, mean time: %0.8lf ms\n", np, mean);
            printf("%0.8lf\n", mean);
        }    
    //--------------------------------------------------------
    }
    //printf("I am process %d and i have received %d messages. My final messages have tag %d and value %d, %d\nThe execution time is %lf ms\n",rank, 2*tcnt, tag, buffer[0], buffer[1], time*1000);
    END
    return mean;
}
