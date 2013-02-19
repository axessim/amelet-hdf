// ah5_config.h
// 
// Author: NMT
// Description: ah5 config header.

#ifndef AH5_CONFIG_H_
#define AH5_CONFIG_H_

// See http://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
# ifdef AMELETHDF_C_LIBRARY
#  ifdef __GNUC__
#   define AH5_EXPORT __attribute__ ((dllexport))
#  else
#   define AH5_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#  endif
# else
#  ifdef __GNUC__
#   define AH5_EXPORT __attribute__ ((dllimport))
#  else
#   define AH5_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#  endif
# endif
#define AH5_LOCAL
#else
# if __GNUC__ >= 4
#  define AH5_EXPORT __attribute__ ((visibility ("default")))
#  define AH5_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define AH5_PUBLIC
#  define AH5_LOCAL
# endif
#endif

#endif  // AH5_CONFIG_H_
