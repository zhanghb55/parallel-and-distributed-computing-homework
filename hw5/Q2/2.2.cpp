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
    for(int i = 0;i < 1000;i++)
	for(int j = 0;j < 1000;j++);
    if(all_resources.empty()){
        return 1;
    }
    else{
        return all_resources.back() + 1;
    }
}

void consume(resources i,int id){
    printf("Resources %d is poped by thread %d.\n",i,id);
}

void producer(int id){
    while(true){
        resources x = produce();   
        sem_wait(&s);
        all_resources.push(x);
        printf("Resources %d is pushed by thread %d.\n",x,id);
        sem_post(&s);
        sem_post(&n);
	if(id == MaxSize)break;
    }
}
void consumer(int id){
    while(true){
        resources x;
        sem_wait(&n);
        sem_wait(&s);
        x = all_resources.front();
        all_resources.pop();
        sem_post(&s);
        consume(x,id);
    }
}

int main(){
    int p_count = 8;
    int c_count = 4;
    sem_init(&n,0,0);
    sem_init(&s,0,1);
    
	#pragma omp parallel num_threads(p_count + c_count)
	{
	    int id = omp_get_thread_num();
	    #pragma omp parallel sections
	    {
		#pragma omp section
		{
		    if(id < p_count)
		        producer(id);
		}
		#pragma omp section
		{
		    if(id >= p_count)
		        consumer(id);
		}
	    }
	}
}
