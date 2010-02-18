.. _vector:

``vector``
==========


Types
-----

( see :ref:`simpletype` ``single_t`` definition ).

.. code-block:: fortran

    type vector_t
        type(single_t) :: single
        integer, dimension(:), allocatable :: ivalue
        real, dimension(:), allocatable :: rvalue
        complex, dimension(:), allocatable :: cvalue
    end type vector_t


``read_vector``
---------------

``(=> read)``

.. code-block:: fortran

    ! read a vector_t floating type
    subroutine read(file_id, path, vector)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(vector_t), intent(inout) :: vector



``vector_clear_content``
------------------------

``(=> clear_content)``

.. code-block:: fortran

    ! clear vector
    subroutine clear_content(vector)
        type(vector_t), intent(inout) :: vector

