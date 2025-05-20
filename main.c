#include "matrix_core.h"
#include "matrix_special.h"
#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");

    // Тест экспоненты
    matrix* D = matrix_create(3, 3);
    if (!D) return 1;

    matrix_set(D, 0, 0, 1.0);
    matrix_set(D, 1, 1, 2.0);
    matrix_set(D, 2, 2, -1.0);

    matrix* expD = matrix_exp(D, 1e-10);
    if (!expD) {
        matrix_free(D);
        return 1;
    }
    printf("Матричная экспонента:\n");
    matrix_print(expD);

    // Тест СЛАУ
    matrix* A = matrix_create(3, 3);
    if (!A) {
        matrix_free(D);
        matrix_free(expD);
        return 1;
    }

    matrix_set(A, 0, 0, 2); matrix_set(A, 0, 1, 1); matrix_set(A, 0, 2, -1);
    matrix_set(A, 1, 0, -3); matrix_set(A, 1, 1, -1); matrix_set(A, 1, 2, 2);
    matrix_set(A, 2, 0, -2); matrix_set(A, 2, 1, 1); matrix_set(A, 2, 2, 2);

    matrix* b = matrix_create(3, 1);
    if (!b) {
        matrix_free(D);
        matrix_free(expD);
        matrix_free(A);
        return 1;
    }

    matrix_set(b, 0, 0, 8);
    matrix_set(b, 1, 0, -11);
    matrix_set(b, 2, 0, -3);

    matrix* x = matrix_solve_gauss(A, b);
    if (!x) {
        printf("\nСистема не имеет решения или матрица вырождена\n");
    } else {
        printf("\nРешение СЛАУ:\n");
        matrix_print(x);

        // Проверка решения
        matrix* check = matrix_create(3, 1);
        if (check) {
            matrix_mul(check, A, x);
            matrix_sub(check, b);
            printf("\nНевязка:\n");
            matrix_print(check);
            printf("Норма невязки: %f\n", matrix_norm(check));
            matrix_free(check);
        }
    }

    // Освобождение памяти
    matrix_free(D);
    matrix_free(expD);
    matrix_free(A);
    matrix_free(b);
    if (x) matrix_free(x);

    return 0;
}
