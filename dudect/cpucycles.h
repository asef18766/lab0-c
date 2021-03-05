#include <stdint.h>
#include <time.h>

static inline int64_t cpucycles(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_nsec;
}
