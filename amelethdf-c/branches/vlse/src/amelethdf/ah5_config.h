// ah5_config.h
// 
// Author: NMT
// Description: ah5 config header.

#ifndef AH5_CONFIG_H_
#define AH5_CONFIG_H_

#ifdef AMELETHDF_C_LIBRARY
# define AH5_EXPORT __declspec(dllexport)
#else
# define AH5_EXPORT __declspec(dllimport)
#endif

#endif  // AH5_CONFIG_H_
