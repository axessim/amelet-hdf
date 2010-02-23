.. _floatingtype:

``floatingType``
================

Types
-----

( see :ref:`simpletype` and :ref:`arrayset` ).

.. code-block:: fortran

    type floatingtype_t
        integer :: floatingtype
        type(singlereal_t) :: singlereal
        type(singlecomplex_t) :: singlecomplex
        type(vector_t) :: vector
        type(dataset_t) :: dataset
        type(arrayset_t) :: arrayset
    end type floatingtype_t


``read_floatingtype``
---------------------

Read a ``floatingtype_t`` at path.

* Fortran interface :

    ``(=> read)``

    .. code-block:: fortran

        ! Reads a floatingtype_t structure
        subroutine read(file_id, path, ft)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            type(floatingtype_t), intent(inout) :: ft

* C interface :

    .. code-block:: c
    
        floatingtype_t read_floatingtype(hid_t loc_id, const char *path)


``issinglereal``
----------------

Return ``true`` if ``ft`` is a ``singlereal_t``

* Fortran interface :

    .. code-block:: fortran

        ! Returns.true. if the input is a float number
        logical function issinglereal(ft)
            type(floatingtype_t), intent(in) :: ft

* C interface :

    .. code-block:: c
    
        int issinglereal(floatingtype_t ft)
        

``isvector``
------------

Return ``true`` if ``ft`` is a ``vector_t``

* Fortran interface :

    .. code-block:: fortran

        ! Returns .true. if the input is a vector
        logical function isvector(ft)
            type(floatingtype_t), intent(in) :: ft

* C interface :

    .. code-block:: c
    
        int isvector(floatingtype_t ft)


``convert_to_real_vector``
--------------------------

Convert a ``floatingtype_t`` to a real array if posible.

* Fortran interface :

    .. code-block:: fortran

        ! Convert a floatingType to a real vector if possible
        function convert_to_real_vector(ft) result(vector)
            type(floatingtype_t), intent(in) :: ft

    
        

``set_floating_type``
---------------------

Set in one go attributes of a ``floatingtype_t``

* Fortran interface :

    .. code-block:: fortran

        ! Create in one go all attributes of a floatingType
        subroutine set_floating_type(file_id, path, label, &
                                     physical_nature, unit, comment)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            character(len=*), intent(in) :: label, physical_nature, unit, comment


