.. _amelethdf:

``amelethdf``
=============


Contants
--------

``EL`` is the maximum length of the relative name of an element
(``EL`` = ``ELEMENT_NAME_LENGTH``)

``AL`` is the maximum length of the absolute name of an element
(``AL`` = ``ABSOLUTE_PATH_NAME_LENGTH``)


Types
-----

    .. code-block:: c

        typedef struct
        {
            char ** childnames;
            int nbchild;
        } children_t;


``hdferr`` & ``check``
----------------------

``hdferr`` is the error code of HDF5 functions.

``check`` checks the ``hdferr`` and stops the program if ``must_stop`` is true.

* Fortran interface :
    .. code-block:: fortran

        subroutine check(message, must_stop)
            ! the printed message
            character(len=*) :: message
            logical, intent(in), optional :: must_stop


``read_children_name``
----------------------

``read_children_name`` returns in  ``children`` the name of a group's children.

* Fortran interface :
    .. code-block:: fortran

            ! Read the children's name of a group
            subroutine read_children_name(file_id, path, children)
                integer(hid_t), intent(in) :: file_id
                character(len=*), intent(in) :: path
                character(len=ELEMENT_NAME_LENGTH), &
                    dimension(:), allocatable :: children

* C interface :
    .. code-block:: c

        children_t read_children_name(hid_t loc_id, const char* path);


``read_string_attribute``
-------------------------

``read_string_atttribute`` reads a string attribute. If ``mandatory`` is true
the program stops if the attribute is not found out. The function returns true
if the attribute is found out, false otherwise.

* Fortran interface :
    .. code-block:: fortran

        ! Reads a string attribute, it can be mandatory
        function read_string_attribute(file_id, path, attr, buf, mandatory) result(here)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            ! the name of the attribute
            character(len=*), intent(in) :: attr
            ! the returned value
            character(len=*), intent(inout) :: buf
            logical, intent(in), optional :: mandatory
            logical :: here

* C interface :
    .. code-block:: c

        char * read_string_attribute(hid_t loc_id, const char* path, char* attr);


``read_float_attribute``
------------------------

``read_float_atttribute`` reads a float attribute. If ``mandatory`` is true
the program stops if the attribute is not found out. The function returns true
if the attribute is found out, false otherwise.

* Fortran interface :
    .. code-block:: fortran

        ! Reads a float attribute, it can be mandatory
        function read_float_attribute(file_id, path, attr, buf, mandatory) result(here)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            ! the name of the attribute
            character(len=*), intent(in) :: attr
            ! the value of the attribute
            real, intent(inout) :: buf
            logical, intent(in), optional :: mandatory
            logical :: here


* C interface :
    .. code-block:: c

        float read_float_attribute(hid_t loc_id, const char* path, char* attr);


``read_int_attribute``
----------------------

``read_int_atttribute`` reads an int attribute. If ``mandatory`` is true
the program stops if the attribute is not found out. The function returns true
if the attribute is found out, false otherwise.

.. code-block:: fortran

    ! Reads an integer attribute, it can be mandatory
    function read_int_attribute(file_id, path, attr, buf, mandatory) result(here)
        integer(hid_t), intent(in) :: file_id
        ! the name of the attribute
        character(len=*), intent(in) :: path
        ! the value of the attribute
        character(len=*), intent(in) :: attr
        integer, intent(inout) :: buf
        logical, intent(in), optional :: mandatory
        logical :: here

* C interface :
    .. code-block:: c

        float read_float_attribute(hid_t loc_id, const char* path, char* attr);

