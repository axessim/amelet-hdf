/* test path tools
 */

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include <ah5_exp_cmesh.h>
#include <ahh5_cmesh.h>

int tests_run = 0;

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
    POLY_CLOSE, 8,
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
    1,2,3,4,5,6,7,8,
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
  AH5_index_t map3[] = {14,15};
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
  cmesh.nb_intersections = 16;
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
  cmesh.intersections[15] = (AH5_intersection_t){INTER_FREE, {5,8,5}, 2, 10};
  /* Build polygons */
  cmesh.nb_polygontypes[0] = 20;
  cmesh.nb_polygontypes[1] = 2;
  cmesh.polygontypes = polygontypes;
  cmesh.nb_polygonnodes = 50;
  cmesh.polygonnodes = polygonnodes;
  
  /* Build regions */
  cmesh.nb_regions = 11;
  cmesh.regions =
      (AH5_region_t*)malloc(
          cmesh.nb_polygontypes[0]
          * cmesh.nb_polygontypes[1]
          * sizeof(AH5_region_t));
  cmesh.regions[0] = (AH5_region_t){0.1, 11};
  cmesh.regions[1] = (AH5_region_t){0.2, 13};
  cmesh.regions[2] = (AH5_region_t){0.3, 15};
  
  cmesh.regions[3] = (AH5_region_t){0.1, 12};
  cmesh.regions[4] = (AH5_region_t){0.2, 14};
  cmesh.regions[5] = (AH5_region_t){0.3, 16};
  
  cmesh.regions[6] = (AH5_region_t){0.4, 17};
  cmesh.regions[7] = (AH5_region_t){ 0.5, 19};
  
  cmesh.regions[8] = (AH5_region_t){0.4, 18};
  cmesh.regions[9] = (AH5_region_t){0.5, 20};

  cmesh.regions[10] = (AH5_region_t){0.6, -1};

  /* The groups */
  cmesh.nb_groupgroups = 0;
  cmesh.nb_groups = 5;
  cmesh.groups =
      (AH5_cgroup_t*)malloc(
          cmesh.nb_groups * sizeof(AH5_cgroup_t));
  cmesh.groups[0] = (AH5_cgroup_t){"map1", "face", "PEC", 8, map1};
  cmesh.groups[1] = (AH5_cgroup_t){"map2", "face", "PEC", 6, map2};
  cmesh.groups[2] = (AH5_cgroup_t){"map3", "face", "PEC", 2, map3};

  cmesh.groups[3] = (AH5_cgroup_t){"map1m", "volume", "free", 3, map1m};
  cmesh.groups[4] = (AH5_cgroup_t){"map2p", "volume", "free", 3, map2p};

  /* Interpret the conform mesh. */
  mu_assert("Interpret conform mesh.",
            AHH5_interpret_cmesh(&cmesh_hl, &cmesh));

  /* Print */
  AHH5_print_cmesh(&cmesh_hl, 0);

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
               cmesh_hl.nb_intersections, 16);
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

  mu_assert_eq("Intersections [15] type.",
               cmesh_hl.intersections[15].type, INTER_FREE);
  mu_assert_eq("Intersections [15] i.",
               cmesh_hl.intersections[15].index[0], 5);
  mu_assert_eq("Intersections [15] j.",
               cmesh_hl.intersections[15].index[1], 8);
  mu_assert_eq("Intersections [15] k.",
               cmesh_hl.intersections[15].index[2], 5);
  mu_assert_eq("Intersections [15] normal.",
               cmesh_hl.intersections[15].normal, 2);
  mu_assert_ne("Intersections [15] polygon.",
               cmesh_hl.intersections[15].polygon, NULL);
  mu_assert_eq("Intersections [15] polygon sign.",
               cmesh_hl.intersections[15].polygon->path.orientation,
               AHH5_FORWARD_POLYGONAL_PATH);
  mu_assert_eq("Intersections [15] polygon type.",
               cmesh_hl.intersections[15].polygon->path.type, POLY_CLOSE);
  mu_assert_eq("Intersections [15] polygon nb nodes.",
               cmesh_hl.intersections[15].polygon->path.nb_nodes, 8);
  mu_assert_eq("Intersections [15] polygon node 1.",
               cmesh_hl.intersections[15].polygon->path.nodes_index[0], 1);
  mu_assert_eq("Intersections [15] polygon node 2.",
               cmesh_hl.intersections[15].polygon->path.nodes_index[1], 2);
  mu_assert_ne("Intersections [15] polygon region",
               cmesh_hl.intersections[15].polygon->region, NULL);
  mu_assert_eq("Intersections [15] polygon type.",
               cmesh_hl.intersections[15].polygon->region->path.type, POLY_CLOSE);
  mu_assert_eq("Intersections [15] polygon nb nodes.",
               cmesh_hl.intersections[15].polygon->region->path.nb_nodes, 0);

  /*release memory*/
  AHH5_free_cmesh(&cmesh_hl);
  
  return NULL;
}

static char *test_intersection()
{
  AHH5_intersection_t a, b;

  a.index[0] = 0;
  a.index[1] = 0;
  a.index[2] = 0;
  a.normal = 0;
  a.type = 0;
  a.polygon = NULL;

  b.index[0] = 0;
  b.index[1] = 0;
  b.index[2] = 0;
  b.normal = 0;
  b.type = 0;
  b.polygon = NULL;

  mu_assert_eq("cmp", AHH5_intersection_cmp(&a, &b), 0);
  
  a.index[0] = 1;
  mu_assert_gt("cmp", AHH5_intersection_cmp(&a, &b), 0);
  mu_assert_lt("cmp", AHH5_intersection_cmp(&b, &a), 0);

  a.index[0] = 0;
  a.index[1] = 1;
  mu_assert_gt("cmp", AHH5_intersection_cmp(&a, &b), 0);
  mu_assert_lt("cmp", AHH5_intersection_cmp(&b, &a), 0);

  a.index[1] = 0;
  a.normal = 1;
  mu_assert_gt("cmp", AHH5_intersection_cmp(&a, &b), 0);
  mu_assert_lt("cmp", AHH5_intersection_cmp(&b, &a), 0);

  a.normal = 0;
  a.type = 1;
  mu_assert_gt("cmp", AHH5_intersection_cmp(&a, &b), 0);
  mu_assert_lt("cmp", AHH5_intersection_cmp(&b, &a), 0);

  a.type = 0;
  a.polygon = (AHH5_polygon_t*)1;
  mu_assert_gt("cmp", AHH5_intersection_cmp(&a, &b), 0);
  mu_assert_lt("cmp", AHH5_intersection_cmp(&b, &a), 0);

  return NULL;
}

// Make a function for run all tests.
static char *all_tests()
{
  mu_run_test(test_interpret_cmesh);
  mu_run_test(test_intersection);

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
