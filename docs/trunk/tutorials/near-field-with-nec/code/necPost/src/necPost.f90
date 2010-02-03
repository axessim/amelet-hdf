! ============================================================================
! Name        : necPost.f90
! Author      : Cyril Giraudon
! Version     :
! Copyright   : Your copyright notice
! Description : Write an output simulation file
! ============================================================================

module tools
    use amelethdf
    use h5lt

  contains
    ! Print floatingType attributes in path
    subroutine set_floating_type(file_id, path, label, physical_nature, unit, comment)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: path
        character(len=*), intent(in) :: label, physical_nature, unit, comment

        ! Writes /floatingType/an_arrayset/data attributes
        call h5ltset_attribute_string_f(file_id, trim(path), "label", &
                                        trim(label), hdferr)
        call check("Can't write label for "//path)

        call h5ltset_attribute_string_f(file_id, trim(path), "physicalNature", &
                                        trim(physical_nature), hdferr)
        call check("Can't write physical nature for "//path)

        call h5ltset_attribute_string_f(file_id, trim(path), "unit", &
                                        trim(unit), hdferr)
        call check("Can't write unit for "//path)

        call h5ltset_attribute_string_f(file_id, trim(path), "comment", &
                                        trim(comment), hdferr)
        call check("Can't write comment for "//path)
    end subroutine set_floating_type

    pure function deg2rad(degree) result(radian)
        real, intent(in) :: degree

        real, parameter :: confac = 3.141593/180.0
        real :: radian

        radian = degree*confac
    end function deg2rad
end module tools

! This program write an HDF5 file with the following arraySet structure
!
! data.h5
! |-- simulation
! |   `-- outputs
! `-- floatingType
!     `-- an_arrayset
!         |-- data[@physicaNature=electricField
!         |        @unit=voltPerMeter]
!         `-- ds
!             |-- dim1[@physicalNature=length
!             |        @unit=meter]
!             `-- dim2[@physicalNature=length
!                      @unit=meter]
!
!    real, dimension(10, 10) :: data
!    real, dimension(10) :: dim1, dim2
!
program necPost
    use tools

    implicit none

    integer, parameter :: necfile = 1
    integer(hid_t) :: file_id, grp_id
    character(len=AL) :: command_line_buffer
    character(len=AL) :: working_dir, filename, path
    character(len=2*AL) :: line
    integer(hsize_t), dimension(:), allocatable :: dims
    integer :: line_number, line_index, nb_fields, i, offset
    real, dimension(:, :), allocatable :: fields_buffer
    complex, dimension(:, :), allocatable :: fields
    real :: x, y, z, ex_m, ex_p, ey_m, ey_p, ez_m, ez_p

    real, dimension(10, 10) :: data
    real, dimension(10) :: dim1, dim2
    character(len=AL) :: label, physical_nature, unit, comment
    character(len=AL), dimension(:), allocatable :: outputs
    character(len=AL), dimension(:), allocatable :: buf


    ! HDF5 library initialization
    call h5open_f(hdferr)
    print *, "HDF5 library initialized"


    ! Command line reading
    ! The simulation working dirrectory
    call getarg(1, command_line_buffer)
    if (len(command_line_buffer) == 0) then
        call check("Can't read the simulation working directory")
    endif
    working_dir = trim(command_line_buffer)
    print *, "Working directory : ", trim(working_dir)
    ! The output.h5 file name
    call getarg(2, command_line_buffer)
    if (len(command_line_buffer) == 0) then
        call check("Can't read the output file name")
    endif
    filename = trim(command_line_buffer)
    print *, "Output Amelet file : ", trim(filename)


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

    ! Output file generation
    ! File creation
    print *
    print *, "Create ", trim(filename), " ..."
    call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr, &
                     H5P_DEFAULT_F, H5P_DEFAULT_F)
    call check("Can't create "//trim(filename))

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
    dims = (/10/)
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


    ! close filename
    call h5fclose_f(file_id, hdferr)
    call check("Can't close "//trim(filename))

    ! close HDF5 library
    call h5close_f(hdferr)
    call check("Can't close HFD5")
end program necPost
