#ifndef MATRIX_CORE_H_INCLUDED
#define MATRIX_CORE_H_INCLUDED

#include <stddef.h>

struct matrix {
    double* data;
    size_t rows;
    size_t cols;
};

typedef struct matrix matrix;

// �������� � ��������
matrix* matrix_create(size_t rows, size_t cols);
void matrix_free(matrix* m);

// ������ � ���������
double matrix_get(const matrix* m, size_t row, size_t col);
void matrix_set(matrix* m, size_t row, size_t col, double value);

// ����-�����
void matrix_print(const matrix* m);

// ����������� �������
matrix* matrix_zero(size_t rows, size_t cols);
matrix* matrix_identity(size_t n);

// ��������� �� ��������
double* matrix_ptr(matrix* m, size_t row, size_t col);
const double* matrix_cptr(const matrix* m, size_t row, size_t col);

#endif // MATRIX_CORE_H_INCLUDED
