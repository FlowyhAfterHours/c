#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TESTS_STATS
  #define TESTS_STATS
  static size_t total_cases = 0;
  static size_t run_cases = 0;
  static double total_time = .0f;
#endif // !TESTS_STATS

#define DO_NOTHING_XD() \
  do { \
    (void) total_cases; \
    (void) total_time; \
    (void) run_cases; \
  } while(0)

#define RUNNING_TEST_TYPE_MSG(func) \
  "Running " #func " tests"

#define RUNNING_CASES_MSG(func, cases) \
  "---> Running " #func " with " #cases " test cases . . .\n"

#define PRETTY_BOX_CORNER "#"
#define PRETTY_BOX_HORIZONTAL_LINE "-"
#define PRETTY_BOX_VERTICAL_LINE "|"

#define PRETTY_PRINT_HORIZONTAL_LINE(len) \
  do { \
  for (size_t i = 0; i < len; i++) \
    printf(PRETTY_BOX_HORIZONTAL_LINE); \
  } while(0)

#define PRETTY_PRINT_VERTICAL_LINE_STR(str) \
  do {\
    printf(PRETTY_BOX_VERTICAL_LINE); \
    printf(" "); \
    printf("%s", str); \
    printf(" "); \
    printf(PRETTY_BOX_VERTICAL_LINE); \
    printf("\n"); \
  } while(0)

#define STRLEN(s) (sizeof(s) - 1)
 
#define PRETTY_PRINT_TEST_TYPE_MSG(func) \
  do { \
    if (STRLEN(#func) == 0) \
      break; \
    \
    const char* test_run_msg = RUNNING_TEST_TYPE_MSG(func); \
    size_t test_run_msg_len = STRLEN(RUNNING_TEST_TYPE_MSG(func)); \
    \
    printf(PRETTY_BOX_CORNER); \
    PRETTY_PRINT_HORIZONTAL_LINE(test_run_msg_len + 2); \
    printf(PRETTY_BOX_CORNER); \
    printf("\n"); \
    \
    PRETTY_PRINT_VERTICAL_LINE_STR(test_run_msg); \
    \
    printf(PRETTY_BOX_CORNER); \
    PRETTY_PRINT_HORIZONTAL_LINE(test_run_msg_len + 2); \
    printf(PRETTY_BOX_CORNER); \
    printf("\n"); \
  } while(0)


#define TIMER_SINGLE_TEST(func, cases) \
  do { \
    DO_NOTHING_XD(); \
    clock_t start; \
    clock_t end; \
    double timeTaken; \
    printf(RUNNING_CASES_MSG(func, cases)); \
    start = clock(); \
    (void) func; \
    end = clock(); \
    timeTaken = ((double) end - start) / CLOCKS_PER_SEC; \
    printf("     Success! Passed cases: %zu\n", run_cases); \
    run_cases = cases; \
    total_cases += cases; \
    printf("     Runtime: %lf[s]\n", timeTaken); \
  } while(0)

#define TIMER_MULTIPLE_TESTS(func) \
  do { \
    DO_NOTHING_XD(); \
    PRETTY_PRINT_TEST_TYPE_MSG(func); \
    clock_t start; \
    clock_t end; \
    double timeTaken; \
    start = clock(); \
    (void) func; \
    end = clock(); \
    timeTaken = ((double) end - start) / CLOCKS_PER_SEC; \
    printf("\n%s total runtime: %lf[s]\n\n", #func, timeTaken); \
    run_cases = 0; \
    total_time += timeTaken; \
  } while(0)

#define PRINT_TOTALS() \
  do { \
    printf("Total time elapsed: %lf[s]\n", total_time); \
    printf("Total cases: %zu\n", total_cases); \
  } while(0)

#endif // !TEST_UTILS_H