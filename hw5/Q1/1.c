#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include  <sys/time.h>
struct compressed_matrix
{
    int row_size;
    int col_size;
    int element_size;
    int* row;
    int* col;
    double* element;
};
//获取随机数
double getRandData(int min,int max){
    double m1 = (double)(rand() % 101) / 101;
    min ++;
    double m2 = (double)((rand() % (max - min + 1) + min));
    m2--;
    return m1 + m2;
}
double* init_vector(int vector_size){
    double* vector = (double*)malloc(sizeof(double) * vector_size);
    for(int i = 0;i < vector_size;i++){
        vector[i] = getRandData(1,10);
    }
    return vector;
}

struct compressed_matrix init_matrix(FILE*stream){
    int row_size,col_size,element_size;
    fscanf(stream,"%d%d%d",&row_size,&col_size,&element_size);
    struct compressed_matrix mat;
    mat.col_size = col_size;
    mat.row_size = row_size;
    mat.element_size = element_size;
    mat.row = (int*)malloc(row_size * sizeof(int));
    mat.col = (int*)malloc(element_size * sizeof(int));
    mat.element = (double*)malloc(element_size * sizeof(double));
    for(int i = 0;i < row_size;i++){
        fscanf(stream,"%d",&mat.row[i]);
        mat.row[i]--;
    }
    for(int i = 0;i < element_size;i++){
        fscanf(stream,"%d",&mat.col[i]);
        mat.col[i]--;
    }
    for(int i = 0;i < element_size;i++){
        fscanf(stream,"%lf",&mat.element[i]);
    }
    return mat;
}

double* serial(struct compressed_matrix matrix,double* vector){
    double*res = (double*)malloc(matrix.row_size * sizeof(double));
    memset(res,0,matrix.row_size * sizeof(double));
    for(int i = 0;i < matrix.row_size;i++){
        int begin = matrix.row[i];
        int end;
        if(i < matrix.row_size - 1)
            end = matrix.row[i + 1] - 1;
        else 
            end = matrix.element_size - 1;
        for(int j = begin;j <= end;j++){
            res[i] += vector[matrix.col[j]] * matrix.element[j];
        }
    }
    return res;
}
double* parallel(struct compressed_matrix matrix,double* vector,int thread_count){
    double*res = (double*)malloc(matrix.row_size * sizeof(double));
    memset(res,0,matrix.row_size * sizeof(double));
    #pragma omp parallel for num_threads(thread_count)
    for(int i = 0;i < matrix.row_size;i++){
        int begin = matrix.row[i];
        int end;
        if(i < matrix.row_size - 1)
            end = matrix.row[i + 1] - 1;
        else 
            end = matrix.element_size - 1;
        for(int j = begin;j <= end;j++){
            res[i] += vector[matrix.col[j]] * matrix.element[j];
        }
    }
    return res;
}
void print_vector(double*res,int len){
    printf("{");
    for(int i = 0;i < len;i++){
        printf("%f",res[i]);
        if(i < len - 1)printf(",");
    }
    printf("}\n");
}
void free_matrix(struct compressed_matrix matrix){
    free(matrix.col);
    free(matrix.row);
    free(matrix.element);
}
int main(int argc, const char * argv[]){
    if(argc < 3){
        abort();
    }
    int thread_count = atoi(argv[1]);
    FILE* matrix_file = fopen(argv[2],"r");
    struct timeval begin,end;
    struct compressed_matrix matrix = init_matrix(matrix_file);
    double* vector = init_vector(matrix.col_size);
    gettimeofday(&begin,NULL);
    double* res = serial(matrix,vector);
    gettimeofday(&end,NULL);
    printf("\n\n");
    printf("        Serial_time is:%ldμs\n",end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
    gettimeofday(&begin,NULL);
    double* parallel_res = parallel(matrix,vector,thread_count);
    gettimeofday(&end,NULL);
    printf("\n\n");
    printf("        There are %d threads.\n",thread_count);
    printf("        Parallel_time is:%ldμs\n",end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
    //print_vector(res,matrix.row_size);
    //print_vector(parallel_res,matrix.row_size);
    free_matrix(matrix);
    free(vector);
    free(res);
    fclose(matrix_file);
    free(parallel_res);
}