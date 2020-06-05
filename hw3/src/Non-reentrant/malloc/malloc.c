#include <pthread.h>
#include <stdlib.h>
void* init(void*argv){
    int* array = malloc(sizeof(int) * 10);
    for(int i = 0;i < 10;i++)array[i] = 0;
    free(array);
    return argv;
}

int main(){
    pthread_t id;
    pthread_create(&id, NULL, init, NULL);
    pthread_join(id, NULL);
    return 0;
}