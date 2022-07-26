#include <vector.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void vector_create_test(void);
static void vector_resize_test(void);
static void vector_push_back_failure_test(void);
static void vector_push_back_1_test(void);
static void vector_push_back_to_more_than_half_full_test(void);
static void vector_push_back_to_full_test(void);
static void vector_pop_back_failure_test(void);
static void vector_pop_back_1_test(void);
static void vector_pop_back_to_empty_test(void);
static void vector_find_test(void);
static void vector_at_test(void);
static void vector_update_test(void);
static void vector_delete_test(void);
static void vector_insert_failure_test(void);
static void vector_insert_1_test(void);
static void vector_insert_at_begin_test(void);
static void vector_insert_at_end_test(void);
static void vector_insert_after_end_test(void);
static void vector_insert_to_more_than_half_full_test(void);

static void vector_create_test(void)
{
  {
    // Invalid argument/s - invalid elem_size
    Vector* const v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);

    vector_destroy(v);
  }
  {
    // Valid argument/s
    Vector* const v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
}

static void vector_resize_test(void)
{
  {
    // Invalid argument/s - inavlid vector
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);

    int result = vector_resize(&v, 2);
    assert(result == -1);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - inavlid new_size
    Vector* v = vector_create(sizeof(int), 4);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_resize(&v, 0);
    assert(result == -1);

    result = vector_resize(&v, 5);
    assert(result == -1);

    vector_destroy(v);
  }
  { 
    // Valid argument/s - shrink
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    register const size_t new_size = 2;
    int result = vector_resize(&v, new_size);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(result == 0);
    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == new_size);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
  {
    // Valid argument/s - enlarge
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    register const size_t new_size = 10;
    int result = vector_resize(&v, new_size);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(result == 0);
    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == new_size);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
}

static void vector_push_back_failure_test(void)
{
  {
    // Invalid argument/s - invalid vector
    register const int pushed_val = 42;
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_push_back(&v, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_push_back(&v, &(int){pushed_val});
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_push_back(&v, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
}

static void vector_push_back_1_test(void)
{
  {
    register const int pushed_val = 42;
    // Valid argument/s - push 1
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    int result = vector_push_back(&v, &(int){pushed_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 1);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
}

static void vector_push_back_to_more_than_half_full_test(void)
{
  {
    // Valid argument/s - push more than half of VECTOR_INITIAL_CAPACITY, force to resize
    register const int pushed_val_1 = 37;
    register const int pushed_val_2 = 21;
    register const int pushed_val_3 = 42;

    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    int result = vector_push_back(&v, &(int){pushed_val_1});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 1);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    result = vector_push_back(&v, &(int){pushed_val_2});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 2);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    result = vector_push_back(&v, &(int){pushed_val_3});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == 2 * VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 3);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
}

static void vector_push_back_to_full_test()
{
  {
    // Valid argument/s - push until full, check if push after full is dropped, pop to empty, check if pop after empty is dropped
    register const size_t max_size = 3;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    // Should fail to push once more
    int result = vector_push_back(&v, &(int){max_size + 1});
    assert(result == 1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == true);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) != 2 * max_size);
    assert(vector_get_capacity(v) == max_size);
    assert(vector_get_curr_size(v) == max_size);
    assert(vector_get_max_size(v) == max_size);

    vector_destroy(v);
  }
}

static void vector_pop_back_failure_test(void)
{
  {
    // Invalid argument/s - invalid vector
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_pop_back(v, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_pop_back(v, &(int){17});
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_pop_back(v, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Popback on empty
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_pop_back(v, &(int){17});
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
    
    vector_destroy(v);
  }
}

static void vector_pop_back_1_test(void)
{
  {
    register const int pushed_val = 42;
    // Valid argument/s - push 1 and pop 1
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    int result = vector_push_back(&v, &(int){pushed_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 1);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    result = vector_pop_back(v, &(int){pushed_val});
    assert(result == 0);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
}

static void vector_pop_back_to_empty_test(void)
{
  {
    // Valid argument/s - push until full, check if push after full is dropped, pop to empty, check if pop after empty is dropped
    register const size_t max_size = 3;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    // Should fail to push once more
    int result = vector_push_back(&v, &(int){max_size + 1});
    assert(result == 1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == true);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) != 2 * max_size);
    assert(vector_get_capacity(v) == max_size);
    assert(vector_get_curr_size(v) == max_size);
    assert(vector_get_max_size(v) == max_size);

    int* temp = malloc(sizeof(*temp));
    for (size_t i = max_size; i >= 1; i--)
    {
      int result = vector_pop_back(v, temp);
      assert(result == 0);
      assert(*temp == (int) i);
      assert(vector_is_empty(v) == (i == 1));
      assert(vector_is_full(v) == false);

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i - 1);
      assert(vector_get_max_size(v) == max_size);
    }

    // Should fail to pop once more
    result = vector_pop_back(v, temp);
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) != 2 * max_size);
    assert(vector_get_capacity(v) == max_size);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == max_size);

    vector_destroy(v);
    free(temp);
  }
}

static void vector_find_test(void)
{
  {
    // Invalid argument/s - invalid vector
    register const int find_val = 42;
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    size_t result = vector_find(v, NULL);
    assert(result == SIZE_MAX);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_find(v, &(int){find_val});
    assert(result == SIZE_MAX);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    size_t result = vector_find(v, NULL);
    assert(result == SIZE_MAX);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Find on empty
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    size_t result = vector_find(v, &(int){17});
    assert(result == SIZE_MAX);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Valid argument/s - push some values and find all
    register const size_t max_size = 4;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    for (size_t i = 1; i <= max_size; i++)
    {
      size_t result = vector_find(v, &(int){i << 1});
      assert(result == i - 1);
    }

    size_t result = vector_find(v, &(int){21});
    assert(result == SIZE_MAX);

    vector_destroy(v);
  }
}

static void vector_at_test(void)
{
  {
    // Invalid argument/s - invalid vector
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_at(v, SIZE_MAX, &(int){42});
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_at(v, SIZE_MAX, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid index, out of bounds, at on empty
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_at(v, SIZE_MAX, &(int){42});
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    result = vector_at(v, 0, &(int){42});
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_push_back(&v, &(int){42});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_at(v, 0, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Valid argument/s - push some values and at them
    register const size_t max_size = 4;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    int* at_value = malloc(sizeof(*at_value));
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_at(v, i - 1, at_value);
      assert(*at_value == (int)(i << 1));
      assert(result == 0);
    }

    vector_destroy(v);
    free(at_value);
  }
}

static void vector_update_test(void)
{
  {
    // Invalid argument/s - invalid vector
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_update(v, SIZE_MAX, &(int){42});
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_update(v, SIZE_MAX, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid index, out of bounds, update on empty
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_update(v, SIZE_MAX, &(int){42});
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    result = vector_update(v, 0, &(int){42});
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_push_back(&v, &(int){42});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_update(v, 0, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Valid argument/s - push some values and update them
    register const size_t max_size = 4;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    int* at_value = malloc(sizeof(*at_value));
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_update(v, i - 1, &(int){3 * i});
      assert(result == 0);
      result = vector_at(v, i - 1, at_value);
      assert(*at_value == (int)(3 * i));
    }

    vector_destroy(v);
    free(at_value);
  }
}

static void vector_delete_test(void)
{
  {
    // Invalid argument/s - invalid vector
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_delete(v, SIZE_MAX);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    result = vector_delete(v, SIZE_MAX);
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid index, out of bounds, delete on empty
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_delete(v, SIZE_MAX);
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    result = vector_delete(v, 0);
    assert(result == 1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Valid argument/s - push 1, delete 1
    register const int pushed_val = 42;

    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    int result = vector_push_back(&v, &(int){pushed_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 1);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    result = vector_delete(v, 0);
    assert(result == 0);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == 0);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);
  }
  {
    // Valid argument/s - push some values and delete them in some order
    register const size_t max_size = 4;

    Vector* v = vector_create(sizeof(int), max_size);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);
  
    for (size_t i = 1; i <= max_size; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == (i == max_size));

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == i);
      assert(vector_get_max_size(v) == max_size);
    }

    int result = vector_delete(v, max_size - 1);
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    size_t find_result = vector_find(v, &(int){2 * (max_size)});
    assert(find_result == SIZE_MAX);

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) != 2 * max_size);
    assert(vector_get_capacity(v) == max_size);
    assert(vector_get_curr_size(v) == max_size - 1);
    assert(vector_get_max_size(v) == max_size);

    for (size_t i = 0; i < max_size - 1; i++)
    {
      result = vector_delete(v, 0);
      assert(result == 0);
      assert(vector_is_empty(v) == (i + 2 == max_size));
      assert(vector_is_full(v) == false);

      int to_find = (i + 1) * 2;
      find_result = vector_find(v, &(int){to_find});
      assert(find_result == SIZE_MAX);
      (void) find_result;

      assert(vector_get_elem_size(v) == sizeof(int));
      assert(vector_get_capacity(v) != 2 * max_size);
      assert(vector_get_capacity(v) == max_size);
      assert(vector_get_curr_size(v) == max_size - i - 2);
      assert(vector_get_max_size(v) == max_size);
    }

    vector_destroy(v);
  }
}

static void vector_insert_failure_test(void)
{
  {
    // Invalid argument/s - invalid vector
    register const int inserted_val = 42;
    register const size_t index = 0;
    Vector* v = vector_create(0, VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v == NULL);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == -1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid elem
    register const size_t index = 0;    
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    int result = vector_insert(&v, index, NULL);
    assert(result == -1);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    vector_destroy(v);
  }
  {
    // Invalid argument/s - invalid index
    register const int inserted_val = 42;  
    register const size_t index = 1;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < index; i++)
    {
      int result = vector_push_back(&v, &(int){i});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    int result = vector_insert(&v, index + 1, &(int){inserted_val});
    assert(result == 1);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    vector_destroy(v);    
  }
}

static void vector_insert_1_test(void)
{
  {
    // Valid argument/s
    register const int inserted_val = 42;
    register const size_t index = 0;
    register const size_t pushed_elems = 1;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      int result = vector_push_back(&v, &(int){inserted_val / 2});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int* check_elem = malloc(sizeof(*check_elem));
    result = vector_at(v, index, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val);

    result = vector_at(v, index + 1, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val / 2);

    vector_destroy(v);    
    free(check_elem);
  }
}

static void vector_insert_at_begin_test(void)
{
  {
    // Valid argument/s
    register const int inserted_val = 42;
    register const size_t index = 0;
    register const size_t pushed_elems = 5;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int* check_elem = malloc(sizeof(*check_elem));
    result = vector_at(v, index, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val);

    for (size_t i = 1; i <= pushed_elems; i++)
    {
      result = vector_at(v, i, check_elem);
      assert(result == 0);
      assert(*check_elem == (int)((i - 1) << 1));
    }

    assert(vector_get_curr_size(v) == pushed_elems + 1);

    vector_destroy(v);    
    free(check_elem);
  }
}

static void vector_insert_at_end_test(void)
{
  {
    // Valid argument/s
    register const int inserted_val = 42;
    register const size_t index = 4;
    register const size_t pushed_elems = 5;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int* check_elem = malloc(sizeof(*check_elem));
    result = vector_at(v, index, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val);

    for (size_t i = 0; i < index; i++)
    {
      result = vector_at(v, i, check_elem);
      assert(result == 0);
      assert(*check_elem == (int)(i << 1));
    }

    result = vector_at(v, index + 1, check_elem);
    assert(result == 0);
    assert(*check_elem == (pushed_elems - 1) << 1);

    assert(vector_get_curr_size(v) == pushed_elems + 1);

    vector_destroy(v);    
    free(check_elem);
  }
}

static void vector_insert_after_end_test(void)
{
  {
    // Valid argument/s
    register const int inserted_val = 42;
    register const size_t index = 5;
    register const size_t pushed_elems = 5;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int* check_elem = malloc(sizeof(*check_elem));
    result = vector_at(v, index, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      result = vector_at(v, i, check_elem);
      assert(result == 0);
      assert(*check_elem == (int)(i << 1));
    }

    assert(vector_get_curr_size(v) == pushed_elems + 1);

    vector_destroy(v);    
    free(check_elem);
  }
}

static void vector_insert_to_more_than_half_full_test(void)
{
  {
    // Valid argument/s
    register const int inserted_val = 42;
    register const size_t index = 0;
    register const size_t pushed_elems = 2;
    Vector* v = vector_create(sizeof(int), VECTOR_DEFAULT_CAPACITY_OPTION);
    assert(v != NULL);
    assert(vector_is_empty(v) == true);
    assert(vector_is_full(v) == false);

    for (size_t i = 0; i < pushed_elems; i++)
    {
      int result = vector_push_back(&v, &(int){i << 1});
      assert(result == 0);
      assert(vector_is_empty(v) == false);
      assert(vector_is_full(v) == false);

      assert(vector_get_curr_size(v) == i + 1);
    }

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY);
    assert(vector_get_curr_size(v) == pushed_elems);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    int result = vector_insert(&v, index, &(int){inserted_val});
    assert(result == 0);
    assert(vector_is_empty(v) == false);
    assert(vector_is_full(v) == false);

    int* check_elem = malloc(sizeof(*check_elem));
    result = vector_at(v, index, check_elem);
    assert(result == 0);
    assert(*check_elem == inserted_val);

    for (size_t i = 1; i <= pushed_elems; i++)
    {
      result = vector_at(v, i, check_elem);
      assert(result == 0);
      assert(*check_elem == (int)((i - 1) << 1));
    }

    assert(vector_get_elem_size(v) == sizeof(int));
    assert(vector_get_capacity(v) == VECTOR_INITIAL_CAPACITY << 1);
    assert(vector_get_curr_size(v) == pushed_elems + 1);
    assert(vector_get_max_size(v) == VECTOR_MAXIMUM_CAPACITY);

    vector_destroy(v);    
    free(check_elem);
  }
}

void vector_main_test(void)
{
  // Create
  vector_create_test();
  // Resize
  vector_resize_test();
  // Push back
  vector_push_back_failure_test();
  vector_push_back_1_test();
  vector_push_back_to_more_than_half_full_test();
  vector_push_back_to_full_test();
  // Pop back
  vector_pop_back_failure_test();
  vector_pop_back_1_test();
  vector_pop_back_to_empty_test();
  // Find
  vector_find_test();
  // At
  vector_at_test();
  // Update
  vector_update_test();
  // Delete
  vector_delete_test();
  // Insert
  vector_insert_failure_test();
  vector_insert_1_test();
  vector_insert_at_begin_test();
  vector_insert_at_end_test();
  vector_insert_after_end_test();
  vector_insert_to_more_than_half_full_test();
}