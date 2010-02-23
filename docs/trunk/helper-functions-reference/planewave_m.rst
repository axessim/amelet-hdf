.. _planewave:

``planewave``
=============

Plane wave structure definition and management.

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

Read a ``planewave_t`` object at path.

* Fortran interface :

    ``(=> read)``

    .. code-block:: fortran

        ! read a planewave
        subroutine read(file_id, path, pw)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(planewave_t), intent(inout) :: pw

* C interface :

    .. code-block:: c
    
        planewave_t read_planewave(hid_t file_id, const char* path)
        

``islinear``
------------

Return true if planewave polarization is linear

* Fortran interface :

    .. code-block:: fortran

        ! Returns .true. if planewave polirazation is LINEAR
        logical function islinear(planewave)
            type(planewave_t), intent(in) :: planewave

* C interface :

    .. code-block:: c

        int islinear(planewave_t pw)
        

``iselliptic``
--------------

Return true if planewave polarization is elliptic

* Fortran interface :

    .. code-block:: fortran

        ! Returns .true. if planewave polarization is ELLIPTIC
        logical function iselliptic(planewave)
            type(planewave_t), intent(in) :: planewave

* C interface :

    .. code-block:: c
    
        int iselliptic(planewave_t pw)
        
