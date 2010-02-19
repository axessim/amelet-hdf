.. _simulation:

``simulation_m``
================

Constants
---------

.. code-block:: fortran

    C_SIMULATION


Types
-----

.. code-block:: fortran

    type simulation_t
        character(len=AL) :: name = ""
        character(len=AL), dimension(:), allocatable :: inputs, outputs
    end type simulation_t


``read_simulation``
-------------------

Read a ``simulation_t`` at path.

* Fortran interface

    ``(=> read)``

    .. code-block:: fortran

        ! Reads a simulation
        subroutine read(file_id, sim_path, sim)
            integer(hid_t), intent(in) :: file_id
            character(len=*), intent(in) :: sim_path
            type(simulation_t), intent(inout) :: sim

* C interface

    .. code-block:: c
    
        simulation_t read_simulation(hid_t file_id, const char* sim_path)


``print_simulation``
--------------------

Print a ``simulation_t`` to the console.

* Fortran interface 

    ``(=> printt)``

    .. code-block:: fortran

        ! Print a simulation
        subroutine printt(sim)
            type(simulation_t), intent(in) :: sim

* C interface

    .. code-block:: c
    
        void print_simulation(simulation_t sim)
        
