#include <stdio.h>
#include <stdlib.h>
#include  <sys/time.h>
#define row 1000
#define col 1000

int matrix[row][col];
int vector[row];

int res[col];
int main(){
    struct timeval begin,end;
    for(int i = 0;i < col;i++){
        vector[i] = rand() % 10;//Initialize Vector
    }
    for(int i = 0;i < row;i++){
        for(int j = 0;j < col;j++){
            matrix[i][j] = rand() % 10;//Initialize Matrix
      }
    }
    gettimeofday(&begin,NULL);
    for(int i = 0;i < row;i++){
        for(int j = 0;j < col;j++){
            res[i] += matrix[i][j] * vector[j];//Multiplication
        }
    }
    gettimeofday(&end,NULL);
    printf("Serial_time is:%ldμs\n",end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
    return 0;
}