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
   * In this context the polygonal path must insed a quad edges inclued.
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
   * Interpret the low level conform mesh to height level api.
   *
   * @param[in] low The low level cmesh.
   * @param[out] height the height level cmesh.
   *
   * @return true if success.
   */
  char AHH5_interpret_cmesh(AHH5_cmesh_t *height, AH5_cmesh_t *low);

  /**
   * Dump into low level API the given height level conform mesh.
   *
   * @param height the height level cmesh.
   * @param low The low level cmesh.
   *
   * @return true if success.
   */
  char AHH5_dump_cmesh(AH5_cmesh_t *low, AHH5_cmesh_t *height);

  char AHH5_read_cmesh(hid_t file_id, const char *path, AHH5_cmesh_t *cmesh);

  char AHH5_write_cmesh(hid_t file_id, AHH5_cmesh_t *cmesh);

  void AHH5_print_cmesh(const AHH5_cmesh_t *cmesh, int space);

  void AHH5_free_cmesh(AHH5_cmesh_t *cmesh);

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
  int AHH5_intersection_cmp(const AHH5_intersection_t *a, const AHH5_intersection_t *b);

#ifdef __cplusplus
}
#endif

#endif /* _AHH5_CMESH_H_ */
