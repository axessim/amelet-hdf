/**
 * @file   ah5_exp_cmesh.c
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Oct 31 15:14:11 2013
 *
 * @brief  ah5_cmesh.h implementation
 *
 *
 */

#include "ah5_exp_cmesh.h"
#include "ah5_exp_algo.h"

int AH5_intersection_cmp(const AH5_intersection_t *a, const AH5_intersection_t *b)
{
  int i;

  for (i = 0; i < DIM; ++i)
    if (a->index[i] != b->index[i])
      return b->index[i] - a->index[i];

  if (a->normal != b->normal)
    return b->normal - a->normal;

  if (a->type != b->type)
    return b->type - a->type;

  return b->polygon_id - a->polygon_id;
}

char AH5_cmesh_sort_intersection(AH5_cmesh_t *cmesh)
{
  qsort(cmesh->intersections, cmesh->nb_intersections,
        sizeof(AH5_intersection_t), AH5_intersection_cmp);
  return AH5_TRUE;
}

char AH5_cmesh_compute_offset(const AH5_cmesh_t *cmesh,
                              hsize_t *polygon_nodes_offsets,
                              hsize_t *regions_index)
{
  unsigned int i, count;

  if (polygon_nodes_offsets != NULL)
  {
    polygon_nodes_offsets[0] = 0;

    for (i = 1; i < cmesh->nb_polygontypes[POLYGON_SIZE]; ++i)
      polygon_nodes_offsets[i] = polygon_nodes_offsets[i - 1]
                                 + cmesh->polygontypes[i * 2 - 1];
  }

  if (regions_index != NULL)
  {
    count = 0;

    for (i = 0; i < cmesh->nb_polygontypes[POLYGON_SIZE]; ++i)
    {
      regions_index[i] = -1;

      if (cmesh->polygontypes[i * 2] == POLY_THROUGH)
        regions_index[i] = count++;
    }
  }
}
