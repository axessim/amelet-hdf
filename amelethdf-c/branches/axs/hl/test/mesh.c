/**
 * @file   mesh.c
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Fri Jul 11 13:49:21 2014
 * 
 * @brief  Test ahh5_mesh.h
 * 
 * 
 */

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include <ahh5_mesh.h>

int tests_run = 0;

static char *test_axis_build_linspace()
{
  AH5_axis_t axis;

  mu_assert("axis build linspace",
            ahh5_axis_build_linspace(&axis, 0.1, 1.0, 10));
  mu_assert_approx_equal("axis build linspace",
                         axis.nodes[0], 0.1, 0.00001);
  mu_assert_approx_equal("axis build linspace",
                         axis.nodes[1], 1.1, 0.00001);
  mu_assert_approx_equal("axis build linspace",
                         axis.nodes[9], 9.1, 0.00001);
  mu_assert_eq("axis build linspace", axis.nb_nodes, 11);
  
  return NULL;
}

// Make a function for run all tests.
static char *all_tests()
{
  mu_run_test(test_axis_build_linspace);

  return NULL; // And do not forget to return NULL at end to say success.
}


// All files must have the main function.
int main(int argc, char **argv)
{
  char *result = all_tests();

  if (result != 0)
    printf("%s\n", result);
  else
    printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
