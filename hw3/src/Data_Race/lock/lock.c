#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double res = 1;
int count = 8;//线程数量
int accuracy = 1000;//计算精度，用多项式项数表示
pthread_mutex_t lock;

void *Add(void *ptr) {
    long thread_order = (int)ptr;//第i个线程，用于表示线程序号
    long n = thread_order;//第n项

    long loop_num = accuracy / count;
    if(accuracy % count)loop_num++;
    for(long j = 0;j < loop_num;j++){
        n += j * count;//n % count = thread_order
        long long denominator = 1;
        for(int i = 2;i <= n;i++){
            denominator *= i;
        }
        double term = 0;
        if(denominator > 0)term = 1.0/denominator;

        pthread_mutex_lock(&lock);
        res += term;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
int main(int argc,char **argv) {
    if(argc == 3){
        count = atoi(argv[1]);
        accuracy = atoi(argv[2]);
    }
    pthread_t handle[count];
    long i;
    for(i = 1;i <= count;i++){
   
        pthread_create(&handle[i - 1],NULL,Add,(void*)i);
    }
    for(int i = 1;i <= count;i++){
        pthread_join(handle[i - 1],NULL);
    }
    printf("%f\n",res);
}