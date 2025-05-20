#ifndef MATRIX_OPS_H_INCLUDED
#define MATRIX_OPS_H_INCLUDED

#include "matrix_core.h"

// ������� ��������
matrix* matrix_copy(const matrix* m);
int matrix_add(matrix* a, const matrix* b);
int matrix_sub(matrix* a, const matrix* b);
void matrix_scalar_mul(matrix* m, double scalar);
void matrix_scalar_div(matrix* m, double scalar);

// ��������� ������
int matrix_mul(matrix* result, const matrix* a, const matrix* b);
int matrix_mul2(matrix* result, const matrix* a, const matrix* b);

// ��������� ��������
void matrix_transpose(matrix* m);
void matrix_swap_rows(matrix* m, size_t i1, size_t i2);
double matrix_norm(const matrix* m);

#endif // MATRIX_OPS_H_INCLUDED
