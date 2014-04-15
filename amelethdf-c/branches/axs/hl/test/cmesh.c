/* test path tools
 */

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include <ah5_exp_cmesh.h>
#include <ahh5_cmesh.h>

int tests_run = 0;

static char *test_sort_intersection()
{
  int i;
  AH5_cmesh_t cmesh;
  AH5_intersection_t inters[10];
  inters[9] = (AH5_intersection_t){INTER_FILL,       {2, 5, 7}, 0, 6};
  inters[8] = (AH5_intersection_t){INTER_FILL,       {2, 6, 7}, 0, 5};
  inters[7] = (AH5_intersection_t){INTER_FILL,       {2, 6, 8}, 0, 5};
  inters[6] = (AH5_intersection_t){INTER_FILL,       {2, 6, 8}, 0, 5};
  inters[5] = (AH5_intersection_t){INTER_FILL,       {2, 6, 8}, 0, 5};
  inters[4] = (AH5_intersection_t){INTER_STRUCTURED, {2, 6, 8}, 0, 5};
  inters[3] = (AH5_intersection_t){INTER_FILL,       {2, 6, 8}, 1, 5};
  inters[2] = (AH5_intersection_t){INTER_FREE,       {2, 6, 8}, 1, 5};
  inters[1] = (AH5_intersection_t){INTER_FREE,       {2, 6, 8}, 1, 6};
  inters[0] = (AH5_intersection_t){INTER_STRUCTURED, {2, 6, 8}, 1, 5};

  cmesh.nb_intersections = 10;
  cmesh.intersections = (AH5_intersection_t *)malloc(cmesh.nb_intersections
                        * sizeof(AH5_intersection_t));

  cmesh.intersections[0] = (AH5_intersection_t){INTER_FREE, {2, 6, 8}, 1, 5};
  cmesh.intersections[1] = (AH5_intersection_t){INTER_FILL, {2, 6, 8}, 0, 5};
  cmesh.intersections[2] = (AH5_intersection_t){INTER_STRUCTURED, {2, 6, 8}, 0, 5};
  cmesh.intersections[3] = (AH5_intersection_t){INTER_FILL, {2, 5, 7}, 0, 6};
  cmesh.intersections[4] = (AH5_intersection_t){INTER_STRUCTURED, {2, 6, 8}, 1, 5};
  cmesh.intersections[5] = (AH5_intersection_t){INTER_FREE, {2, 6, 8}, 1, 6};
  cmesh.intersections[6] = (AH5_intersection_t){INTER_FILL, {2, 6, 7}, 0, 5};
  cmesh.intersections[7] = (AH5_intersection_t){INTER_FILL, {2, 6, 8}, 1, 5};
  cmesh.intersections[8] = (AH5_intersection_t){INTER_FILL, {2, 6, 8}, 0, 5};
  cmesh.intersections[9] = (AH5_intersection_t){INTER_FILL, {2, 6, 8}, 0, 5};

  mu_assert("Sort intersection",
            AH5_cmesh_sort_intersection(&cmesh));
  

  for (i = 0; i < 10; ++i)
  {
    mu_assert_eq("test x",
                 cmesh.intersections[i].index[0],
                 inters[i].index[0]);
    mu_assert_eq("test y",
                 cmesh.intersections[i].index[1],
                 inters[i].index[1]);
    mu_assert_eq("test z",
                 cmesh.intersections[i].index[2],
                 inters[i].index[2]);
    mu_assert_eq("test normal",
                 cmesh.intersections[i].normal,
                 inters[i].normal);
    mu_assert_eq("test type",
                 cmesh.intersections[i].type,
                 inters[i].type);
    mu_assert_eq("test polygon_id",
                 cmesh.intersections[i].polygon_id,
                 inters[i].polygon_id);
  }

  return NULL;
}

static char *test_compute_offset()
{
  char polygontypes[] = {POLY_THROUGH, 2,
                         POLY_FIRST, 5,
                         POLY_THROUGH, 2,
                         POLY_LAST, 5,
                         POLY_INSIDE, 2,
                         POLY_THROUGH, 2,
                         POLY_FIRST, 5,
                         POLY_THROUGH, 2,
                         POLY_LAST, 5,
                         POLY_INSIDE, 2
                        };

  int i;
  AH5_cmesh_t cmesh;
  hsize_t polygon_nodes_offsets[10];
  hsize_t regions_index[10];

  hsize_t offsets_ref[] = {0, 2, 7, 9, 14, 16, 18, 23, 25, 30, 32};
  hsize_t regions_index_ref[] = {0, -1, 1, -1, -1, 2, -1, 3, -1, 4, -1};

  cmesh.nb_polygontypes[0] = 10;
  cmesh.nb_polygontypes[1] = 2;
  cmesh.polygontypes = polygontypes;

  mu_assert("compute nodes offset",
            AH5_cmesh_compute_offset(&cmesh,
                                     polygon_nodes_offsets,
                                     /*regions_index=*/NULL));

  for (i = 0; i < 10; ++i)
  {
    mu_assert_eq("check nodes offset",
                 polygon_nodes_offsets[i],
                 offsets_ref[i]);
  }

  mu_assert("compute regions index",
            AH5_cmesh_compute_offset(&cmesh,
                                     /*polygon_nodes_offsets=*/NULL,
                                     regions_index));

  for (i = 0; i < 10; ++i)
  {
    mu_assert_eq("check regions index",
                 regions_index[i],
                 regions_index[i]);
  }

  mu_assert("compute regions index and nodes offset",
            AH5_cmesh_compute_offset(&cmesh,
                                     polygon_nodes_offsets,
                                     regions_index));

  for (i = 0; i < 10; ++i)
  {
    mu_assert_eq("check nodes offset",
                 polygon_nodes_offsets[i],
                 offsets_ref[i]);
    mu_assert_eq("check regions index",
                 regions_index[i],
                 regions_index[i]);
  }

  return NULL;
}

static char *test_interpret_cmesh()
{
  /*
    
   */
  AH5_cmesh_t cmesh;
  AHH5_cmesh_t cmesh_hl;

  int i;

  float x[] = {1., 2., 3.};
  float y[] = {1., 2., 3., 4.};
  float z[] = {1., 2., 3., 4., 5.};
  float nodes[] = {
    /*map 1*/
    2.2, 1.2, 3.,
    2.5, 2.0, 3.,
    3.0, 2.5, 3.,
    3.1, 3.0, 3.,
    3.3, 3.3, 3.,
    4.0, 3.5, 3.,
    4.2, 3.7, 3.,
    /*map 2*/
    3.2, 1.2, 3.,
    3.5, 2.0, 3.,
    4.0, 2.5, 3.,
    4.5, 3.0, 3.,
    4.7, 3.2, 3.,
    /*map 3*/
    1.2, 3.2, 3.,
    1.6, 3.6, 3.,
    /*region nodes*/
    2., 2., 0.,
    2., 3., 0.,
    3., 2., 0.,
    3., 3., 0.,
    3., 4., 0.,
    4., 2., 0.,
    4., 3., 0.,
    4., 4., 0.,
    5., 2., 0.,
    5., 3., 0.
  };
  char polygontypes[] = {
    /*Intersect poly*/
    /*map 1*/
    POLY_LAST, 2,
    POLY_THROUGH, 2,
    POLY_THROUGH, 2,
    POLY_THROUGH, 3,
    POLY_FIRST, 2,
    /*map 2*/
    POLY_LAST, 2,
    POLY_THROUGH, 2,
    POLY_THROUGH, 2,
    POLY_FIRST, 2,
    /*map 3*/
    POLY_INSIDE, 2,
    /*Regions poly*/
    /*map 1*/
    POLY_CTHROUGH, 1,
    POLY_CTHROUGH, 3,
    POLY_CTHROUGH, 4,
    POLY_CTHROUGH, 1,
    POLY_CTHROUGH, 3,
    /*map 2*/
    POLY_CTHROUGH, 1,
    POLY_CTHROUGH, 4,
    POLY_CTHROUGH, 3,
    POLY_CTHROUGH, 1
  };
  AH5_index_t polygonnodes[] = {
    /*Intersect poly*/
    /*map 1*/
    0, 1,
    1, 2,
    2, 3,
    3, 4, 5,
    5, 6,
    /*map 2*/
    7, 8,
    8, 9,
    9, 10,
    10, 11,
    /*map 3*/
    12, 13,
    /*Regions poly*/
    /*map1*/
    16,
    17, 15, 14,
    20, 9, 8, 2,
    17,
    21, 18, 17,
    /*map 2*/
    19,
    20, 3, 2, 16,
    13, 22, 19,
    20
  };

  /* intersection index of each groups */
  AH5_index_t map1[] = {0,1,2,3,4,5,6,7};
  AH5_index_t map2[] = {8,9,10,11,12,13};
  AH5_index_t map3[] = {14};
  AH5_index_t map1p[] = {1,3,5};
  AH5_index_t map1m[] = {2,4,6};
  AH5_index_t map2p[] = {9, 11};
  AH5_index_t map2m[] = {10,12};
  
  /* Build the grid */
  cmesh.grid.x.nb_nodes = 3;
  cmesh.grid.x.nodes = x;
  cmesh.grid.y.nb_nodes = 4;
  cmesh.grid.y.nodes = y;
  cmesh.grid.z.nb_nodes = 5;
  cmesh.grid.z.nodes = z;
  /* the nodes */
  cmesh.nb_nodes[0] = 24;
  cmesh.nb_nodes[1] = 3;
  cmesh.nodes = nodes;
  /* Build the intersections */
  cmesh.nb_intersections = 15;
  cmesh.intersections =
      (AH5_intersection_t*)malloc(
          cmesh.nb_intersections * sizeof(AH5_intersection_t));
  /* map 1 */
  cmesh.intersections[ 0] = (AH5_intersection_t){INTER_FREE, {2,1,3}, 2, 0};
  cmesh.intersections[ 1] = (AH5_intersection_t){INTER_FREE, {2,2,3}, 2, 1};
  cmesh.intersections[ 2] = (AH5_intersection_t){INTER_FREE, {3,2,3}, 2, 2};
  cmesh.intersections[ 3] = (AH5_intersection_t){INTER_FREE, {3,3,3}, 2, 3};
  cmesh.intersections[ 4] = (AH5_intersection_t){INTER_FREE, {4,3,3}, 2, 4};
  /**/
  cmesh.intersections[ 5] = (AH5_intersection_t){INTER_FREE, {2,2,3}, 2, 1};
  cmesh.intersections[ 6] = (AH5_intersection_t){INTER_FREE, {3,2,3}, 2, 2};
  cmesh.intersections[ 7] = (AH5_intersection_t){INTER_FREE, {3,3,3}, 2, 3};
  /* map 2 */
  cmesh.intersections[ 8] = (AH5_intersection_t){INTER_FREE, {3,1,3}, 2, 5};
  cmesh.intersections[ 9] = (AH5_intersection_t){INTER_FREE, {3,2,3}, 2, 6};
  cmesh.intersections[10] = (AH5_intersection_t){INTER_FREE, {4,2,3}, 2, 7};
  cmesh.intersections[11] = (AH5_intersection_t){INTER_FREE, {4,3,3}, 2, 8};
  /**/
  cmesh.intersections[12] = (AH5_intersection_t){INTER_FREE, {3,2,3}, 2, 6};
  cmesh.intersections[13] = (AH5_intersection_t){INTER_FREE, {4,2,3}, 2, 7};
  /**/
  /* map 3 */
  cmesh.intersections[14] = (AH5_intersection_t){INTER_FREE, {1,3,3}, 2, -9};
  /* Build polygons */
  cmesh.nb_polygontypes[0] = 19;
  cmesh.nb_polygontypes[1] = 2;
  cmesh.polygontypes = polygontypes;
  cmesh.nb_polygonnodes = 42;
  cmesh.polygonnodes = polygonnodes;
  
  /* Build regions */
  cmesh.nb_regions = 10;
  cmesh.regions =
      (AH5_region_t*)malloc(
          cmesh.nb_polygontypes[0]
          * cmesh.nb_polygontypes[1]
          * sizeof(AH5_region_t));
  cmesh.regions[0] = (AH5_region_t){0.1, 10};
  cmesh.regions[1] = (AH5_region_t){0.2, 12};
  cmesh.regions[2] = (AH5_region_t){0.3, 14};
  
  cmesh.regions[3] = (AH5_region_t){0.1, 11};
  cmesh.regions[4] = (AH5_region_t){0.2, 13};
  cmesh.regions[5] = (AH5_region_t){0.3, 15};
  
  cmesh.regions[6] = (AH5_region_t){0.4, 16};
  cmesh.regions[7] = (AH5_region_t){ 0.5, 18};
  
  cmesh.regions[8] = (AH5_region_t){0.4, 17};
  cmesh.regions[9] = (AH5_region_t){0.5, 19};

  /* The groups */
  cmesh.nb_groupgroups = 0;
  cmesh.nb_groups = 5;
  cmesh.groups =
      (AH5_cgroup_t*)malloc(
          cmesh.nb_groups * sizeof(AH5_cgroup_t));
  cmesh.groups[0] = (AH5_cgroup_t){"map1", "face", "PEC", 8, map1};
  cmesh.groups[1] = (AH5_cgroup_t){"map2", "face", "PEC", 6, map2};
  cmesh.groups[2] = (AH5_cgroup_t){"map3", "face", "PEC", 1, map3};

  cmesh.groups[3] = (AH5_cgroup_t){"map1m", "volume", "free", 3, map1m};
  cmesh.groups[4] = (AH5_cgroup_t){"map2p", "volume", "free", 3, map2p};

  /* Interpret the conform mesh. */
  mu_assert("Interpret conform mesh.",
            AHH5_interpret_cmesh(&cmesh_hl, &cmesh));

  mu_assert_eq("The number of group.",
               cmesh_hl.nb_groups, 5);
  mu_assert_eq("The number of groupgroups.",
               cmesh_hl.nb_groupgroups, 0);
  mu_assert_str_equal("The number of group 0.",
                      cmesh_hl.groups[0].path, "map1");
  mu_assert_str_equal("The number of group 1.",
                      cmesh_hl.groups[1].path, "map2");
  mu_assert_str_equal("The number of group 2.",
                      cmesh_hl.groups[2].path, "map3");
  mu_assert_str_equal("The number of group 0.",
                      cmesh_hl.groups[3].path, "map1m");
  mu_assert_str_equal("The number of group 1.",
                      cmesh_hl.groups[4].path, "map2p");
  
  mu_assert_eq("The nodes numbers.",
               cmesh_hl.nb_nodes[0], 24);
  mu_assert_eq("The nodes number (dims).",
               cmesh_hl.nb_nodes[1], 3);
  for (i = 0; i < 3*24; ++i)
    mu_assert_eq("The nodes values.",
                 cmesh_hl.nodes[i], nodes[i]);

  mu_assert_eq("The intersections number.",
               cmesh_hl.nb_intersections, 15);
  mu_assert_eq("Intersections [0] type.",
               cmesh_hl.intersections[0].type, INTER_FREE);
  mu_assert_eq("Intersections [0] i.",
               cmesh_hl.intersections[0].index[0], 2);
  mu_assert_eq("Intersections [0] j.",
               cmesh_hl.intersections[0].index[1], 1);
  mu_assert_eq("Intersections [0] k.",
               cmesh_hl.intersections[0].index[2], 3);
  mu_assert_eq("Intersections [0] normal.",
               cmesh_hl.intersections[0].normal, 2);
  mu_assert_ne("Intersections [0] polygon.",
               cmesh_hl.intersections[0].polygon, NULL);
  mu_assert_eq("Intersections [0] polygon sign.",
               cmesh_hl.intersections[0].polygon->path.orientation,
               AHH5_FORWARD_POLYGONAL_PATH);
  mu_assert_eq("Intersections [0] polygon type.",
               cmesh_hl.intersections[0].polygon->path.type, POLY_LAST);
  mu_assert_eq("Intersections [0] polygon nb nodes.",
               cmesh_hl.intersections[0].polygon->path.nb_nodes, 2);
  mu_assert_eq("Intersections [0] polygon node 1.",
               cmesh_hl.intersections[0].polygon->path.nodes_index[0], 0);
  mu_assert_eq("Intersections [0] polygon node 2.",
               cmesh_hl.intersections[0].polygon->path.nodes_index[1], 1);
  mu_assert_eq("Intersections [0] polygon region.",
               cmesh_hl.intersections[0].polygon->region, NULL);

  mu_assert_eq("Intersections [1] type.",
               cmesh_hl.intersections[1].type, INTER_FREE);
  mu_assert_eq("Intersections [1] i.",
               cmesh_hl.intersections[1].index[0], 2);
  mu_assert_eq("Intersections [1] j.",
               cmesh_hl.intersections[1].index[1], 2);
  mu_assert_eq("Intersections [1] k.",
               cmesh_hl.intersections[1].index[2], 3);
  mu_assert_eq("Intersections [1] normal.",
               cmesh_hl.intersections[1].normal, 2);
  mu_assert_eq("Intersections [1] polygon sign.",
               cmesh_hl.intersections[1].polygon->path.orientation,
               AHH5_FORWARD_POLYGONAL_PATH);
  mu_assert_eq("Intersections [1] polygon type.",
               cmesh_hl.intersections[1].polygon->path.type, POLY_THROUGH);
  mu_assert_eq("Intersections [1] polygon nb nodes.",
               cmesh_hl.intersections[1].polygon->path.nb_nodes, 2);
  mu_assert_eq("Intersections [1] polygon node 1.",
               cmesh_hl.intersections[1].polygon->path.nodes_index[0], 1);
  mu_assert_eq("Intersections [1] polygon node 2.",
               cmesh_hl.intersections[1].polygon->path.nodes_index[1], 2);
  mu_assert_ne("Intersections [1] polygon region.",
               cmesh_hl.intersections[1].polygon->region, NULL);
  mu_assert_eq("Intersections [1] polygon region polygonal path type.",
               cmesh_hl.intersections[1].polygon->region->path.type, POLY_CTHROUGH);
  mu_assert_eq("Intersections [1] polygon region polygonal path nb nodes.",
               cmesh_hl.intersections[1].polygon->region->path.nb_nodes, 1);
  mu_assert_eq("Intersections [1] polygon region polygonal path first node.",
               cmesh_hl.intersections[1].polygon->region->path.nodes_index[0], 16);
  mu_assert_eq("Intersections [1] polygon region area.",
               cmesh_hl.intersections[1].polygon->region->area, (float)0.1);
  

  mu_assert_eq("Intersections [14] type.",
               cmesh_hl.intersections[14].type, INTER_FREE);
  mu_assert_eq("Intersections [14] i.",
               cmesh_hl.intersections[14].index[0], 1);
  mu_assert_eq("Intersections [14] j.",
               cmesh_hl.intersections[14].index[1], 3);
  mu_assert_eq("Intersections [14] k.",
               cmesh_hl.intersections[14].index[2], 3);
  mu_assert_eq("Intersections [14] normal.",
               cmesh_hl.intersections[14].normal, 2);
  mu_assert_ne("Intersections [14] polygon.",
               cmesh_hl.intersections[14].polygon, NULL);
  mu_assert_eq("Intersections [14] polygon sign.",
               cmesh_hl.intersections[14].polygon->path.orientation,
               AHH5_BACKWARD_POLYGONAL_PATH);
  mu_assert_eq("Intersections [14] polygon region",
               cmesh_hl.intersections[14].polygon->region, NULL);
  
  return NULL;
}

// Make a function for run all tests.
static char *all_tests()
{
  mu_run_test(test_sort_intersection);
  mu_run_test(test_compute_offset);
  mu_run_test(test_interpret_cmesh);

  return NULL; // And do not forget to return NULL at end to say success.
}


// All files must have the main function.
int main(int argc, char **argv)
{
  char *result = all_tests();

  if (result != 0)
    printf("%s\n", result);
  else
    printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
