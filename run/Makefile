run:
	bash ./batchrun_scg.sh

bin:
	riscv64-unknown-linux-gnu-gcc -O2 -static -o ./new_spmm_scg ./new_spmm_scg.c -I/opt/gem5/include -L. -lm5

qemu-bin:
	riscv64-unknown-linux-gnu-gcc -O3 -static -o ./spmm_scg_x86 ./spmm_scg_x86.c
	riscv64-unknown-linux-gnu-gcc -O3 -static -o ./spmm_csc_x86 ./spmm_csc_x86.c

qemu-test:
	qemu-riscv64 ./spmm_scg_x86 4096 4096 128 0.5
	qemu-riscv64 ./spmm_csc_x86 4096 4096 128 0.5
	qemu-riscv64 ./spmm_scg_x86 500 100 100 0.5
	qemu-riscv64 ./spmm_csc_x86 500 100 100 0.5
	qemu-riscv64 ./spmm_scg_x86 2048 2048 128 0.5
	qemu-riscv64 ./spmm_csc_x86 2048 2048 128 0.5
