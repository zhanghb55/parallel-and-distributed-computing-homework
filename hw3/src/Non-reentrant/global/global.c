#include <pthread.h>

int global = 10;

void* fun(void* argv){
    global = 20;
    return NULL;
}
int main() {
  pthread_t t;
  pthread_create(&t, NULL, fun, NULL);
  global = 43;
  pthread_join(t, NULL);
  return global;
}