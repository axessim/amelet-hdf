// test path tools

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include "ah5.h"

int tests_run = 0;

// Make a function for a group of coherent tests. Here is a simple example.
static char* test_sanity() {
  mu_assert("A little sentence to say that I test"
            AH5_TRUE) // The test
  return NULL; // And do not forget to return NULL at end to say success.
}


// Build the following tree using relative and absolute path into HDF5 file.
//
//  /test_path_usage.h5
//  `-- grp1/
//      +-- grp2/
//      |   `-- grp3/
//      `-- grp4/
//
// Is a simple usage of test file without tests.
//
static char* test_path_usage() {
  hid_t file_id, grp1_id, grp2_id, grp3_id, grp4_id;

  file_id = AH5_create_test_file("test_path_usage.h5");

  // Build group from the file root by relative path (group name).
  grp1_id = H5Gcreate(file_id, "grp1", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  // Build group from a node with them name 
  grp2_id = H5Gcreate(grp1_id, "grp2", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  // Build group from a node with relative path 
  grp3_id = H5Gcreate(grp1_id, "grp2/grp3", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  // Build group with absolute path
  grp4_id = H5Gcreate(grp1_id, "/grp1/grp4", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  
  AH5_close_test_file(file_id);

  return NULL;
}


// Make a function for run all tests.
static char * all_tests() {
    mu_run_test(test_sanity);
    mu_run_test(test_path_usage);
    return NULL; // And do not forget to return NULL at end to say success.
}


// All files must have the main function.
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


