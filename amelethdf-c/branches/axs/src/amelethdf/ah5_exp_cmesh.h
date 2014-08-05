/**
 * @file   ah5_exp_cmesh.h
 * @author Nathanaël MUOT <nathanael.muot@axessim.fr>
 * @date   Thu Oct 31 08:51:23 2013
 *
 * @brief  Tools for read/write conform mesh.
 *
 * Typical HDF5 tree:
 *
 * /conform_mesh.h5
 * `-- mesh/
 *     `-- $gmesh/
 *         `-- $cmesh[@type=conform]/
 *             +-- cartesianGrid/
 *             |   +-- x[]
 *             |   +-- y[]
 *             |   `-- z[]
 *             +-- intersections
 *             +-- nodes
 *             +-- polygonTypes
 *             +-- polygonNodes
 *             +-- regions
 *             +-- group\
 *             `-- groupGroup\
 */

#ifndef _AH5_EXP_CMESH_H_
#define _AH5_EXP_CMESH_H_

#include "ah5_general.h"
#include "ah5_c_mesh.h"

#ifdef __cplusplus
extern "C" {
#endif

  /* @def DIM
   * The default mesh dimension
   */
#define DIM 3

  /* \def POLYGON_SIZE
   * The dimension defining the number of polygons.
   *
   * @code
   * cmesh.nb_polygontypes[POLYGON_SIZE]
   * @endcode
   */
#define POLYGON_SIZE 0

  typedef int AH5_index_t;

  typedef struct
  {
    char            *path;
    char            *type;       //TODO Need a enum
    char            *entitytype; //TODO Need a enum
    hsize_t         nb_groupelts;
    AH5_index_t     *groupelts;
  } AH5_cgroup_t;

  typedef struct
  {
    AH5_axis_t      x;
    AH5_axis_t      y;
    AH5_axis_t      z;
  } AH5_cartesian_grid_t;

  typedef enum
  {
    INTER_INVALID       = -1,
    INTER_FILL          = 1,
    INTER_FREE          = 2,
    INTER_STRUCTURED    = 3,
    INTER_USER_TYPE     = 4
  } AH5_intersection_type_t;

  typedef struct
  {
    AH5_intersection_type_t type;
    AH5_index_t             index[DIM];
    char                    normal;
    AH5_index_t             polygon_id;
  } AH5_intersection_t;

  /**
   * @enum AH5_polygon_type_t
   *
   * Define the intersection polygon type (how the surface pass through the
   * quad).
   */
  typedef enum
  {
    POLY_INVALID       = -1,/**Not valid polygon.*/
    POLY_FIRST         = 1, /**Only the first node is in the quad edges, the
                             * other nodes are inside.*/
    POLY_LAST          = 2, /**Only the last node is in the quad edges, the
                             * other nodes are inside*/
    POLY_INSIDE        = 3, /**All the node are inside the quad.*/
    POLY_HOLE          = 4, /**All the node are inside the quad and it is close*/

    POLY_THROUGH       = 5, /**The two ends nodes are in the quad edges.*/
    POLY_CTHROUGH      = 6, /**Close the through path.*/
    POLY_CLOSE         = 7  /**The polygon is close and caring by the quad's edges*/
  } AH5_polygon_type_t;

  /**
   * @struct AH5_region_t
   *
   * This structure given more information on intersection to define a closed region.
   *
   * The polgygone can follow the quad edges or not for drow an area insid the
   * quad. The first polygon's node is the next node for define the flose area.
   *
   *       (0)                   2   1
   *     +--*------+ 1           +---*------+
   *     |   \     |             |    \     |
   *     |    \  A |          (0)*  A  \    |
   *     |     \   |             | \    \   |
   *     +------*--+             +--*----*--+
   *            (1) 0               (1)  0
   *
   * (0) and (1) are the intersection nodes. 0, 1 and 2 are the nodes who close
   * the region A.
   */
  typedef struct
  {
    float       area;       /**The region area define by the close polygon.*/
    AH5_index_t polygon_id; /**Define the polygon who close the area*/
  } AH5_region_t;

  typedef struct
  {
    AH5_cartesian_grid_t grid;
    hsize_t              nb_intersections;
    AH5_intersection_t   *intersections;
    hsize_t              nb_nodes[2];
    float                *nodes;
    hsize_t              nb_polygontypes[2];
    char                 *polygontypes;
    hsize_t              nb_polygonnodes;
    AH5_index_t          *polygonnodes;
    hsize_t              nb_regions;
    AH5_region_t         *regions;
    hsize_t              nb_groups;
    AH5_cgroup_t         *groups;
    hsize_t              nb_groupgroups;
    AH5_groupgroup_t     *groupgroups;
  } AH5_cmesh_t;

  /**
   * Initialized intersection
   *
   * @param[inout] inter a valid intersection instance
   * @param type the intersection type (Fill, Free, Structured ...)
   * @param index the index under the grid (i, j, k)
   * @param normal the quad index (0=x, 1=y, 2=z)
   * @param polygon_id the polygon index under the conform mesh polygons list
   *
   * @return true is success.
   **/
  AH5_PUBLIC char AH5_init_intersection(AH5_intersection_t *inter, AH5_intersection_type_t type, AH5_index_t i, AH5_index_t j, AH5_index_t k, char normal, AH5_index_t polygon_id);

  /**
   * Initialized region
   *
   * @param[inout] region a valid region instance
   * @param area The region area define by the close polygon.
   * @param polygon_id Define the polygon who close the area
   **/
  AH5_PUBLIC char AH5_init_region(AH5_region_t *region, float area, AH5_index_t polygon_id);

  /**
   * Initialized conform group
   *
   * @param[inout] group a valid group instance
   * @param path the group mesh path
   * @param type the type (node or element)
   * @param entitytype the element type (edge, face, volume)
   * @param nb_groupelts the number of elements
   * @param groupelts the elements array
   **/
  AH5_PUBLIC char AH5_init_conform_group(AH5_cgroup_t *group, const char *path, const char *type, const char *entitytype, hsize_t nb_groupelts, const AH5_index_t *groupelts);

  /**
   * Read conform mesh group at given location (file_id, path).
   *
   * @param[in] file_id the hdf5 open node id.
   * @param[in] path the group path relatively to file_id node.
   * @param[out] cgroup the conform group.
   *
   * @return true is success.
   */
  AH5_PUBLIC char AH5_read_cmsh_group(hid_t file_id, const char *path, AH5_cgroup_t *cgroup);

  /**
   * Read the conform mesh at given location (file_id, path).
   *
   * @param file_id[in] the hdf5 open node id.
   * @param path[in] the conform mesh path relatively to file_id node.
   * @param cmesh[out] the conform mesh.
   *
   * @return true is success.
   */
  AH5_PUBLIC char AH5_read_cmesh(hid_t file_id, const char *path, AH5_cmesh_t *cmesh);

  /**
   * Write conform mesh groups.
   *
   * @param file_id the hdf5 open node id.
   * @param cgroup the conform mesh groups list.
   * @param nb_grp the number of groups.
   *
   * @return true is success.
   */
  AH5_PUBLIC char AH5_write_cmsh_group(hid_t file_id, AH5_cgroup_t *cgroup, int nb_grp);

  /**
   * Write conform mesh.
   *
   * @param file_id the hdf5 open node id.
   * @param cmesh the conform mesh.
   *
   * @return true is success.
   */
  AH5_PUBLIC char AH5_write_cmesh(hid_t file_id, AH5_cmesh_t *cmesh);

  /**
   * Print in standard output the given conform mesh 'space' white space at
   * right column.
   *
   * @param cmesh the conform nesh
   * @param space the number of white space at right.
   */
  AH5_PUBLIC void AH5_print_cmesh(const AH5_cmesh_t *cmesh, int space);

  /**
   * Free memory
   *
   * @param cmesh
   */
  AH5_PUBLIC void AH5_free_cmesh(AH5_cmesh_t *cmesh);
  AH5_PUBLIC void AH5_free_intersection(AH5_intersection_t *intersection);
  AH5_PUBLIC void AH5_free_region(AH5_region_t *region);
  AH5_PUBLIC void AH5_free_cartesian_grid(AH5_cartesian_grid_t *grid);
  AH5_PUBLIC void AH5_free_cgroup(AH5_cgroup_t *group);

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
  AH5_PUBLIC int AH5_intersection_cmp(const AH5_intersection_t *a, const AH5_intersection_t *b);

  /**
   * Sort the intersection into lexical order (cell index, normal, type, polygon
   * index)
   *
   * @param cmesh the conform mesh to sorted.
   *
   * @return true if success.
   */
  AH5_PUBLIC char AH5_cmesh_sort_intersection(AH5_cmesh_t *cmesh);

  /**
   * Compute the offset tables for read polygon and region in arbitrary order.
   *
   * If the given pointer for polygon or regions is NULL the specified tableau
   * is not evaluated. In the other case the tableau must be alloyed with at
   * least the number of polygons define under the conform mesh.
   *
   * @param[in] cmesh
   * @param[out] polygon_nodes_offsets
   * @param[out] regions_index
   *
   * @return true if success.
   */
  AH5_PUBLIC char AH5_cmesh_compute_offset(
        const AH5_cmesh_t *cmesh,
    hsize_t *polygon_nodes_offsets,
    hsize_t *regions_index);

#ifdef __cplusplus
}
#endif

#endif // _AH5_EXP_CMESH_H_
