#include "../src/spmm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ElemType vs0[ENUM] = {1, 2, 3, 4, 5, 6, 7, 8},
         vs1[ENUM] = {1, 2, 0, 3, 2, 0, 1, 0};

ElemType vd[ENUM], ref[ENUM];

void prepare_data() {
  uint64_t vs0_addr = (uintptr_t)vs0;
  uint64_t vs1_addr = (uintptr_t)vs1;
  LDPRF(0, vs0_addr, vs1_addr);
  LDPRF(1, vs1_addr, vs0_addr);
  STPS(0, 0);
  STPS(1, 1);
}

void ref_merge() {
  for (int i = 0; i< ENUM;i++){
    ref[i] = vs0[i] + vs1[i];
  }
}

int main() {
  int tem = -1;

  prepare_data();

  MERGE(2, 0, 1);
  
  ref_merge();

  uint64_t wb_addr = (uintptr_t)vd;
  STRES(wb_addr, 2);

  if (memcmp(ref, vd, VLEN / 8) != 0) {
    printf("error\n");
    exit(1);
  }

  printf("pass!!!\n");
}
