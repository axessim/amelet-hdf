/**
 * @file   ahh5_mesh.h
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Jan 30 12:59:47 2014
 *
 * @brief
 *
 *
 */

#ifndef _AHH5_MESH_H_
#define _AHH5_MESH_H_

#include <ah5_c_mesh.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Build a linear rang
   *
   * @param axis
   * @param start
   * @param step
   * @param size
   *
   * @return
   */
  char ahh5_axis_build_linspace(AH5_axis_t *axis, float start, float step, int size);

#ifdef __cplusplus
}
#endif

#endif /* _AHH5_MESH_H_ */
