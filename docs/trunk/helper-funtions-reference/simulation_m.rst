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

``(=> read)``


.. code-block:: fortran

    ! Reads a simulation
    subroutine read(file_id, sim_path, sim)
        integer(hid_t), intent(in) :: file_id
        character(len=*), intent(in) :: sim_path
        type(simulation_t), intent(inout) :: sim


``print_simulation``
--------------------

``(=> printt)``

.. code-block:: fortran

    ! Print a simulation
    subroutine printt(sim)
        type(simulation_t), intent(in) :: sim

