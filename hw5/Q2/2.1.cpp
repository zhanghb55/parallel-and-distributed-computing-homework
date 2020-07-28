#include <iostream>
#include <queue>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#define MaxSize 50
using namespace std;

typedef int resources;
sem_t n ,s ;

queue<resources> all_resources;

resources produce(){
    for(int i = 0;i < 5000;i++);
    if(all_resources.empty()){
        return 1;
    }
    else{
        return all_resources.back() + 1;
    }
}

void consume(resources i){
    printf("Resources %d is poped.\n",i);
}

void producer(){
    while(true){
        resources x = produce();   
        sem_wait(&s);
        all_resources.push(x);
        printf("Resources %d is pushed.\n",x);
        sem_post(&s);
        sem_post(&n);
    }
}
void consumer(){
    while(true){
        resources x;
        sem_wait(&n);
        sem_wait(&s);
        x = all_resources.front();
        all_resources.pop();
        sem_post(&s);
        consume(x);
    }
}

int main(){
    sem_init(&n,0,0);
    sem_init(&s,0,1);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            producer();
        }
        #pragma omp section
        {
            consumer();
        }
    }
}
