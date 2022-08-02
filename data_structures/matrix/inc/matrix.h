#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct Matrix Matrix;

Matrix* matrix_create(size_t rows, size_t columns, size_t elem_size);
void matrix_destroy(Matrix* m);

size_t matrix_get_rows(const Matrix* m);
size_t matrix_get_columns(const Matrix* m);
size_t matrix_get_elem_size(const Matrix* m);

int matrix_update(Matrix* restrict m, size_t row, size_t column, void* restrict new_value);
int matrix_at(Matrix* restrict m, size_t row, size_t column, void* restrict elem);
int matrix_update_symmetric(Matrix* restrict m, size_t row, size_t column, void* restrict new_value);

int matrix_add_rows(Matrix** m, size_t new_rows);
int matrix_add_columns(Matrix** m, size_t new_columns);
int matrix_expand(Matrix** m, size_t new_fields);

Matrix* matrix_copy(const Matrix* m);

#endif // !MATRIX_H
