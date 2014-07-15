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

#if !__GNUC__
# define __compar_fn_t int (*)(const void* p1, const void* p2)
#endif

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
        sizeof(AH5_intersection_t), (__compar_fn_t)AH5_intersection_cmp);
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

void AH5_free_cmesh(AH5_cmesh_t *cmesh)
{
  int i;

  if (cmesh)
  {
    AH5_free_cartesian_grid(&(cmesh->grid));

    if (cmesh->intersections)
    {
      for (i = 0; i < cmesh->nb_intersections; ++i)
        AH5_free_intersection(cmesh->intersections + i);
      free(cmesh->intersections);
      cmesh->intersections = NULL;
    }
    cmesh->nb_intersections = 0;

    if (cmesh->nodes)
    {
      free(cmesh->nodes);
      cmesh->nodes = NULL;
    }
    cmesh->nb_nodes[0] = 0;
    cmesh->nb_nodes[0] = 0;

    if (cmesh->polygontypes)
    {
      free(cmesh->polygontypes);
      cmesh->polygontypes = NULL;
    }
    cmesh->nb_polygontypes[0] = 0;
    cmesh->nb_polygontypes[0] = 0;

    if (cmesh->polygonnodes)
    {
      free(cmesh->polygonnodes);
      cmesh->polygonnodes = NULL;
    }
    cmesh->nb_polygonnodes = 0;

    if (cmesh->regions)
    {
      for (i = 0; i < cmesh->nb_regions; ++i)
        AH5_free_region(cmesh->regions + i);
      free(cmesh->regions);
      cmesh->regions = NULL;
    }
    cmesh->nb_regions = 0;

    if (cmesh->groups)
    {
      for (i = 0; i < cmesh->nb_groups; ++i)
        AH5_free_cgroup(cmesh->groups + i);
      free(cmesh->groups);
      cmesh->groups = NULL;
    }
    cmesh->nb_groups = 0;

    if (cmesh->groupgroups)
    {
      for (i = 0; i < cmesh->nb_groupgroups; ++i)
        AH5_free_groupgroup(cmesh->groupgroups + i);
      free(cmesh->groupgroups);
      cmesh->groupgroups = NULL;
    }
    cmesh->nb_groupgroups = 0;
  }
}

void AH5_free_intersection(AH5_intersection_t *intersection)
{
  /*no memory allocated*/
}

void AH5_free_cartesian_grid(AH5_cartesian_grid_t *grid)
{
  /*no memory allocated*/
}

void AH5_free_region(AH5_region_t *region)
{
  /*no memory allocated*/
}

void AH5_free_cgroup(AH5_cgroup_t *group)
{
  if (group)
  {
    if (group->path)
    {
      free(group->path);
      group->path = NULL;
    }

    if (group->groupelts)
    {
      free(group->groupelts);
      group->groupelts = NULL;
      group->nb_groupelts = 0;
    }
  }
}
