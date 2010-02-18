.. _arrayset:

``arrayset``
============

Types
-----

( see :ref:`simpletype` ``single_t`` definition ).

.. code-block:: fortran

    type arrayset_t
        type(single_t) :: single
        type(dataset_t) :: data
        type(vector_t), dimension(:), allocatable :: dims
    end type arrayset_t


``read_arrayset``
-----------------

``(=> read)``

.. code-block:: fortran

    ! reads an arrayset_t
    subroutine read(file_id, path, arr)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(arrayset_t), intent(inout) :: arr


``arrayset_to_string``
----------------------

``(=> to_string2)``

.. code-block:: fortran

    ! return a string representation of arrayset2_t
    character(len=AL) function to_string2(arr)
        type(arrayset2_t), intent(in) :: arr

