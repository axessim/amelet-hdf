.. _linkoutputrequest:

``linkoutputrequest``
=====================

Constants
---------

.. code-block:: fortran

    C_LINK
    C_OUTPUT_REQUEST


Types
-----

.. code-block:: fortran

    type simulation_t
        character(len=AL) :: name = ""
        character(len=AL), dimension(:), allocatable :: inputs, outputs
    end type simulation_t


``read_link``
-------------

``(=> read)``

.. code-block:: fortran

    ! Reads a (subject/object) of link/outputRequest
    subroutine read(loc_id, path, link)
        integer(hid_t), intent(in) :: loc_id
        character(len=*), intent(in) :: path
        type(link_t), intent(inout) :: link


``isdataonmesh``
----------------

.. code-block:: fortran

    ! Returns .true. if link is a DATA_ON_MESH
    logical function isdataonmesh(link)
        type(link_t), intent(in) :: link



