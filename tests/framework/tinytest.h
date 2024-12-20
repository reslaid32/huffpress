#ifndef TINYTEST_H
#define TINYTEST_H

#include <stdio.h>

typedef int tinytestret_t, ttret_t;
typedef struct _s_tiny_test {
    ttret_t (*func)(void);
    char const *name;
} tinytest_t, ttest_t;

extern int m_nttcheckqty;

#define tinytestdone() return 0
#define tinytestfail() return __LINE__
#define tinytestcheck(x) do { ++m_nttcheckqty; if (!(x)) ttfail(); } while (0)
#define ttdone() tinytestdone()
#define ttfail() tinytestfail()
#define ttcheck(x) tinytestcheck(x)
#define tt_suit(cttArrTests, nNumTests) tinytest_suit(cttArrTests, nNumTests)
ttret_t tinytest_suit(const ttest_t *cttArrTests, int nNumTests);
#define ttnumtests(cttArrTests) sizeof(cttArrTests) / sizeof(cttArrTests[0])

#endif // TINYTEST_H