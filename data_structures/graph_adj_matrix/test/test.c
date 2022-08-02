#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

extern void vector_main_test(void);
extern void matrix_main_test(void);
extern void graph_main_test(void);

int main(void)
{
  TIMER_MULTIPLE_TESTS(vector_main_test());
  TIMER_MULTIPLE_TESTS(matrix_main_test());
  TIMER_MULTIPLE_TESTS(graph_main_test());
  setbuf(stdout, NULL);
  printf("All tests passed!\n");
  return 0;
}
