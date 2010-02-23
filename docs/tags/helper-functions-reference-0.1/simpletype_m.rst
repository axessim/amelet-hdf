.. _simpletype:

``simpletype``
==============

Types
-----

.. code-block:: fortran

    ! Base type, common with all floating types
    type single_t
        character(len=EL) :: label = ""
        character(len=EL) :: physical_nature = ""
        character(len=EL) :: unit = ""
        character(len=EL) :: comment = ""
    end type single_t

    type singlereal_t
        type(single_t) :: single
        real :: value
    end type singlereal_t

    type singlecomplex_t
        type(single_t) :: single
        complex :: value
    end type singlecomplex_t

    type dataset_t
        type(single_t) :: single
        integer, dimension(:), allocatable :: ivalue
        real, dimension(:), allocatable :: rvalue
        complex, dimension(:), allocatable :: cvalue
    end type dataset_t

    type linearlistofreal1_t
        real :: first
        real :: last
        integer :: number_of_values
    end type linearlistofreal1_t

    type linearlistofreal2_t
        real :: first
        real :: step
        integer :: number_of_values
    end type linearlistofreal2_t

    type logarithmlistofreal_t
        real :: first
        real :: last
        integer :: number_of_values
    end type logarithmlistofreal_t

    type perdecadelistofreal_t
        real :: first
        integer :: number_of_decades
        integer :: number_of_values_per_decade
    end type perdecadelistofreal_t

    type linearlistofinteger2_t
        integer :: first
        integer :: step
        integer :: number_of_values
    end type linearlistofinteger2_t



``read_singlereal``
-------------------

Read a ``singlereal_t`` at path.

* Fortran interface :

    .. code-block:: fortran

        ! read a singlereal_t floating type
        subroutine read_singlereal(file_id, path, singlereal)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(singlereal_t), intent(out) :: singlereal

* C interface :

    .. code-block:: c
    
        singlereal_t read_single_real(hid_t loc_id, const char *path)



``singlereal_to_string``
------------------------

Return a ``singlereal_t`` as a string.

* Fortran interface :

    .. code-block:: fortran

        ! return a string representation of singlereal_t
        character(len=S_SINGLE_LENGTH) function singlereal_to_string(singlereal)
            type(singlereal_t), intent(inout) :: singlereal


* C interface :

    .. code-block:: c
    
        char* singlereal_to_string(singlereal_t singlereal)


``read_singlecomplex``
----------------------

Read a ``singlecomplex_t`` at path.

* Fortran interface :

    .. code-block:: fortran

        ! read a singlecomplex_t floating type
        subroutine read_singlecomplex(file_id, path, singlecomplex)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(singlecomplex_t), intent(out) :: singlecomplex

* C interface :

    .. code-block:: c
    
        singlecomplex_t read_single_complex(hid_t loc_id, const char *path)


``singlecomplex_to_string``
---------------------------

Return a ``singlecomplex_t`` as a string.

* Fortran interface :

    .. code-block:: fortran

        ! return a string representation of singlecomplex_t
        character(len=S_SINGLE_LENGTH) function singlecomplex_to_string(singlecomplex)
            type(singlecomplex_t), intent(inout) :: singlecomplex

* C interface :

    .. code-block:: c
    
        char* singlecomplex_to_string(singlecomplex_t singlecomplex)


