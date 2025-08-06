# RISC-V SpMM

platform: [xixi-gem5](https://github.com/xixi-shredp/xixi-gem5.git) (branch: spmm-dev)

## Gem5 Config

### function units: (in `boom-gem5/system/cpus/`)

```python
class My_SPMM_Unit(FUDesc):
    opList = [ OpDesc(opClass='SpMMLDVALIDX'),
               OpDesc(opClass='SpMMVSMV'),
               OpDesc(opClass='SpMMLDPRF'),
               OpDesc(opClass='SpMMVSMUL'),
               OpDesc(opClass='SpMMSTPS'),
               OpDesc(opClass='SpMMMERGE'),
               OpDesc(opClass='SpMMSTRES'),
              ]
    count = 1
```

remember to set latency and pipelined for OpDesc.(default: latency = 1, pipelined = true)

### ISA (in `boom-gem5/system/base_system.py`)

```python
class BaseSystem(System):
    ...
    def addCPU(self, CPU):
        self.cpu = CPU
        self.cpu.isa = RiscvISA(
          enable_spmm = True,
          spmm_vlen=128,
          spmm_elen=8,
          spmm_fp=False,
        )
        # enable_spmm default: True
        # spmm_vlen default: 256
    ...
```

- support `int8_t/int16_t/int32_t/int64_t`. (set by spmm_elen.)
- float32 has been supported now. (spmm_fp should be True)
- For debugging, the `MaxSpMMVecLenInBits` is set to 8192, spmm_vlen should be less than half of that. (set in `$GEM5_HOME/src/arch/riscv/types.hh`)

> **NOTE**:
> xixi-gem5 cann't support int and float simultaneously. Currently, the float is supported at the default commit (spmm-dev). If you want to use int-version, please checkout to the int-spmm branch.

### CPU (in `boom-gem5/system/cpus/`)

Added Parameters:

```python
class OoO4WCPU(DerivO3CPU):
    ...
    SpMMBufferSize = 8   # default: 4
    numPhysSpMMRegs = 64 # default: 64
```

## Debug Flag

- SpMMRegs
- SpMMBuffer

## Instruction ASM Usage

in `src/spmm.h` (examples are in `tests/*.c`)

```C
  LDVALIDX(1, 2, addr);     // 1: vs1, 2: vs2. VS1 and VS2 should be imm. (Don't use var.)
  VSMV(1, tem, 0);          // 1: vs1, 0: index. VS1 and index should be imm. (Don't use var.)
  LDPRF(1, addr, pre_addr); // 1: vs1. VS1 should be imm. (Don't use var.)
  VSMUL(2, 0, 1, 0);        // 2: vd, 0: vs1, 1: vs2, 0: index. VS1 VS2, Vd and index should be imm. (Don't use var.)
  STPS(buf_index, 1);       // 1: vs1. VS1 should be imm. (Don't use var.) 
  STRES(wb_addr, buf_index);
  MERGE(2, 0, 1);
```

Notice: STPS, STRES, MERGE with related buffer index should not be used consecutively. (may be issued out of order.)

## Test

1. build executable files first in `./tests/`

```bash
make -C tests
```

2. tests single case

```bash
make test-LDVALIDX
```

3. tests multiple cases

set TEST variables in Makefile first, and then make.

```bash
make # this will run all cases in TEST
```

## TODO List

- [ ] support other ElemType.
- [x] support ISA to enable SpMM Extension by parameter.
- [ ] supprt asm inline by complier.
