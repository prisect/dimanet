/* libdntest.h - minimal assertion helpers for the DimaNet test suite. */
#ifndef LIBDNTEST_H
#define LIBDNTEST_H

#include <math.h>
#include <stdio.h>
#include <time.h>

#define TEST_TOLERANCE 0.001

/* Track the number of passes and fails. */
static int ltests = 0;
static int lfails = 0;

/* Print a summary of the results. */
#define lresults()                                                          \
    do {                                                                    \
        if (lfails == 0)                                                    \
            printf("-- ALL TESTS PASSED: (%d/%d)\n", ltests, ltests);      \
        else                                                                \
            printf("-- SOME TESTS FAILED: (%d/%d)\n", ltests - lfails,     \
                   ltests);                                                 \
    } while (0)

/* Run a test function and print its result. */
#define lrun(name, fn)                                                      \
    do {                                                                    \
        const int ts = ltests;                                              \
        const int fs = lfails;                                              \
        const clock_t start = clock();                                      \
        printf("\t%-20s", name);                                            \
        fn();                                                               \
        printf("pass:%4d   fail:%2d   %4dms\n",                             \
               (ltests - ts) - (lfails - fs), lfails - fs,                  \
               (int)((clock() - start) * 1000 / CLOCKS_PER_SEC));           \
    } while (0)

/* Assert that a condition is true. */
#define lok(test)                                                           \
    do {                                                                    \
        ++ltests;                                                           \
        if (!(test)) {                                                      \
            ++lfails;                                                       \
            printf("%s:%d assertion failed: %s\n", __FILE__, __LINE__,      \
                   #test);                                                  \
        }                                                                   \
    } while (0)

/* Assert that two integers are equal. */
#define lequal(a, b)                                                        \
    do {                                                                    \
        ++ltests;                                                           \
        if ((a) != (b)) {                                                   \
            ++lfails;                                                       \
            printf("%s:%d (%lld != %lld)\n", __FILE__, __LINE__,            \
                   (long long)(a), (long long)(b));                         \
        }                                                                   \
    } while (0)

/* Assert that two doubles are within TEST_TOLERANCE of each other. */
#define lfequal(a, b)                                                       \
    do {                                                                    \
        ++ltests;                                                           \
        if (fabs((double)(a) - (double)(b)) > TEST_TOLERANCE) {             \
            ++lfails;                                                       \
            printf("%s:%d (%f != %f)\n", __FILE__, __LINE__,                \
                   (double)(a), (double)(b));                               \
        }                                                                   \
    } while (0)

#endif /* LIBDNTEST_H */
