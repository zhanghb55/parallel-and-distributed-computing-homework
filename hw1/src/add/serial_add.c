#include <stdio.h>
#include  <sys/time.h>
#define N 1000000

int a[N],b[N],c[N];
int main(int argc, char const *argv[]) {
    for(int i = 0;i < N;i++){
        a[i] = 0;
        b[i] = 0;
    }
    struct timeval begin,end;
    gettimeofday(&begin,NULL);
    for(int i = 0;i < N;i++){
        c[i] = a[i] + b[i];
    }
    gettimeofday(&end,NULL);
    printf("Serial_time is:%ldμs\n",end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
    return 0;
}
