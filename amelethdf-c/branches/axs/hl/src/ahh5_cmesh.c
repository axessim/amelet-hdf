/**
 * @file   ahh5_cmesh.c
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Oct 31 09:34:21 2013
 *
 * @brief  ahh5_cmesh.h implemetation.
 *
 *
 */

#include <ah5_exp_cmesh.h>

#include "ahh5_cmesh.h"

#define AHH5_MEMCPY_(dst, src, attr_name, attr_size, ctype)             \
  dst->attr_size = src->attr_size;                                      \
  dst->attr_name = (ctype*)malloc(dst->attr_size * sizeof(ctype));      \
  memcpy(dst->attr_name, src->attr_name, dst->attr_size * sizeof(ctype))

/**
 * @def  AHH5_MEMCOPY(dst, src, attr_name, ctype)
 * Short cut to copy AmeletHDF members.
 *
 * @code
 * dst->nb_attr_name = src->nb_attr_name;
 * dst->attr_name = (ctype*)malloc(dst->attr_size * sizeof(ctype));
 * memcpy(dst->attr_name, src->attr_name, dst->nb_attr_name * sizeof(ctype));
 * @endcode
 */
#define AHH5_MEMCOPY(dst, src, attr_name, ctype)                \
  AHH5_MEMCPY_(dst, src, attr_name, nb_ ## attr_name, ctype)

/**
 * @brief Copies cartesian grid axis.
 *
 * Copies the axis values from the location pointed by src to the axis pointed
 * by dst.
 *
 * In the event that an error occurred during the copied the memory block is
 * defined has null (dst->nodes = NULL; dst->nb_nodes = 0) and NULL is return.
 *
 * @param dst
 * @param src
 *
 * @return destination is returned or NULL when a error is happened during the
 * copy.
 */
AH5_axis_t *AH5_axis_copy(AH5_axis_t *dst, const AH5_axis_t *src)
{
  dst->nb_nodes = src->nb_nodes;
  dst->nodes = (float *)malloc(dst->nb_nodes * sizeof(float));
  memcpy(dst->nodes, src->nodes, dst->nb_nodes * sizeof(float));

  if (dst->nodes == NULL)
  {
    dst->nb_nodes = 0;
    return NULL;
  }

  return dst;
}


/**
 * Load polygonal path.
 *
 * @param path
 * @param polygon_id
 * @param low
 * @param polygon_nodes_offsets
 */
void AHH5_load_polygonal_path(
  AHH5_polygonal_path_t *path, AH5_index_t polygon_id,
  AH5_cmesh_t *low, hsize_t *polygon_nodes_offsets)
{
  int polygon_sign, k;
  hsize_t nb_nodes;

  polygon_sign = AHH5_FORWARD_POLYGONAL_PATH;

  if (polygon_id < 0)
  {
    polygon_id = -polygon_id;
    polygon_sign = AHH5_BACKWARD_POLYGONAL_PATH;
  }

  if (polygon_id < low->nb_polygontypes[POLYGON_SIZE])
  {
    path->orientation = polygon_sign;
    path->type = low->polygontypes[polygon_id * 2];
    nb_nodes = low->polygontypes[polygon_id * 2 + 1];
    path->nb_nodes = nb_nodes;

    path->nodes_index = (AH5_index_t*)malloc(nb_nodes * sizeof(AH5_index_t));

    for (k = 0; k < nb_nodes; ++k)
      path->nodes_index[k] = low->polygonnodes[polygon_nodes_offsets[polygon_id] + k];
  }
}


char AHH5_interpret_cmesh(AHH5_cmesh_t *height, AH5_cmesh_t *low)
{
  int i, j;
  AH5_index_t polygon_id, nb_nodes;
  hsize_t *polygon_nodes_offsets;
  hsize_t *regions_index;

  polygon_nodes_offsets = (hsize_t *)malloc(low->nb_polygontypes[POLYGON_SIZE]
                          * sizeof(hsize_t));
  regions_index = (hsize_t *)malloc(low->nb_polygontypes[POLYGON_SIZE]
                                    * sizeof(hsize_t));
  AH5_cmesh_compute_offset(low, polygon_nodes_offsets, regions_index);


  AH5_axis_copy(&height->grid.x, &low->grid.x);
  AH5_axis_copy(&height->grid.y, &low->grid.y);
  AH5_axis_copy(&height->grid.z, &low->grid.z);

  height->nb_nodes[0] = low->nb_nodes[0];
  height->nb_nodes[1] = low->nb_nodes[1];
  nb_nodes = (AH5_index_t)(height->nb_nodes[0] * height->nb_nodes[1]);
  height->nodes = (float *)malloc(nb_nodes * sizeof(float));
  memcpy(height->nodes, low->nodes, nb_nodes * sizeof(float));

  AHH5_MEMCOPY(height, low, groups, AH5_cgroup_t);
  AHH5_MEMCOPY(height, low, groupgroups, AH5_groupgroup_t);

  height->nb_intersections = low->nb_intersections;
  height->intersections
  = (AHH5_intersection_t *)malloc(height->nb_intersections
                                  * sizeof(AHH5_intersection_t));

  for (i = 0; i < height->nb_intersections; ++i)
  {
    height->intersections[i].type = low->intersections[i].type;
    height->intersections[i].normal = low->intersections[i].normal;
    height->intersections[i].polygon = NULL;

    for (j = 0; j < DIM; ++j)
      height->intersections[i].index[j] = low->intersections[i].index[j];

    if (height->intersections[i].type != INTER_STRUCTURED)
    {
      polygon_id = low->intersections[i].polygon_id;

      if (abs(polygon_id) < low->nb_polygontypes[POLYGON_SIZE])
      {
        height->intersections[i].polygon
        = (AHH5_polygon_t *)malloc(sizeof(AHH5_polygon_t));
        AHH5_load_polygonal_path(&(height->intersections[i].polygon->path),
                                 polygon_id, low, polygon_nodes_offsets);

        height->intersections[i].polygon->region = NULL;

        if (height->intersections[i].polygon->path.type == POLY_THROUGH)
        {
          height->intersections[i].polygon->region
          = (AHH5_region_t *)malloc(sizeof(AHH5_region_t));
          height->intersections[i].polygon->region->area
          = low->regions[regions_index[abs(polygon_id)]].area;
          AHH5_load_polygonal_path(&(height->intersections[i].polygon->region->path),
                                   low->regions[regions_index[abs(polygon_id)]].polygon_id,
                                   low, polygon_nodes_offsets);
        }
      }
    }
  }

  free(polygon_nodes_offsets);
  free(regions_index);
  
  return 1;
}

char AHH5_dump_cmesh(AH5_cmesh_t *low, AHH5_cmesh_t *height)
{
  return 1;
}

int AHH5_intersection_cmp(const AHH5_intersection_t *a, const AHH5_intersection_t *b)
{
  int i;

  for (i = 0; i < DIM; ++i)
    if (a->index[i] != b->index[i])
      return a->index[i] - b->index[i];

  if (a->normal != b->normal)
    return a->normal - b->normal;

  if (a->type != b->type)
    return a->type - b->type;

  return (int)(a->polygon - b->polygon);
}
