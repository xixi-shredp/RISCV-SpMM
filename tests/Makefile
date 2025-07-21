# TEST_PROGS = LDVALIDX LDPRF VSMUL STPS MERGE
# SRCS = $(patsubst %, %.c, $(TEST_PROGS))

TEST_PROGS = LDVALIDX_fp
SRCS = $(patsubst %, %.cc, $(TEST_PROGS))


default:bin

%: %.c ../src/spmm.h
	riscv64-unknown-linux-gnu-gcc $^ -DElemType=int8_t -static -o $@

%: %.cc ../src/spmm.h
	riscv64-unknown-linux-gnu-g++ -std=c++23 $^ -DElemType=float -static -o $@

bin: $(TEST_PROGS)

clean:
	rm $(TEST_PROGS)

.PHONY: default bin
