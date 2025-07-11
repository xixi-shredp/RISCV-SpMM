#include "../src/spmm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

ElemType vs0[ENUM] = {1, 2, 3, 4, 5, 6, 7, 8},
         vs1[ENUM] = {1, 2, 0, 3, 2, 0, 1, 0};
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
    printf("error in " #x ": tem = %d, ref = %d\n", tem, vd[x]);               \
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
  int tem = -1;

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
