#include "stdlib.h"
#include <time.h>
#include "random.h"

unsigned int Myrandom(int MIN,int MAX)
{
    time_t t = time(NULL);
    unsigned int seed = (unsigned int)t;
    unsigned int rnd;
    srand(seed); /* seed是一个种子数值，可以使用当前时间戳等 */
    rnd=rand()%(MAX+1-MIN)+MIN;
    return rnd;
}