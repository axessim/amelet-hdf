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

``(=> read)``

.. code-block:: fortran

    ! Reads a floatingtype_t structure
    subroutine read(file_id, path, ft)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(floatingtype_t), intent(inout) :: ft


``issinglereal``
----------------

.. code-block:: fortran

    ! Returns.true. if the input is a float number
    logical function issinglereal(ft)
        type(floatingtype_t), intent(in) :: ft


``isvector``
------------

.. code-block:: fortran

    ! Returns .true. if the input is a vector
    logical function isvector(ft)
        type(floatingtype_t), intent(in) :: ft


``convert_to_real_vector``
--------------------------

.. code-block:: fortran

    ! Convert a floatingType to a real vector if possible
    function convert_to_real_vector(ft) result(vector)
        type(floatingtype_t), intent(in) :: ft


``set_floating_type``
---------------------

.. code-block:: fortran

    ! Create in one go all attributes of a floatingType
    subroutine set_floating_type(file_id, path, label, &
                                 physical_nature, unit, comment)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), intent(in) :: label, physical_nature, unit, comment

