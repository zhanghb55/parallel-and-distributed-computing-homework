/**
 * Author:  TripleZ<me@triplez.cn>
 * Date:    2018-08-17
 */

#include <immintrin.h>
#include <stdio.h>
#include  <sys/time.h>
#define N 1000000
__m256i vec1[N/8 + 1];
__m256i vec2[N/8 + 1];
__m256i res[N/8 + 1];

int main(int argc, char const *argv[]) {
    for(int i = 0;i < N/8;i++){
        vec1[i] = _mm256_set1_epi32(0);
        vec2[i] = _mm256_set1_epi32(0);
    }
    struct timeval begin,end;
    gettimeofday(&begin,NULL);
    for(int i = 0;i < N/8;i++){
        res[i] = _mm256_add_epi32(vec1[i],vec2[i]);
    }
    gettimeofday(&end,NULL);
    printf("Avx_time is:%ldμs\n",end.tv_sec*1000000 + end.tv_usec - begin.tv_sec*1000000 - begin.tv_usec);
    return 0;
}
