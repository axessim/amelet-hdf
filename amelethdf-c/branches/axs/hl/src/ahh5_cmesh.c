/**
 * @file   ahh5_cmesh.c
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Oct 31 09:34:21 2013
 *
 * @brief  ahh5_cmesh.h implemetation.
 *
 *
 */

#include <stdio.h>

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

char AHH5_init_polygonal_path(
    AHH5_polygonal_path_t *poly,
    AH5_polygon_type_t type,
    hsize_t nb_nodes,
    const AH5_index_t *nodes_index,
    int orientation)
{
  if (!poly)
    return AH5_FALSE;

  poly->type = type;
  poly->orientation = orientation;
  
  poly->nb_nodes = 0;
  poly->nodes_index = NULL;
  if (nodes_index && nb_nodes)
  {
    poly->nb_nodes = nb_nodes;
    poly->nodes_index = (AH5_index_t*)malloc(nb_nodes * sizeof(AH5_index_t));
    memcpy(poly->nodes_index, nodes_index, nb_nodes * sizeof(AH5_index_t));
  }

  return AH5_TRUE;
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

  if (polygon_id < (int)low->nb_polygontypes[POLYGON_SIZE])
  {
    path->orientation = polygon_sign;
    path->type = low->polygontypes[polygon_id * 2];
    nb_nodes = low->polygontypes[polygon_id * 2 + 1];
    path->nb_nodes = nb_nodes;

    path->nodes_index = (AH5_index_t*)malloc(nb_nodes * sizeof(AH5_index_t));

    for (k = 0; k < (int)nb_nodes; ++k)
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
  for (i = 0; i < height->nb_groups; ++i)
  {
    /*AHH5_MEMCOPY copy memory so 'path' is copied as point, however a deep
     * copy is expected*/
    height->groups[i].path
        = malloc((strlen(low->groups[i].path)+1)*sizeof(char));
    strcpy(height->groups[i].path, low->groups[i].path);
    height->groups[i].groupelts
        = malloc(low->groups[i].nb_groupelts*sizeof(AH5_index_t));
    memcpy(height->groups[i].groupelts, low->groups[i].groupelts,
           low->groups[i].nb_groupelts*sizeof(AH5_index_t));
  }
  AHH5_MEMCOPY(height, low, groupgroups, AH5_groupgroup_t);
  for (i = 0; i < height->nb_groupgroups; ++i)
  {
    /*AHH5_MEMCOPY copy memory so 'path' is copied as point, however a deep
     * copy is expected*/
    height->groupgroups[i].path
        = malloc((strlen(low->groupgroups[i].path)+1)*sizeof(char));
    strcpy(height->groupgroups[i].path, low->groupgroups[i].path);
  }

  height->nb_intersections = low->nb_intersections;
  height->intersections
  = (AHH5_intersection_t *)malloc(height->nb_intersections
                                  * sizeof(AHH5_intersection_t));

  for (i = 0; i < (int)height->nb_intersections; ++i)
  {
    height->intersections[i].type = low->intersections[i].type;
    height->intersections[i].normal = low->intersections[i].normal;
    height->intersections[i].polygon = NULL;

    for (j = 0; j < DIM; ++j)
      height->intersections[i].index[j] = low->intersections[i].index[j];

    if (height->intersections[i].type != INTER_STRUCTURED)
    {
      polygon_id = low->intersections[i].polygon_id;

      if (abs(polygon_id) < (int)low->nb_polygontypes[POLYGON_SIZE])
      {
        height->intersections[i].polygon
        = (AHH5_polygon_t *)malloc(sizeof(AHH5_polygon_t));
        AHH5_load_polygonal_path(&(height->intersections[i].polygon->path),
                                 polygon_id, low, polygon_nodes_offsets);

        height->intersections[i].polygon->region = NULL;

        /*Load the intersection who pass through the quad and make two simple region. */
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

        /*Load the intersections who make a complex intersection.*/
        if (height->intersections[i].polygon->path.type == POLY_CLOSE)
        {
          height->intersections[i].polygon->region
              = (AHH5_region_t *)malloc(sizeof(AHH5_region_t));
          height->intersections[i].polygon->region->area
              = low->regions[regions_index[abs(polygon_id)]].area;
          AHH5_init_polygonal_path(
              &(height->intersections[i].polygon->region->path),
              height->intersections[i].polygon->path.type, 0, NULL, 0);
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


void AHH5_free_cmesh(AHH5_cmesh_t *cmesh)
{
  int i;
  
  if (cmesh)
  {
    if (cmesh->intersections)
    {
      for (i = 0; i < cmesh->nb_intersections; ++i)
        AHH5_free_intersection(cmesh->intersections + i);
      free(cmesh->intersections);
      cmesh->intersections = NULL;
    }
    cmesh->nb_intersections = 0;

    if (cmesh->groupgroups)
    {
      for (i = 0; i < cmesh->nb_groupgroups; ++i)
        AH5_free_groupgroup(cmesh->groupgroups + i);
      free(cmesh->groupgroups);
      cmesh->groupgroups = NULL;
    }
    cmesh->nb_groupgroups = 0;

    if (cmesh->groups)
    {
      for (i = 0; i < cmesh->nb_groups; ++i)
        AH5_free_cgroup(cmesh->groups + i);
      free(cmesh->groups);
      cmesh->groups = NULL;
    }
    cmesh->nb_groups = 0;
  }
}


void AHH5_free_intersection(AHH5_intersection_t *intersection)
{
  if (intersection)
    AHH5_free_polygon(intersection->polygon);
}


void AHH5_free_polygon(AHH5_polygon_t *polygon)
{
  if (polygon)
  {
    AHH5_free_polygonal_path(&(polygon->path));
    if (polygon->region)
      AHH5_free_region(polygon->region);
  }
}


void AHH5_free_region(AHH5_region_t *region)
{
  if (region)
    AHH5_free_polygonal_path(&(region->path));
}


void AHH5_free_polygonal_path(AHH5_polygonal_path_t *path)
{
  if (path) {
    free(path->nodes_index);
    path->nodes_index = NULL;
  }
}


void AHH5_print_cmesh(const AHH5_cmesh_t *cmesh, int space)
{
  int i;
  printf("%*s-conform mesh\n", space, "");

  for (i = 0; i < cmesh->nb_intersections; ++i)
    AHH5_print_intersection(cmesh->intersections + i, space + 2, cmesh);
}


void AHH5_print_intersection(
    const AHH5_intersection_t *intersection, int space,
    const AHH5_cmesh_t *cmesh)
{
  printf("%*s-intersection: type=%lu, index=[%lu, %lu, %lu], normal=%lu\n", space, "",
         (unsigned long) intersection->type,
         (unsigned long) intersection->index[0],
         (unsigned long) intersection->index[1],
         (unsigned long) intersection->index[2],
         (unsigned long) intersection->normal);
  if (intersection->polygon)
    AHH5_print_polygon(intersection->polygon, space + 2, cmesh);
}


void AHH5_print_polygon(
    const AHH5_polygon_t *polygon, int space,
    const AHH5_cmesh_t *cmesh)
{
  printf("%*s-polygon:\n", space, "");
  AHH5_print_polygonal_path(&(polygon->path), space + 2, cmesh);
  if (polygon->region)
    AHH5_print_region(polygon->region, space + 2, cmesh);
}


void AHH5_print_region(
    const AHH5_region_t *region, int space,
    const AHH5_cmesh_t *cmesh)
{
  printf("%*s-region: area=%e\n", space, "",
         region->area);
  AHH5_print_polygonal_path(&(region->path), space + 2, cmesh);
}


void AHH5_print_polygonal_path(
    const AHH5_polygonal_path_t *path, int space,
    const AHH5_cmesh_t *cmesh)
{
  int i;
  printf("%*s-polygonal path: type=%lu, orientation=%lu\n", space, "",
         (unsigned long) path->type, (unsigned long) path->orientation);
  printf("%*s nb nodes: %lu", space + 2, "", path->nb_nodes);

  if (!cmesh)
  {
    printf("[");
    for (i = 0; i < path->nb_nodes; ++i)
      printf("%*s %lu, ", space + 4, "", (unsigned long) path->nodes_index[i]);
    printf("]\n");
  }
  else
  {
    printf("\n");
    for (i = 0; i < path->nb_nodes; ++i)
      printf("%*s %lu, (%e, %e, %e)\n",
             space + 4, "",
             (unsigned long) path->nodes_index[i],
             cmesh->nodes[3 * path->nodes_index[i]],
             cmesh->nodes[3 * path->nodes_index[i] + 1],
             cmesh->nodes[3 * path->nodes_index[i] + 2]);
  }
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

  return (int)(a->polygon) - (int)(b->polygon);
}
