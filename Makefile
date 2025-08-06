GEM5 = gem5-riscv
GEM5_DEBUG = /opt/gem5/build/RISCV/gem5.debug
# TEST = /opt/gem5/tests/test-progs/hello/bin/riscv/linux/hello
TEST = MERGE 
# LDVALIDX LDPRF VSMUL STPS MERGE

TEST_PROGRAM = $(patsubst %,./tests/%,$(TEST))

DEBUG_FLAG = --debug-flags=SpMMRegs,SpMMBuffer
GEM5_FLAGS = ./boom-gem5/simulate.py --cpu="OoO-4W"

default:
	make $(patsubst %,test-%,$(TEST))

test-%:
	$(GEM5) $(DEBUG_FLAG) $(GEM5_FLAGS) run-benchmark -c ./tests/$*

#gdb --args $(GEM5_DEBUG) $(GEM5_FLAGS) run-benchmark -c ./tests/$*

.PHONY: default spmm
