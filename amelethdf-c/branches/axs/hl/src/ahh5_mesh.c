/**
 * @file   ahh5_mesh.c
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Jan 30 13:02:25 2014
 *
 * @brief
 *
 *
 */

#include "ahh5_mesh.h"

char ahh5_axis_build_linspace(AH5_axis_t *axis, float start, float step, int size)
{
  char success = AH5_TRUE;
  int i;

  axis->nb_nodes = size + 1;
  axis->nodes = (float *)malloc(axis->nb_nodes * sizeof(float));

  for (i = 0; i < axis->nb_nodes; ++i)
    axis->nodes[i] = start + step * i;

  return success;
}
