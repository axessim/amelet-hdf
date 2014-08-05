/**
 * @file   ahh5_cmesh.h
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Oct 31 09:29:38 2013
 *
 * @brief  Height level API for conform mesh.
 *
 *
 */

#ifndef _AHH5_CMESH_H_
#define _AHH5_CMESH_H_

#include <ah5_exp_cmesh.h>

#include "ahh5_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AHH5_FORWARD_POLYGONAL_PATH 1
#define AHH5_BACKWARD_POLYGONAL_PATH -1

  /**
   * @struct AHH5_polygonal_path_t
   *
   * A polygonal path is a connected series of line segments. More formally, a
   * polygonal path P is a curve specified by a sequence of points
   * \scriptstyle(A_1, A_2, \dots, A_n) called its vertices so that the curve
   * consists of the line segments connecting the consecutive vertices.
   *
   * In this context the polygonal path must insert a quad's edges.
   *
   */
  typedef struct _AHH5_polygonal_path_t
  {
    AH5_polygon_type_t type;
    hsize_t            nb_nodes;     /**The number of nodes in polygonal path.*/
    AH5_index_t        *nodes_index; /**The nodes index in nodes array.*/
    int                orientation;  /***/
  } AHH5_polygonal_path_t, AHH5_pp_t;

  /**
   * @struct AHH5_region_t
   *
   * This structure given more information on intersection to define a closed region.
   *
   * @see AH5_region_t
   */
  typedef struct
  {
    AHH5_polygonal_path_t path; /**Define the polygonal path who close the area*/
    float                 area; /**The region area define by the close polygon.*/
  } AHH5_region_t;

  /**
   * @struct AHH5_polygon_t
   *
   * A polygon is a close polygonal path. The polygon is define by two
   * polygonal path. The intersection polygonal path (insed the quad) and the
   * region polygonal path, who define only the intersection through the quad.
   *
   * In this context the polygon must insed a quad edges inclued.
   *
   */
  typedef struct _AHH5_polygon_t
  {
    AHH5_polygonal_path_t path;    /**Define the intersection polygon path*/
    AHH5_region_t         *region; /**Define the polygonal path who close the
                                    * polygon if it is.*/
  } AHH5_polygon_t;

  /**
   * @struc AHH5_intersection_t
   *
   * Define a quad surface intersection.
   *
   */
  typedef struct _AHH5_intersection_t
  {
    AH5_intersection_type_t type;        /**The intersection type.*/
    AH5_index_t             index[DIM];  /**The quad's index.*/
    char                    normal;      /**The quad's normal*/
    AHH5_polygon_t          *polygon;    /**The intersection polygon if make sens.*/
  } AHH5_intersection_t;


  /**
   * @struct AHH5_cmesh_t
   *
   * The conforme mesh top level structure.
   */
  typedef struct _AHH5_cmesh_t
  {
    AH5_cartesian_grid_t grid;
    hsize_t              nb_nodes[2];
    float                *nodes;
    hsize_t              nb_intersections;
    AHH5_intersection_t *intersections;
    hsize_t              nb_groups;
    AH5_cgroup_t         *groups;
    hsize_t              nb_groupgroups;
    AH5_groupgroup_t     *groupgroups;
  } AHH5_cmesh_t;

  /** 
   * Initialized polygon path
   * 
   * @param poly[inout] a valid polygon path
   * @param nb_nodes the number of nodes
   * @param nodes_index the nodes index array
   * @param orientation the way to walk under the path (forward or backward)
   * 
   * @return true if success.
   */
  AHH5_PUBLIC char AHH5_init_polygonal_path(
      AHH5_polygonal_path_t *poly,
      AH5_polygon_type_t type,
      hsize_t nb_nodes,
      const AH5_index_t *nodes_index,
      int orientation);
  
  /**
   * Interpret the low level conform mesh to height level api.
   *
   * @param[in] low The low level cmesh.
   * @param[out] height the height level cmesh.
   *
   * @return true if success.
   */
  AHH5_PUBLIC char AHH5_interpret_cmesh(AHH5_cmesh_t *height, AH5_cmesh_t *low);

  /**
   * Dump into low level API the given height level conform mesh.
   *
   * @param[in] height the height level cmesh.
   * @param[out] low The low level cmesh.
   *
   * @return true if success.
   */
  AHH5_PUBLIC char AHH5_dump_cmesh(AH5_cmesh_t *low, AHH5_cmesh_t *height);

  AHH5_PUBLIC char AHH5_read_cmesh(hid_t file_id, const char *path, AHH5_cmesh_t *cmesh);

  AHH5_PUBLIC char AHH5_write_cmesh(hid_t file_id, AHH5_cmesh_t *cmesh);

  AHH5_PUBLIC void AHH5_free_cmesh(AHH5_cmesh_t *cmesh);
  AHH5_PUBLIC void AHH5_free_intersection(AHH5_intersection_t *intersection);
  AHH5_PUBLIC void AHH5_free_polygon(AHH5_polygon_t *polygon);
  AHH5_PUBLIC void AHH5_free_region(AHH5_region_t *region);
  AHH5_PUBLIC void AHH5_free_polygonal_path(AHH5_polygonal_path_t *path);
  
  /** 
   * Print the given conform mesh into the standard output.
   * 
   * @param cmesh[in] a valid conform mesh instance.
   * @param space[in] the space indent level.
   */
  AHH5_PUBLIC void AHH5_print_cmesh(const AHH5_cmesh_t *cmesh, int space);

  /** 
   * Print the given intersection into the standard output.
   *
   * If the conform mesh if given the intersection path's nodes are display, in
   * other case the node's index is display.
   * 
   * @param intersection[in] a valid intersection instance.
   * @param space[in] the space indent level.
   * @param cmesh[in] a valid conform mesh instance or NULL
   */
  AHH5_PUBLIC void AHH5_print_intersection(
      const AHH5_intersection_t *intersection, int space,
      const AHH5_cmesh_t *cmesh);

  /** 
   * Print the given polygon into the standard output.
   *
   * If the conform mesh if given the intersection path's nodes are display, in
   * other case the node's index is display.
   *
   * @param polygon[in] a valid polygon instance.
   * @param space[in] the space indent level.
   * @param cmesh[in] a valid conform mesh instance or NULL
   */
  AHH5_PUBLIC void AHH5_print_polygon(
      const AHH5_polygon_t *polygon, int space,
      const AHH5_cmesh_t *cmesh);

  /** 
   * Print the given polygon into the standard output.
   *
   * If the conform mesh if given the intersection path's nodes are display, in
   * other case the node's index is display.
   * 
   * @param region [in] a valid region instance.
   * @param space[in] the space indent level.
   * @param cmesh[in] a valid conform mesh instance or NULL
   */
  AHH5_PUBLIC void AHH5_print_region(
      const AHH5_region_t *region, int space,
      const AHH5_cmesh_t *cmesh);

  /** 
   * Print the given polygonal path into the standard output.
   *
   * If the conform mesh if given the intersection path's nodes are display, in
   * other case the node's index is display.
   * 
   * @param path[in] a valid polygonal path instance.
   * @param space[in] the space indent level.
   * @param cmesh[in] a valid conform mesh instance or NULL
   */
  AHH5_PUBLIC void AHH5_print_polygonal_path(
      const AHH5_polygonal_path_t *path, int space,
      const AHH5_cmesh_t *cmesh);

  // Some tools functions
  /**
   * Compare two intersection.
   *
   * This function is conform to AH5_cmp_func_t.
   *
   * @param a
   * @param b
   *
   * @return b - a as a integer.
   */
  AHH5_PUBLIC int AHH5_intersection_cmp(
      const AHH5_intersection_t *a, const AHH5_intersection_t *b);

#ifdef __cplusplus
}
#endif

#endif /* _AHH5_CMESH_H_ */
