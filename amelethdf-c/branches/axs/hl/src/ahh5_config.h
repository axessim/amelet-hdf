#ifndef AHH5_CONFIG_H
#define AHH5_CONFIG_H


#ifdef __cplusplus
extern "C" {
#endif

// See http://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
# ifdef AMELETHDF_C_HL_LIBRARY
#  ifdef __GNUC__
#   define AHH5_PUBLIC __attribute__ ((dllexport))
#  else
#   define AHH5_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#  endif
# else
#  ifdef __GNUC__
#   define AHH5_PUBLIC __attribute__ ((dllimport))
#  else
#   define AHH5_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#  endif
# endif
#define AHH5_LOCAL
#else
# if __GNUC__ >= 4
#  define AHH5_PUBLIC __attribute__ ((visibility ("default")))
#  define AHH5_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define AHH5_PUBLIC
#  define AHH5_LOCAL
# endif
#endif

#ifdef __cplusplus
}
#endif

#endif // AHH5_CONFIG_H