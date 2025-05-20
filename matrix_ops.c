#include "matrix_ops.h"
#include <math.h>
#include <string.h>
#include <float.h>
int matrix_sub(matrix* a, const matrix* b) {
    if (a->rows != b->rows || a->cols != b->cols) return -1;
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        a->data[i] -= b->data[i];
    }
    return 0;
}

void matrix_scalar_mul(matrix* m, double scalar) {
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        m->data[i] *= scalar;
    }
}

void matrix_scalar_div(matrix* m, double scalar) {
    if (fabs(scalar) < DBL_EPSILON) return;
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        m->data[i] /= scalar;
    }
}

void matrix_swap_rows(matrix* m, size_t i1, size_t i2) {
    if (i1 == i2 || i1 >= m->rows || i2 >= m->rows) return;
    for (size_t j = 0; j < m->cols; j++) {
        double tmp = matrix_get(m, i1, j);
        matrix_set(m, i1, j, matrix_get(m, i2, j));
        matrix_set(m, i2, j, tmp);
    }
}

matrix* matrix_copy(const matrix* m) {
    matrix* copy = matrix_create(m->rows, m->cols);
    if (!copy) return NULL;

    memcpy(copy->data, m->data, m->rows * m->cols * sizeof(double));
    return copy;
}

int matrix_add(matrix* a, const matrix* b) {
    if (a->rows != b->rows || a->cols != b->cols) return -1;

    for (size_t i = 0; i < a->rows * a->cols; i++) {
        a->data[i] += b->data[i];
    }
    return 0;
}

int matrix_mul(matrix* result, const matrix* a, const matrix* b) {
    if (a->cols != b->rows || result->rows != a->rows || result->cols != b->cols)
        return -1;

    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            matrix_set(result, i, j, sum);
        }
    }
    return 0;
}

int matrix_mul2(matrix* result, const matrix* a, const matrix* b) {
    return matrix_mul(result, a, b);
}

void matrix_transpose(matrix* m) {
    matrix* temp = matrix_copy(m);
    if (!temp) return;

    size_t tmp = m->rows;
    m->rows = m->cols;
    m->cols = tmp;

    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            matrix_set(m, i, j, matrix_get(temp, j, i));
        }
    }

    matrix_free(temp);
}

double matrix_norm(const matrix* m) {
    double max = 0.0;
    for (size_t i = 0; i < m->rows; i++) {
        double sum = 0.0;
        for (size_t j = 0; j < m->cols; j++) {
            sum += fabs(matrix_get(m, i, j));
        }
        if (sum > max) max = sum;
    }
    return max;
}
