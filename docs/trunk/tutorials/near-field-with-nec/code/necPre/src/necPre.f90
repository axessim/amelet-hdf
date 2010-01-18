! ============================================================================
! Name        : necPre.f90
! Author      : Cyril Giraudon
! Version     :
! Copyright   : Your copyright notice
! Description : Hello World in Fortran
! ============================================================================

module nec_model
    implicit none

    type gw_t
        integer :: itg, ns
        real :: xw1, yw1, zw1, xw2, yw2, zw2, rad
    end type gw_t

    type ex_t
        integer :: source_type, tag
        real :: real_part, imaginary_part
    end type ex_t

    type ld_t
        integer :: ldtype, ldtag, ldtagf, ldtagt
        real :: zlr, zli, zlc
    end type ld_t

    type ne_t
        ! Coordinate system 0 -> rectangular coordinates
        integer :: near, nrx, nry, nrz
        real :: xnr, ynr, znr, dxnr, dynr, dznr
    end type ne_t

contains
    function gw_to_string(gw) result(string)
        type(gw_t), intent(in) :: gw
        character(len=80) :: string
        write(string, '(a2,i3,i5,7f10.4)') "GW", gw%itg, gw%ns, &
                                          gw%xw1, gw%yw1, gw%zw1, &
                                          gw%xw2, gw%yw2, gw%zw2, gw%rad
    end function
end module nec_model

program necPre
    use amelethdf
    use nec_model

    implicit none

    character(len=AL) :: command_line_buffer = ""
    character(len=AL) :: filename = ""
    character(len=AL) :: output_folder = ""
    character(len=AL) :: inputnec = "input.nec"

    character(len=AL) :: working_directory, path, path2, path3, buf
    character(len=EL), dimension(:), allocatable :: children_name, &
                                                    children_name2, &
                                                    children_name3, &
                                                    predefined_labels, &
                                                    predefined_output_requests
    integer :: i, j, k, id
    integer, dimension(:), allocatable :: node_numbers
    integer :: numnec, nb_nodes, elt_type, elt_ind
    integer :: node1, node2, nb_wires, id_wires
    logical :: found

    real :: radius

    ! file identifier
    integer(hid_t) :: file_id

    ! Amelet types
    type(simulation_t) :: sim
    type(structured_mesh_t) :: smesh
    type(unstructured_mesh_t) :: umesh
    type(umesh_group_t), pointer :: ugroup
    type(planewave_t) :: pw
    type(floatingtype_t) :: ft
    type(link_t) :: link

    ! Nec structures
    type(gw_t), dimension(:), allocatable :: nec_wires

    ! Write working directory
    call getcwd(working_directory)
    print *, "Working directory : ", working_directory


    ! Arguments handling
    ! Input file name
    call getarg(1, command_line_buffer)
    if (len(command_line_buffer) == 0) then
        call check("Can't read the input file name")
    endif
    filename = trim(command_line_buffer)
    print *, "Input file name : ", filename
    ! output folder name
    command_line_buffer = ""
    call getarg(2, command_line_buffer)
    if (len(command_line_buffer) == 0) then
        call check("Can't read the input file name")
    endif
    output_folder = trim(command_line_buffer)
    print *, "Output folder : ", output_folder

    ! create / open input.nec file
    open(unit=numnec, file=trim(output_folder)//'/'//inputnec, &
         form='formatted', status='replace')
    write(numnec, "(a40)"), "CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA"
    write(numnec, "(a2)"), "CE"



    ! HDF5 library initialization
    hdferr = 0
    call h5open_f(hdferr)
    print *, "HDF5 library initialized"

    print *, "Reading ", trim(filename), " ..."
    call h5fopen_f(filename, H5F_ACC_RDONLY_F, file_id, hdferr, H5P_DEFAULT_F)
    call check("Can't open "//trim(filename))

    ! All categories
    print *
    print *, "Reading categories"
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, "/", children_name)
    do i=1, size(children_name)
        print *, "Category : ", children_name(i)
    enddo

    ! Simulations
    print *
    print *, "Reading simulation ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_SIMULATION, children_name)
    do i=1, size(children_name)
        call readSimulation(file_id, C_SIMULATION//trim(children_name(i)), sim)
        call printSimulation(sim)
    enddo

    ! Meshes
    print *
    print *, "Reading Mesh ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_MESH, children_name)
    do i=1, size(children_name)
        path = trim(C_MESH//children_name(i))
        print *, "Mesh group : ", trim(path)
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, path, children_name2)
        do j=1, size(children_name2)
            path2 = path//"/"//trim(children_name2(i))
            print *
            print *, "Reading ", trim(path2), " ..."
            if (allocated(children_name3)) deallocate(children_name3)
            call read_children_name(file_id, path2, children_name3)
            do k=1, size(children_name3)
                path3 = trim(path2)//"/"//trim(children_name3(i))
                print *
                print *, "Reading ", trim(path3), " ..."
                if (isStructured(file_id, trim(path3))) then
                    call readStructuredMesh(file_id, trim(path3), smesh)
                    call printStructuredMesh(smesh)
                else
                    call readUnstructuredMesh(file_id, trim(path3), umesh)
                    call printUnstructuredMesh(umesh)
                endif
            enddo
        enddo
    enddo

    ! Generate the array containing the number of points per element
    allocate(node_numbers(size(umesh%elements)))
    call umesh_generate_node_numbers(umesh, size(node_numbers), node_numbers)

    ! Electromagnetic sources
    print *
    print *, "Reading Electromagnetic Sources ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_ELECTROMAGNETIC_SOURCE, children_name)
    do i=1, size(children_name)
        path = trim(C_ELECTROMAGNETIC_SOURCE)//trim(children_name(i))
        print *, "Electromagnetic Sources : ", trim(path)
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        if (like(path, C_ELECTROMAGNETIC_SOURCE//C_PLANE_WAVE)) then
            do j=1, size(children_name2)
                path2 = trim(path)//"/"//trim(children_name2(j))
                print *, path2
                call read_planewave(file_id, path2, pw)
                print *, "Theta : ", pw%theta
                print *, "Phi : ", pw%phi
                print *, "Polarization : ", pw%polarization
                print *, "Polarization is linear : ", islinear(pw)
                print *, "Polarization is elliptic : ", iselliptic(pw)
                print *, "Magnitude : ", pw%magnitude%singlecomplex%value
            enddo
        endif
    enddo

    ! Global environment
    print *
    print *, "Reading global environment ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_GLOBAL_ENVIRONMENT, children_name)
    do i=1, size(children_name)
        path = trim(C_GLOBAL_ENVIRONMENT)//trim(children_name(i))
        print *, "Global environment : ", trim(path)
        call read_floatingtype(file_id, trim(path), ft)
        print *, "Value : ", convert_to_real_vector(ft), "Hz"
    enddo

    ! Label
    print *
    print *, "Reading label ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_LABEL, children_name)
    do i=1, size(children_name)
        path = trim(C_LABEL)//trim(children_name(i))
        if (path == "/label/predefinedLabels") then
            if (allocated(predefined_labels)) deallocate(predefined_labels)
            call read_string_vector(file_id, path, predefined_labels)
            print *, "Predefined labels : ", predefined_labels(:)
        else if (path == "/label/predefinedOutputRequests") then
            if (allocated(predefined_output_requests)) then
                deallocate(predefined_output_requests)
            endif
            call read_string_vector(file_id, path, predefined_output_requests)
            print *, "Predefined output requests : ", predefined_output_requests(:)
        else
            if (allocated(children_name2)) deallocate(children_name2)
            call read_string_vector(file_id, path, children_name2)
            print *, "Label : ", children_name2(:)
        endif
    enddo

    ! Link wireRadius -> Give the number of wires
    print *
    print *, "Reading wireRadius links ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_LINK, children_name)
    nb_wires = 0
    do i=1, size(children_name)
        path = trim(C_LINK)//trim(children_name(i))
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        do j=1, size(children_name2)
            path2 = trim(path)//"/"//trim(children_name2(j))
            call read_link(file_id, path2, link)
            ! /label/predefinedLabels#wireRadius handling
            if (link%subject == "/label/predefinedLabels") then
                if (predefined_labels(id+1) == "wireRadius") then
                    ! We take into account only groups
                    if (like(link%object, "/mesh/*/*/group/*")) then
                        print * ,"  Wire radius on group !!"
                        ugroup => umesh_get_group_by_name(umesh, link%object)
                        if (associated(ugroup)) then
                            nb_wires = nb_wires + size(ugroup%elements)
                        endif
                    endif
                endif
            endif
        enddo
    enddo
    print *, "The number of wires is : ", nb_wires
    allocate(nec_wires(nb_wires))

    ! Build nec wire model
    print *
    id_wires = 0
    do i=1, size(children_name)
        path = trim(C_LINK)//trim(children_name(i))
        print *, "Link group : ", trim(path)
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        do j=1, size(children_name2)
            print *, "children name 2 : ", trim(children_name2(j))
            path2 = trim(path)//"/"//trim(children_name2(j))
            print *, "Link : ", trim(path2)
            call read_link(file_id, path2, link)
            print *, "Subject : ", trim(link%subject)
            print *, "Object : ", trim(link%object)
            print *, "Is dataonmesh : ", isdataonmesh(link)
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
                                node1 = umesh%element_nodes(sum(node_numbers(1:elt_ind))+1)
                                node2 = node1 + 1
                                print *, k , elt_ind, elt_type, nb_nodes, &
                                         node1, node2
                                nec_wires(id_wires)%xw1 = umesh%nodes(1,node1+1)
                                nec_wires(id_wires)%yw1 = umesh%nodes(2,node1+1)
                                nec_wires(id_wires)%zw1 = umesh%nodes(3,node1+1)
                                nec_wires(id_wires)%xw2 = umesh%nodes(1,node1+1)
                                nec_wires(id_wires)%yw2 = umesh%nodes(2,node1+1)
                                nec_wires(id_wires)%zw2 = umesh%nodes(3,node1+1)
                                nec_wires(id_wires)%rad = radius
                                write(*, "(7f8.4)"), umesh%nodes(:,node1+1), &
                                                umesh%nodes(:,node2+1), &
                                                radius
                            enddo
                        endif
                    endif
                endif
            endif
        enddo
    enddo


    ! voltage source links -> Give the number of voltage source
    ! Hypothesis : we take into account only one voltage source
    ! -> the first read
    print *
    print *, "Reading voltage source links ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_LINK, children_name)
    nb_wires = 0
    do i=1, size(children_name)
        path = trim(C_LINK)//trim(children_name(i))
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        do j=1, size(children_name2)
            path2 = trim(path)//"/"//trim(children_name2(j))
            call read_link(file_id, path2, link)
            print *, "Subject : ", trim(link%subject)
            if (like(link%subject, "/electromagneticSource/generator/*")) then
                found = read_string_attribute(file_id, link%subject, "type", buf)
                print *, "Type : ", trim(buf)
            endif
        enddo
    enddo





    ! Link
    print *
    print *, "Reading links ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_LINK, children_name)
    do i=1, size(children_name)
        path = trim(C_LINK)//trim(children_name(i))
        print *, "Link group : ", trim(path)
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        do j=1, size(children_name2)
            path2 = trim(path)//"/"//trim(children_name2(i))
            print *, "Link : ", trim(path2)
            call read_link(file_id, path2, link)
            print *, "Subject : ", trim(link%subject)
            print *, "Object : ", trim(link%object)
            print *, "Is dataonmesh : ", isdataonmesh(link)
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
                                elt_ind = ugroup%elements(k)
                                elt_type = umesh%elements(elt_ind+1)
                                nb_nodes = umesh_number_of_nodes(elt_type)
                                node1 = umesh%element_nodes(sum(node_numbers(1:elt_ind))+1)
                                node2 = node1 + 1
                                print *, k , elt_ind, elt_type, nb_nodes, &
                                         node1, node2
                                write(*, *), umesh%nodes(:,node1+1), &
                                         umesh%nodes(:,node2+1)
                            enddo
                        endif
                    endif
                endif
            endif
        enddo
    enddo

    ! Output Request
    print *
    print *, "Reading output request ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_OUTPUT_REQUEST, children_name)
    do i=1, size(children_name)
        path = trim(C_OUTPUT_REQUEST)//trim(children_name(i))
        print *, "Output request group : ", trim(path)
        if (allocated(children_name2)) deallocate(children_name2)
        call read_children_name(file_id, trim(path), children_name2)
        do j=1, size(children_name2)
            path2 = trim(path)//"/"//trim(children_name2(i))
            print *, "Output request : ", trim(path2)
            call read_link(file_id, path2, link)
            print *, "Subject : ", link%subject
            print *, "Object : ", link%object
        enddo
    enddo

    ! Write nec wires to input.nec
    do i=1, nb_wires
        write(numnec, "(a80)"), gw_to_string(nec_wires(i))
    enddo

    ! Physical Models
    print *, "\nReading Physical models ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_PHYSICAL_MODEL, children_name)
    do i=1, size(children_name)
        print *, "Physical models : ", children_name(i)
    enddo

    call h5fclose_f(file_id, hdferr)

    close(numnec)
    print *, "End"
end program necPre
