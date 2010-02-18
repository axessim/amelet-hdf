.. _hdfpath:

``hdfpath``
===========

``basename``
------------

Return the base name of pathname path.

* Fortran interface :
    .. code-block:: fortran

        ! Returns the name of the leaf
        ! With /foo/bar/baz returns baz
        function basename(path)
            character(len=*), intent(in) :: path
            character(len=len(path)) :: basename

* C interface :
    .. code-block:: c

        char* path_basename(char *path)



``dirname``
-----------

Return the directory name of pathname path.

* Fortran interface :
    .. code-block:: fortran

        ! Returns the name of the parent director
        ! With /foo/bar/baz returns /foo/bar
        function dirname(path)
            character(len=*), intent(in) :: path
            character(len=len(path)) :: dirname

* C interface :
    .. code-block:: c

        char* path_dirname(char *path)


``exists``
----------

Return true if path refers to an existing path.


* Fortran interface :
    .. code-block:: fortran

        ! Returns true if path exists in loc_id
        function exists(loc_id, path) result(link_exists)
            integer(hid_t), intent(in) :: loc_id
            character(len=*), intent(in) :: path
            logical :: link_exists

* C interface :
    .. code-block:: c

        int path_exists(hid_t loc_id, char* path)


``isleaf``
----------

Return true if path is an existing table or dataset.

* Fortran interface :
    .. code-block:: fortran

        ! returns true if path is a leaf (table, dataset)
        logical function isleaf(loc_id, path)
            integer(hid_t), intent(in) :: loc_id
            character(len=*), intent(in) :: path


* C interface :
    .. code-block:: c

        int path_isleaf(hid_t loc_id, char* path)


``isgroup``
-----------

Return true if path is an existing group.

* Fortran interface :
    .. code-block:: fortran

        ! returns true if path is a group
        logical function isgroup(loc_id, path)
            integer(hid_t), intent(in) :: loc_id
            character(len=*), intent(in) :: path

* C interface :
    .. code-block:: c

        int path_isgroup(hid_t loc_id, char* path)


``like``
--------

Return true if path matches pattern.

Example : ``like("/foo/bar/baz", "/foo/*/baz") returns true``

* Fortran interface :
    .. code-block:: fortran

        ! Return true if a path looks like a pattern
        ! For "/foo/bar/baz" & "/foo/*/baz" return true
        recursive function like(path, pattern) result (res)
            character(len=*), intent(in) :: path
            character(len=*), intent(in) :: pattern

* C interface :
    .. code-block:: c

        int path_like(char * path, char* pattern)
        


``element``
-----------

Returns the nth element in path

Example : ``element("/foo/bar/baz", 2) returns "bar"``

* Fortran interface
    .. code-block:: fortran

        ! returns the nth element of path
        ! for ("/foo/bar/baz", 2) returns bar
        recursive function element(path, ind, back) result (res)
            character(len=*), intent(in) :: path
            integer, intent(in) :: ind
            logical, optional, intent(in) :: back

* C interface
    .. code-block:: c

        char * path_element(char *path, int index, int back)


``join``
--------

Join one or more path components intelligently.

* Fortran interface :
    .. code-block:: fortran

        ! joins paths of an array of path
        ! for ["/foo", "bar"] returns "/foo/bar"
        function join(paths, sep_)
            character(len=*), dimension(:), intent(in) :: paths
            character(len=size(paths)*len(paths)) :: join
            character, intent(in), optional :: sep_

* C interface :
    .. code-block:: c

        char* join_path(char **paths, int nbpath)

