#include "../src/spmm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ElemType vs0[ENUM] = {1, 2, 3, 4, 5, 6, 7, 8},
         vs1[ENUM] = {1, 2, 0, 3, 2, 0, 1, 0};

ElemType vd[ENUM];

void prepare_data() {
  uint64_t vs0_addr = (uintptr_t)vs0;
  uint64_t vs1_addr = (uintptr_t)vs1;
  LDPRF(0, vs0_addr, vs1_addr);
  LDPRF(1, vs1_addr, vs0_addr);
}

int main() {
  int tem = -1;

  prepare_data();

  uint64_t buf_index = 0;

  uint64_t wb_addr = (uintptr_t)vd;

  STPS(buf_index, 0);
  STPS(buf_index, 0);
  STPS(buf_index, 0); // may execute out of order.
  STRES(wb_addr, buf_index);
  if (memcmp(vs0, vd, VLEN / 8) != 0) {
    printf("error\n");
    exit(1);
  }

  buf_index = 1;
  wb_addr = (uintptr_t)vd;
  STPS(buf_index, 1);
  STPS(buf_index, 1);
  STPS(buf_index, 1);
  STRES(wb_addr, buf_index);
  if (memcmp(vs1, vd, VLEN / 8) != 0) {
    printf("error\n");
    exit(1);
  }

  printf("pass!!!\n");
}
