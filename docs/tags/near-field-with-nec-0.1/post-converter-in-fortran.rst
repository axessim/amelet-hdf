The post converter in fortran
=============================


The "post converter" project within the SDK
-------------------------------------------

Project creation and configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SDK permits to create a runnable project in a few steps :

* Open the SDK.
* Create a new "Fortran Output Wrapper Skeletal Project" named "postNec"
* Edit the Makefile and set the following variables :

 * The fortran compiler executable : FORTRAN_COMPILER = ifort | gfortran ... 
 * The fortran compiler options  : COMPILER_OPTIONS = -O2 -g -Wall -c
 * The HDF5 library root folder : HDF5_ROOT = /usr/local
 * The Amelet-HDF fortran help functions : AMELET_ROOT = /user/local

* Build the project with the menu "Project / Build Project". There should a
  a new binary file in the bin and Binaries folders.

A post converter takes in input two arguments :

* The working directory path (``/the/simulation/path/workingDir``) 
  of the simulation
* The output file path (``/the/simu/path/outputDir/output.h5``) 
  of the simulation

The post converter can be launched with the following command line :

::

    > necPost /the/simulation/path/workingDir /the/simu/path/outputDir/output.h5


The fortran code
----------------

In this project, we have to read the Nec output file and create an 
Amelet-HDf output.h5.

The Nec output file is presented below :

.. literalinclude:: code/necPost/test/simulation/simu1/workingDir/nec.out
    :linenos:

Interesting information are below the title 
``-------- NEAR ELECTRIC FIELDS --------``. We have to parse the file and
read the electric field data and finally generate the Amelet-HDF output.h5.

In this part, we manipulate HDF5 subroutines instead of amelet-hdf functions
except for a few operations we present as we go along.

Reading the outputs
-------------------

After the command line handling, we read the Nec output file and
store field values in a buffer ``fields_buffer`` :

.. code-block:: fortran

    ! Read workingDir/nec.out
    open(unit=necfile, file=trim(working_dir)//"/nec.out", &
         form="formatted", status="old", access="stream")
    line_index = 0
    do
        read(necfile, "(a200)", iostat=hdferr) line
        if (hdferr < 0) exit
        line_index = line_index + 1
        if (index(line, "-------- NEAR ELECTRIC FIELDS --------") > 0) then
            line_number = line_index
            ! Read : ------- LOCATION -------     ------- EX ------   ...
            read(necfile, "(a200)", iostat=hdferr) line
            ! Read : X         Y         Z       MAGNITUDE   PHASE ...
            read(necfile, "(a200)", iostat=hdferr) line
            ! Read : METERS    METERS    METERS     VOLTS/M  DEG ...
            read(necfile, "(a200)", iostat=hdferr) line
            nb_fields = 0
            inquire(necfile, pos=offset)
            allocate(fields_buffer(1, 9))
            do
                read(necfile, *, iostat=hdferr) fields_buffer(1, :)
                if (hdferr > 0) exit
                print *, "Buffer : ", fields_buffer(1, :)
                nb_fields = nb_fields + 1
            enddo
            deallocate(fields_buffer)
            allocate(fields_buffer(nb_fields, 9))

            read(necfile, "(a200)", pos=offset-1) line
            print *
            do i=1, nb_fields
                read(necfile, *, iostat=hdferr) fields_buffer(i, :)
                print *, "Near fields : ", fields_buffer(i, 1:3)
            enddo
        endif
    enddo
    print *, "Near fields line number : ", line_number
    close(necfile)

.. note:: 
    
    We open the file with the ``access="stream"`` to navigate in the file like
    a stream.
    The ``inquire`` function allows to record the position in the file and
    the read function can take the ``pos`` argument to read data from a given
    offset.


``output.h5`` Amelet-HDF file creation
--------------------------------------

The ``output.h5`` file is then created. Note the ``H5F_ACC_TRUNC_F`` to truncate
an existing file (many tests will be necessary).

.. code-block:: fortran

    ! Output file generation
    print *
    print *, "Create ", trim(filename), " ..."
    call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr, &
                     H5P_DEFAULT_F, H5P_DEFAULT_F)
    call check("Can't create "//trim(filename))


Amelet-HDF attributes are added, we use the high level HDF5 subroutine 
``h5ltset_attribute_string_f``. There is no difficulty in this part.

.. code-block:: fortran

    ! Amelet attributes
    print *, "Write FORMAT attribute..."
    call h5ltset_attribute_string_f(file_id, "/", &
                                    "FORMAT", "AMELET-HDF", hdferr)
    call check("Can't write FORMAT attribute for "//path)

    print *, "Write AMELETHDF_FORMAT_VERSION attribute..."
    call h5ltset_attribute_string_f(file_id, "/", &
                                    "AMELETHDF_FORMAT_VERSION", &
                                    "1.0.0", hdferr)
    call check("Can't write AMELETHDF_FORMAT_VERSION attribute for "//path)

    print *, "Write entryPoint attribute..."
    call h5ltset_attribute_string_f(file_id, "/", &
                                    "entryPoint", &
                                    "/simulation/this_simulation", hdferr)
    call check("Can't write entryPoint attribute for "//path)



The simulation outputs is straightforward, only one element is appended to
the list. Note the usage of the ``write_string_dataset`` from Amelet-HDF
helper functions.

.. code-block:: fortran

    ! Writes /simulation group
    print *, "Creates /simulation ..."
    path = "simulation"
    call h5gcreate_f(file_id, trim(path), grp_id, hdferr)
    call check("Can't create /simulation")
    call h5gclose_f(grp_id, hdferr)

    ! Writes /simulation/this_simulation group
    print *, "Creates /simulation/this_simulation ..."
    path = "simulation/this_simulation"
    call h5gcreate_f(file_id, trim(path), grp_id, hdferr)
    call check("Can't create /simulation")
    call h5gclose_f(grp_id, hdferr)

    ! Adds this_simulation attributes
    print *, "Writes /", trim(path)//"@module ..."
    call h5ltset_attribute_string_f(file_id, trim(path), &
                                    "module", "a_module", hdferr)
    call check("Can't write module attribute for "//path)
    print *, "Writes /", trim(path)//"@version ..."
    call h5ltset_attribute_string_f(file_id, trim(path), &
                                    "version", "1.0.0", hdferr)
    call check("Can't write version attribute for "//path)

    ! Writes simulation/this_simulation/outputs
    allocate(outputs(1))
    outputs(1) = "/floatingType/near_field"
    path = "simulation/this_simulation/outputs"
    print *, "Writes /", trim(path) //" ..."
    call write_string_dataset(file_id, trim(path), outputs, shape(outputs))
    call check("Can't create "//path)


Numerical data are conventionally stored in external files.
The external elements dataset aims at declaring where elements are really
written :

.. code-block:: fortran

    ! Since near field data are stored in an external element
    ! We have to create an /externalElement object
    print *, "Creates /externalElement/external_element ..."
    path = "/externalElement"
    call h5gcreate_f(file_id, trim(path), grp_id, hdferr)
    call check("Can't create /externalElement")
    call h5gclose_f(grp_id, hdferr)
    path = "/externalElement/external_element"
    allocate(external_element(3, 2))
    external_element(1,1) = "/mesh/result_mesh_group/near_field"
    external_element(2,1) = "near_field.h5"
    external_element(3,1) = "/mesh/result_mesh_group/near_field"
    external_element(1,2) = "/floatingType/near_field"
    external_element(2,2) = "near_field.h5"
    external_element(3,2) = "/floatingType/near_field"
    call write_string_dataset(file_id, path, external_element, &
                              shape(external_element))
    ! close filename
    call h5fclose_f(file_id, hdferr)
    call check("Can't close "//trim(filename))


Here, a file named ``near_field`` is generated, it will contain the
electric fields.




``near_file.h5`` Amelet-HDF file creation
-----------------------------------------

``near_field.h5`` Amelet-HDF file is then created :

.. code-block:: fortran

    ! Field file creation
    print *
    print *, "filename : ", trim(filename)
    print *, "dirname : ", trim(dirname(filename))
    if (allocated(buf)) deallocate(buf)
    allocate(buf(2))
    buf(1) = trim(dirname(filename))
    buf(2) = "near_field.h5"
    filename = join(buf)
    print *, "near field filename : ", trim(filename)
    print *
    print *, "Create ", trim(filename), " ..."
    call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr, &
                     H5P_DEFAULT_F, H5P_DEFAULT_F)
    call check("Can't create "//trim(filename))


.. note::

    We use ``dirname`` and ``join`` functions from Amelet-HDF helper function
    to generate the absolute name of ``near_field.h5`` file.

In ``near_field.h5``, we append the points mesh for locating computed fields :

.. code-block:: fortran

    ! Points mesh for locating computed fields
    print *, "/mesh group creation ..."
    call h5gcreate_f(file_id, "mesh", grp_id, hdferr)
    call check("Can't create /mesh group")
    call h5gclose_f(grp_id, hdferr)

    print *, "/mesh/result_mesh group creation ..."
    call h5gcreate_f(file_id, "mesh/result_mesh", grp_id, hdferr)
    call check("Can't create /mesh/result_mesh group")
    call h5gclose_f(grp_id, hdferr)

    print *, "/mesh/result_mesh/near_field group creation ..."
    call h5gcreate_f(file_id, "mesh/result_mesh/near_field", grp_id, hdferr)
    call check("Can't create /mesh/result_mesh/near_field group")
    call h5gclose_f(grp_id, hdferr)

    print *, "/mesh/result_mesh/near_field/nodes group creation ..."
    allocate(dims(2))
    dims = (/3, size(fields_buffer, 1)/)
    call h5ltmake_dataset_float_f(file_id, &
                                  "/mesh/result_mesh/near_field/nodes", &
                                  2, &
                                  dims, transpose(fields_buffer(:,1:3)), hdferr)
    call check("Can't write dataset for "//path)
    deallocate(dims)




As complex numbers are stored in (real part, imaginary part) way
in Amelet-HDF, we convert the magnitudes and the phases just read above.

.. code-block:: fortran

    ! Conversion of fields_buffer from magnitude / phase
    ! To complex form (in fields)
    allocate(fields(3, size(fields_buffer,1)))
    do i=1, size(fields_buffer, 1)
        ex_m = fields_buffer(i, 4)
        ex_p = deg2rad(fields_buffer(i, 5))
        ey_m = fields_buffer(i, 6)
        ey_p = deg2rad(fields_buffer(i, 7))
        ez_m = fields_buffer(i, 8)
        ez_p = deg2rad(fields_buffer(i, 9))
        fields(1, i) = cmplx(ex_m*cos(ex_p), ex_m*sin(ex_p))
        fields(2, i) = cmplx(ey_m*cos(ex_p), ey_m*sin(ex_p))
        fields(3, i) = cmplx(ez_m*cos(ex_p), ez_m*sin(ex_p))
    enddo

Then we create the ``/floatingType/near_field`` elements :

.. code-block:: fortran

    ! Near field dataset creation
    ! floatingType group creation
    print *, "/floatingType group creation ..."
    call h5gcreate_f(file_id, "floatingType", grp_id, hdferr)
    call check("Can't create /floatingType group")
    call h5gclose_f(grp_id, hdferr)

    ! arraySet group creation
    print *, "/floatingType/near_field group creation ..."
    call h5gcreate_f(file_id, "floatingType/near_field", grp_id, hdferr)
    call check("Can't create /floatingType/near_field group")
    call h5gclose_f(grp_id, hdferr)

    ! arraySet ds group creation
    print *, "/floatingType/near_field/ds group creation ..."
    call h5gcreate_f(file_id, "floatingType/near_field/ds", grp_id, hdferr)
    call check("Can't create /floatingType/near_field/ds group")
    call h5gclose_f(grp_id, hdferr)

    ! Writes floatingType attribute
    print *, "Writes /floatingType/near_field/@floatingType ..."
    call h5ltset_attribute_string_f(file_id, "floatingType/near_field", &
                                    "floatingType", "arraySet", hdferr)
    call check("Can't write floatingType attribute for floatingType/near_field")

    ! Writes /floatingType/near_field/data
    print *, "Writes /floatingType/near_field/data ..."
    call write_complex_dataset(file_id, "/floatingType/near_field/data", &
                               fields, shape(fields))


.. note::

    +------------------------------------------------------------------------+
    | Here we have used the ``write_complex_dataset`` subroutine.            | 
    | This subroutine comes from Amelet-HDF helper functions.                |
    +------------------------------------------------------------------------+
    | .. code-block:: fortran                                                |
    |                                                                        |
    |   subroutine write_complex_dataset(file_id, path, values, values_shape)|
    |       ! File identifier                                                |
    |       integer(hid_t), intent(in) :: file_id                            |     
    |       ! Path of the dataset                                            |
    |       character(len=*), intent(in) :: path                             |
    |       ! Shape of the values array                                      |
    |       integer, dimension(:), intent(in) :: values_shape                |
    |       ! Complex values to be stored                                    |
    |       complex, dimension(product(values_shape)), intent(in) :: values  |
    |                                                                        |
    +------------------------------------------------------------------------+

.. code-block:: fortran

    ! Writes /floatingType/near_field/data attributes
    path = "/floatingType/near_field/data"
    call set_floating_type(file_id, path, "Electric Field", "electricField", &
                           "voltPerMeter", "electric field")

    ! Writes /floatingType/near_field/ds/dim1
    path = "/floatingType/near_field/ds/dim1"
    print *, "Writes ", trim(path), " ..."
    allocate(buf(3))
    buf = (/"Ex", "Ey", "Ez"/)
    call write_string_dataset(file_id, path, buf, shape(buf))
    call check("Can't write dataset for "//path)
    deallocate(buf)
    ! Writes /floatingType/near_field/ds/dim1 floatingType attributes ...
    call set_floating_type(file_id, trim(path), "Ex Ey Ez components", &
                           "component", "", "Ex Ey Ez components")

    ! Writes /floatingType/near_field/ds/dim2
    path = "/floatingType/near_field/ds/dim2"
    print *, "Writes ",  trim(path), " ..."
    allocate(dims(1))
    dims = (/1/)
    call h5ltmake_dataset_float_f(file_id, trim(path), 1, dims, dim2, hdferr)
    call check("Can't write dataset for "//path)
    deallocate(dims)
    ! Writes /floatingType/near_field/ds/dim2 floatingType attributes ...
    call set_floating_type(file_id, trim(path), "Mesh points", "meshEntity", &
                           "", "The fields location")
    ! Writes meshEntity attribute
    print *, "Writes /floatingType/near_field/dim2@meshEntity ..."
    call h5ltset_attribute_string_f(file_id, "/floatingType/near_field/ds/dim2", &
                                    "meshEntity", &
                                    "/mesh/result_mesh/near_fiel/nodes", &
                                    hdferr)
    call check("Can't write floatingType attribute for floatingType/near_field")

    ! close filename
    call h5fclose_f(file_id, hdferr)
    call check("Can't close "//trim(filename))


The ``set_floating_type`` function from the Amelet-HDF helper function allows
to set ``floatingType`` characteristics in on go.


Finally we close the HDF5 library :

.. code-block:: fortran

    ! close HDF5 library
    call h5close_f(hdferr)
    call check("Can't close HFD5")


In the ``outputDir`` too files has been created :

* ``output.h5`` : This is the main output file for the simulation. Its is a 
  convention.
* ``near_field.h5`` : This is the file containing the numerical data.

