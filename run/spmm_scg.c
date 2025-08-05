#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "gem5/m5ops.h"

int M, K, N;
float SPARSITY;

#define TILE_M 32
#define TILE_N 32

// SCG格式结构体
typedef struct {
    int num_groups;
    float *val;      // 所有非零值
    int *col_idx;    // 每个值对应的原始列号
    int *row_idx;    // 非满组需要的原始行号
    int *row_nnz;    // 每组非零元素数量，-1 表示满组
} SCGMatrixA;

SCGMatrixA A = {0};
float **B = NULL;
float **C = NULL;

float rand_float() {
    return (float)(rand() % 1000) / 100.0f;
}

int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void generate_and_convert_scg() {
    typedef struct {
        float val;
        int col;
    } Entry;

    Entry **rows = (Entry **)malloc(sizeof(Entry *) * M);
    int *row_lengths = (int *)calloc(M, sizeof(int));
    int max_nnz_per_row = 0;

    for (int i = 0; i < M; ++i) {
        rows[i] = (Entry *)malloc(sizeof(Entry) * K);
        for (int j = 0; j < K; ++j) {
            if (((float)rand() / RAND_MAX) > SPARSITY) {
                rows[i][row_lengths[i]].val = rand_float();
                rows[i][row_lengths[i]].col = j;
                row_lengths[i]++;
            }
        }
        if (row_lengths[i] > max_nnz_per_row)
            max_nnz_per_row = row_lengths[i];
    }

    A.num_groups = max_nnz_per_row;
    A.row_nnz = (int *)malloc(sizeof(int) * A.num_groups);
    A.val = (float *)malloc(sizeof(float) * M * max_nnz_per_row);
    A.col_idx = (int *)malloc(sizeof(int) * M * max_nnz_per_row);
    A.row_idx = (int *)malloc(sizeof(int) * M * max_nnz_per_row);

    int val_ptr = 0, row_ptr = 0;

    for (int g = 0; g < A.num_groups; ++g) {
        typedef struct {
            int row;
            float val;
            int col;
        } GroupEntry;

        GroupEntry *group_entries = (GroupEntry *)malloc(sizeof(GroupEntry) * M);
        int count = 0;

        for (int i = 0; i < M; ++i) {
            if (row_lengths[i] > g) {
                group_entries[count].row = i;
                group_entries[count].val = rows[i][g].val;
                group_entries[count].col = rows[i][g].col;
                count++;
            }
        }

        A.row_nnz[g] = (count == M) ? -1 : count;

        if (count != M)
            qsort(group_entries, count, sizeof(GroupEntry), cmp_int);

        for (int i = 0; i < count; ++i) {
            A.val[val_ptr] = group_entries[i].val;
            A.col_idx[val_ptr++] = group_entries[i].col;
            if (A.row_nnz[g] != -1)
                A.row_idx[row_ptr++] = group_entries[i].row;
        }
        free(group_entries);
    }

    for (int i = 0; i < M; ++i) free(rows[i]);
    free(rows);
    free(row_lengths);
}

void generate_dense_B() {
    B = (float **)malloc(sizeof(float *) * K);
    for (int i = 0; i < K; ++i) {
        B[i] = (float *)malloc(sizeof(float) * N);
        for (int j = 0; j < N; ++j)
            B[i][j] = rand_float();
    }
}

void allocate_C() {
    C = (float **)malloc(sizeof(float *) * M);
    for (int i = 0; i < M; ++i) {
        C[i] = (float *)calloc(N, sizeof(float));
    }
}

void spmm_outer_scg() {
    int val_ptr = 0, row_ptr = 0;
    m5_reset_stats(0, 0);

    for (int i0 = 0; i0 < M; i0 += TILE_M) {
        for (int j0 = 0; j0 < N; j0 += TILE_N) {
            for (int g = 0; g < A.num_groups; ++g) {
                int count = A.row_nnz[g];
                if (count == -1) {
                    for (int i = i0; i < i0 + TILE_M && i < M; ++i) {
                        float a_val = A.val[val_ptr];
                        int col = A.col_idx[val_ptr++];
                        for (int j = j0; j < j0 + TILE_N && j < N; ++j) {
                            C[i][j] += a_val * B[col][j];
                        }
                    }
                } else {
                    for (int i = 0; i < count; ++i) {
                        int row = A.row_idx[row_ptr++];
                        if (row < i0 || row >= i0 + TILE_M) continue;
                        float a_val = A.val[val_ptr];
                        int col = A.col_idx[val_ptr++];
                        for (int j = j0; j < j0 + TILE_N && j < N; ++j) {
                            C[row][j] += a_val * B[col][j];
                        }
                    }
                }
            }
            val_ptr = 0;
            row_ptr = 0;
        }
    }
    m5_dump_stats(0, 0);
}

void cleanup() {
    if (A.val) free(A.val);
    if (A.col_idx) free(A.col_idx);
    if (A.row_idx) free(A.row_idx);
    if (A.row_nnz) free(A.row_nnz);
    if (B) {
        for (int i = 0; i < K; ++i) free(B[i]);
        free(B);
    }
    if (C) {
        for (int i = 0; i < M; ++i) free(C[i]);
        free(C);
    }
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

    srand(0);

    generate_and_convert_scg();
    generate_dense_B();
    allocate_C();

    spmm_outer_scg();

    m5_exit(0);

    cleanup();
    return 0;
}
