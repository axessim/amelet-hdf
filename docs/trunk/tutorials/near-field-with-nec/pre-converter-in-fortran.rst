The pre converter in fortran
============================

Now we have generated the simulation input.h5 file, we can create the
Nec ``pre`` converter.

The ``pre`` converter is written in fortran with the helper functions
provided with the SDK (the use of helper function is optional).


The Nec input file
------------------

According to the Nec distribution, the simulation input file is localized in
``nec2c/Input/EX1.nec``, the file is reporter hereafter :

::

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW  0,7,0.,0.,-.25,0.,0.,.25,.001
    GE
    EX  0    0    4    0    1.
    XQ
    LD  0    0    4    4   10.     3.000E-09 5.300E-11
    PQ
    NE  0    1    1   15   .001      0         0          0.        0.      .01786
    EN


Nec input file are in ASCII flat format and is based upon the division of a 
line is 10 columns :

===== ===== ===== ===== ===== ===== ===== ===== ===== =====
 2     5     10    20    30    40    50    60    70    80
===== ===== ===== ===== ===== ===== ===== ===== ===== =====
 GA    I1    I2    I3    F1    F2    F3  
===== ===== ===== ===== ===== ===== ===== ===== ===== =====

Each card has a particular meaning relative to the first column value (see the 
Nec documentation for much detailed description) :

* ``CE`` is a comment line
* ``GW`` defines a nth segments straight line
* ``GE`` marks the end of the geometry definition
* ``EX`` defines a 1V voltage source on the fourth segment
* ``XQ`` says 'ready for execution'
* ``LD`` adds a load impedance (R=10.Ohm, L=3.e-9H, C=5.3e-11F) 
  on the fourth segment
* ``PQ`` : Print control for charges on wire
* ``NE`` is a near field calculation request
* ``EN`` is the end of the card

Finally, the ``pre`` converter aims at creating the "card" from the
Amelet-HDF input file.



The pre converter project
--------------------------

Amelet-HDF helper functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fortran helper functions will be helpful for this task, 
we are going to compile them now.

First of all, fetch and compile the Amelet-HDF fortran help functions.
The compilation process is straightforward :

:: 

> ls 
> amelethdf-fortran.tgz
> tar xvfz amelethdf-fortran.tgz
> mkdir amelethdf-build
> cd amelethdf-build
> ccmake ../amelethdf-fortran .
> make


The "pre converter" project within the SDK
------------------------------------------

Project creation and configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SDK permits to create a runnable project in a few steps :

* Open the SDK.
* Create a new "Fortran Input Wrapper Skeletal Project" named "preNec"
* Edit the Makefile and set the following variables :

 * The fortran compiler executable : FORTRAN_COMPILER = ifort | gfortran ... 
 * The fortran compiler options  : COMPILER_OPTIONS = -O2 -g -Wall -c
 * The HDF5 library root folder : HDF5_ROOT = /usr/local
 * The Amelet-HDF fortran help functions : AMELET_ROOT = /user/local

* Build the project with the menu "Project / Build Project". There should a
  a new binary file in the bin and Binaries folders.


The first test
^^^^^^^^^^^^^^

Create the "test/simulation/simu1/inputDir/" folder and import the ``input.h5``
just created before.

Then select the project "preNec" and configure the launch options by 
"Run / Run configurations". Add a new entry in "Fortran Local Application".
In the Arguments tab add 
"test/simulation/simu1/inputDir/input.h5 test/result/simu1/workingDir".
the pre converter will read input data from 
test/simulation/simu1/inputDir/input.h5 and write output data in 
test/result/simu1/workingDir. Apply modifications and close the window.

Select "Run / Run" the launch the converter. A lot of things should be written
in the console. Nothing in generated in the outputDir folder.

The conversion
^^^^^^^^^^^^^^

The convert part has to translate Amelet-HDF data into the nec card format.
We decide the Nec input file will be named "input.nec".

An important thing to notice, CW Nec card defines a straight line wire 
with n segments. This concept does not exist in Amelet-HDF so we have to 
describe the wire by 7 1-segment straight lines. The voltage source card
is also modified. The input.nec looks like :

::

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW 0,1,0.,0.,-0.25,0.,0.,-0.17857143,.001
    GW 0,1,0.,0.,-0.17857143,0.,0.,-0.10714286,.001
    GW 0,1,0.,0.,-0.10714286,0.,0.,-0.03571429,.001
    GW 1,1,0.,0.,-0.03571429,0.,0.,0.03571429,.001
    GW 0,1,0.,0.,0.03571429,0.,0.,0.10714286,.001
    GW 0,1,0.,0.,0.10714286,0.,0.,0.17857143,.001
    GW 0,1,0.,0.,0.17857143,0.,0.,0.25,.001
    GE
    EX  0    1    1    0    1.
    XQ
    LD  0    1    1    1   10.     3.000E-09 5.300E-11
    PQ
    NE  0    1    1   15   .001      0         0          0.        0.      .01786
    EN

The second modification is because fortran does not use the same free
formatting string schema than C language. It is more common the write
data in columns as the original Nec defines it. We lost a little bit of 
precision but it is not relevant relative to the result.

Finally, the pre converter has to create an input file looking like the
following example :


..  GW format
    --+++-----++++++++++----------++++++++++----------++++++++++----------+++++++++


::

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW  0    1    0.0000    0.0000   -0.2500    0.0000    0.0000   -0.1786    0.0001
    GW  0    1    0.0000    0.0000   -0.1786    0.0000    0.0000   -0.1071    0.0001
    GW  0    1    0.0000    0.0000   -0.1071    0.0000    0.0000   -0.0357    0.0001
    GW  1    1    0.0000    0.0000   -0.0357    0.0000    0.0000    0.0357    0.0001
    GW  0    1    0.0000    0.0000    0.0357    0.0000    0.0000    0.1071    0.0001
    GW  0    1    0.0000    0.0000    0.1071    0.0000    0.0000    0.1786    0.0001
    GW  0    1    0.0000    0.0000    0.1786    0.0000    0.0000    0.2500    0.0001
    GE
    EX  0    1    1    0    1.
    XQ
    LD  0    1    1    1   10.     3.000E-09 5.300E-11
    PQ
    NE  0    1    1   15   .001      0         0          0.        0.      .01786
    EN


The same modification has to be performed on EX card, the file become :

::

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW  0    1    0.0000    0.0000   -0.2500    0.0000    0.0000   -0.1786    0.0001
    GW  0    1    0.0000    0.0000   -0.1786    0.0000    0.0000   -0.1071    0.0001
    GW  0    1    0.0000    0.0000   -0.1071    0.0000    0.0000   -0.0357    0.0001
    GW  1    1    0.0000    0.0000   -0.0357    0.0000    0.0000    0.0357    0.0001
    GW  0    1    0.0000    0.0000    0.0357    0.0000    0.0000    0.1071    0.0001
    GW  0    1    0.0000    0.0000    0.1071    0.0000    0.0000    0.1786    0.0001
    GW  0    1    0.0000    0.0000    0.1786    0.0000    0.0000    0.2500    0.0001
    GE
    EX  0    1    1    0    1.0000    0.0000
    XQ
    LD  0    1    1    1   10.     3.000E-09 5.300E-11
    PQ
    NE  0    1    1   15   .001      0         0          0.        0.      .01786
    EN


Finally the load element format is adapted to the fortran language and the
implicitly defined computation location request is replaced by an explicit point
definition.

the final referenced nec file is :

::

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW  0    1    0.0000    0.0000   -0.2500    0.0000    0.0000   -0.1786    0.0010
    GW  0    1    0.0000    0.0000   -0.1786    0.0000    0.0000   -0.1071    0.0010
    GW  0    1    0.0000    0.0000   -0.1071    0.0000    0.0000   -0.0357    0.0010
    GW  1    1    0.0000    0.0000   -0.0357    0.0000    0.0000    0.0357    0.0010
    GW  0    1    0.0000    0.0000    0.0357    0.0000    0.0000    0.1071    0.0010
    GW  0    1    0.0000    0.0000    0.1071    0.0000    0.0000    0.1786    0.0010
    GW  0    1    0.0000    0.0000    0.1786    0.0000    0.0000    0.2500    0.0010
    GE
    EX  0    1    1    0    1.0000    0.0000                                        
    XQ
    LD  0    1    1    1 1.000E+01 3.000E-09 5.300E-11                              
    PQ
    NE  0    1    1    1    0.0000    0.0000    0.1786    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0000    0.0000    0.2500    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0000    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0179    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0357    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0536    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0714    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.0893    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1071    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1250    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1429    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1607    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1786    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.1964    0.0000    0.0000    0.0000
    NE  0    1    1    1    0.0010    0.0000    0.2143    0.0000    0.0000    0.0000
    EN


Along the process use the "compare with" tool of Eclipse to compare
the reference ``input.nec`` and the generated ``input.nec`` : 
create a file "input.nec" in ``test/reference/simu1/workingDir``. On the 
over side, the
pre converter will create ``test/result/simu1/workingDir/input.nec``. 
By selecting ``test/result`` and ``test/reference``, right-click the
``compare-with/each other``, Eclipse allows to follows the difference between
the reference and the awaited result.



The fortran code
----------------

The converter are coded in fortran.

.. note::

    The fortran code is contained is a file named necPre.f90 in the code folder.



Nec card model
^^^^^^^^^^^^^^

The first thing we do is a nec card model in fortran :

.. code-block:: fortran

    module nec_model
        implicit none

        integer, parameter :: CARD_LEN = 80

        ! Nec GW card
        type gw_t
            integer :: itg, ns
            real :: xw1, yw1, zw1, xw2, yw2, zw2, rad
        end type gw_t

        ! Nec EX card
        type ex_t
            integer :: source_type, tag, m
            integer :: c19 = 0, c20 = 0
            real :: real_part, imaginary_part
        end type ex_t

        ! Nec LD card
        type ld_t
            integer :: ldtype, ldtag, ldtagf, ldtagt
            real :: zlr, zli, zlc
        end type ld_t

        ! Nec NE card
        type ne_t
            ! Coordinate system 0 -> rectangular coordinates
            integer :: near, nrx, nry, nrz
            real :: xnr, ynr, znr, dxnr, dynr, dznr
        end type ne_t

    contains
        function gw_to_string(gw) result(string)
            type(gw_t), intent(in) :: gw
            character(len=CARD_LEN) :: string
            write(string, '(a2,i3,i5,7f10.4)') "GW", gw%itg, gw%ns, &
                                               gw%xw1, gw%yw1, gw%zw1, &
                                               gw%xw2, gw%yw2, gw%zw2, gw%rad
        end function

        function ex_to_string(ex) result(string)
            type(ex_t), intent(in) :: ex
            character(len=CARD_LEN) :: string
            write(string, '(a2,i3,3i5,2f10.4)') "EX", ex%source_type, ex%tag, &
                                                ex%m, ex%c19, &
                                                ex%real_part, ex%imaginary_part
        end function

        function ld_to_string(ld) result(string)
            type(ld_t), intent(in) :: ld
            character(len=CARD_LEN) :: string
            write(string, '(a2,i3,3i5,3es10.3)') "LD", ld%ldtype, ld%ldtag, &
                                                ld%ldtagf, ld%ldtagt, &
                                                ld%zlr, ld%zli, ld%zlc
        end function

        function ne_to_string(ne) result(string)
            type(ne_t), intent(in) :: ne
            character(len=CARD_LEN) :: string
            write(string, '(a2,i3,3i5,6f10.4)') "NE", ne%near,  &
                                                ne%nrx, ne%nry, ne%nrz, &
                                                ne%xnr, ne%ynr, ne%znr, &
                                                ne%dxnr, ne%dynr, ne%dznr
        end function

        function generate_tag_wire() result(tag)
            integer :: ref_tag = 0
            integer :: tag

            ref_tag = ref_tag + 1
            tag = ref_tag
        end function generate_tag_wire
    end module nec_model


This code declares a Nec_model module, types represent the Nec cards met in
the example. in addition, the module provides function to write cards in
a file according the preceding format. See the Nec documentation for further
details.



Nec input file creation
^^^^^^^^^^^^^^^^^^^^^^^

Now, the first thing to do is the creation of the 
``input.nec`` file, add the following fortran code line after the reading
of ``output_folder`` :

.. code-block:: fortran

    ! create / open input.nec file
    open(unit=numnec, file=trim(output_folder)//'/'//inputnec, &
         form='formatted', status='replace')
    write(numnec, "(a40)"), "CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA"
    write(numnec, "(a2)"), "CE"

``status`` is ``replace`` because a lot of tries will be performed and this 
permits to overwrite an existing file.

Compile and run the project and compare ``test/reference`` and ``test/result``
folders. The two first line are identical.

A pre converter takes in input two arguments :

* The input file path (``/the/simulation/path/inputDir/input.h5``) 
  of the simulation
* The working directory path (``/the/simu/path/workingDir``) 
  of the simulation

The pre converter can be launched with the following command line :

::

    > necPre /the/simu/path/inputDir/input.h5 /the/simulation/path/workingDir


First step with HDF5 library
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

HDF5 library has to be initialized to handle properly elements and constants. 
At the same time we open the HDF5 input file :

.. code-block:: fortran

    ! HDF5 library initialization
    hdferr = 0
    call h5open_f(hdferr)
    print *, "HDF5 library initialized"

    print *, "Reading ", trim(filename), " ..."
    call h5fopen_f(filename, H5F_ACC_RDONLY_F, file_id, hdferr, H5P_DEFAULT_F)
    call check("Can't open "//trim(filename))

Functions stating with ``h5`` come from the HDF5 library. The trailing ``_``
marks the fortran binding.

The ``check`` function is provided in the Amelet-HDF helper functions. It checks
the value of ``hdferr``. If there is an error, the message is print at the
console and the program is stopped.

The constant ``H5F_ACC_RDONLY_F`` signifies the file is opened in read only
mode. We can't write into it. Finally ``file_id`` is the identifier 
of the HDF5 file in our program.

Next we read the ``entryPoint`` attribute of the file (we suppose the file is
a correct Amelet-HDF file). If the value of ``entryPoint`` does not begin
with ``/simulation/*`` the program stops :

.. code-block:: fortran

    found = read_string_attribute(file_id, "/", "entryPoint", simulation)
    print *, "entry point : ", trim(simulation)

    if (.not. like(simulation, "/simulation/*")) then
        print *, "The entry point is not a simulation..."
        print *, "stop !!!"
        stop
    endif

If the ``entryPoint`` is a simulation, the simulation is read :

.. code-block:: fortran

    call read_simulation(file_id, trim(simulation), sim)

Read information are stored in the ``sim`` object which has been declared as 
follow at the beginning of the program :

.. code-block:: fortran

    ! Amelet types
    type(simulation_t) :: sim
    type(structured_mesh_t) :: smesh
    type(unstructured_mesh_t) :: umesh
    type(umesh_group_t), pointer :: ugroup
    type(planewave_t) :: pw
    type(floatingtype_t) :: ft
    type(link_t) :: link

``simulation_t`` type is a fortran type modeling an Amelet-HDF simulation :

.. code-block:: fortran

    ! The simulation type
    type simulation_t
        character(len=AL) :: name = ""
        character(len=AL), dimension(:), allocatable :: inputs, outputs
    end type simulation_t

This type has a name and two arrays containing the inputs and outputs of the
simulation. The ``read_simulation`` function fill in these arrays.

Walking through the simulation's inputs
---------------------------------------

For each element of ``inputs`` we take a decision :

* Either the information is read immediately
* Either the reading is suspended and delayed until the links discovery.

Globally the algorithm revolved around the ``like`` function, its signature
is as follow :

.. code-block:: fortran

    ! Return true if a path looks like a patter
    ! For "/foo/bar/baz" & "/foo/*/baz" return true
    function like(path, pattern)
    character(len=*), intent(in) :: path
    character(len=*), intent(in) :: pattern

The function take 2 arguments :

* A string ``path``. A path is a string looking like 
  ``/some/thing/in/an/amelet/file``
* A string ``pattern``. ``pattern`` must be as string looking like
  ``/some/*/*/an/amelet/*``
  ``*`` is a jocker character and can replace of whatever string.
  
``like`` returns ``true`` if pattern looks like ``path``.
  
Thanks to ``like``, simulation's inputs are checked relative to the 
possibilities of the integrated solver.


.. code-block:: fortran

    print *
    print *, "--Handle inputs ..."
    ! We read inputs except links
    do j=1, size(sim%inputs)
        path = sim%inputs(j)


Let start with the mesh elements :

.. code-block:: fortran

        if (like(path, "/mesh/*")) then
            print *, "+A mesh !!! : ", trim(path)
            if (allocated(children_name2)) deallocate(children_name2)
            call read_children_name(file_id, trim(path), children_name2)
            path2 = trim(path)//"/"//trim(children_name2(1))
            ! The mesh is read
            ! The umesh structured is filled in with read data
            call umesh_read(file_id, trim(path2), umesh)
            ! Generate the array containing the offset of element
            ! in elementNodes
            call umesh_generate_offsets(umesh)

If the input is a *mesh group*, its children are returned with the 
``read_children_name`` subroutine. this subroutine is intensively used
to walk through the element hierarchy. The ``children`` string array
is populated with the name of the children of the group.

.. code-block:: fortran

        ! Read the children's name of a group
        subroutine read_children_name(file_id, path, children)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: path
            character(len=ELEMENT_NAME_LENGTH), &
                dimension(:), allocatable :: children

The mesh is read thanks to the ``umesh_read`` subroutine as well as
the ``offset`` field. The ``offset`` field is an array containing the
offset of elements in ``elementNodes``.

.. code-block:: fortran

        else if (like(path, "/electromagneticSource/generator/*")) then
            print *, "+A generator !!!"

If the input is a *generator* the handle is delayed until a link used it.

.. note:: 

    It a subjective choice and not a general rule.


.. code-block:: fortran

        else if (like(path, "/label/*")) then
            print *, "+Labels !!! "
            if (path == "/label/predefinedLabels") then
                if (allocated(predefined_labels)) deallocate(predefined_labels)
                call read_string_vector(file_id, path, predefined_labels)
                print *, "  Predefined labels : ", predefined_labels(:)
            else if (path == "/label/predefinedOutputRequests") then
                if (allocated(predefined_output_requests)) then
                    deallocate(predefined_output_requests)
                endif
                call read_string_vector(file_id, path, predefined_output_requests)
                print *, "  Predefined output requests : ", predefined_output_requests(:)
            else
                if (allocated(children_name2)) deallocate(children_name2)
                call read_string_vector(file_id, path, children_name2)
                print *, "  Label : ", children_name2(:)
            endif


It this code snippet the predefined label lists are read :

* ``/label/predefinedLabels``
* ``/label/predefinedOutputRequest``


This operation is performed with the ``read_string_vector`` subroutine, 
its signature is presented here below :

.. code-block:: fortran

    subroutine read_string_vector(file_id, path, vector)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), dimension(:), allocatable :: vector


This subroutine read a one dimensional string dataset and put the values
in the vector array.

Just after a RLC circuit is detected, but we choose to handle it during
the links management.

.. code-block:: fortran

        else if (like(path, "/physicalModel/multiport/RLC/*")) then
            print *, "+RLC !!!"


Then the global environment is inspected :

.. code-block:: fortran

        else if (like(path, "/globalEnvironment/*")) then
            print *, "+Global environment !!!"
            if (allocated(children_name2)) deallocate(children_name2)
            call read_children_name(file_id, trim(path), children_name2)
            path2 = trim(path)//"/"//trim(children_name2(1))
            print *, "  Environment : ", trim(path2)
            call read_floatingtype(file_id, trim(path2), ft)
            frequency = convert_to_real_vector(ft)
            print *, "  Value : ", frequency, "Hz"
        else
            print *, "-Unknown : ", trim(path)
        endif
    enddo

The global environment is a floating type. As a consequence we use the
``read_floatingtype`` subroutine :

.. code-block:: fortran

    subroutine read(file_id, path, ft)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        type(floatingtype_t), intent(inout) :: ft

with the ``floatingtype_t`` type defined by :

.. code-block:: fortran

    type floatingtype_t
        integer :: floatingtype
        type(singlereal_t) :: singlereal
        type(singlecomplex_t) :: singlecomplex
        type(vector_t) :: vector
        type(dataset_t) :: dataset
        type(arrayset_t) :: arrayset
    end type floatingtype_t

The ``floatingtype_t`` type is a container fortran ``type``. The field 
``integer floatingtype`` gives the real type and the children field
which holds data.

Children types are defined by :

.. code-block:: fortran

    ! Base type, common with all floating types
    type single_t
        character(len=EL) :: label = ""
        character(len=EL) :: physical_nature = ""
        character(len=EL) :: unit = ""
        character(len=EL) :: comment = ""
    end type single_t

    type singlereal_t
        type(single_t) :: single
        real :: value
    end type singlereal_t

    type singlecomplex_t
        type(single_t) :: single
        complex :: value
    end type singlecomplex_t

    type dataset_t
        type(single_t) :: single
        integer, dimension(:), allocatable :: ivalue
        real, dimension(:), allocatable :: rvalue
        complex, dimension(:), allocatable :: cvalue
    end type dataset_t

    type dataset_t
        type(single_t) :: single
        integer, dimension(:), allocatable :: dims
        integer, dimension(:), allocatable :: ivalue
        real, dimension(:), allocatable :: rvalue
        complex, dimension(:), allocatable :: cvalue
    end type dataset_t

(See the ``test/floatingtypetest.f90`` file for further explanation)



Links and output requests management
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: fortran

    ! Now we read links & output requests
    print *
    print *, "--Handle links & outputRequests ..."
    do j=1, size(sim%inputs)
        path = sim%inputs(j)
        print *
        print *, "Sim inputs : ", trim(path)

.. code-block:: fortran

        if (like(path, "/link/*")) then
            print *
            print *, "+Links !!! : ", trim(path)
            call read_links(trim(path))


A link is an instance of the ``link`` type :

.. code-block:: fortran

    type link_t
        character(len=AL) :: name = ""
        character(len=AL) :: subject = "", object = ""
    end type link_t

The field of ``link_t`` are defined relative to Amelet-HDF specification.


When a *link* pattern is met the subroutine ``read_links`` is executed 
(comments are given in the code in order to facilitate the code review) :

.. code-block:: fortran

    ! Read links
    subroutine read_links(link_group)
        character(len=*), intent(in) :: link_group

        character(len=AL) :: path
        integer :: j, k
        character(len=EL), dimension(:), allocatable :: children_name

        path = trim(link_group)
        if (allocated(children_name)) deallocate(children_name)
        call read_children_name(file_id, trim(path), children_name)

        ! wireRadius links -> gives the number of wires
        print *
        print *, "Reading wireRadius links ..."
        nb_wires = 0
        
        !
        ! 'wireRadius' links are handled a first time
        ! Thanks to this information, the number of wires in the
        ! structure can be calculated and memory allocated
        !
        do j=1, size(children_name)
            path2 = trim(path)//"/"//trim(children_name(j))
            call read_link(file_id, path2, link)
            print *, "--Subject : ", trim(link%subject)
            ! /label/predefinedLabels#wireRadius handling
            if (link%subject == "/label/predefinedLabels") then
                found = read_int_attribute(file_id, path2, &
                                           "subject_id", id, .true.)
                if (predefined_labels(id+1) == "wireRadius") then
                    ! We take into account only groups
                    if (like(link%object, "/mesh/*/*/group/*")) then
                        print * ,"  Wire radius on group !!"
                        print *, "  Mesh : ", trim(link%object)
                        ugroup => umesh_get_group_by_name(umesh, link%object)
                        if (associated(ugroup)) then
                            ! The number of wires is updated
                            nb_wires = nb_wires + size(ugroup%elements)
                        endif
                    endif
                endif
            endif
        enddo

        print *
        print *, "The number of wires is : ", nb_wires
        ! We allocate the memory for the linear structure
        allocate(nec_wires(nb_wires))
        ! An array to keep a relation between Nec wires and Amelet wires
        allocate(nec_amelet(nb_wires))

        ! Build nec wire model
        print *
        id_wires = 0

        !
        ! Secondly we look for the 'wireRadius' once again.
        ! Amelet wires are converter into Nec segment.
        ! 

        do j=1, size(children_name)
            path2 = trim(path)//"/"//trim(children_name(j))
            call read_link(file_id, path2, link)
            print *, "--Subject : ", trim(link%subject)
            ! /label/predefinedLabels#wireRadius handling
            if (link%subject == "/label/predefinedLabels") then
                found = read_int_attribute(file_id, path2, "subject_id", id)
                print *, "id : ", id
                print *, "Label : ", trim(predefined_labels(id+1))
                if (predefined_labels(id+1) == "wireRadius") then
                    found = read_float_attribute(file_id, path2, "radius", radius)
                    print *, "  Radius : ", radius
                    ! Group management
                    if (like(link%object, "/mesh/*/*/group/*")) then
                        print * ,"  Wire radius on group !!"
                        
                        ugroup => umesh_get_group_by_name(umesh, link%object)
                        if (associated(ugroup)) then
                            print *, "  Group path :", trim(ugroup%name)
                            do k=1, size(ugroup%elements)
                                id_wires = id_wires + 1
                                elt_ind = ugroup%elements(k)
                                elt_type = umesh%elements(elt_ind+1)
                                nb_nodes = umesh_number_of_nodes(elt_type)
                                node1 = umesh%element_nodes(&
                                            umesh%offsets(elt_ind+1))
                                node2 = node1 + 1
                                print *, k , elt_ind, elt_type, nb_nodes, &
                                         node1, node2
                                nec_wires(id_wires)%ns = 1
                                nec_wires(id_wires)%xw1 = umesh%nodes(1,node1+1)
                                nec_wires(id_wires)%yw1 = umesh%nodes(2,node1+1)
                                nec_wires(id_wires)%zw1 = umesh%nodes(3,node1+1)
                                nec_wires(id_wires)%xw2 = umesh%nodes(1,node2+1)
                                nec_wires(id_wires)%yw2 = umesh%nodes(2,node2+1)
                                nec_wires(id_wires)%zw2 = umesh%nodes(3,node2+1)
                                nec_wires(id_wires)%rad = radius
                                write(*, "(7f8.4)"), umesh%nodes(:,node1+1), &
                                                     umesh%nodes(:,node2+1), &
                                                     radius
                                nec_amelet(id_wires) = elt_ind
                            enddo
                        endif
                        nullify(ugroup)
                    endif
                endif
            endif
        enddo


        ! 
        ! Voltage source links -> Give the number of voltage sources
        ! Hypothesis : we take into account only one voltage source
        ! -> the first read
        !
        print *
        print *, "Reading voltage source links ..."
        do j=1, size(children_name)
            path2 = trim(path)//"/"//trim(children_name(j))
            call read_link(file_id, path2, link)
            print *, "--Subject : ", trim(link%subject)
            if (like(link%subject, "/electromagneticSource/generator/*")) then
                found = read_string_attribute(file_id, link%subject, "type", buf)
                print *, "  Type : ", trim(buf)
                print *, "  Object : ", trim(link%object)
                if (like(link%object, "/mesh/*/*/selectorOnMesh/elements")) then
                    found = read_string_attribute(file_id, path2, &
                                                  "object_shortName", buf)
                    print *, "  It is an element : ", trim(buf)
                    elt_ind = &
                        umesh_get_index_by_short_name_in_some(umesh%som_element, &
                                                              trim(buf))
                    print *, "  Amelet wire index : ", elt_ind
                    print *, "  Nec wire index : ", get_index(nec_amelet, elt_ind)
                    nec_wires(get_index(nec_amelet, elt_ind))%itg = 1
                    nec_generator%source_type = 0
                    nec_generator%tag = 1
                    nec_generator%m = 1
                    nec_generator%real_part = 1
                    nec_generator%imaginary_part = 0
                endif
            endif
        enddo


        ! Write nec wires to input.nec
        do i=1, nb_wires
            write(numnec, "(a80)"), gw_to_string(nec_wires(i))
        enddo
        write(numnec, "(a2)"), "GE"
        write(numnec, "(a80)"), ex_to_string(nec_generator)
        write(numnec, "(a2)"), "XQ"


        !
        ! load links -> Give the number of loads
        ! Hypothesis : we take into account only one load RLC model
        ! -> the first read
        !
        print *
        print *, "Reading load links ..."
        do j=1, size(children_name)
            path2 = trim(path)//"/"//trim(children_name(j))
            call read_link(file_id, path2, link)
            print *, "--Subject : ", trim(link%subject)
            if (like(link%subject, "/physicalModel/multiport/RLC/*")) then
                print *, "Object : ", trim(link%object)
                if (like(link%object, "/mesh/*/*/selectorOnMesh/elements")) then
                    found = read_string_attribute(file_id, path2, &
                                                  "object_shortName", buf)
                    print *, "It is an element : ", trim(buf)
                    elt_ind = &
                        umesh_get_index_by_short_name_in_some(umesh%som_element, &
                                                              trim(buf))
                    print *, "Amelet wire index : ", elt_ind
                    print *, "Nec wire index : ", get_index(nec_amelet, elt_ind)

                    ! Resistance value
                    found = read_string_attribute(file_id, link%subject, "R", buf)
                    call read_floatingtype(file_id, trim(buf), ft)
                    nec_load%zlr = ft%singlereal%value
                    print *, "Resistance value : ", nec_load%zlr

                    ! Inductance value
                    found = read_string_attribute(file_id, link%subject, "L", buf)
                    call read_floatingtype(file_id, trim(buf), ft)
                    nec_load%zli = ft%singlereal%value
                    print *, "Inductance value : ", nec_load%zli

                    ! Capacitance value
                    found = read_string_attribute(file_id, link%subject, "C", buf)
                    call read_floatingtype(file_id, trim(buf), ft)
                    nec_load%zlc = ft%singlereal%value
                    print *, "Capacitance value : ", nec_load%zlc

                    ! RLC type
                    found = read_int_attribute(file_id, link%subject, "type", &
                                               nec_load%ldtype, .true.)
                    print *, "RLC model : ", nec_load%ldtype

                    ! RLC Model
                    if (nec_load%ldtype==1) then
                        nec_load%ldtype = 0
                    else if (nec_load%ldtype==8) then
                        nec_load%ldtype = 1
                    endif
                    if (nec_wires(get_index(nec_amelet, elt_ind))%itg == 0) then
                        nec_wires(get_index(nec_amelet, elt_ind))%itg = &
                            generate_tag_wire()
                    endif
                    nec_load%ldtag = 1
                    nec_load%ldtagf = 1
                    nec_load%ldtagt = 1
                endif
            endif
        enddo

        ! Write loads wires to input.nec
        write(numnec, "(a80)"), ld_to_string(nec_load)
        write(numnec, "(a2)"), "PQ"
    end subroutine read_links


In this code, we use many more subroutines and types for the first time.

The type ``selector_on_mesh_node_t`` represents the
``/mesh/$gmesh/$mesh/selectorOnMesh/node`` table :

.. code-block:: fortran

    type selector_on_mesh_node_t
        character(len=EL), dimension(:), allocatable :: short_name
        integer, dimension(:), allocatable :: index
    end type selector_on_mesh_node_t


The type ``selector_on_mesh_element_t`` represents the
``/mesh/$gmesh/$mesh/selectorOnMesh/element`` table :

.. code-block:: fortran

    type selector_on_mesh_element_t
        character(len=EL), dimension(:), allocatable  :: short_name
        integer, dimension(:), allocatable  :: index
        real, dimension(:), allocatable  :: v1, v2, v3
    end type selector_on_mesh_element_t


The type ``group_t`` represents the ``/mesh/$gmesh/$mesh/group`` table :

.. code-block:: fortran

    type group_t
        character(len=AL) :: name = ""
        character(len=EL) :: type = ""
        character(len=EL) :: entity_type = ""
        integer, dimension(:), allocatable :: elements
    end type group_t


Finally, the type ``unstructured_mesh_t`` represents an unstructured
mesh as Amelet-HDF defines it.

.. code-block:: fortran

    type unstructured_mesh_t
        character(len=AL) :: name = ""
        real, dimension(:,:), allocatable :: nodes
        integer, dimension(:), allocatable :: elements
        integer, dimension(:), allocatable :: offsets
        integer, dimension(:), allocatable :: element_nodes
        type(group_t), dimension(:), allocatable :: groups
        type(groupgroup_t), dimension(:), allocatable :: groupgroups
        type(selector_on_mesh_node_t) :: som_node
        type(selector_on_mesh_element_t) :: som_element
    end type unstructured_mesh_t


We use the function ``umesh_get_group_by_name`` to return a reference on
a ``group_t`` in a ``unstructured_mesh_t`` by its name ;

.. code-block:: fortran

    function umesh_get_group_by_name(umesh, path) result(group)
        type(unstructured_mesh_t), target, intent(in) :: umesh
        character(len=*), intent(in) :: path
        type(group_t), pointer :: group

And the function ``umesh_get_index_by_short_name_in_some`` returns index of
an element by its name is a ``selector_on_mesh_element_t`` of a
``unstructured_mesh_t``.

.. code-block:: fortran

    function umesh_get_index_by_short_name_in_some(some, short_name) result(ind)
        type(selector_on_mesh_element_t), intent(in) :: some
        character(len=*), intent(in) :: short_name

A last function ``read_int_attribute`` allows to read an integer 
attribute ``attr`` in ``path``. The result is put in ``buf`` and the function
return ``logical``. If the function return ``false`` the attribute has
not been found.

An optional parameter indicates that the attribute is mandatory (the program
stops) are optional (the program continues).

.. code-block:: fortran

    function read_int_attribute(file_id, path, attr, buf, mandatory) result(here)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path, attr
        integer, intent(inout) :: buf
        logical, intent(in), optional :: mandatory


Now we handle output requests. For this the ``read_output_requests`` 
subroutine is executed :

.. code-block:: fortran

        else if (like(path, "/outputRequest/*")) then
            print *
            print *, "+OutputRequest !!!"
            call read_output_requests(trim(path))
        endif


.. code-block:: fortran

    subroutine read_output_requests(request_group)
        character(len=*), intent(in) :: request_group

        character(len=AL) :: path
        integer :: j, k
        character(len=EL), dimension(:), allocatable :: children_name

        print *
        print *, "  Reading output request ..."

        path = trim(request_group)
        call read_children_name(file_id, trim(path), children_name)
        do j=1, size(children_name)
            path2 = trim(path)//"/"//trim(children_name(j))
            call read_link(file_id, path2, link)
            print *, "--Subject : ", trim(link%subject)
            if (like(link%subject, "/label/predefinedOutputRequests")) then
                print *, "  Object : ", trim(link%object)
                found = read_int_attribute(file_id, path2, "subject_id", ibuf)
                print *, "  Output request : ",  trim(predefined_output_requests(ibuf+1))

                if (predefined_output_requests(ibuf+1) /= "electricField") then
                    print *
                    print *, "Not an electricField output request : ", &
                        predefined_output_requests(ibuf+1)
                    print *, "STOP !!!"
                    stop
                endif

                found = read_string_attribute(file_id, link%object, &
                                              "type", buf)
                ! If electric field request
                if (ibuf==0 .and. like(link%object, "/mesh/*/*/group/*")) then
                    write(*, "(a25)", advance='no') "  Electric field on group"
                    if (buf == "node") then
                        print *, "of nodes"
                    endif
                    ugroup => umesh_get_group_by_name(umesh, link%object)
                    allocate(nec_fields(size(ugroup%elements)))
                    do k=1,size(ugroup%elements)
                        elt_ind = ugroup%elements(k)
                        print *, "Node : ", elt_ind, umesh%nodes(:,elt_ind)
                        nec_fields(k)%near = 0
                        nec_fields(k)%nrx = 1
                        nec_fields(k)%nry = 1
                        nec_fields(k)%nrz = 1
                        nec_fields(k)%xnr = umesh%nodes(1, elt_ind)
                        nec_fields(k)%ynr = umesh%nodes(2, elt_ind)
                        nec_fields(k)%znr = umesh%nodes(3, elt_ind)
                        nec_fields(k)%dxnr = 0
                        nec_fields(k)%dynr = 0
                        nec_fields(k)%dznr = 0
                    enddo
                endif
            endif
        enddo

	    ! Write loads wires to input.nec
	    do i=1,size(nec_fields)
	        write(numnec, "(a80)"), ne_to_string(nec_fields(i))
	    enddo
    end subroutine read_output_requests

There's nothing new in this part. The predefined output request 
``electricField`` is awaited, if it is not found the program stops. Each member
of the output request group is converted is a Nec output request card.


.. code-block:: fortran

    ! End of inputs management
    enddo

