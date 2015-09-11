#ifndef _BENCH_H_
#define _BENCH_H_

typedef unsigned long long u64;
typedef unsigned int uint;
typedef unsigned long long ull;

/** From the Linux Kernel */
static inline unsigned long long read_tscp(unsigned int *aux)
{
        unsigned long low, high;
        asm volatile("cpuid");
        asm volatile("rdtsc"
                        : "=a" (low), "=d" (high), "=c" (*aux));
        return low | ((u64)high << 32);
}

#endif
