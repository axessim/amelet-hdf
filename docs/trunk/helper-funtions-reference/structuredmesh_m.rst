.. _structuredmesh:

``structuredmesh``
==================

Types
-----

.. code-block:: fortran

    type group_t
        character(len=AL) :: name = ""
        character(len=EL) :: type = ""
        character(len=EL) :: entityType = ""
        integer, dimension(:, :), allocatable :: elements
    end type group_t

    type structured_mesh_t
        character(len=AL) :: name
        real, dimension(:), allocatable :: x, y, z
        type(group_t), dimension(:), allocatable :: groups
        type(groupgroup_t), dimension(:), allocatable :: groupgroups
    end type structured_mesh_t



``smesh_read``
--------------

``(=> read)``

.. code-block:: fortran

    ! Reads a structured mesh and store the result in smesh
    subroutine read(file_id, mesh_path, smesh)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: mesh_path
        type(structured_mesh_t), intent(inout) :: smesh


``smesh_print``
---------------

``(=> printt)``

.. code-block:: fortran

    ! Print to the console smesh characteristics
    subroutine printt(smesh)
        type(structured_mesh_t), intent(in) :: smesh

