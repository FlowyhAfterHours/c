#include <matrix.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Matrix
{
  size_t rows;
  size_t columns;
  size_t elem_size;
  uint8_t elems[];
};

Matrix* matrix_create(const size_t rows, const size_t columns, const size_t elem_size)
{
  if (rows == 0 || columns == 0 || elem_size == 0)
    return NULL;
  
  register const size_t matrix_capacity = rows * columns * elem_size;
  Matrix* m = calloc(1, sizeof(*m) + matrix_capacity);

  if (m == NULL)
    return NULL;

  m->rows = rows;
  m->columns = columns;
  m->elem_size = elem_size;

  return m;
}

void matrix_destroy(Matrix* m)
{
  free(m);
}

size_t matrix_get_rows(const Matrix* m)
{
  if (m == NULL)
    return 0;

  return m->rows;
}

size_t matrix_get_columns(const Matrix* m)
{
  if (m == NULL)
    return 0;

  return m->columns;
}

size_t matrix_get_elem_size(const Matrix* m)
{
  if (m == NULL)
    return 0;

  return m->elem_size;
}

#define MATRIX_ELEM(matrix, row, column) \
  &(matrix)->elems[((row) * (matrix)->columns * (matrix)->elem_size) + ((column) * (matrix)->elem_size)]

#define MATRIX_ELEM_OFFSET(matrix, row, column, OFFSET) \
  &(matrix)->elems[((row) * (matrix)->columns * (matrix)->elem_size) + ((column) * (matrix)->elem_size) + ((OFFSET) * (matrix)->elem_size)]

int matrix_update(Matrix* restrict m, size_t row, size_t column, void* restrict new_value)
{
  if (m == NULL || new_value == NULL)
    return -1;

  if (row >= m->rows || column >= m->columns)
    return 1;

  uint8_t* const ptr = MATRIX_ELEM(m, row, column); // matrix.c:61

  memcpy(ptr, new_value, m->elem_size);

  return 0;
}

int matrix_at(Matrix* restrict m, size_t row, size_t column, void* restrict elem)
{
  if (m == NULL || elem == NULL)
    return -1;

  if (row >= m->rows || column >= m->columns)
    return 1;

  const uint8_t* const ptr = MATRIX_ELEM(m, row, column); // matrix.c:61

  memcpy(elem, ptr, m->elem_size);

  return 0;
}

int matrix_update_symmetric(Matrix* restrict m, size_t row, size_t column, void* restrict new_value)
{
  if (m == NULL || new_value == NULL)
    return -1;

  if (row >= m->rows || column >= m->columns)
    return 1;

  int result = matrix_update(m, row, column, new_value);

  if (result != 0)
    return result;

  return matrix_update(m, column, row, new_value);
}

int matrix_add_rows(Matrix** const m, const size_t new_rows)
{
  if (m == NULL || *m == NULL)
    return -1;
  
  if (new_rows == 0)
    return 1;

  register const size_t previous_rows = (*m)->rows;
  register const size_t columns = (*m)->columns;
  register const size_t elem_size = (*m)->elem_size;

  register const size_t new_matrix_capacity = (previous_rows + new_rows) * columns * elem_size;
  Matrix* const temp = realloc(*m, sizeof(**m) + new_matrix_capacity);

  if (temp == NULL)
    return 2;
  
  temp->rows = previous_rows + new_rows;
  temp->columns = columns;
  temp->elem_size = elem_size;

  // Zero out the new rows
  register const size_t new_bytes = new_rows * columns * elem_size;
  uint8_t* const ptr = MATRIX_ELEM(temp, previous_rows, 0);
  memset(ptr, 0, new_bytes);

  *m = temp;

  return 0;
}

/* Shift memory and zero out new columns */
/**/
/* Visualisation: */
/**/
/* matrix_add_columns(3x3 matrix, 2) */
/**/
/* State 1: */
/* |(0,0) |(1,0) |(2,0) */
/* |1 2 3 |4 5 6 |7 8 9 [ ] [ ] [ ] [ ] [ ] [ ] */
/* move ptr at 7 (row 2, col 0) right by 4 elems: */
/**/
/* State 2: */
/* |(0,0) |(1,0) <----4----> */
/* |1 2 3 |4 5 6 7 8 9 [ ] 7 8 9 [0] [0], */
/* memset two elems after 9 to 0, move ptr at 4 (row 1, col 0) right by 2 elems: */
/**/
/* State 3: */
/* (0,0) <-2-> */
/* 1 2 3 4 5 4 5 6 [0] [0] 7 8 9 [0] [0], memset two elems after 3 to 0 */
/* memset two elems after 6 to 0, don't move the last pointer it is in the right place (one less memmove): */
/**/
/* State 4, finish: */
/* 1 2 3 [0] [0] 4 5 6 [0] [0] 7 8 9 [0] [0] */
int matrix_add_columns(Matrix** m, size_t new_columns)
{
  if (m == NULL || *m == NULL)
    return -1;
  
  if (new_columns == 0)
    return 1;

  /* Save input matrix values */
  register const size_t rows = (*m)->rows;
  register const size_t previous_columns = (*m)->columns;
  register const size_t elem_size = (*m)->elem_size;

  /* Realloc new space */
  register const size_t new_matrix_capacity = rows * (previous_columns + new_columns) * elem_size;
  Matrix* const temp = realloc(*m, sizeof(**m) + new_matrix_capacity);

  if (temp == NULL)
    return 2;
 
  /* Calculate no. bytes for memmoves/memsets */
  register const size_t moved_bytes = previous_columns * elem_size;
  register const size_t new_bytes = new_columns * elem_size;
  
  size_t move_offset = new_columns * (rows - 1);

  /* Move/set loop, see above visualisation example for reference */
  for (size_t i = 1; i < rows; i++) 
  {
    uint8_t* src = MATRIX_ELEM(temp, rows - i, 0);
    uint8_t* dest = MATRIX_ELEM_OFFSET(temp, rows - i, 0, move_offset);
    memmove(dest, src, moved_bytes);
    memset(MATRIX_ELEM_OFFSET(temp, rows - i, previous_columns - 1, move_offset + 1), 0, new_bytes);
    move_offset -= new_columns;
  }

  /* Set last empty space */
  uint8_t* last_ptr = MATRIX_ELEM_OFFSET(temp, 0, 0, previous_columns);
  memset(last_ptr, 0, new_columns * elem_size);

  /* Update matrix values */
  temp->rows = rows;
  temp->columns = previous_columns + new_columns;
  temp->elem_size = elem_size;

  *m = temp;

  return 0;
}

int matrix_expand(Matrix** m, size_t new_fields)
{
  if (m == NULL || *m == NULL)
    return -1;

  if (new_fields == 0)
    return 1;

  int result = matrix_add_rows(m, new_fields);

  if (result != 0)
    return result;

  return matrix_add_columns(m, new_fields);
}

Matrix* matrix_copy(const Matrix* const m)
{
  if (m == NULL)
    return NULL;

  register const size_t copy_size = sizeof(*m) + m->rows * m->columns * m->elem_size;

  Matrix* const m_copy = calloc(1, copy_size);

  if (m_copy == NULL)
    return NULL;

  memcpy(m_copy, m, copy_size);

  return m_copy; 
}