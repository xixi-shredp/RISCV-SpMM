#include "../src/spmm.h"
#include "test_data.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef FP_EN
ElemType vals[ENUM] = FP_DATA1, idxs[ENUM] = FP_DATA2;
#define FMT "%f"
#else
ElemType vals[ENUM] = INT_DATA1, idxs[ENUM] = INT_DATA2;
#define FMT "%d"
#endif

ElemType scg[ENUM * 2] = {0};

void prepare_data() {
  for (int i = 0; i < ENUM; i++) {
    scg[i * 2] = vals[i];
    scg[i * 2 + 1] = idxs[i];
  }
}

#define TEST_LDVALIDX_VSMV(x)                                                  \
  VSMV(1, tem, x);                                                             \
  if (tem != vals[x]) {                                                        \
    printf("error in " #x ": tem = " FMT ", ref = " FMT "\n", tem, vals[x]);   \
    exit(1);                                                                   \
  }                                                                            \
  VSMV(2, tem, x);                                                             \
  if (tem != idxs[x]) {                                                        \
    printf("error in " #x ": tem = " FMT ", ref = " FMT "\n", tem, idxs[x]);   \
    exit(1);                                                                   \
  }

int main() {
  prepare_data();
  uint64_t addr = (uintptr_t)scg;
  LDVALIDX(1, 2, addr);
  ElemType tem = -1;
  TEST_LDVALIDX_VSMV(0);
  TEST_LDVALIDX_VSMV(1);
  TEST_LDVALIDX_VSMV(2);
  TEST_LDVALIDX_VSMV(3);
  TEST_LDVALIDX_VSMV(4);
  TEST_LDVALIDX_VSMV(5);
  TEST_LDVALIDX_VSMV(6);
  TEST_LDVALIDX_VSMV(7);
  printf("pass!!!\n");
}
