#ifndef SPMM_H
#define SPMM_H

#include <stdint.h>

#define ElemType float
#define FP_EN
#define VLEN 256
#define ELEN (sizeof(ElemType) * 8)

#define ENUM (VLEN / ELEN)

#define LDVALIDX(vr0, vr1, rd) \
  asm volatile(".insn r 0x0b, 0x0, 0x0, %0, x" #vr1 ", x" #vr0 \
              ::"r"(rd):)

#ifdef FP_EN
#define VSMV(vr0, rd, index) \
  asm volatile(".insn r 0x0b, 0x1, " #index ", %0, x" #vr0 ", x0" \
              :"=f"(rd):)
#else
#define VSMV(vr0, rd, index) \
  asm volatile(".insn r 0x0b, 0x1, " #index ", %0, x" #vr0 ", x0" \
              :"=r"(rd):)
#endif

#define LDPRF(vrd, rs1, rs2) \
  asm volatile(".insn r 0x0b, 0x2, 0x0, x" #vrd ", %0, %1" \
              :: "r"(rs1), "r"(rs2))

#define VSMUL(vd, vs1, vs2, index) \
  asm volatile(".insn r 0x0b, 0x3, " #index ", x" #vd ", x" #vs1 ", x" #vs2 )

#define STPS(bd, vs1) \
  asm volatile(".insn r 0x0b, 0x4, 0x0, %0, x" #vs1 ", x0" \
              :: "r"(bd))

#define MERGE(bd, bs1, bs2) \
  asm volatile(".insn r 0x0b, 0x5, 0x0, %0, %1, %2" \
              ::"r"(bd), "r"(bs1), "r"(bs2))

#define STRES(rd, bs1) \
  asm volatile(".insn r 0x0b, 0x6, 0x0, %[rg1], %[rg2], x0" \
              :: [rg1]"r"(rd), [rg2]"r"(bs1): "memory")

#endif
