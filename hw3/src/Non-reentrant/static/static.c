#include <pthread.h>



void* fun(void* argv){
    int* res = (int*)argv;
    static int x;
    x = *res;
    return NULL;
}
int main() {
    pthread_t t1,t2;
    pthread_create(&t1, NULL, fun, &t1);
    pthread_create(&t2, NULL, fun, &t2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
