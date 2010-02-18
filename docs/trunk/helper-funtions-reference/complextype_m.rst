.. _complextype:

``complextype``
===============

``write_complex_attribute``
---------------------------

``(=> write_attribute)``

.. code-block:: fortran

    ! Writes a complex attribute at path
    subroutine write_attribute(file_id, path, name, value)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), intent(in) :: name
        complex, intent(in) :: value


``read_complex_attribute``
--------------------------

``(=> read_attribute)``

.. code-block:: fortran

    ! Read a complex attribute at path
    function read_attribute(file_id, path, name, value, mandatory) result(here)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), intent(in) :: name
        complex, intent(out) :: value
        logical, intent(in), optional :: mandatory



``read_1d_complex_dataset``
---------------------------

``(=> read_dataset)``

.. code-block:: fortran

    ! Read a 1d complex dataset
    subroutine read_dataset(file_id, path, values)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        complex, dimension(:), allocatable, intent(out) :: values


``write_complex_dataset``
-------------------------

``(=> write_nd_dataset)``

.. code-block:: fortran

    ! Write a nD complex dataset
    subroutine write_nd_dataset(file_id, path, values, values_shape)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        integer, dimension(:), intent(in) :: values_shape
        complex, dimension(product(values_shape)), intent(in) :: values

