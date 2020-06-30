#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define row 1000
#define col 1000

int matrix[row][col];
int vector[col];
int res[row];

int s_res[col];
void serial(){
    for(int i = 0;i < row;i++){
        for(int j = 0;j < col;j++){
            s_res[i] += matrix[i][j] * vector[j];
        }
    }
}
int main(int argc, char **argv)
{ 
    int my_id = 0;
    int temp = 100;
    int p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status_p;
    if(my_id == 0){
        for(int i = 0;i < col;i++){
            vector[i] = rand() % 10;
        }
        for(int i = 0;i < row;i++){
            for(int j = 0;j < col;j++){
                matrix[i][j] = rand() % 10;
            }
        }
    }
    if(my_id == 0){
        for(int i = 1;i < p;i++){
            MPI_Send(matrix[i * row / p],col * row / p,MPI_INT,i,i,MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(matrix[my_id * row / p],col * row / p,MPI_INT,0,my_id,MPI_COMM_WORLD,&status_p);
    }


    MPI_Bcast(vector,col,MPI_INT,0,MPI_COMM_WORLD);

    for(int i = my_id * row / p;i < (my_id + 1) * row / p; i++){
        for(int j = 0;j < col;j++){
            res[i] += matrix[i][j] * vector[j];
        }
    }
    if(my_id == 0){
        for(int i = 1;i < p;i++){
            MPI_Recv(res + i * row / p,row / p,MPI_INT,i,i,MPI_COMM_WORLD,&status_p);
        }
        serial();
        int flag = 1;
        for(int i = 0;i < row;i++){
            if(res[i] != s_res[i]){
                printf("Error!\n");
                flag = 0;
            }
        }
        if(flag){
            printf("Correct calculation!\n");
        }
    }
    else{
        MPI_Send(res + my_id * row / p,row / p,MPI_INT,0,my_id,MPI_COMM_WORLD);
    }

    MPI_Finalize();


    return 0;
}
