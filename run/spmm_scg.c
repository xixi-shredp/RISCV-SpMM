#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gem5/m5ops.h"

#define GROUP_HEIGHT 4

int M, K, N;        // matrix dimensions
float SPARSITY;

// SCG format structure
typedef struct {
    int num_groups;
    float* val;
    int* col_idx;
    int* row_nnz;
    int* row_idx;
} SCGMatrixA;

float **B;
float **C;
SCGMatrixA A;

float rand_float() {
    return (float)(rand() % 1000) / 100.0f;
}

int cmp_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void generate_random_scg() {
    int group_count = (M + GROUP_HEIGHT - 1) / GROUP_HEIGHT;
    A.num_groups = group_count;

    int max_nnz_per_row = (int)((1.0f - SPARSITY) * K);
    if (max_nnz_per_row < 1) max_nnz_per_row = 1;

    int estimated_total_nnz = M * max_nnz_per_row;
    A.val = (float*)malloc(sizeof(float) * estimated_total_nnz);
    A.col_idx = (int*)malloc(sizeof(int) * estimated_total_nnz);
    A.row_nnz = (int*)malloc(sizeof(int) * group_count);
    A.row_idx = (int*)malloc(sizeof(int) * estimated_total_nnz);

    int val_ptr = 0;
    int row_idx_ptr = 0;

    for (int g = 0; g < group_count; g++) {
        int base_row = g * GROUP_HEIGHT;
        int valid_rows = ((base_row + GROUP_HEIGHT) <= M) ? GROUP_HEIGHT : (M - base_row);

        int full_group = 1;
        int temp_val_ptr = val_ptr;

        for (int i = 0; i < valid_rows; i++) {
            int r = base_row + i;
            int nnz = (int)((1.0f - SPARSITY) * K);
            if (nnz < 1) nnz = 1;
            if (nnz > K) nnz = K;

            if (nnz != GROUP_HEIGHT) full_group = 0;

            int* cols = (int*)malloc(sizeof(int) * nnz);
            int* flags = (int*)calloc(K, sizeof(int));
            int count = 0;
            while (count < nnz) {
                int c = rand() % K;
                if (!flags[c]) {
                    flags[c] = 1;
                    cols[count++] = c;
                }
            }
            qsort(cols, nnz, sizeof(int), cmp_int);

            for (int j = 0; j < nnz; j++) {
                A.val[val_ptr] = rand_float();
                A.col_idx[val_ptr] = cols[j];
                if (!full_group) A.row_idx[row_idx_ptr++] = r;
                val_ptr++;
            }

            free(cols);
            free(flags);
        }

        A.row_nnz[g] = full_group ? -1 : valid_rows;
    }
}

void generate_dense_B() {
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < N; ++j)
            B[i][j] = rand_float();
}

void spmm_outer_scg() {
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            C[i][j] = 0.0f;

    int row_idx_ptr = 0;
    int val_ptr = 0;

    m5_reset_stats(0, 0);
    for (int g = 0; g < A.num_groups; g++) {
        int nnz = A.row_nnz[g];
        int group_rows = (nnz == -1) ? GROUP_HEIGHT : nnz;

        for (int i = 0; i < group_rows; i++) {
            int row = (nnz == -1) ? (g * GROUP_HEIGHT + i) : A.row_idx[row_idx_ptr++];
            float a_val = A.val[val_ptr];
            int col = A.col_idx[val_ptr++];
            for (int j = 0; j < N; j++) {
                C[row][j] += a_val * B[col][j];
            }
        }
    }
    m5_dump_stats(0, 0);
}

void cleanup() {
    free(A.val);
    free(A.col_idx);
    free(A.row_nnz);
    free(A.row_idx);

    for (int i = 0; i < K; ++i) free(B[i]);
    free(B);
    for (int i = 0; i < M; ++i) free(C[i]);
    free(C);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <M> <K> <N> <SPARSITY>\n", argv[0]);
        return 1;
    }

    M = atoi(argv[1]);
    K = atoi(argv[2]);
    N = atoi(argv[3]);
    SPARSITY = atof(argv[4]);

    srand(1);

    B = (float **)malloc(K * sizeof(float *));
    for (int i = 0; i < K; ++i) B[i] = (float *)malloc(N * sizeof(float));

    C = (float **)malloc(M * sizeof(float *));
    for (int i = 0; i < M; ++i) C[i] = (float *)malloc(N * sizeof(float));

    generate_random_scg();
    generate_dense_B();
    spmm_outer_scg();

    cleanup();
    m5_exit(0);
    return 0;
}
