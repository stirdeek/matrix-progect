#include "matrix_special.h"
#include "matrix_ops.h"
#include <math.h>
#include <float.h>

matrix* matrix_exp(const matrix* m, double eps) {
    if (m->rows != m->cols) return NULL;

    matrix* result = matrix_identity(m->rows);
    matrix* term = matrix_copy(m);
    if (!result || !term) goto error;

    double factorial = 1.0;
    unsigned n = 1;

    while (matrix_norm(term) >= eps) {
        if (matrix_add(result, term) != 0) goto error;

        matrix* new_term = matrix_create(m->rows, m->cols);
        if (!new_term || matrix_mul(new_term, term, m) != 0) {
            matrix_free(new_term);
            goto error;
        }

        matrix_free(term);
        term = new_term;

        n++;
        factorial *= n;
        matrix_scalar_div(term, factorial);
    }

    matrix_free(term);
    return result;

error:
    matrix_free(result);
    matrix_free(term);
    return NULL;
}

matrix* matrix_solve_gauss(const matrix* A, const matrix* b) {
    if (!A || !b || A->rows != A->cols || A->rows != b->rows || b->cols != 1)
        return NULL;

    // Создаем расширенную матрицу [A|b] правильного размера
    matrix* aug = matrix_create(A->rows, A->cols + 1);
    if (!aug) return NULL;

    // Заполнение расширенной матрицы
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->cols; j++) {
            matrix_set(aug, i, j, matrix_get(A, i, j));
        }
        matrix_set(aug, i, A->cols, matrix_get(b, i, 0));
    }

    // Прямой ход
    for (size_t k = 0; k < aug->rows; k++) {
        // Выбор главного элемента
        size_t max_row = k;
        double max_val = fabs(matrix_get(aug, k, k));

        for (size_t i = k + 1; i < aug->rows; i++) {
            double val = fabs(matrix_get(aug, i, k));
            if (val > max_val) {
                max_val = val;
                max_row = i;
            }
        }

        if (max_val < DBL_EPSILON) {
            matrix_free(aug);
            return NULL;
        }

        if (max_row != k) {
            matrix_swap_rows(aug, k, max_row);
        }

        // Нормализация
        double pivot = matrix_get(aug, k, k);
        for (size_t j = k; j < aug->cols; j++) {
            matrix_set(aug, k, j, matrix_get(aug, k, j) / pivot);
        }

        // Исключение
        for (size_t i = 0; i < aug->rows; i++) {
            if (i != k) {
                double factor = matrix_get(aug, i, k);
                for (size_t j = k; j < aug->cols; j++) {
                    matrix_set(aug, i, j,
                        matrix_get(aug, i, j) - factor * matrix_get(aug, k, j));
                }
            }
        }
    }

    // Извлечение решения
    matrix* x = matrix_create(A->rows, 1);
    if (x) {
        for (size_t i = 0; i < A->rows; i++) {
            matrix_set(x, i, 0, matrix_get(aug, i, A->cols));
        }
    }

    matrix_free(aug);
    return x;
}
