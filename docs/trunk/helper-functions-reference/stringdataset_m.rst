.. _stringdataset:

``stringdataset``
=================

``get_dataset_lmn``
-------------------

Return dimensional information about a string 2D dataset :

#. ``l`` : ``l`` is the string length
#. ``m`` : ``m`` is the first dimension size
#. ``n`` : ``n`` is the second dimension size


* Fortran interface :

    .. code-block:: fortran

        ! Read the length of strings contains in a dataset
        ! The dataset must be a string dataset (no check)
        function get_dataset_lmn(file_id, path)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            integer, dimension(3) :: get_dataset_lmn

* C interface :

    .. code-block:: c
    
        int *get_dataset_lmn(hid_t file_id, char* path)


``read_string_dataset1``
------------------------

This function reads a (m x n) string dataset (rows x columns) of l characters

* Fortran interface :

    .. code-block:: fortran

        ! Read a (m x n) string dataset (rows x columns) of l characters
        subroutine read_string_dataset1(file_id, path, data, l, m, n)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            integer, intent(in) :: l, m, n
            character(len=*), dimension(m, n), intent(inout) :: data

* C interface :

    .. code-block:: c
    
        char *** read_string_dataset1(hid_t file_id, const char * path,
                                      int l, int m, int n)

                                      
``read_string_vector``
----------------------

This function reads a 1D string vector

* Fortran interface :

    ``(=> read_vector)``
     
    .. code-block:: fortran

        ! Read a 1D string vector
        subroutine read_vector(file_id, path, vector)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            character(len=*), dimension(:), allocatable :: vector
 

* C interface :

    .. code-block:: c

        children_t read_string_vector(hid_t file_id, char* path)

 
``write_string_dataset``
------------------------

This function writes a multidimensional string dataset

* Fortran interface :

    ``(=> write_nd_dataset)``

    .. code-block:: fortran

        ! Write a nd string dataset
        subroutine write_nd_dataset(file_id, path, values, values_shape)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            integer, dimension(:), intent(in) :: values_shape
            character(len=*), dimension(product(values_shape)), intent(in) :: values

* C interface :

    .. code-block:: c

        void write_string_dataset(hid_t file_id, char* path, int values_len, int rank,
                                  hsize_t* dims, char** values)
        // values_len : string length

