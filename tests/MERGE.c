#include "../src/spmm.h"
#include "test_data.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef FP_EN
ElemType vs0[ENUM] = FP_DATA1, vs1[ENUM] = FP_DATA2;
#define FMT "%f"
#else
ElemType vs0[ENUM] = INT_DATA1, vs1[ENUM] = INT_DATA2;
#define FMT "%d"
#endif

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
  ElemType tem = -1;

  prepare_data();

  MERGE(2, 0, 1);

  ref_merge();

  uint64_t wb_addr = (uintptr_t)vd;
  STRES(wb_addr, 2);

  if (memcmp(ref, vd, VLEN / 8) != 0) {
    printf("error\n");
    printf("ref  dut\n");
    for (int i = 0 ;i < ENUM;i++){
      printf(FMT " " FMT "\n", ref[i], vd[i]);
    }
    exit(1);
  }

  printf("pass!!!\n");
}
