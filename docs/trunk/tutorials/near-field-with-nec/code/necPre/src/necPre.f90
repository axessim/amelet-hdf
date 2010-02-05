! ============================================================================
! Name        : necPre.f90
! Author      : Cyril Giraudon
! Version     :
! Copyright   : Your copyright notice
! Description : necPre converter in Fortran
! ============================================================================

! Nec model card module
! Nec cards are modeled as fortran types and are accompanied by
! serialisation function
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
    integer :: numnec = 1, nb_nodes, elt_type, elt_ind, ibuf
    integer :: node1, node2, nb_wires, id_wires
    logical :: found

    real :: radius
    real, dimension(1) :: frequency

    ! input.h5 file identifier
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
    ! The only generator
    type(ex_t) :: nec_generator
    ! The only load
    type(ld_t) :: nec_load
    ! Near field output request
    type(ne_t), dimension(:), allocatable :: nec_fields
    ! Link between amelet wires and new nec wires
    integer, dimension(:), allocatable :: nec_amelet


    ! Some initializations
    umesh%name = ""

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
    open(unit=numnec, file=join((/output_folder, inputnec/)), &
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

    ! Read the simulation instance
    print *
    print *, "Reading simulation ..."
    if (allocated(children_name)) deallocate(children_name)
    call read_children_name(file_id, C_SIMULATION, children_name)
    do i=1, size(children_name)
        call readSimulation(file_id, C_SIMULATION//trim(children_name(i)), sim)
        call printSimulation(sim)
        print *
        print *, "--Handle inputs ..."
        ! We read inputs except links
        do j=1, size(sim%inputs)
            path = sim%inputs(j)
            if (like(path, "/mesh/*")) then
                print *, "+A mesh !!! : ", trim(path)
                if (allocated(children_name2)) deallocate(children_name2)
                call read_children_name(file_id, trim(path), children_name2)
                path2 = trim(path)//"/"//trim(children_name2(1))
                call readUnstructuredMesh(file_id, trim(path2), umesh)
                !call printUnstructuredMesh(umesh)
                ! Generate the array containing the number
                ! of points per element
                call umesh_generate_offsets(umesh)
            else if (like(path, "/electromagneticSource/generator/*")) then
                print *, "+A generator !!!"
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
            else if (like(path, "/physicalModel/multiport/RLC/*")) then
                print *, "+RLC !!!"
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
        ! Now we read links & output requests
        print *
        print *, "--Handle links & outputRequests ..."
        do j=1, size(sim%inputs)
            path = sim%inputs(j)
            print *
            print *, "Sim inputs : ", trim(path)
            if (like(path, "/link/*")) then
                print *
                print *, "+Links !!! : ", trim(path)
                call read_links(trim(path))
            else if (like(path, "/outputRequest/*")) then
                print *
                print *, "+OutputRequest !!!"
                call read_output_requests(trim(path))
            endif
        enddo
    enddo

    write(numnec, "(a2)"), "EN"

    ! Close the input.h5
    call h5fclose_f(file_id, hdferr)

    ! Close the generated input.nec file
    close(numnec)
    print *, "End"

contains
    ! From an integer array return the index
    ! matching value
    function get_index(array, value) result(ind)
        integer, dimension(:), intent(in) :: array
        integer, intent(in) :: value
        integer :: i, ind

        ind = 0
        do i=1, size(array)
            if (array(i) == value) then
                ind = i
                exit
            endif
        enddo
    end function get_index

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
                            nb_wires = nb_wires + size(ugroup%elements)
                        endif
                    endif
                endif
            endif
        enddo

        print *
        print *, "The number of wires is : ", nb_wires
        allocate(nec_wires(nb_wires))
        allocate(nec_amelet(nb_wires))

        ! Build nec wire model
        print *
        id_wires = 0
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

        ! voltage source links -> Give the number of voltage source
        ! Hypothesis : we take into account only one voltage source
        ! -> the first read
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


        ! load links -> Give the number of loads
        ! Hypothesis : we take into account only one load RLC model
        ! -> the first read
        print *
        print *, "Reading load source links ..."
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

    ! Output Request
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
end program necPre
