#include "../src/spmm.h"

#include <stdio.h>
#include <stdlib.h>

ElemType vals[ENUM] = {1, 2, 3, 4, 5, 6, 7, 8};
ElemType pre_vals[ENUM] = {-1, -2, -3, -4, -5, -6, -7, -8};

#define TEST_LDPRF_VSMV(x)                                                     \
  VSMV(1, tem, x);                                                             \
  if (tem != vals[x]) {                                                        \
    printf("error in " #x ": tem = %ld, ref = %d\n", tem, vals[x]);             \
    exit(1);                                                                   \
  }

int main() {
  uint64_t addr = (uint64_t)(uintptr_t)vals;
  uint64_t pre_addr = (uint64_t)(uintptr_t)vals;
  LDPRF(1, addr, pre_addr);
  int64_t tem = -1;
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
