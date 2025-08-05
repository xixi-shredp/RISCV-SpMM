TEST_PROGS = LDVALIDX LDPRF VSMUL STPS MERGE
SRCS = $(patsubst %, %.c, $(TEST_PROGS))


default:bin

%: %.c ../src/spmm.h
	riscv64-unknown-linux-gnu-gcc $^ -static -o $@

%: %.cc ../src/spmm.h
	riscv64-unknown-linux-gnu-g++ -std=c++23 $^ -static -o $@

bin: $(TEST_PROGS)

clean:
	rm $(TEST_PROGS)

.PHONY: default bin
