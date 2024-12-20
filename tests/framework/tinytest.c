#include "tinytest.h"

int m_nttcheckqty = 0x0;

ttret_t tinytest_suit(const tinytest_t *cttArrTests, int nNumTests) {
    printf("%s", "\n\nTests:\n");
    int failed = 0;
    for (int i = 0; i < nNumTests; ++i) {
        printf(" %02d%s%-25s ", i, ": ", cttArrTests[i].name);
        int linerr = cttArrTests[i].func();
        if (linerr == 0)
            printf("%s", "OK\n");
        else {
            printf("%s%d\n", "Failed, line: ", linerr);
            ++failed;
        }
    }
    printf("\n%s%d\n", "Total checks: ", m_nttcheckqty);
    printf("%s[ %d / %d ]\r\n\n\n", "Tests PASS: ", nNumTests - failed, nNumTests);
    return failed;
}