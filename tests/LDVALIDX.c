#include "../src/spmm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint16_t vals[ENUM] = {1, 2, 3, 4, 5, 6, 7, 8},
         idxs[ENUM] = {1, 2, 0, 3, 2, 0, 1, 0};
uint16_t scg[ENUM * 2] = {0};

void prepare_data() {
  for (int i = 0; i < ENUM; i++) {
    scg[i * 2] = vals[i];
    scg[i * 2 + 1] = idxs[i];
  }
}

#define TEST_LDVALIDX_VSMV(x)                                                  \
  VSMV(1, tem, x);                                                             \
  if (tem != vals[x]) {                                                        \
    printf("error in " #x ": tem = %d, ref = %d\n", tem, vals[x]);             \
    exit(1);                                                                   \
  }                                                                            \
  VSMV(2, tem, x);                                                             \
  if (tem != idxs[x]) {                                                        \
    printf("error in " #x ": tem = %d, ref = %d\n", tem, idxs[x]);             \
    exit(1);                                                                   \
  }

int main() {
  prepare_data();
  uint64_t addr = (uintptr_t)scg;
  LDVALIDX(1, 2, addr);
  int tem = -1;
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
