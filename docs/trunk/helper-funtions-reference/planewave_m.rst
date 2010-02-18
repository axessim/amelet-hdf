.. _planewave:

``planewave``
=============

Constants
---------

.. code-block:: fortran

    C_PLANE_WAVE

Types
-----

.. code-block:: fortran

    type planewave_t
        real :: theta, phi, linear_polarization
        integer :: polarization
        complex :: elliptical_Polarization_etheta, elliptical_polarization_ephi
        type(floatingtype_t) :: magnitude
    end type planewave_t



``read_planewave``
------------------

``(=> read)``

.. code-block:: fortran

    ! read a planewave
    subroutine read(file_id, path, pw)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(planewave_t), intent(inout) :: pw


``islinear``
------------

.. code-block:: fortran

    ! Returns .true. if planewave polirazation is LINEAR
    logical function islinear(planewave)
        type(planewave_t), intent(in) :: planewave


``iselliptic``
--------------


.. code-block:: fortran

    ! Returns .true. if planewave polarization is ELLIPTIC
    logical function iselliptic(planewave)
        type(planewave_t), intent(in) :: planewave

