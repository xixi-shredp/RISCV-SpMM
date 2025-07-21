#include "../src/spmm.h"

#include <stdio.h>
#include <stdlib.h>

ElemType vals[ENUM] = {1.5, 2.0, 3.3, 4.8, 5.3, 6.2, 7.2, 8.9};
ElemType pre_vals[ENUM] = {-1.1, -2.5, -3.3, -4.9, -5.3, -6.2, -7.0, -8.1};

#define TEST_LDPRF_VSMV(x)                                                     \
  VSMV(1, tem, x);                                                             \
  if (tem != vals[x]) {                                                        \
    printf("error in " #x ": tem = %lf, ref = %lf\n", tem, vals[x]);             \
    exit(1);                                                                   \
  }

int main() {
  uint64_t addr = (uint64_t)(uintptr_t)vals;
  uint64_t pre_addr = (uint64_t)(uintptr_t)vals;
  LDPRF(1, addr, pre_addr);
  ElemType tem = -1;
  TEST_LDPRF_VSMV(0);
  TEST_LDPRF_VSMV(1);
  TEST_LDPRF_VSMV(2);
  TEST_LDPRF_VSMV(3);
  TEST_LDPRF_VSMV(4);
  TEST_LDPRF_VSMV(5);
  TEST_LDPRF_VSMV(6);
  TEST_LDPRF_VSMV(7);
  printf("pass!!!\n");
}
