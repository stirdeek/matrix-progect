#include "matrix_core.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

matrix* matrix_create(size_t rows, size_t cols) {
    matrix* m = malloc(sizeof(matrix));
    if (!m) return NULL;

    m->data = calloc(rows * cols, sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    return m;
}

void matrix_free(matrix* m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

double matrix_get(const matrix* m, size_t row, size_t col) {
    assert(row < m->rows && col < m->cols);
    return m->data[row * m->cols + col];
}

void matrix_set(matrix* m, size_t row, size_t col, double value) {
    assert(row < m->rows && col < m->cols);
    m->data[row * m->cols + col] = value;
}

double* matrix_ptr(matrix* m, size_t row, size_t col) {
    assert(row < m->rows && col < m->cols);
    return &m->data[row * m->cols + col];
}

const double* matrix_cptr(const matrix* m, size_t row, size_t col) {
    assert(row < m->rows && col < m->cols);
    return &m->data[row * m->cols + col];
}

matrix* matrix_zero(size_t rows, size_t cols) {
    matrix* m = matrix_create(rows, cols);
    if (m) {
        for (size_t i = 0; i < rows * cols; i++) {
            m->data[i] = 0.0;
        }
    }
    return m;
}

matrix* matrix_identity(size_t n) {
    matrix* m = matrix_zero(n, n);
    if (m) {
        for (size_t i = 0; i < n; i++) {
            matrix_set(m, i, i, 1.0);
        }
    }
    return m;
}

void matrix_print(const matrix* m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.4f ", matrix_get(m, i, j));
        }
        printf("\n");
    }
}
