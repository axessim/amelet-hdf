/**
 * a minimal unit testing framework for C
 * http://www.jera.com/techinfo/jtns/jtn002.html
 *
 *
 * The tests are split in a set of tests suite. The test suite can be
 * define for each publiched functions for exemeple.  Technily a test
 * suite is define by a simple function was return NULL If all tests
 * are checked. At first test was faild the test suite function return
 * a string corresponding to the faild unitiare test. The tests suite
 * fonction have the folowing shape:
 *
 *    char* test_suite_name()
 *    {
 *        if (0 == 1)
 *            return "Message to explain what's wrong!";
 *        return NULL; // succese
 *    }
 *
 * To help the developper some unitaire macros are define like:
 *
 *  - mu_assert(char* message, bool test)
 *  - mu_assert_equal(char* message, xxx* va1, xxx* val2)
 *  - mu_assert_str_equal(char* message, char* str1, char* str2)
 *
 * Finaly all test suite are run by 'mu_run_test'. The tipical usage is:
 *
 *    // **do not forget to set the counter to zero**
 *    tests_run = 0;
 *
 *    char *test_suite1();
 *    char *test_suite2();
 *
 *    char *all_tests()
 *    {
 *        mu_run_test(test_suite1);
 *        mu_run_test(test_suite2);
 *        return 0;
 *    }
 *
 *    int main(int argc, char **argv)
 *    {
 *        char *result;
 *        tests_run = 0;
 *        result = all_tests();
 *
 *        if (result != 0)
 *        {
 *            printf("%s\n", result);
 *        }
 *        else
 *        {
 *            printf("ALL TESTS PASSED\n");
 *        }
 *        printf("Tests run: %d\n", tests_run);
 *
 *        return result != 0;
 *    }
 */

#ifndef _TESTS_TEST_H_
#define _TESTS_TEST_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <hdf5.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/** Return value for success */
#define MU_FINISHED_WITHOUT_ERRORS NULL

/** The test suite counter. */
extern int tests_run;
char *__message__;

/** Run the test suite fonction. */
#define mu_run_test(test) do {                            \
    printf("Start test block '%s'\n", #test);             \
    __message__ = test();                                 \
    tests_run++;                                          \
    if (__message__ != 0)                                 \
    {                                                     \
      printf("'%s' FAILED in test: ", #test);             \
      return __message__;                                 \
    }                                                     \
    printf("Test block '%s' OK\n", #test);                \
    printf("--------------------------------------\n");   \
  } while (0)

/**
 * Some macros for make unitaire tests. This macros are used inside
 * test suite fonction.
 *
 * mu_assert(char* message, bool test)
 * mu_assert_tree(char* message, bool test)
 * mu_assert_false(char* message, bool test)
 * mu_assert_equal(char* message, xxx val1, xxx val2)
 * mu_str_assert_equal(char* message, char* str1, char* str2)
 */
#define MU_ASSERT_WITH_MSG(message, test, errormsg, ...) do {       \
    if (!(test)) {                                                  \
      printf("Test %s:%d '%s' ", __FILE__, __LINE__, message);      \
      if ((errormsg) != NULL)                                       \
        printf(errormsg, __VA_ARGS__);                              \
      printf(" FAILED.\n");                                         \
      return message;                                               \
    }                                                               \
  } while (0)

#define mu_assert(message, test) do {                                   \
    if (!(test)) {                                                      \
      printf("Test %s:%d '%s' FAILED.\n", __FILE__, __LINE__, message); \
      return message;                                                   \
    }                                                                   \
  } while (0)


#define mu_assert_true(message, test) mu_assert((message), (test))
#define mu_assert_false(message, test) mu_assert((message), !(test))

#define mu_assert_equal(message, str1, str2) mu_assert(message, (str1) == (str2))
#define mu_assert_not_equal(message, str1, str2) mu_assert(message, (str1) != (str2))

#define mu_assert_eq(message, str1, str2)                               \
  MU_ASSERT_WITH_MSG(message, (str1) == (str2), "(0x%x == 0x%x)", str1, str2)
#define mu_assert_eqf(message, str1, str2)                               \
  MU_ASSERT_WITH_MSG(message, (str1) == (str2), "(%e == %e)", str1, str2)
#define mu_assert_ne(message, str1, str2) mu_assert(message, (str1) != (str2))
#define mu_assert_ge(message, str1, str2) mu_assert(message, (str1) >= (str2))
#define mu_assert_gt(message, str1, str2) mu_assert(message, (str1) > (str2))
#define mu_assert_le(message, str1, str2) mu_assert(message, (str1) <= (str2))
#define mu_assert_lt(message, str1, str2) mu_assert(message, (str1) < (str2))
#define mu_assert_close(message, str1, str2, tol) mu_assert(message, abs((str1) - (str2)) < (tol))

#define mu_assert_str_equal(message, str1, str2) mu_assert(message, !strcmp(str1, str2))

//! A simple function to allocate a new string.
__inline char* new_string(char *src)  {
    char *dst = malloc((strlen(src) + 1) * sizeof(char));
    strcpy(dst, src);
    return dst;
}

//! Build a test file from file name and the extension.
__inline hid_t AH5_build_test_file_from_name(const char* name, const char* ext)  {
  char *file_name;
  hid_t file_id;
  file_name = (char*)malloc((strlen(name) + strlen(ext) + 1)* sizeof(char));
  strcpy(file_name, name);
  strcat(file_name, ext);
  file_id = H5Fcreate((file_name), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  free(file_name);
  return file_id;
}

//! copy from disk
__inline void filecopy(const char *input, const char *output)
{
    signed char buf[100];
    void *pb = (void *) buf;
    FILE *fpi, *fpo;
    size_t n;

    fpi = fopen(input, "rb");
    fpo = fopen(output, "wb");

    if (fpi && fpo)
    {
        while ((n = fread(pb, sizeof(char), 100, fpi)))
            fwrite(pb, sizeof(char), n, fpo);
    }
    fclose(fpi);
    fclose(fpo);
}

//! Used the test suite fonction name to build the test file.
#if __GNUC__
# define AH5_auto_test_file() AH5_build_test_file_from_name(__func__, ".test.h5")
#else
# define AH5_auto_test_file() AH5_build_test_file_from_name(__FUNCTION__, ".test.h5")
#endif

//! Close the test file.
#define AH5_close_test_file(file_id) H5Fclose((file_id))

#endif // _TESTS_TEST_H_
