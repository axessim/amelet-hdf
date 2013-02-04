//
// a minimal unit testing framework for C
// http://www.jera.com/techinfo/jtns/jtn002.html
//
// 
//

#ifndef _TESTS_TEST_H_
#define _TESTS_TEST_H_

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)
extern int tests_run;


//
// Tools to build test file.
//
// XXX(nmt):
// For the moment, a hard file is build. In next version I would like worked in
// memory or cleaned the working directory.
//
#define AH5_create_test_file(filename) H5Fcreate((filename), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)
#define AH5_close_test_file(file_id) H5Fclose((file_id))


#endif // _TESTS_TEST_H_
