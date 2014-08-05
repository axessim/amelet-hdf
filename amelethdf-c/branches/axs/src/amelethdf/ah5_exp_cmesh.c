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

      switch (cmesh->polygontypes[i * 2])
      {
        case POLY_THROUGH:
        case POLY_CLOSE:
          regions_index[i] = count++;
          break;
      }
    }
  }
}


char AH5_init_intersection(AH5_intersection_t *inter, AH5_intersection_type_t type, AH5_index_t i, AH5_index_t j, AH5_index_t k, char normal, AH5_index_t polygon_id)
{
  if (!inter)
    return AH5_FALSE;

  inter->type = type;
  inter->polygon_id = polygon_id;
  inter->normal = normal;
  inter->index[i] = i;
  inter->index[j] = j;
  inter->index[k] = k;
  return AH5_TRUE;
}

char AH5_init_region(AH5_region_t *region, float area, AH5_index_t polygon_id)
{
  if (!region)
    return AH5_FALSE;

  region->area = area;
  region->polygon_id = polygon_id;

  return AH5_TRUE;
}

char AH5_init_conform_group(AH5_cgroup_t *group, const char *path, const char *type, const char *entitytype, hsize_t nb_groupelts, const AH5_index_t *groupelts)
{
  if (!group)
    return AH5_FALSE;

  group->path = NULL;
  if (path)
  {
    group->path = (char*)malloc(sizeof(char) * (strlen(path)+1));
    strcpy(group->path, path);
  }

  group->type = NULL;
  if (type)
  {
    group->type = (char*)malloc(sizeof(char) * (strlen(type)+1));
    strcpy(group->type, type);
  }

  group->entitytype = NULL;
  if (entitytype)
  {
    group->entitytype = (char*)malloc(sizeof(char) * (strlen(entitytype)+1));
    strcpy(group->entitytype, entitytype);
  }

  group->groupelts = NULL;
  group->nb_groupelts = 0;
  if (groupelts && nb_groupelts)
  {
    group->groupelts = (AH5_index_t*)malloc(sizeof(AH5_index_t) * nb_groupelts);
    memcpy(group->groupelts, groupelts, sizeof(AH5_index_t) * nb_groupelts);
  }

  return AH5_TRUE;
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
