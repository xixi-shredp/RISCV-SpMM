run:
	bash ./batchrun_scg.sh

bin:
	riscv64-unknown-linux-gnu-gcc -O2 -static -o ./spmm_scg ./spmm_scg.c -I/opt/gem5/include -L/opt/gem5/util/m5/build/riscv/out -lm5
