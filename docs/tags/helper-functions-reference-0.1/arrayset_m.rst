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

Read an ``arrayset_t`` at path.

* Fortran interface :

    ``(=> read)``

    .. code-block:: fortran

        ! reads an arrayset_t
        subroutine read(file_id, path, arr)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(arrayset_t), intent(inout) :: arr

* C interface :

    .. code-block:: c
    
        arrayset_t read_arrayset(hid_t file_id, const char* path)


``arrayset_to_string``
----------------------

Return an ``arrayset_t`` as a string.

* Fortran interface :

    ``(=> to_string2)``

    .. code-block:: fortran

        ! return a string representation of arrayset2_t
        character(len=AL) function to_string2(arr)
            type(arrayset2_t), intent(in) :: arr

* C interface :

    .. code-block:: c
    
        arrayset2_t read_arrayset2(hid_t loc_id, const char* path)
        
