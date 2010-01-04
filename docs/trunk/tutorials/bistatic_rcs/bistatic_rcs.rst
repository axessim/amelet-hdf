Bistatic RCS of a small PEC sphere (MLFMM use case)
===================================================

This tutorial presents a simple simulation use case, the bistatic rcs of a 
small sphere. Several topics are approached :

* Mesh
* Material
* Excitation
* Desired output
* Simulation settings


Input
-----

The simulation is defined by the following items:

* the type of analysis: bistatic RCS analysis (plus near-field),
* the CAD geometry / the mesh: a small PEC sphere at :math:`x = y = z = 0`,
  radius :math:`r = 100mm`,
* the excitation: an x-polarized plane wave travelling into the z-direction,
* the material parameters: perfect electric conductor (PEC),
* the frequency simulation settings: :math:`f = 2.9 . . . 3.1GHz`,
  :math:`\Delta f = 0.1GHz`
* the desired output:

  * the bistatic RCS on the :math:`\phi = 0` and :math:`\phi= 90` cuts,
    :math:`\theta = 0, 1, 2, . . . , 180`.
  * the near-field in the three centered :math:`(220 \times 220)mm^2` planes at 
    :math:`z = -10, 0, 10 mm`.

.. figure:: sphere.png
  :align: center

  A beautiful figure




Amelet-HDF input file
---------------------

In this section, the preceding simulation is adapted to the Amelet-HDF syntax.

First of all, an Amelet-HDF instance is an HDF5 file, so the first thing to do
is create an HDF5 file : ``data.h5`` or ``data.ah5`` (indeed, Amelet-HDF extension
file is `ah5`)

The file has an 'entryPoint' attribute : ``/simulation/bistatic_rcs``. So, 
the file has to be read by beginning with the simulation 
``/simulation/bistatic_rcs``.


The ``/simulation/bistatic_rcs`` simulation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A ``/simulation/bistatic_rcs`` exists, we conclude the name of the simulation
is "bistatic_rcs". The attributes of ``bistatic_rcs`` are :

* ``module``, the name of the module (the solver) equals `MLFMM-MOD`
* ``version`` = 1.0.0, that is module version


The ``bistatic_rcs`` group has two children datasets : ``inputs`` and ``outputs``:

* ``inputs`` : the inputs dataset contains all input information for 
  the simulation. Input information are referenced by their absolute name
  in the Amelet-HDF file.
* ``outputs`` : the outputs dataset contains all output information generated 
  by the simulation. Output information are referenced by their absolute name
  in the Amelet-HDF file.

``inputs`` and ``outputs`` are (1 column x n rows) string datasets.

For the moment, ths Amelet-HDF file looks like :

::

    data.ah5
    `-- simulation
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


The ``/simulation/bistatic_rcs/inputs`` dataset
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``inputs`` dataset is :

+---------------------------------------------------+
| ``/mesh/sphere``                                  |
+---------------------------------------------------+
| ``/physicalModel/perfectlyConducting``            |
+---------------------------------------------------+
| ``/link/sphere_links``                            |
+---------------------------------------------------+
| ``/electromagneticSource/planeWave/x_plane_wave`` |
+---------------------------------------------------+
| ``/outputRequest/field_requests``                 |
+---------------------------------------------------+
| ``/globalEnvironment/ge``                         |
+---------------------------------------------------+

In the ``inputs`` dataset, we find out :

* A mesh object named sphere (``/mesh/sphere``), it is a group, 
  we have to recursively walk through this group to discover all mesh
  characteristic.
* A physical model, in this example a PEC 
  (``/physicalModel/perfectlyConducting``)
* A link (``/link/sphere_links``)
* An electromagnetic source, in this example a plane wave 
  (``/electromagneticSource/planeWave/x_plane_wave``)
* Some output requests (``/outputRequest/field_requests``)
* A global environment definition (``/globalEnvironment/ge``)



The ``/simulation/bistatic_rcs/outputs`` dataset
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``outputs`` dataset contains name of the output information.

The ``inputs`` dataset is :

+---------------------------------------------------+
| ``/arraySet/far_field_theta_phi``                 |
+---------------------------------------------------+
| ``/extensionType/DataSet/near_field``             |
+---------------------------------------------------+


Here we see an arraySet named ``far_field_theta_phi``
(``/arraySet/far_field_theta_phi``) and a 'dataset' named ``near_field``
(``/extensionType/DataSet/near_field``).

Another object ``DataSet`` is defined is ``/extensionType``because 
it is not defined in the Amelet-HDF format. What we see is that it
contains some arraySet by the ``linksDefinition`` element usage.

The ``linksDefinition`` dataset looks like :

+---------------------------------------------------+
| ``/arraySet/near_field_plane1``                   |
+---------------------------------------------------+
| ``/arraySet/near_field_plane2``                   |
+---------------------------------------------------+
| ``/arraySet/near_field_plane3``                   |
+---------------------------------------------------+


.. warning:: 
    ``/extensionType/DataSet`` is not the same object than a 
    ``floatingType`` = ``dataSet``.
    In fact, ``/extensionType/DataSet`` is a framework infotype which can contain
    many arraySets. This trick permits to shape the output (an Infotype/ArraySet 
    and a InfoType/DataSet containing three Infotype/ArraySet) 
    returned to the platform.

::

    data.ah5
    |-- arraySet/
    |   |-- far_field_theta_phi
    |   |-- near_field_plane1
    |   |-- near_field_plane2
    |   `-- near_field_plane3
    |-- extensionType/
    |   `-- DataSet/
    |       `-- linksDefinition
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


Mesh
^^^^

The first line of ``/simulation/bistatic_rcs/inputs`` indicates
``/mesh/sphere`` is used in the simulation.

In ``/mesh/sphere`` we find out an HDF5 group named ``surfaces`` 
(``/mesh/sphere/surfaces``). This group has the attribute
``type`` with the value ``unstructured``. So this is an unstructured mesh. 

In particular, this mesh has four groups :

* ``faceGroup``, it is a face element group
* ``plane1``, it is a node group
* ``plane2``, it is a node group
* ``plane3``, it is a ndoe group


::

    data.ah5
    |-- mesh/
    |   `-- sphere
    |       `-- surfaces
    |           |-- nodes
    |           |-- elementTypes
    |           |-- elementNodes
    |           `-- group
    |               |-- faceGroup
    |               |-- plane1
    |               |-- plane2
    |               `-- plane3  
    |-- arraySet/
    |   |-- far_field_theta_phi
    |   |-- near_field_plane1
    |   |-- near_field_plane2
    |   `-- near_field_plane3
    |-- extensionType/
    |   `-- DataSet/
    |       `-- linksDefinition
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs

Physical Models
^^^^^^^^^^^^^^^

In this example, only one material model is taken into account : PEC. In
Amelet-HDF, PEC is a predefined material model called 
``/physicalModel/perfectlyConducting``.


Electromagnetic sources
^^^^^^^^^^^^^^^^^^^^^^^

The electromagnetic source is a plane wave named ``x_plane_wave``: 
``/electromagneticSource/planeWave/x_plane_wave``.
The attributes of ``x_plane_wave`` give the position of the null phase point 
(xo, yo, zo), the direction and the polarization of the incident plane wave. 
Angle definitions are defined as shown in chapter 8.2 of Amelet-HDF
specification.

The magnitude of the plane wave is defined in the group ``magnitude`` inside 
``x_plane_wave``.
The reading of the attribute ``magnitude`` give the following information :

* Value of magnitude : ``1+0j``,
* Physical nature : ``electricField`` i.e. electric field,
* Unit : ``voltPerMetter`` equivalent tot V/m.

::

    data.ah5
    |-- electromagneticSource
    |   `-- planeWave
    |       `-- x_plane_wave[@xo=0
    |           |            @yo=0
    |           |            @zo=0
    |           |            @theta=
    |           |            @phi=
    |           |            @linearPolarization=]
    |           `-- magnitude[@floatingType=singleComplex
    |                         @physicalNature=electricField
    |                         @unit=voltPerMeter
    |                         @value=(1,0)]
    |-- physicalModel/
    |   `-- perfeclyConducting
    |-- mesh/
    |   `-- sphere
    |       `-- surfaces
    |           |-- nodes
    |           |-- elementTypes
    |           |-- elementNodes
    |           `-- group
    |               |-- faceGroup
    |               |-- plane1
    |               |-- plane2
    |               `-- plane3  
    |-- arraySet/
    |   |-- far_field_theta_phi
    |   |-- near_field_plane1
    |   |-- near_field_plane2
    |   `-- near_field_plane3
    |-- extensionType/
    |   `-- DataSet/
    |       `-- linksDefinition
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


Links
^^^^^

The third line of ``/simulation/bistatic_rcs/inputs`` indicates that
the links are defined in ``/link/sphere_links`` and there are two links inside 
``sphere_links`` :

* ``incident_field``
* ``skin_material``

The meaning of links is given by the attributes, here we have :

* ``incident_field`` : a link between the subject `'x_plane_wave`` and 
  the unstructured mesh object ``/mesh/sphere/surfaces``
* ``skin_material`` : it is a link between the subject 
  ``/physicalModel/perfectlyConducting`` and the object
  ``/mesh/sphere/surfaces/group/faceGroups`` (a group of the unstructured mesh).
  This group is the sphere.



The desired outputs
^^^^^^^^^^^^^^^^^^^

The ``/outputRequest`` category defines the desired output for the simulation. 
The four line of ``/simulation/bistatic_rcs/inputs`` indicates that the
simulation use the ``/outputRequest/field_requests`` object.

So the output resquest of the simulation are defined in 
``/outputRequest/field_requests``.

Inside we find out four output requests :

* ``far_field_theta_phi``
* ``near_field_plane1``
* ``near_field_plane2``
* ``near_field_plane3``

Output request are defined like links, but the ``subject`` attribute are often
simple string. That's the link subject comes from the ``/label`` children 
datasets which contain labels definition useful for the simulation.

In particular, output request label are defined in 
``/label/output_request_label`` :

+---------------------------------------------------+
| ``near field``                                    |
+---------------------------------------------------+
| ``far field``                                     |
+---------------------------------------------------+



The ``far_field_theta_phi`` output request
##########################################

This output request has the name ``far_field_theta_phi``.

``far_field_theta_phi`` has the following common attributes :

* ``subject`` : ``/label/output_request_label``, the subject identifier is
  contained ``/label/output_request_label``.
* ``subject_id`` : 1, the subject identifier is the second element 
  of ``subject``, it is a far field computation request.
* ``object`` : "", the output request is global
* ``output`` : ``/arraySet/far_field_theta_phi``, result will be stored in 
  ``/arraySet/far_field_theta_phi``

The list of output request types can be chosen in a predefined list, 
which exists in the Amelet-HDF, or can be created by the module integrator.

Others attribute specific to this output request are added, they are not
specified in Amelet-HDF, they are free and understable only by the
module integrator :

* ``theta_min`` = 0.0
* ``theta_max`` = 180
* ``nb_theta`` = 181
* ``phi_min`` = 0 
* ``phi_max`` = 90
* ``nb_phi`` = 2


The ``near_field_plane1`` output request
########################################

This output request has the name ``near_field_plane1``. 

``near_field_plane1`` has the following common attributes :

* ``subject`` : ``/label/output_request_label``, the subject identifier is
  contained ``/label/output_request_label``.
* ``subject_id`` : 0, the subject identifier is the first element 
  of ``subject``, it is a near field computation request.
* ``object`` : ``/arraySet/near_field_plane1/group/plane1``, the computation is apply
  on nodes of the ``/arraySet/near_field_plane1/group/plane1`` group.
* ``output`` : ``/arraySet/near_field_plane1``, result will be stored in 
  ``/arraySet/near_field_plane1``


The ``globalEnvironment`` object
################################

Finally, in ``/globalEnvironment``, we find out the global environment of 
the simulation (time duration, frequency spectrum ...). For ``bistatic_rcs``,
the input global environment is ``/globalEnvironment/ge`` as mentioned 
at the end of ``/simulation/bistatic_rcs/inputs``.

``/globalEnvironment/ge`` contains a ``frequency`` dataset 
(``floatingType`` = ``vector``) in which we find out three values : 
:math:`2.9 \times 10^9, 3 \times 10^9` and :math:`3,1 \times 10^9`, there are 
the three frequencies at which the computation will be performed.

Finally the simulation instance looks like :

::

    data.ah5
    |-- globalEnvironment/
    |   `-- ge
    |       `-- frequency[@floatingType=vector
    |                     @physicalNature=frequency
    |                     @unit=Hz]
    |-- outputRequest/
    |   `-- field_requests
    |       |-- near_field_plane1[@subject=/label/output_request_label
    |       |                     @subject_id=0
    |       |                     @object=/mesh/sphere/surfaces/group/plane1
    |       |                     @output= /arraySet/near_field_plane1]
    |       |-- near_field_plane2[@subject=/label/output_request_label
    |       |                     @subject_id=0
    |       |                     @object=/mesh/sphere/surfaces/group/plane2
    |       |                     @output= /arraySet/near_field_plane2]
    |       |-- near_field_plane3[@subject=/label/output_request_label
    |       |                     @subject_id=0
    |       |                     @object=/mesh/sphere/surfaces/group/plane3
    |       |                     @output= /arraySet/near_field_plane3]
    |       `-- far_field_theta_phi[@subject=/label/output_request_label
    |                               @subject_id=1
    |                               @object=""
    |                               @theta_min=0.0
    |                               @theta_max=180.
    |                               @nb_theta=181
    |                               @phi_min=0.0
    |                               @phi_max=90.
    |                               @nb_phi=2
    |                               @output=/arraySet/far_field_theta_phi]
    |-- links/
    |   `-- sphere_links/
    |       |-- incident_field[@subject=/electromagneticSource/planeWave/x_plane_wave]
    |       |                  @object=/mesh/sphere/surfaces]
    |       `-- skin_material[@subject=/physicalModel/perfectlyConducting
    |                         @object=/mesh/sphere/surfaces/group/faceGroup]
    |-- electromagneticSource/
    |   `-- planeWave/
    |       `-- x_plane_wave[@xo=0
    |           |            @yo=0
    |           |            @zo=0
    |           |            @theta=0.0
    |           |            @phi=0.0
    |           |            @linearPolarization=90.]
    |           `-- magnitude[@floatingType=singleComplex
    |                         @physicalNature=electricField
    |                         @unit=voltPerMeter
    |                         @value=(1,0)]
    |-- physicalModel/
    |   `-- perfeclyConducting
    |-- mesh/
    |   `-- sphere/
    |       `-- surfaces/
    |           |-- nodes
    |           |-- elementTypes
    |           |-- elementNodes
    |           `-- group
    |               |-- faceGroup[@type=element
    |               |             @entityType=face]
    |               |-- plane1[@type=node]
    |               |-- plane2[@type=node]
    |               `-- plane3[@type=node]  
    |-- arraySet/
    |   |-- far_field_theta_phi
    |   |-- near_field_plane1
    |   |-- near_field_plane2
    |   `-- near_field_plane3
    |-- extensionType/
    |   `-- DataSet/
    |       `-- linksDefinition
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


