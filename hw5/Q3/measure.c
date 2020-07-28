#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFLEN 1000000

char buf[BUFLEN];

int main(int argc, char **argv)
{ 
    int my_id = 0;
    int p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status_p;
    MPI_Barrier(MPI_COMM_WORLD);
    double my_start,my_end,my_elapsed,elapsed;
    my_start = MPI_Wtime();
    if(my_id == 0){
        for(int i = 1;i < p;i++){
            MPI_Send(buf,BUFLEN,MPI_CHAR,i,i,MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(buf,BUFLEN,MPI_CHAR,0,my_id,MPI_COMM_WORLD,&status_p);
    }
    my_end = MPI_Wtime();

    my_elapsed = my_end - my_start;

    MPI_Reduce(&my_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    if(my_id == 0){
	printf("Time delay is %f s\n",elapsed);
        printf("Bandwidth is %f Mbit/s\n", BUFLEN * 1.0 / (1048576* elapsed));
    }
    MPI_Finalize();


    return 0;
}

