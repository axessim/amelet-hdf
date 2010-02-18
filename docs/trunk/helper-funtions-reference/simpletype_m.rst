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

.. code-block:: fortran

    ! read a singlereal_t floating type
    subroutine read_singlereal(file_id, path, singlereal)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(singlereal_t), intent(out) :: singlereal



``singlereal_to_string``
------------------------

.. code-block:: fortran

    ! return a string representation of singlereal_t
    character(len=S_SINGLE_LENGTH) function singlereal_to_string(singlereal)
        type(singlereal_t), intent(inout) :: singlereal



``read_singlecomplex``
----------------------

.. code-block:: fortran

    ! read a singlecomplex_t floating type
    subroutine read_singlecomplex(file_id, path, singlecomplex)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(singlecomplex_t), intent(out) :: singlecomplex


``singlecomplex_to_string``
---------------------------

.. code-block:: fortran

    ! return a string representation of singlecomplex_t
    character(len=S_SINGLE_LENGTH) function singlecomplex_to_string(singlecomplex)
        type(singlecomplex_t), intent(inout) :: singlecomplex

