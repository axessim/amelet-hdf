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

Read a ``vector_t`` at path.

* Fortran interface :

    ``(=> read)``

    .. code-block:: fortran

        ! read a vector_t floating type
        subroutine read(file_id, path, vector)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(vector_t), intent(inout) :: vector

* C interface :

    .. code-block:: c
    
        vector_t read_vector(hid_t loc_id, const char* path)
        

``vector_clear_content``
------------------------

Clear the content of a ``vector_t``

* Fortran interface :

    ``(=> clear_content)``

    .. code-block:: fortran

        ! clear vector
        subroutine clear_content(vector)
            type(vector_t), intent(inout) :: vector

* C interface :

    .. code-block:: c
    
        vector_t vector_clear_content(vector_t vec)


