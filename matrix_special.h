#ifndef MATRIX_SPECIAL_H_INCLUDED
#define MATRIX_SPECIAL_H_INCLUDED

#include "matrix_core.h"

// ��������� ����������
matrix* matrix_exp(const matrix* m, double eps);

// ������� ����
matrix* matrix_solve_gauss(const matrix* A, const matrix* b);

#endif // MATRIX_SPECIAL_H_INCLUDED
