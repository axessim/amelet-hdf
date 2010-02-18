.. _mesh:

``mesh`` namespace
====================


Constants
---------

``C_MESH``


``isstructured``
----------------

.. code-block:: fortran

    ! Returns true is path is a structured mesh
    function isstructured(file_id, path)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        logical :: isStructured




