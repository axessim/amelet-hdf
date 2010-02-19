.. _unstructuredmesh:

``unstructuredmesh``
====================

Types
-----

``(umesh_group_t => group_t)``

.. code-block:: fortran

    type selector_on_mesh_node_t
        character(len=EL), dimension(:), allocatable :: short_name
        integer, dimension(:), allocatable :: index
    end type selector_on_mesh_node_t

    type selector_on_mesh_element_t
        character(len=EL), dimension(:), allocatable  :: short_name
        integer, dimension(:), allocatable  :: index
        real, dimension(:), allocatable  :: v1, v2, v3
    end type selector_on_mesh_element_t

    type group_t
        character(len=AL) :: name = ""
        character(len=EL) :: type = ""
        character(len=EL) :: entity_type = ""
        integer, dimension(:), allocatable :: elements
    end type group_t

    type unstructured_mesh_t
        character(len=AL) :: name = ""
        real, dimension(:,:), allocatable :: nodes
        integer, dimension(:), allocatable :: elements
        integer, dimension(:), allocatable :: offsets
        integer, dimension(:), allocatable :: element_nodes
        type(group_t), dimension(:), allocatable :: groups
        type(groupgroup_t), dimension(:), allocatable :: groupgroups
        type(selector_on_mesh_node_t) :: som_node
        type(selector_on_mesh_element_t) :: som_element
    end type unstructured_mesh_t




``umesh_read``
--------------

Read an ``unstructuredmesh_t`` at path.

* Fortran interface :

    ``(=> read)``

    .. code-block:: fortran

        ! Reads an unstructured mesh
        subroutine read(file_id, mesh_path, umesh)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: mesh_path
            type(unstructured_mesh_t), intent(inout) :: umesh



``umesh_print``
---------------

Print an ``unstructuredmesh_t``

* Fortran interface :

    ``(=> printt)``

    .. code-block:: fortran

        ! Prints an unstructured mesh to the console
        subroutine printt(umesh)
            type(unstructured_mesh_t), intent(in) :: umesh




``umesh_generate_node_numbers``
-------------------------------

Return an integer array containing the number of nodes for element_index
elements.

* Fortran interface :

    ``(=> generate_node_numbers)``

    .. code-block:: fortran

        ! Return a number of nodes array for the element in umesh
        ! inferior or equal than element_index
        subroutine generate_node_numbers(umesh, element_index, node_numbers)
            type(unstructured_mesh_t), intent(in) :: umesh
            integer, intent(in) :: element_index
                integer, dimension(size(umesh%elements)), intent(out) :: node_numbers




``umesh_number_of_nodes``
-------------------------

Return the number of nodes given an element type.

* Fortran interface :

    ``(=> number_of_nodes)``

    .. code-block:: fortran

        ! Return the number of nodes for a given element type
        elemental function number_of_nodes(element_type)
            integer, intent(in) :: element_type
            integer :: number_of_nodes



``umesh_get_group_by_name``
---------------------------

Return a ``group_t`` reference from its path

* Fortran interface :

    ``(=> get_group_by_name)``

    .. code-block:: fortran

        ! Look for a group in mesh with a given name
        function get_group_by_name(umesh, path) result(group)
            type(unstructured_mesh_t), target, intent(in) :: umesh
            character(len=*), intent(in) :: path
            type(group_t), pointer :: group




``umesh_get_index_by_short_name_in_some``
-----------------------------------------

Return the index of an element (in ``elementType``) from its name in a
``selector_on_mesh_element_t``.

* Fortran interface :

    ``(=> get_index_by_short_name_in_some)``

    .. code-block:: fortran

        ! Given a selector_on_mesh_element_t (some) object, return the index
        ! matching a short_name
        function get_index_by_short_name_in_some(some, short_name) result(ind)
            type(selector_on_mesh_element_t), intent(in) :: some
            character(len=*), intent(in) :: short_name


``umesh_generate_offsets``
--------------------------

Generate ``unstructuredmesh_t%offsets`` field.

* Fortran interface :

    ``(=> generate_offsets)``

    .. code-block:: fortran

        ! Generates the offsets field of an unstructured mesh
        subroutine generate_offsets(umesh)
            type(unstructured_mesh_t), intent(inout) :: umesh


