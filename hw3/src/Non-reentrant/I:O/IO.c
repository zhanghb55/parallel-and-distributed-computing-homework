
#include <pthread.h>
#include <stdio.h>


int Global;
void *fun(void *x) {
  Global = 42;
  printf("%d\n",Global);
  return x;
}
int main() {
  pthread_t t;
  pthread_create(&t, NULL, fun, NULL);
  Global = 43;
  pthread_join(t, NULL);
  return Global;
}
