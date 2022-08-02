#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

extern void matrix_main_test(void);

int main(void)
{
  TIMER_MULTIPLE_TESTS(matrix_main_test());
  setbuf(stdout, NULL);
  printf("All tests passed!\n");
  return 0;
}
