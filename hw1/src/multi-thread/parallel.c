#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define N 1000000

int a[N],b[N],c[N];

int main(int argc,char* argv[]) {
    printf("Input the number of the threads:");
    int n;
    scanf("%d",&n);
    omp_set_num_threads(n);
    struct timeval begin,end;
    gettimeofday(&begin,NULL);
    #pragma omp parallel for
    for(int i = 0;i < N;i++){
        a[i] = b[i] + c[i];
    }
    gettimeofday(&end,NULL);
    printf("Run time with %d threads is %d μs\n",n,end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
}
