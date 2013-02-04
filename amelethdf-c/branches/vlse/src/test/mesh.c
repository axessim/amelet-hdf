// test path tools

#include <string.h>
#include <stdio.h>

#include "utest.h"
#include "ah5.h"

#define MESHFILE "test_mesh.h5"

// the unstructured element id 
typedef enum _AH5_uelement_type_t
{
    UELE_INVALID            = -1,
    UELE_BAR2               = 1,
    UELE_BAR3               = 2,
    UELE_TRI3               = 11,
    UELE_TRI6               = 12,
    UELE_QUAD4              = 13,
    UELE_QUAD8              = 14,
    UELE_TETRA4             = 101,
    UELE_PYRA5              = 102,
    UELE_PENTA6             = 103,
    UELE_HEXA8              = 104,
    UELE_TETRA10            = 108,
    UELE_HEXA20             = 109,
} AH5_uelement_type_t;


// build a simple unstructured mesh two tetra,
void build_umesh_1(AH5_umesh_t *umesh)  {
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
    umesh->groups = (AH5_ugroup_t*)malloc(umesh->nb_groups*sizeof(AH5_ugroup_t));
    umesh->groups[0].path = (char*)malloc(5*sizeof(char));
    strcpy(umesh->groups[0].path, "name");
    umesh->groups[0].type = (char*)malloc(8*sizeof(char));
    strcpy(umesh->groups[0].type, "element");
    umesh->groups[0].entitytype = (char*)malloc(5*sizeof(char));
    strcpy(umesh->groups[0].entitytype, "face");
    umesh->groups[0].nb_groupelts = 1;
    umesh->groups[0].groupelts = (int*)malloc(umesh->groups[0].nb_groupelts*sizeof(int));
    umesh->groups[0].groupelts[0] = 2;
    // group of group 
    umesh->nb_groupgroups = 0;
    umesh->groupgroups = NULL;
    // selector on mesh
    umesh->nb_som_tables = 0;
    umesh->som_tables = NULL;
}


int tests_run = 0;

static char* test_write_umesh() {
    AH5_umesh_t umesh;
    hid_t file_id, mesh_node;

    
    // Bad used
    mu_assert("Can not write an empty umesh!",
        !AH5_write_umesh(file_id, "/", NULL));

    // Test on simple unstructured mesh
    file_id = AH5_create_test_file(MESHFILE);
    build_umesh_1(&umesh);
    
    // write at root
    mu_assert("Write an simple umesh.",
              AH5_write_umesh(file_id, "/mesh", &umesh));
    AH5_free_umesh(&umesh);

    // Check if all the mesh parts are written.
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/nodes"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementNodes"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/elementTypes"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/groupGroup"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/group"));
    mu_assert("Check nodes in file.",
              AH5_path_valid(file_id, "/mesh/group/name"));
    
    // Now read the mesh
    mu_assert("The written mesh is readable.",
              AH5_read_umesh(file_id, "mesh", &umesh));
  
    // Check some value
    mu_assert("check nodes size",
              umesh.nb_nodes[0] == 5);
    mu_assert("check nodes size",
              umesh.nb_nodes[1] == 3);
  
    // Close and clean
    AH5_free_umesh(&umesh);
    AH5_close_test_file(file_id);

    return NULL;
}

static char * all_tests() {
    mu_run_test(test_write_umesh);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
  
    return result != 0;
}


