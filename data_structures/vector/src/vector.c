#include <vector.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Vector
{
  size_t elem_size;
  size_t curr_size;
  size_t capacity;
  size_t max_size;
  uint8_t elems[];
};

Vector* vector_create(const size_t elem_size, const size_t max_size)
{
  if (elem_size == 0)
    return NULL;

  register const size_t max_capacity = max_size == VECTOR_DEFAULT_CAPACITY_OPTION ? VECTOR_MAXIMUM_CAPACITY : max_size;
  register const size_t initial_capacity = VECTOR_INITIAL_CAPACITY <= max_capacity ? VECTOR_INITIAL_CAPACITY : max_capacity;

  Vector* const v = calloc(1, sizeof(*v) + elem_size * initial_capacity);

  if (v == NULL)
    return NULL;

  v->elem_size = elem_size;
  v->max_size = max_capacity;
  v->capacity = initial_capacity;

  return v;
}

void vector_destroy(Vector* const v)
{
  if (v == NULL)
    return;

  free(v);
}

size_t vector_get_elem_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->elem_size;
}

size_t vector_get_curr_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->curr_size;
}

size_t vector_get_capacity(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->capacity;
}

size_t vector_get_max_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->max_size;
}

bool vector_is_empty(const Vector* const v)
{
  if (v == NULL)
    return false;

  return v->curr_size == 0;
}

bool vector_is_full(const Vector* const v)
{
  if (v == NULL)
    return false;

  return v->curr_size == v->capacity;
}

int vector_resize(Vector** const v, size_t new_size)
{
  if (v == NULL || *v == NULL)
    return -1;

  if (new_size > (*v)->max_size || new_size == 0)
    return -1;

  register const size_t prev_size = (*v)->curr_size;
  register const size_t elem_size = (*v)->elem_size;

  Vector* const temp = realloc(*v, sizeof(**v) + new_size * elem_size);

  if (temp == NULL)
    return 1;

  temp->capacity = new_size;
  
  if (prev_size > new_size)
    temp->curr_size = prev_size;

  temp->elem_size = elem_size;

  *v = temp;

  return 0;
}

// Double the size if more than half full
#define RESIZE_CHECK(vector) \
  do { \
    register const size_t capacity = (vector)->capacity; \
    register const size_t double_size = capacity << 1; \
    register const size_t half_size = capacity>> 1; \
    if ((vector)->curr_size > half_size) \
    (void) vector_resize(&(vector), double_size); \
  } while(0) 

int vector_push_back(Vector** const restrict v, const void* const restrict new_elem)
{
  if (v == NULL || *v == NULL)
    return -1;

  if (new_elem == NULL)
    return -1;
  
  if (vector_is_full(*v))
    return 1;

  register const size_t elem_size = (*v)->elem_size;

  uint8_t* const ptr = &(*v)->elems[(*v)->curr_size * elem_size];
  memcpy(ptr, new_elem, elem_size);

  (*v)->curr_size++;

  RESIZE_CHECK(*v);

  return 0;
}

int vector_pop_back(Vector* const restrict v, void* const restrict popped_elem)
{
  if (v == NULL)
    return -1;

  if (popped_elem == NULL)
    return -1;
  
  if (vector_is_empty(v))
    return 1;

  register const size_t elem_size = v->elem_size;
  const uint8_t* const ptr = &v->elems[(v->curr_size - 1) * elem_size];

  memcpy(popped_elem, ptr, elem_size);

  v->curr_size--;

  return 0;
}

size_t vector_find(const Vector* const restrict v, const void* const restrict found_elem)
{
  if (v == NULL || found_elem == NULL)
    return ELEM_NOT_FOUND;

  register const size_t curr_size = v->curr_size;

  for (size_t i = 0; i < curr_size; i++)
  {
    if (memcmp(&v->elems[i * v->elem_size], found_elem, v->elem_size) == 0)
      return i;
  }

  return ELEM_NOT_FOUND;
}

int vector_at(Vector* const restrict v, const size_t index, void* const restrict at_elem)
{
  if (v == NULL)
    return -1;

  if (at_elem == NULL)
    return -1;

  if (index >= v->curr_size)
    return 1;

  const uint8_t* const ptr = &v->elems[index * v->elem_size];
  memcpy(at_elem, ptr, v->elem_size);

  return 0;
}

int vector_update(Vector* const restrict v, const size_t index, const void* const restrict new_value)
{
  if (v == NULL)
    return -1;

  if (new_value == NULL)
    return -1;

  if (index >= v->curr_size)
    return 1;

  uint8_t* const ptr = &v->elems[index * v->elem_size];
  memcpy(ptr, new_value, v->elem_size);

  return 0;
}

int vector_delete(Vector* const v, const size_t index)
{
  if (v == NULL)
    return -1;

  if (vector_is_empty(v))
    return 1;

  if (index >= v->curr_size)
    return 1;

  if (index == v->curr_size - 1)
  {
    v->curr_size--;
    return 0;
  }

  register const size_t elem_size = v->elem_size;

  uint8_t* to_del = &v->elems[index * elem_size];
  uint8_t* to_move = &v->elems[(index + 1) * elem_size];
  register const size_t no_elems_to_move = v->curr_size - (index + 1); // 0 1 2 4, delete 1: 4 - (1 + 1) = 2
  memmove(to_del, to_move, no_elems_to_move * elem_size);

  v->curr_size--;

  return 0;
}

int vector_insert(Vector** v, size_t index, const void* restrict new_elem)
{
  if (v == NULL || *v == NULL || new_elem == NULL)
    return -1;

  if (index > (*v)->curr_size)
    return 1;

  if (vector_is_empty(*v) && index > 0)
    return 1;

  register const size_t elem_size = (*v)->elem_size;


  if (index != (*v)->curr_size) {
    // Make one empty slot by moving the allocated memory
    uint8_t* src = &(*v)->elems[(index) * elem_size];
    uint8_t* dest = &(*v)->elems[(index + 1) * elem_size];
    // Ex: 0 1 2 4, curr_size = 4, index = 1, insert 5 at 1, move 1 2 4, final: 0 5 1 2 4, we had to move 3 elems, so 4 - 1 = 3
    register const size_t no_elems_to_move = (*v)->curr_size - (index); 
    memmove(dest, src, no_elems_to_move * elem_size);
  }

  // Memcpy new element at new empty slot
  uint8_t* const ptr = &(*v)->elems[index * (*v)->elem_size];
  memcpy(ptr, new_elem, (*v)->elem_size);

  (*v)->curr_size++;

  // Check if we need to resize
  RESIZE_CHECK(*v);

  return 0;
}

Vector* vector_copy(const Vector* const v)
{
  if (v == NULL)
    return NULL;

  register const size_t copy_size = sizeof(*v) + v->elem_size * v->capacity;

  Vector* const v_copy = calloc(1, copy_size);

  if (v_copy == NULL)
    return NULL;
  
  memcpy(v_copy, v, copy_size);

  return v_copy;
}
