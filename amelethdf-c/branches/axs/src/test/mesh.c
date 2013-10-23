// test path tools

#include <string.h>
#include <stdio.h>

#include <ah5.h>
#include "utest.h"

//! Test suite counter.
int tests_run = 0;

//! Build a simple unstructured mesh two tetra,
void build_umesh_1(AH5_umesh_t *umesh)
{
    int i;
    // init nodes
    umesh->nb_nodes[0] = 5;
    umesh->nb_nodes[1] = 3;
    umesh->nodes = (float*)malloc(umesh->nb_nodes[0]*umesh->nb_nodes[1]*sizeof(float));
    for (i = 0; i < umesh->nb_nodes[0]*umesh->nb_nodes[1]; i++)
        umesh->nodes[i] = i;
    // init element
    umesh->nb_elementtypes = 3;
    umesh->elementtypes = (char*)malloc(umesh->nb_elementtypes*sizeof(char));
    umesh->elementtypes[0] = UELE_TETRA4;
    umesh->elementtypes[1] = UELE_TETRA4;
    umesh->elementtypes[2] = UELE_TRI3;

    umesh->nb_elementnodes = 4+4+3;
    umesh->elementnodes = (int*)malloc(umesh->nb_elementnodes*sizeof(int));
    umesh->elementnodes[0] = 0;
    umesh->elementnodes[1] = 1;
    umesh->elementnodes[2] = 2;
    umesh->elementnodes[3] = 3;

    umesh->elementnodes[4] = 1;
    umesh->elementnodes[5] = 2;
    umesh->elementnodes[6] = 3;
    umesh->elementnodes[7] = 4;

    umesh->elementnodes[8] = 1;
    umesh->elementnodes[9] = 2;
    umesh->elementnodes[10] = 3;

    // build group
    umesh->nb_groups = 1;
    umesh->groups = (AH5_ugroup_t*) malloc(umesh->nb_groups * sizeof(AH5_ugroup_t));
    umesh->groups[0].path = (char*) malloc(5 * sizeof(char));
    strcpy(umesh->groups[0].path, "name");
    umesh->groups[0].type = (char*) malloc( 8 * sizeof(char));
    strcpy(umesh->groups[0].type, "element");
    umesh->groups[0].entitytype = (char*) malloc( 5 * sizeof(char));
    strcpy(umesh->groups[0].entitytype, "face");
    umesh->groups[0].nb_groupelts = 1;
    umesh->groups[0].groupelts = (int*) malloc(umesh->groups[0].nb_groupelts * sizeof(int));
    umesh->groups[0].groupelts[0] = 2;
    // group of group
    umesh->nb_groupgroups = 0;
    umesh->groupgroups = NULL;
    // selector on mesh
    umesh->nb_som_tables = 0;
    umesh->som_tables = NULL;
}


//! Test write groupgroup function.
char* test_write_groupgroup()
{
#define DIM0 2
#define LENGTH 13
    AH5_groupgroup_t gg;
    hid_t file_id, filetype, memtype, space, dset;
    char groupgroupnames[DIM0][LENGTH] = {"group_name_1", "group_name_2"};
    char **rdata;
    size_t sdim;
    int ndims, i;
    hsize_t dims[1];

    file_id = AH5_auto_test_file();

    // Chek some bad usages.
    gg.path = NULL;
    gg.nb_groupgroupnames = -1;
    gg.groupgroupnames = NULL;

    mu_assert_false("Bad number of group of group.",
                    AH5_write_groupgroup(file_id, &gg, -1));

    AH5_write_str_dataset(file_id, "dataset_name",
                          DIM0, LENGTH, groupgroupnames[0]);

    // build a groupgroup with relative path.
    gg.path = "gg_name";
    gg.nb_groupgroupnames = 2;
    gg.groupgroupnames = groupgroupnames;
    mu_assert_true("Write valide data.",
                    AH5_write_groupgroup(file_id, &gg, 1));

    // Test the written data using hdf5 API.
    dset = H5Dopen(file_id, "/groupGroup/gg_name", H5P_DEFAULT);
    filetype = H5Dget_type(dset);
    sdim = H5Tget_size(filetype);
    space = H5Dget_space(dset);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    rdata = (char **) malloc(dims[0] * sizeof (char *));
    rdata[0] = (char *) malloc(dims[0] * sdim * sizeof (char));
    for (i=1; i<dims[0]; i++)
        rdata[i] = rdata[0] + i * sdim;
    memtype = H5Tcopy(H5T_C_S1);
    mu_assert("HDF5 error in H5Tset_size.", H5Tset_size(memtype, sdim) >= 0);
    mu_assert("HDF5 error in H5Dread.",
              H5Dread(dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]) >= 0);
    for (i = 0; i < dims[0]; i++)
        mu_assert_str_equal("Check the groupGroup.",
                            rdata[i], groupgroupnames[i]);
    // Release resources.
    free(rdata[0]);
    free(rdata);
    mu_assert("HDF5 error in H5Dclose.", H5Dclose(dset) >= 0);
    mu_assert("HDF5 error in H5Sclose.", H5Sclose(space) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(filetype) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(memtype) >= 0);

    // Close and release.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


//! Test function to write unstructured mesh group.
char* test_write_unstructured_mesh_group()
{
    hid_t file_id;
    AH5_ugroup_t ugrp;
    int i;

    file_id = AH5_auto_test_file();

    // Check some bad usages.
    ugrp.path = NULL;
    ugrp.type = NULL;
    ugrp.entitytype = NULL;
    ugrp.groupelts = NULL;
    ugrp.nb_groupelts = 0;

    mu_assert_false("Negative numbre of groups.",
                    AH5_write_umsh_group(file_id, &ugrp, -1));
    mu_assert_false("Empty group.",
                    AH5_write_umsh_group(file_id, &ugrp, 1));

    // Write a simple group by relative path name.
    ugrp.path = "grp_name";
    ugrp.type = AH5_V_ELEMENT;
    ugrp.entitytype = AH5_V_FACE;
    ugrp.nb_groupelts = 10;
    ugrp.groupelts = (int*)malloc(ugrp.nb_groupelts*sizeof(int));
    for (i = 0; i < ugrp.nb_groupelts; i++)
        ugrp.groupelts[i] = i;

    mu_assert_true("Write a simple group by relative path [1].",
                   AH5_write_umsh_group(file_id, &ugrp, 1));
    mu_assert("Check 'group' category nodes in file [1].",
              AH5_path_valid(file_id, "/group"));
    mu_assert("Check 'group' nodes in file [1].",
              AH5_path_valid(file_id, "/group/grp_name"));


    // Write a simple group by abs path name.
    ugrp.path = "/mesh/$mesh_group/$mesh_name/group/grp_name_2";

    mu_assert_true("Write a simple group by relative path [2].",
                   AH5_write_umsh_group(file_id, &ugrp, 1));
    mu_assert("Check 'group' category nodes in file [2].",
              AH5_path_valid(file_id, "/group"));
    mu_assert("Check 'group' nodes in file [2].",
              AH5_path_valid(file_id, "/group/grp_name_2"));

    // Close and release.
    free(ugrp.groupelts);
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


//! test umesh write function
char *test_write_umesh()
{
    AH5_umesh_t umesh;
    AH5_msh_instance_t mesh;
    hid_t file_id, loc_id;

    // Bad used
    mu_assert("Can not write an empty umesh!",
              !AH5_write_umesh(file_id, NULL));

    // Test on simple unstructured mesh
    file_id = AH5_auto_test_file();
    build_umesh_1(&umesh);

    // Write the mesh into '/mesh' node.
    loc_id = H5Gcreate(file_id, "/mesh", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    mu_assert("Write an simple umesh.",
              AH5_write_umesh(loc_id, &umesh));

    // Check if all the mesh parts are written.
    mu_assert("Check 'nodes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/nodes"));
    mu_assert("Check 'elementNodes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementNodes"));
    mu_assert("Check 'elementTypes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementTypes"));
    mu_assert("Check 'groupGroup' nodes in file.",
              AH5_path_valid(file_id, "/mesh/groupGroup"));
    mu_assert("Check 'group' nodes in file.",
              AH5_path_valid(file_id, "/mesh/group"));
    mu_assert("Check 'group/name' nodes in file.",
              AH5_path_valid(file_id, "/mesh/group/name"));

    //TODO: Used hdf5 API to check the behaviours not the read function.
    // Now read the mesh
    mu_assert("The written mesh is readable.",
              AH5_read_msh_instance(file_id, "/mesh", &mesh));

    // Check some value
    mu_assert_equal("Check mesh type",
                    mesh.type, MSH_UNSTRUCTURED);
    mu_assert_equal("Check nodes size",
                    mesh.data.unstructured.nb_nodes[0], 5);
    mu_assert_equal("Check nodes size",
                    mesh.data.unstructured.nb_nodes[1], 3);
    mu_assert_str_equal("Check the mesh path.",
                        mesh.path, "/mesh");

    // Close and release
    AH5_free_umesh(&umesh);
    AH5_free_msh_instance(&mesh);
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


//! Write an unstructured nodes mesh
char *test_write_unstructured_nodes_mesh()
{
    AH5_umesh_t umesh;
    hid_t file_id, loc_id;
    int i;

    // Build the nesh
    // init nodes
    umesh.nb_nodes[0] = 5;
    umesh.nb_nodes[1] = 3;
    umesh.nodes = (float*)malloc(umesh.nb_nodes[0]*umesh.nb_nodes[1]*sizeof(float));
    for (i = 0; i < umesh.nb_nodes[0]*umesh.nb_nodes[1]; ++i)
        umesh.nodes[i] = i;
    // init element
    umesh.nb_elementtypes = 0;
    umesh.elementtypes = NULL;
    umesh.nb_elementnodes = 0;
    umesh.elementnodes = NULL;
    // build group
    umesh.nb_groups = 1;
    umesh.groups = (AH5_ugroup_t*)malloc(umesh.nb_groups * sizeof(AH5_ugroup_t));
    umesh.groups[0].path = (char*)malloc(5 * sizeof(char));
    strcpy(umesh.groups[0].path, "name");
    umesh.groups[0].type = (char*)malloc( 5 * sizeof(char));
    strcpy(umesh.groups[0].type, "node");
    umesh.groups[0].entitytype = NULL;
    umesh.groups[0].nb_groupelts = 5;
    umesh.groups[0].groupelts = (int*)malloc(umesh.groups[0].nb_groupelts * sizeof(int));
    for (i = 0; i < umesh.groups[0].nb_groupelts; ++i)
      umesh.groups[0].groupelts[i] = i;
    // group of group
    umesh.nb_groupgroups = 0;
    umesh.groupgroups = NULL;
    // selector on mesh
    umesh.nb_som_tables = 0;
    umesh.som_tables = NULL;

    // Test on simple unstructured mesh
    file_id = AH5_auto_test_file();

    // Write the mesh into '/mesh' node.
    loc_id = H5Gcreate(file_id, "/mesh", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    mu_assert("Write an simple umesh.",
              AH5_write_umesh(loc_id, &umesh));

    // Check if all the mesh parts are written.
    mu_assert("Check 'nodes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/nodes"));
    mu_assert("Check 'elementNodes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementNodes"));
    mu_assert("Check 'elementTypes' nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementTypes"));
    mu_assert("Check 'groupGroup' nodes in file.",
              AH5_path_valid(file_id, "/mesh/groupGroup"));
    mu_assert("Check 'group' nodes in file.",
              AH5_path_valid(file_id, "/mesh/group"));
    mu_assert("Check 'group/name' nodes in file.",
              AH5_path_valid(file_id, "/mesh/group/name"));


    // Close and release
    AH5_free_umesh(&umesh);
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


//! Check the mesh write function.
char *test_write_mesh()
{
    AH5_mesh_t mesh;
    hid_t file_id;
    // Add mesh group.
    mesh.nb_groups = 1;
    mesh.groups = (AH5_msh_group_t*) malloc(mesh.nb_groups * sizeof(AH5_msh_group_t));
    mesh.groups[0].path = new_string("mesh_group_name");
    mesh.groups[0].nb_mlk_instances = 0;
    mesh.groups[0].nb_msh_instances = 1;
    // Add unstructured mesh instance.
    mesh.groups[0].msh_instances = (AH5_msh_instance_t*) malloc(mesh.groups[0].nb_msh_instances * sizeof(AH5_msh_instance_t));
    mesh.groups[0].msh_instances[0].path = new_string("mesh_name");
    mesh.groups[0].msh_instances[0].type = MSH_UNSTRUCTURED;
    // Filling the mesh data.
    build_umesh_1(&(mesh.groups[0].msh_instances[0].data.unstructured));

    // Write mesh category.
    file_id = AH5_auto_test_file();
    mu_assert("Write mesh [1].",
               AH5_write_mesh(file_id, &mesh));

    // Release resource.
    free(mesh.groups[0].path);
    free(mesh.groups[0].msh_instances[0].path);
    free(mesh.groups);

    // Read data (build data with full path) and rename a node to build a copy.
    mu_assert("Read mesh [1].",
              AH5_read_mesh(file_id, &mesh));
    free(mesh.groups[0].path);
    mesh.groups[0].path = new_string("mesh_group_name_copy");
    mu_assert("Write mesh [2].",
               AH5_write_mesh(file_id, &mesh));

    // Clean and release resource.
    AH5_free_mesh(&mesh);
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


//! Test read unstructured mesh function.
char *test_read_umesh()
{
    AH5_msh_instance_t mesh;
    AH5_umesh_t umesh;
    hid_t file_id, loc_id;

    // Write a simple mesh test.
    file_id = AH5_auto_test_file();
    build_umesh_1(&umesh);
    loc_id = H5Gcreate(file_id, "/mesh", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    AH5_write_umesh(loc_id, &umesh);
    AH5_free_umesh(&umesh);

    // Now read the mesh
    mu_assert("The written mesh is readable.",
              AH5_read_msh_instance(file_id, "/mesh", &mesh));

    // Check some value
    mu_assert("Check mesh type",
              mesh.type == MSH_UNSTRUCTURED);
    mu_assert("Check nodes size",
              mesh.data.unstructured.nb_nodes[0] == 5);
    mu_assert("Check nodes size",
              mesh.data.unstructured.nb_nodes[1] == 3);

    // Check node path values.
    mu_assert_str_equal("Check the mesh path.",
                        mesh.path, "/mesh");

    mu_assert_str_equal("Check group name.",
                        mesh.data.unstructured.groups[0].path,
                        "/mesh/group/name");

    // Close and clean
    AH5_close_test_file(file_id);
    AH5_free_msh_instance(&mesh);

    return MU_FINISHED_WITHOUT_ERRORS;
}


// Run all tests
char *all_tests()
{
    //FIXME: mu_run_test(test_write_groupgroup);
    mu_run_test(test_write_unstructured_mesh_group);
    mu_run_test(test_write_umesh);
    mu_run_test(test_write_unstructured_nodes_mesh);
    mu_run_test(test_read_umesh);
    mu_run_test(test_write_mesh);

    return MU_FINISHED_WITHOUT_ERRORS;
}

// Main function, run tests and print results.
int main(int argc, char **argv)
{
    char *result;
    tests_run = 0;
    result = all_tests();

    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
