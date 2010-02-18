.. _stringdataset:

``stringdataset``
=================

``get_dataset_lmn``
-------------------


.. code-block:: fortran

    ! Reads the length of strings contains in a dataset
    ! The dataset must be a string dataset (no check)
    function get_dataset_lmn(file_id, path)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        integer, dimension(3) :: get_dataset_lmn


``read_string_dataset1``
------------------------

.. code-block:: fortran

    ! Reads a (m x n) string dataset (rows x columns) of l characters
    subroutine read_string_dataset1(file_id, path, data, l, m, n)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        integer, intent(in) :: l, m, n
        character(len=*), dimension(m, n), intent(inout) :: data


``read_string_vector``
----------------------

``(=> read_vector)``
 
.. code-block:: fortran

    ! Read a 1D string vector
    subroutine read_vector(file_id, path, vector)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), dimension(:), allocatable :: vector
 
 
``write_string_dataset``
------------------------

``(=> write_nd_dataset)``

.. code-block:: fortran

    ! Write a nd string dataset
    subroutine write_nd_dataset(file_id, path, values, values_shape)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        integer, dimension(:), intent(in) :: values_shape
        character(len=*), dimension(product(values_shape)), intent(in) :: values

