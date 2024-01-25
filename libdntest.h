/* libdntest.h - library dimanet test */

#ifndef __LIBDNTEST_H__
#define __LIBDNTEST_H__

#include <stdio.h>
#include <math.h>
#include <time.h>

/* How far apart can floats be before we consider them unequal. */
#define LTEST_FLOAT_TOLERANCE 0.001

/* Track the number of passes, fails. */
/* NB this is made for all tests to be in one file. */
static int ltests = 0;
static int lfails = 0;

/* Display the test results. */
#define lresults() do {\
    if (lfails == 0) {\
        printf("ALL TESTS PASSED (%d/%d)\n", ltests, ltests);\
    } else {\
        printf("SOME TESTS FAILED (%d/%d)\n", ltests-lfails, ltests);\
    }\
} while (0)

/* Run a test. Name can be any string to print out, test is the function name to call. */
#define lrun(name, test) do {\
    const int ts = ltests;\
    const int fs = lfails;\
    const clock_t start = clock();\
    printf("\t%-14s", name);\
    test();\
    printf("pass:%2d   fail:%2d   %4dms\n",\
            (ltests-ts)-(lfails-fs), lfails-fs,\
            (int)((clock() - start) * 1000 / CLOCKS_PER_SEC));\
} while (0)


/* Assert a true statement. */
#define lok(test) do {\
    ++ltests;\
    if (!(test)) {\
        ++lfails;\
        printf("%s:%d error \n", __FILE__, __LINE__);\
    }} while (0)

/* Assert two integers are equal. */
#define lequal(a, b) do {\
    ++ltests;\
    if ((a) != (b)) {\
        ++lfails;\
        printf("%s:%d (%d != %d)\n", __FILE__, __LINE__, (a), (b));\
    }} while (0)

/* Assert two floats are equal (Within LTEST_FLOAT_TOLERANCE). */
#define lfequal(a, b) do {\
    ++ltests;\
    if (fabs((double)(a)-(double)(b)) > LTEST_FLOAT_TOLERANCE) {\
        ++lfails;\
        printf("%s:%d (%f != %f)\n", __FILE__, __LINE__, (double)(a), (double)(b));\
    }} while (0)

#endif /*__LIBDNTEST_H__*/
