// test path tools

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include "ah5.h"

int tests_run = 0;

static char* test_join() {
  char path[AH5_ABSOLUTE_PATH_LENGTH];
  char base[] = "/base";
  char g1[] = "g1";
  char g2[] = "/g2  ";
  char g3[] = "/g3 /";
  char g4[] = "/g4 ";

  // init null string
  path[0] = '\0';
  // strcpy usage
  mu_assert("test strcpy usage",
            !strcmp(AH5_join_path(path, base), base));
  mu_assert("'/base' + 'name' -> '/base/name'",
            !strcmp(AH5_join_path(path, g1), "/base/g1"));
  mu_assert("'/base/g1' + 'g2  ' -> '/base/g1/g2  '",
            !strcmp(AH5_join_path(path, g2), "/base/g1/g2  "));
  mu_assert("'/base/g1/g2  ' + 'g3 /' -> '/base/g1/g2/g3 /'",
            !strcmp(AH5_join_path(path, g3), "/base/g1/g2/g3 /"));
  mu_assert("'/base/g1/g2/g3 /' + '/g4 ' -> '/base/g1/g2/g3 /g4 '",
            !strcmp(AH5_join_path(path, g4), "/base/g1/g2/g3 /g4 "));
  return 0;
}

static char* test_trim() {
  char *sample_strings[] =
      {
        "nothing to trim",
        "    trim the front",
        "trim the back     ",
        " trim one char front and back ",
        " trim one char front",
        "trim one char back ",
        "                   ",
        " ",
        "a",
        "",
        NULL
      };
  
  char *results_strings[] =
      {
        "nothing to trim",
        "trim the front",
        "trim the back",
        "trim one char front and back",
        "trim one char front",
        "trim one char back",
        "",
        "",
        "a",
        "",
        ""
      };
  
  char test_buffer[64];
  int index;
  
  for( index = 0; sample_strings[index] != NULL; ++index )
  {
    strcpy(test_buffer, sample_strings[index]);
    mu_assert("test trim",
              !strcmp(results_strings[index], AH5_trim_path(test_buffer)));
  }
  return 0;
}

static char * all_tests() {
  mu_run_test(test_join);
  mu_run_test(test_trim);
  return 0;
}

int main(int argc, char **argv) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  
  return result != 0;
}


