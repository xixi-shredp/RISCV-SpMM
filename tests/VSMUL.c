#include "../src/spmm.h"
#include "test_data.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef FP_EN
ElemType vs0[ENUM] = FP_DATA1, vs1[ENUM] = FP_DATA2;
#define FMT "%f"
#else
ElemType vs0[ENUM] = INT_DATA1, vs1[ENUM] = INT_DATA2;
#define FMT "%d"
#endif

ElemType vd[ENUM];

void prepare_data() {
  uint64_t vs0_addr = (uintptr_t)vs0;
  uint64_t vs1_addr = (uintptr_t)vs1;
  LDPRF(0, vs0_addr, vs1_addr);
  LDPRF(1, vs1_addr, vs0_addr);
}

void ref_vsmul(int imm) {
  for (int i = 0; i < ENUM; i++) {
    vd[i] = vs0[imm] * vs1[i];
  }
}

#define TEST_VSMUL_VSMV(x)                                                     \
  VSMV(2, tem, x);                                                             \
  if (tem != vd[x]) {                                                          \
    printf("error in " #x ": tem = " FMT ", ref = " FMT "\n", tem, vd[x]);     \
    exit(1);                                                                   \
  }

#define TEST_VSMUL(imm)                                                        \
  VSMUL(2, 0, 1, imm);                                                         \
  ref_vsmul(imm);                                                              \
  TEST_VSMUL_VSMV(0);                                                          \
  TEST_VSMUL_VSMV(1);                                                          \
  TEST_VSMUL_VSMV(2);                                                          \
  TEST_VSMUL_VSMV(3);                                                          \
  TEST_VSMUL_VSMV(4);                                                          \
  TEST_VSMUL_VSMV(5);                                                          \
  TEST_VSMUL_VSMV(6);                                                          \
  TEST_VSMUL_VSMV(7);

int main() {
  ElemType tem = -1;

  prepare_data();

  TEST_VSMUL(0);
  TEST_VSMUL(1);
  TEST_VSMUL(2);
  TEST_VSMUL(3);
  TEST_VSMUL(4);
  TEST_VSMUL(5);
  TEST_VSMUL(6);
  TEST_VSMUL(7);
  printf("pass!!!\n");
}
