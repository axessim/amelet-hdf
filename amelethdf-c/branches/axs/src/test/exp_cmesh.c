/* test path tools
 */

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include <ah5_exp_cmesh.h>

int tests_run = 0;

static char *test_sort_intersection()
{
  int i;
  AH5_cmesh_t cmesh;
  AH5_intersection_t inters[10];
  AH5_init_intersection(inters + 9, INTER_FILL,       2, 5, 7, 0, 6);
  AH5_init_intersection(inters + 8, INTER_FILL,       2, 6, 7, 0, 5);
  AH5_init_intersection(inters + 7, INTER_FILL,       2, 6, 8, 0, 5);
  AH5_init_intersection(inters + 6, INTER_FILL,       2, 6, 8, 0, 5);
  AH5_init_intersection(inters + 5, INTER_FILL,       2, 6, 8, 0, 5);
  AH5_init_intersection(inters + 4, INTER_STRUCTURED, 2, 6, 8, 0, 5);
  AH5_init_intersection(inters + 3, INTER_FILL,       2, 6, 8, 1, 5);
  AH5_init_intersection(inters + 2, INTER_FREE,       2, 6, 8, 1, 5);
  AH5_init_intersection(inters + 1, INTER_FREE,       2, 6, 8, 1, 6);
  AH5_init_intersection(inters + 0, INTER_STRUCTURED, 2, 6, 8, 1, 5);

  cmesh.nb_intersections = 10;
  cmesh.intersections = (AH5_intersection_t *)malloc(cmesh.nb_intersections
                        * sizeof(AH5_intersection_t));

  AH5_init_intersection(cmesh.intersections + 0, INTER_FREE, 2, 6, 8, 1, 5);
  AH5_init_intersection(cmesh.intersections + 1, INTER_FILL, 2, 6, 8, 0, 5);
  AH5_init_intersection(cmesh.intersections + 2, INTER_STRUCTURED, 2, 6, 8, 0, 5);
  AH5_init_intersection(cmesh.intersections + 3, INTER_FILL, 2, 5, 7, 0, 6);
  AH5_init_intersection(cmesh.intersections + 4, INTER_STRUCTURED, 2, 6, 8, 1, 5);
  AH5_init_intersection(cmesh.intersections + 5, INTER_FREE, 2, 6, 8, 1, 6);
  AH5_init_intersection(cmesh.intersections + 6, INTER_FILL, 2, 6, 7, 0, 5);
  AH5_init_intersection(cmesh.intersections + 7, INTER_FILL, 2, 6, 8, 1, 5);
  AH5_init_intersection(cmesh.intersections + 8, INTER_FILL, 2, 6, 8, 0, 5);
  AH5_init_intersection(cmesh.intersections + 9, INTER_FILL, 2, 6, 8, 0, 5);

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
                         POLY_CLOSE, 7,
                         POLY_INSIDE, 3,
                         POLY_CTHROUGH, 2,
                         POLY_THROUGH, 9,
                         POLY_FIRST, 5,
                         POLY_FIRST, 3,
                         POLY_THROUGH, 2,
                         POLY_HOLE, 4,
                         POLY_INSIDE, 2
                        };

  int i;
  AH5_cmesh_t cmesh;
  hsize_t polygon_nodes_offsets[13];
  hsize_t regions_index[13];

  hsize_t offsets_ref[] = {0,  2,  7,  9, 14, 21, 24, 26, 35, 40, 43, 45, 49};
  hsize_t regions_index_ref[] = {0, -1, 1, -1, 2, -1, -1, 3, -1, -1, 4, 5, -1};

  cmesh.nb_polygontypes[0] = 13;
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
                 regions_index_ref[i]);
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
                 regions_index_ref[i]);
  }

  return NULL;
}


static char *test_free_cmesh()
{
  AH5_cmesh_t cmesh;
  AH5_index_t groupelts[] = {0, 1, 2, 3, 4, 5, 6, 7};

  /* Build the grid */
  cmesh.grid.x.nb_nodes = 3;
  cmesh.grid.x.nodes = (float*)malloc(cmesh.grid.x.nb_nodes * sizeof(float));
  cmesh.grid.y.nb_nodes = 4;
  cmesh.grid.y.nodes = (float*)malloc(cmesh.grid.y.nb_nodes * sizeof(float));
  cmesh.grid.z.nb_nodes = 5;
  cmesh.grid.z.nodes = (float*)malloc(cmesh.grid.z.nb_nodes * sizeof(float));
  /* the nodes */
  cmesh.nb_nodes[0] = 24;
  cmesh.nb_nodes[1] = 3;
  cmesh.nodes = (float*)malloc(cmesh.nb_nodes[0] * cmesh.nb_nodes[1] * sizeof(float));
  /* Build the intersections */
  cmesh.nb_intersections = 15;
  cmesh.intersections =
      (AH5_intersection_t*)malloc(
          cmesh.nb_intersections * sizeof(AH5_intersection_t));
  /* map 1 */
  AH5_init_intersection(cmesh.intersections + 0, INTER_FREE, 2,1,3, 2, 0);
  AH5_init_intersection(cmesh.intersections + 1, INTER_FREE, 2,2,3, 2, 1);
  AH5_init_intersection(cmesh.intersections + 2, INTER_FREE, 3,2,3, 2, 2);
  AH5_init_intersection(cmesh.intersections + 3, INTER_FREE, 3,3,3, 2, 3);
  AH5_init_intersection(cmesh.intersections + 4, INTER_FREE, 4,3,3, 2, 4);
  /**/
  AH5_init_intersection(cmesh.intersections + 5, INTER_FREE, 2,2,3, 2, 1);
  AH5_init_intersection(cmesh.intersections + 6, INTER_FREE, 3,2,3, 2, 2);
  AH5_init_intersection(cmesh.intersections + 7, INTER_FREE, 3,3,3, 2, 3);
  /* map 2 */
  AH5_init_intersection(cmesh.intersections + 8, INTER_FREE, 3,1,3, 2, 5);
  AH5_init_intersection(cmesh.intersections + 9, INTER_FREE, 3,2,3, 2, 6);
  AH5_init_intersection(cmesh.intersections + 10, INTER_FREE, 4,2,3, 2, 7);
  AH5_init_intersection(cmesh.intersections + 11, INTER_FREE, 4,3,3, 2, 8);
  /**/
  AH5_init_intersection(cmesh.intersections + 12, INTER_FREE, 3,2,3, 2, 6);
  AH5_init_intersection(cmesh.intersections + 13, INTER_FREE, 4,2,3, 2, 7);
  /**/
  /* map 3 */
  AH5_init_intersection(cmesh.intersections + 14, INTER_FREE, 1,3,3, 2, -9);
  /* Build polygons */
  cmesh.nb_polygontypes[0] = 19;
  cmesh.nb_polygontypes[1] = 2;
  cmesh.polygontypes = (char*)malloc(cmesh.nb_polygontypes[1]
                                     * cmesh.nb_polygontypes[0]
                                     * sizeof(char));
  cmesh.nb_polygonnodes = 42;
  cmesh.polygonnodes = (AH5_index_t*)malloc(cmesh.nb_polygonnodes
                                            * sizeof(AH5_index_t));

  /* Build regions */
  cmesh.nb_regions = 10;
  cmesh.regions =
      (AH5_region_t*)malloc(
          cmesh.nb_polygontypes[0]
          * cmesh.nb_polygontypes[1]
          * sizeof(AH5_region_t));
  AH5_init_region(cmesh.regions + 0, 0.1, 10);
  AH5_init_region(cmesh.regions + 1, 0.2, 12);
  AH5_init_region(cmesh.regions + 2, 0.3, 14);

  AH5_init_region(cmesh.regions + 3, 0.1, 11);
  AH5_init_region(cmesh.regions + 4, 0.2, 13);
  AH5_init_region(cmesh.regions + 5, 0.3, 15);

  AH5_init_region(cmesh.regions + 6, 0.4, 16);
  AH5_init_region(cmesh.regions + 7, 0.5, 18);

  AH5_init_region(cmesh.regions + 8, 0.4, 17);
  AH5_init_region(cmesh.regions + 9, 0.5, 19);

  /* The groups */
  cmesh.nb_groupgroups = 0;
  cmesh.groupgroups = NULL;
  cmesh.nb_groups = 1;
  cmesh.groups =
      (AH5_cgroup_t*)malloc(
          cmesh.nb_groups * sizeof(AH5_cgroup_t));
  AH5_init_conform_group(cmesh.groups + 0, "path_name", "element", "face", 8, groupelts);

  AH5_free_cmesh(&cmesh);
  AH5_free_cmesh(NULL);

  return NULL;
}

// Make a function for run all tests.
static char *all_tests()
{
  mu_run_test(test_sort_intersection);
  mu_run_test(test_compute_offset);
  mu_run_test(test_free_cmesh);

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
