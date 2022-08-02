#include <matrix.h>
#include <assert.h>
#include <stdlib.h>

static void matrix_create_test(void);
static void matrix_update_test(void);
static void matrix_at_test(void);
static void matrix_update_symmetric_test(void);
static void matrix_add_rows_1_test(void);
static void matrix_add_rows_multi_test(void);
static void matrix_add_columns_1_test(void);
static void matrix_add_columns_multi_test(void);
static void matrix_add_combined_test(void);
static void matrix_expand_test(void);
static void matrix_copy_test(void);

static void matrix_create_test(void)
{
  {
    // Invalid argument/s - invalid rows
    Matrix* const m = matrix_create(0, 1, 1);
    assert(m == NULL);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid columns
    Matrix* const m = matrix_create(1, 0, 1);
    assert(m == NULL);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid elem_size
    Matrix* const m = matrix_create(1, 1, 0);
    assert(m == NULL);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const size_t rows = 5;
    register const size_t columns = 4;
    Matrix* const m = matrix_create(rows, columns, sizeof(int));
    assert(m != NULL);

    assert(matrix_get_rows(m) == rows);
    assert(matrix_get_columns(m) == columns);
    assert(matrix_get_elem_size(m) == sizeof(int));

    matrix_destroy(m);
  }
}

static void matrix_update_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_update(m, 0, 0, &(int){42});
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid new_value
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 0, NULL);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - out of bounds
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 1, 1, &(int){42});
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 0, &(int){42});
    assert(result == 0);

    matrix_destroy(m);
  }
}

static void matrix_at_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_at(m, 0, 0, &(int){42});
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid elem
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_at(m, 0, 0, NULL);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - out of bounds
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_at(m, 1, 1, &(int){42});
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const int expected_value = 42;
    Matrix* m = matrix_create(2, 2, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 2);
    assert(matrix_get_columns(m) == 2);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 1, &(int){expected_value});
    assert(result == 0);

    int* elem = malloc(sizeof(*elem));
    result = matrix_at(m, 0, 1, elem);
    assert(result == 0);
    assert(*elem == expected_value);

    result = matrix_at(m, 1, 0, elem);
    assert(result == 0);
    assert(*elem != expected_value);

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_update_symmetric_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_update_symmetric(m, 0, 0, &(int){42});
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid new_value
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update_symmetric(m, 0, 0, NULL);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - out of bounds
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update_symmetric(m, 1, 1, &(int){42});
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const int expected_value = 42;
    Matrix* m = matrix_create(2, 2, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 2);
    assert(matrix_get_columns(m) == 2);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update_symmetric(m, 0, 1, &(int){expected_value});
    assert(result == 0);

    int* elem = malloc(sizeof(*elem));
    result = matrix_at(m, 0, 1, elem);
    assert(result == 0);
    assert(*elem == expected_value);

    result = matrix_at(m, 1, 0, elem);
    assert(result == 0);
    assert(*elem == expected_value);

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_add_rows_1_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_add_rows(&m, 1);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid new_rows
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_add_rows(&m, 0);
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const int expected_value = 42;
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 0, &(int){expected_value});
    assert(result == 0);

    result = matrix_add_rows(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == 2);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int* elem = malloc(sizeof(*elem));
    result = matrix_at(m, 0, 0, elem);
    assert(result == 0);
    assert(*elem == expected_value);

    result = matrix_at(m, 1, 0, elem);
    assert(result == 0);
    assert(*elem == 0);

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_add_rows_multi_test(void)
{
  {
    // Valid argument/s
    register const size_t updates = 2;
    register const size_t new_rows = 3;
    register const int expected_value = 42;
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Add some new rows

    int result = matrix_add_rows(&m, new_rows);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == new_rows + 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Update some of them

    for (size_t i = 0; i < updates; i++)
    {
      result = matrix_update(m, i, 0, &(int){expected_value});
      assert(result == 0);
    }

    // Check values

    int* elem = malloc(sizeof(*elem));
    for (size_t i = 0; i < matrix_get_rows(m); i++)
    {
      result = matrix_at(m, i, 0, elem);
      assert(result == 0);
      assert(*elem == ((i < updates) ? expected_value : 0));
    }

    // Add once more

    result = matrix_add_rows(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == new_rows + 2);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));
    
    // Check once again

    for (size_t i = 0; i < matrix_get_rows(m); i++)
    {
      result = matrix_at(m, i, 0, elem);
      assert(result == 0);
      assert(*elem == ((i < updates) ? expected_value : 0));
    }

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_add_columns_1_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_add_columns(&m, 1);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid new_columns
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_add_columns(&m, 0);
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const int expected_value = 42;
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 0, &(int){expected_value});
    assert(result == 0);

    result = matrix_add_columns(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 2);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int* elem = malloc(sizeof(*elem));
    result = matrix_at(m, 0, 0, elem);
    assert(result == 0);
    assert(*elem == expected_value);

    result = matrix_at(m, 0, 1, elem);
    assert(result == 0);
    assert(*elem == 0);

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_add_columns_multi_test(void)
{
  {
    // Valid argument/s
    register const size_t updates = 2;
    register const size_t new_columns = 3;
    register const int expected_value = 42;
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Add some new columns

    int result = matrix_add_columns(&m, new_columns);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == new_columns + 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Update some of them

    for (size_t i = 0; i < updates; i++)
    {
      result = matrix_update(m, 0, i, &(int){expected_value});
      assert(result == 0);
    }

    // Check values

    int* elem = malloc(sizeof(*elem));
    for (size_t i = 0; i < matrix_get_columns(m); i++)
    {
      result = matrix_at(m, 0, i, elem);
      assert(result == 0);
      assert(*elem == ((i < updates) ? expected_value : 0));
    }

    // Add once more

    result = matrix_add_columns(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == new_columns + 2);
    assert(matrix_get_elem_size(m) == sizeof(int));
    
    // Check once again

    for (size_t i = 0; i < matrix_get_columns(m); i++)
    {
      result = matrix_at(m, 0, i, elem);
      assert(result == 0);
      assert(*elem == ((i < updates) ? expected_value : 0));
    }

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_add_combined_test(void)
{
  {
    // Valid argument/s
    register const size_t start_rows = 2;
    register const size_t start_columns = 2;
    register const size_t new_rows = 2;
    register const size_t new_columns = 3;
    register const int expected_value = 42;
    Matrix* m = matrix_create(start_rows, start_columns, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == start_rows);
    assert(matrix_get_columns(m) == start_columns);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Add some new rows

    int result = matrix_add_rows(&m, new_rows);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == start_rows + new_rows);
    assert(matrix_get_columns(m) == start_columns);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Add some new columns

    result = matrix_add_columns(&m, new_columns);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == start_rows + new_rows);
    assert(matrix_get_columns(m) == start_columns + new_columns);
    assert(matrix_get_elem_size(m) == sizeof(int));

    // Update only even indexes

    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        if (i % 2 == 1 || j % 2 == 1) continue;
        result = matrix_update(m, i, j, &(int){expected_value});
        assert(result == 0);
      }

    // Check values

    int* elem = malloc(sizeof(*elem));
    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        result = matrix_at(m, i, j, elem);
        assert(result == 0);
        assert(*elem == ((i % 2 == 1 || j % 2 == 1) ? 0 : expected_value));
      }

    // Add once more

    result = matrix_add_rows(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == start_rows + new_rows + 1);
    assert(matrix_get_columns(m) == start_columns + new_columns);
    assert(matrix_get_elem_size(m) == sizeof(int));

    result = matrix_add_columns(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == start_rows + new_rows + 1);
    assert(matrix_get_columns(m) == start_columns + new_columns + 1);
    assert(matrix_get_elem_size(m) == sizeof(int));
    
    // Check once again

    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        result = matrix_at(m, i, j, elem);
        assert(result == 0);
        if (i == start_rows + new_rows || j == start_columns + new_columns)
          assert(*elem == 0);
        else
          assert(*elem == ((i % 2 == 1 || j % 2 == 1) ? 0 : expected_value));
      }


    matrix_destroy(m);
    free(elem);
  }    
}

static void matrix_expand_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    int result = matrix_expand(&m, 1);
    assert(result == -1);

    matrix_destroy(m);
  }
  {
    // Invalid argument/s - invalid new_fields
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_expand(&m, 0);
    assert(result == 1);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const int expected_value = 42;
    Matrix* m = matrix_create(1, 1, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == 1);
    assert(matrix_get_columns(m) == 1);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int result = matrix_update(m, 0, 0, &(int){expected_value});
    assert(result == 0);

    result = matrix_expand(&m, 1);
    assert(result == 0);
    assert(m != NULL);
    assert(matrix_get_rows(m) == 2);
    assert(matrix_get_columns(m) == 2);
    assert(matrix_get_elem_size(m) == sizeof(int));

    int* elem = malloc(sizeof(*elem));
    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        result = matrix_at(m, i, j, elem);  
        assert(result == 0);
        assert(*elem == ((i == 0 && j == 0) ? expected_value : 0));
      }

    matrix_destroy(m);
    free(elem);
  }
}

static void matrix_copy_test(void)
{
  {
    // Invalid argument/s - invalid matrix
    Matrix* m = matrix_create(0, 0, sizeof(int));
    assert(m == NULL);

    Matrix* m_copy = matrix_copy(m);
    assert(m_copy == NULL);

    matrix_destroy(m);
  }
  {
    // Valid argument/s
    register const size_t rows = 3;
    register const size_t columns = 3;

    Matrix* m = matrix_create(rows, columns, sizeof(int));
    assert(m != NULL);
    assert(matrix_get_rows(m) == rows);
    assert(matrix_get_columns(m) == columns);
    assert(matrix_get_elem_size(m) == sizeof(int));
  
    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        int result = matrix_update(m, i, j, &(int){i * 10 + j});
        assert(result == 0);
      }

    Matrix* m_copy = matrix_copy(m);
    assert(m_copy != NULL);

    assert(matrix_get_rows(m) == matrix_get_rows(m_copy));
    assert(matrix_get_columns(m) == matrix_get_columns(m_copy));
    assert(matrix_get_elem_size(m) == matrix_get_elem_size(m_copy));

    int* m_at = malloc(sizeof(*m_at));
    int* m_copy_at = malloc(sizeof(*m_copy_at));
    for (size_t i = 0; i < matrix_get_rows(m); i++)
      for (size_t j = 0; j < matrix_get_columns(m); j++)
      {
        int m_res = matrix_at(m, i, j, m_at);
        assert(m_res == 0);
        int m_copy_res = matrix_at(m_copy, i, j, m_copy_at);
        assert(m_copy_res == 0);
        assert(*m_at == *m_copy_at);
      }
    
    free(m_at);
    free(m_copy_at);
    matrix_destroy(m);
    matrix_destroy(m_copy);
  }
}

void matrix_main_test(void)
{
  // Create
  matrix_create_test();
  // Update
  matrix_update_test();
  // At
  matrix_at_test();
  // Update symmetric macro
  matrix_update_symmetric_test();
  // Add rows
  matrix_add_rows_1_test();
  matrix_add_rows_multi_test();
  // Add columns
  matrix_add_columns_1_test();
  matrix_add_columns_multi_test();
  // Add rows/columns combined
  matrix_add_combined_test();
  // Expand
  matrix_expand_test();
  // Copy
  matrix_copy_test();
}