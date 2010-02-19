``amelet`` interface module
==============================

+---------------------------+------------------------------------------------------+
| Namespaces                | Elements                                             |
+===========================+======================================================+
| :ref:`amelethdf`          |::                                                    |
|                           |                                                      |
|                           |     check, hdferr                                    |
|                           |     EL                                               | 
|                           |     AL                                               |
|                           |     read_children_name                               |
|                           |     read_string_attribute                            |
|                           |     read_float_attribute                             |
|                           |     read_int_attribute                               |
+---------------------------+------------------------------------------------------+
| :ref:`hdfpath`            |::                                                    |
|                           |                                                      |
|                           |    basename                                          |
|                           |    dirname                                           |
|                           |    exists                                            |
|                           |    isleaf                                            |
|                           |    isgroup                                           |
|                           |    like                                              |
|                           |    element                                           |
|                           |    join                                              |
+---------------------------+------------------------------------------------------+
| :ref:`stringdataset`      |::                                                    |
|                           |                                                      |
|                           |    get_dataset_lmn                                   |
|                           |    read_string_dataset1                              |
|                           |    read_string_vector                                |
|                           |    write_string_dataset                              |
+---------------------------+------------------------------------------------------+

+---------------------------+------------------------------------------------------+
| category                  |::                                                    |
|                           |                                                      |
|                           |    C_ELECTROMAGNETIC_SOURCE                          |
|                           |    C_GLOBAL_ENVIRONMENT                              |
|                           |    C_LABEL                                           |
+---------------------------+------------------------------------------------------+
| :ref:`complextype`        |::                                                    |
|                           |                                                      |
|                           |    create_attribute                                  |
|                           |    read_cattribute                                   |
|                           |    write_complex_type                                |
|                           |    read_attribute                                    |
|                           |    write_complex_dataset                             |
+---------------------------+------------------------------------------------------+
| :ref:`simpletype`         |::                                                    |
|                           |                                                      |
|                           |    singlereal_t                                      |
|                           |    read_singlereal                                   |
|                           |    singlereal_to_string                              |
|                           |    singlecomplex_t                                   |
|                           |    read_singlecomplex                                |
|                           |    singlecomplex_to_string                           |
+---------------------------+------------------------------------------------------+

+---------------------------+------------------------------------------------------+
| :ref:`vector`             |::                                                    |
|                           |                                                      |
|                           |    vector_t                                          |
|                           |    read_vector                                       |
|                           |    vector_clear_content                              |
+---------------------------+------------------------------------------------------+
| :ref:`arrayset`           |::                                                    |
|                           |                                                      |
|                           |    arrayset_t                                        |
|                           |    read_arrayset                                     |
|                           |    arrayset_to_string                                |
+---------------------------+------------------------------------------------------+
| :ref:`floatingtype`       |::                                                    |
|                           |                                                      |
|                           |    floatingtype_t                                    |
|                           |    read_floatingtype                                 |
|                           |    issinglereal                                      |
|                           |    isvector                                          |
|                           |    convert_to_real_vector                            |
|                           |    set_floating_type                                 |
+---------------------------+------------------------------------------------------+

+---------------------------+------------------------------------------------------+
| :ref:`mesh`               |::                                                    |
|                           |                                                      |
|                           |    C_MESH, mesh_names                                |
|                           |    isStructured                                      |
+---------------------------+------------------------------------------------------+
| :ref:`unstructuredmesh`   |::                                                    |
|                           |                                                      |
|                           |    unstructured_mesh_t                               |
|                           |    umesh_print                                       |
|                           |    umesh_read                                        |
|                           |    umesh_group_t                                     |
|                           |    umesh_generate_node_numbers                       |
|                           |    umesh_number_of_nodes                             |
|                           |    umesh_get_group_by_name                           |
|                           |    umesh_get_index_by_short_name_in_some             |
|                           |    umesh_generate_offsets                            |
+---------------------------+------------------------------------------------------+
| :ref:`structuredmesh`     |::                                                    |
|                           |                                                      |
|                           |    smesh_read                                        |
|                           |    smesh_print                                       |
|                           |    structured_mesh_t                                 |
+---------------------------+------------------------------------------------------+
| :ref:`simulation`         |::                                                    |
|                           |                                                      |
|                           |    simulation_t                                      |
|                           |    read_simulation                                   |
|                           |    print_simulation                                  |
|                           |    C_SIMULATION                                      |
+---------------------------+------------------------------------------------------+

+---------------------------+------------------------------------------------------+
| physicalmodel             |::                                                    |
|                           |                                                      |
|                           |    C_PHYSICAL_MODEL                                  |
+---------------------------+------------------------------------------------------+
| :ref:`planewave`          |::                                                    |
|                           |                                                      |
|                           |    planewave_t                                       |
|                           |    read_planewave                                    |
|                           |    islinear                                          |
|                           |    iselliptic                                        |
|                           |    C_PLANE_WAVE                                      |
+---------------------------+------------------------------------------------------+
| :ref:`linkoutputrequest`  |::                                                    |
|                           |                                                      |
|                           |    link_t                                            |
|                           |    read_link                                         | 
|                           |    isdataonmesh                                      |
|                           |    C_LINK                                            |
|                           |    C_OUTPUT_REQUEST                                  |
+---------------------------+------------------------------------------------------+

