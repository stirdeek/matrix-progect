#include "matrix_special.h"
#include "matrix_ops.h"
#include <math.h>
#include <float.h>

matrix* matrix_exp(const matrix* A, double eps) {
    if (A->rows != A->cols) return NULL;

    // Проверяем, является ли матрица диагональной
    int is_diagonal = 1;
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->cols; j++) {
            if (i != j && fabs(matrix_get(A, i, j)) > DBL_EPSILON) {
                is_diagonal = 0;
                break;
            }
        }
        if (!is_diagonal) break;
    }

    if (is_diagonal) {
        // Оптимизированный расчет для диагональной матрицы
        matrix* result = matrix_identity(A->rows);
        if (!result) return NULL;

        for (size_t i = 0; i < A->rows; i++) {
            double diag_val = matrix_get(A, i, i);
            double exp_val = 1.0;
            double term = 1.0;
            int k = 1;

            while (fabs(term) >= eps) {
                term *= diag_val / k;
                exp_val += term;
                k++;
            }
            matrix_set(result, i, i, exp_val);
        }
        return result;
    }

    // Общий случай для недиагональных матриц
    matrix* result = matrix_identity(A->rows);
    matrix* term = matrix_identity(A->rows);
    double factorial = 1.0;
    int k = 1;

    while (1) {
        // Вычисляем следующий член: term = term * A / k
        matrix* next_term = matrix_create(A->rows, A->cols);
        if (!next_term || matrix_mul(next_term, term, A) != 0) {
            matrix_free(next_term);
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        matrix_free(term);
        term = next_term;
        matrix_scalar_div(term, k);
        factorial *= k;

        // Проверяем условие остановки
        if (matrix_norm(term) < eps) {
            matrix_free(term);
            break;
        }

        // Добавляем член к результату
        if (matrix_add(result, term) != 0) {
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        k++;
    }

    return result;
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
