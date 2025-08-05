#include "../src/spmm.h"
#include "test_data.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef FP_EN
ElemType vals[ENUM] = FP_DATA1, pre_vals[ENUM] = FP_DATA2;
#define FMT "%f"
#else
ElemType vals[ENUM] = INT_DATA1, pre_vals[ENUM] = INT_DATA2;
#define FMT "%d"
#endif

#define TEST_LDPRF_VSMV(x)                                                     \
  VSMV(1, tem, x);                                                             \
  if (tem != vals[x]) {                                                        \
    printf("error in " #x ": tem = " FMT ", ref = " FMT "\n", tem, vals[x]);   \
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
