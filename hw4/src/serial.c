#include <stdio.h>
#include <stdlib.h>
#define row 1000
#define col 1000

int matrix[row][col];
int vector[row];

int res[col];
int main(){
    for(int i = 0;i < col;i++){
        vector[i] = rand() % 10;//Initialize Vector
    }
    for(int i = 0;i < row;i++){
        for(int j = 0;j < col;j++){
            matrix[i][j] = rand() % 10;//Initialize Matrix
      }
    }
    for(int i = 0;i < row;i++){
        for(int j = 0;j < col;j++){
            res[i] += matrix[i][j] * vector[j];//Multiplication
        }
    }
    return 0;
}
