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


* The results of the simulation are complex data:

  * the bisatic RCS is function of :

    * the component E\ :math:`\phi` and E\ :math:`\theta`,
    * frequency, 
    * :math:`\phi` observer angle,
    * :math:`\theta` observer angle,
    * :math:`\phi` wave plane incident angle,
    * :math:`\theta` wave plane incident observer angle.

  * the near-field is function of :

    * the position of measurement nodes,
    * component ( E :sub:`x`, E :sub:`y`, E :sub:`z`, H :sub:`x`, H :sub:`y`, H :sub:`z` )
    * frequency,
    * :math:`\phi` wave plane incident angle,
    * :math:`\theta` wave plane incident observer angle.


.. figure:: sphere.png
  :align: center

  The sphere

Amelet-HDF input file
---------------------

In this section, the preceding simulation is adapted to the Amelet-HDF syntax.

First of all, an Amelet-HDF instance is an HDF5 file, so the first thing to do
is create an HDF5 file : ``input.h5`` or ``input.ah5`` (indeed, Amelet-HDF extension
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

    input.ah5
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

The ``outputs`` dataset is :

+---------------------------------------------------+
| ``/floatingType/far_field_theta_phi``             |
+---------------------------------------------------+
| ``/group/near_field``                             |
+---------------------------------------------------+


Here we see an arraySet named ``far_field_theta_phi``
(``/floatingType/far_field_theta_phi``) and a 'dataset' named ``near_field``
(``/group/near_field``).


A group object contains only element names which have something in common. In 
our case, the near field on plane 1, 2 and 3 is an electromagnetic field, so 
we have electric field and magnetic field. We can not mixed different physical 
nature of data in an amelethdf arrayset. We should create an arrayset for 
electric field and another arrayset for magnetic field. So we create a group 
object wich contains the path to the arrayset electric field on plane 1 and the 
path to the the magnetic field on the same plane.

The ``/group/near_field`` dataset looks like :

+---------------------------------------------------+
| ``/floatingType/Enear_field_plane1``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane1``              |
+---------------------------------------------------+
| ``/floatingType/Enear_field_plane2``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane2``              |
+---------------------------------------------------+
| ``/floatingType/Enear_field_plane3``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane3``              |
+---------------------------------------------------+

::

    input.ah5
    |-- floatingType/
    |   |-- far_field_theta_phi
    |   |-- Enear_field_plane1
    |   |-- Hnear_field_plane1
    |   |-- Enear_field_plane2
    |   |-- Hnear_field_plane2
    |   |-- Enear_field_plane3
    |   `-- Hnear_field_plane3
    |-- group/
    |   `-- near_field
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
* ``plane3``, it is a node group


::

    input.ah5
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
    |-- floatingType/
    |   |-- far_field_theta_phi
    |   |-- Enear_field_plane1
    |   |-- Hnear_field_plane1
    |   |-- Enear_field_plane2
    |   |-- Hnear_field_plane2
    |   |-- Enear_field_plane3
    |   `-- Hnear_field_plane3
    |-- group/
    |   `-- near_field
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

    input.ah5
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
    |-- floatingType/
    |   |-- far_field_theta_phi
    |   |-- Enear_field_plane1
    |   |-- Hnear_field_plane1
    |   |-- Enear_field_plane2
    |   |-- Hnear_field_plane2
    |   |-- Enear_field_plane3
    |   `-- Hnear_field_plane3
    |-- group/
    |   `-- near_field
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
* ``output`` : ``/floatingType/far_field_theta_phi``, result will be stored in 
  ``/floatingType/far_field_theta_phi``

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
* ``object`` : ``/mesh/sphere/surfaces/group/plane1``, the computation is apply
  on nodes of the ``/mesh/sphere/surfaces/group/plane1`` group.
* ``output`` : ``/group/near_field``, result will be stored in the 
  emplacement defined in ``/group/near_field``


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

    input.ah5
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
    |       |                     @output= /group/near_field]
    |       |-- near_field_plane2[@subject=/label/output_request_label
    |       |                     @subject_id=0
    |       |                     @object=/mesh/sphere/surfaces/group/plane2
    |       |                     @output= /group/near_field]
    |       |-- near_field_plane3[@subject=/label/output_request_label
    |       |                     @subject_id=0
    |       |                     @object=/mesh/sphere/surfaces/group/plane3
    |       |                     @output= /group/near_field]
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
    |-- group/
    |   `-- near_field
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


Amelet-HDF output files
-----------------------

In this section, the result of the simulation is adapted to the Amelet-HDF syntax.

The output file is output.ah5. The file has an 'entry-Point' attribute : 
``/simulation/bistatic_rcs``. So, the file has to be read by beginning with the 
simulation ``/simulation/bistatic_rcs``.

The ``/simulation/bistatic_rcs`` simulation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A ``/simulation/bistatic_rcs`` exists, we conclude the name of the simulation
is "bistatic_rcs". The attributes of ``bistatic_rcs`` are :

* ``module``, the name of the module (the solver) equals `MLFMM-MOD`
* ``version`` = 1.0.0, that is module version


The ``bistatic_rcs`` group has one children dataset : ``outputs``:

* ``outputs`` : the outputs dataset contains all output information generated 
  by the simulation. Output information are referenced by their absolute name
  in the Amelet-HDF file.

``outputs`` is (1 column x n rows) string datasets.

For the moment, ths Amelet-HDF file looks like :

::

    output.ah5
    `-- simulation
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            `-- outputs



The ``/simulation/bistatic_rcs/outputs`` dataset
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``outputs`` dataset contains name of the output information.

The ``outputs`` dataset is :

+---------------------------------------------------+
| ``/floatingType/far_field_theta_phi``             |
+---------------------------------------------------+
| ``/group/near_field``                             |
+---------------------------------------------------+


Here we see an arraySet named ``far_field_theta_phi``
(``/floatingType/far_field_theta_phi``) and a 'dataset' named ``near_field``
(``/group/near_field``).


A group object contains only element names which have something in common. In 
our case, the near field on plane 1, 2 and 3 is an electromagnetic field, so 
we have electric field and magnetic field. We can not mixed different physical 
nature of data in an amelethdf arrayset. We should create an arrayset for 
electric field and another arrayset for magnetic field. So we create a group 
object wich contains the path to the arrayset electric field on plane 1 and the 
path to the the magnetic field on the same plane.

The ``/group/near_field`` dataset looks like :

+---------------------------------------------------+
| ``/floatingType/Enear_field_plane1``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane1``              |
+---------------------------------------------------+
| ``/floatingType/Enear_field_plane2``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane2``              |
+---------------------------------------------------+
| ``/floatingType/Enear_field_plane3``              |
+---------------------------------------------------+
| ``/floatingType/Hnear_field_plane3``              |
+---------------------------------------------------+

::

    output.ah5
    |-- group/
    |   `-- near_field
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            `-- outputs

In our case arraysets are not stored in the output.ah5, so a 'externalElement' 
is present.


The ``externalElement`` object
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We find out the location of the arraySet in '/externalElement/externalArraySet' 
dataset.

The dimension of 'externalArraySet' is 7 x 3 :

* The first column contains elements names used in the Amelet HDF instance but
  defined in another (external) Amelet HDF instance.
* The second column contains the file’s name the element is defined in.
* The third column contains the name of the elements in the external file.

Each arrayset defined in '/group/near_field' and the /floatingType/far_field_theta_phi
are stored in one file.

The ``/externalElement/externalArraySet`` dataset looks like :

+------------------------------------+---------------------------+------------------------------------+
| /floatingType/far_field_theta_phi  | farfield.h5               | /floatingType/far_field_theta_phi  |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Enear_field_plane1   | monitor_plane1_efield.h5  | /floatingType/Enear_field_plane1   |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Hnear_field_plane1   | monitor_plane1_hfield.h5  | /floatingType/Hnear_field_plane1   |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Enear_field_plane2   | monitor_plane2_efield.h5  | /floatingType/Enear_field_plane2   |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Hnear_field_plane2   | monitor_plane2_hfield.h5  | /floatingType/Hnear_field_plane2   |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Enear_field_plane3   | monitor_plane3_efield.h5  | /floatingType/Enear_field_plane3   |
+------------------------------------+---------------------------+------------------------------------+
| /floatingType/Hnear_field_plane3   | monitor_plane3_hfield.h5  | /floatingType/Hnear_field_plane3   |
+------------------------------------+---------------------------+------------------------------------+

Results are stored in farfield.h5, monitor_plane1_efield.h5, monitor_plane1_hfield.h5, 
monitor_plane2_efield.h5, monitor_plane2_hfield.h5, monitor_plane3_efield.h5 and 
monitor_plane3_hfield.h5 files.

Finally the output.ah5 looks like:

::

    output.ah5
    |-- externalElement
    |   `-- externalArraySet
    |-- group/
    |   `-- near_field
    `-- simulation/
        `-- bistatic_rcs[@module=MLFMM-MOD
            |            @version=1.0.0]
            |-- inputs
            `-- outputs


The external file farfield.h5
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The result of the farfield calculation is stored in ``/floatingType/far_field_theta_phi``
object.

The ``/floatingType/far_field_theta_phi`` arraySet
##################################################

The attribute of ``/floatingType/far_field_theta_phi`` is floatingType and
the value is arraySet. So ``/floatingType/far_field_theta_phi`` is an arraySet.

An arraySet is defined by :

* data, an HDF5 multidimensionnal dataset
* a “ds” group (dimension scale) that contains the HDF5 dataset representing data dimensions.

``/floatingType/far_field_theta_phi/data`` has 6 dimensions, so in 
``/floatingType/far_field_theta_phi/ds``, there are 6 children (dim1 to dim6).

.. warning::
  The names dim1 ... dimN is a Amelet HDF convention and are the only authorized names.
  The first dimension is the most internal loop of data.

The attributes of data and dimN describes the physcial nature of each element.
So for ``/floatingType/far_field_theta_phi`` we have :

* data is a 6 dimensionnal complex dataset:

  * physcicalNature = ``electricField``
  * unit = ``volt``
  * label = ``Farfield``

* dim1:

  * label = ``Electric Field``
  * values are [ "Ephi" , "Etheta" ]

* dim2:

  * physcicalNature = ``frequency``
  * unit = ``hertz``
  * label = ``frequency``
  * values are [ 2.9e9 , 3.0e9 , 3.1e9 ]

* dim3:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Phi_o``
  * values are [ 0 , 90 , 180 , 270 ]

* dim4:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Theta_o``
  * values are [ 0.0 , 1.0 , 2.0 , ... , 179.0 , 180.0 ]

* dim5:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Phi_i``
  * values are [ 0.0 ]

* dim6:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Theta_i``
  * values are [ 180.0 ]

So the farfield.h5 looks like this :

::


    farfield.h5
    `-- floatingType
        `-- far_field_theta_phi
            |-- data
            `-- ds
                |-- dim1
                |-- dim2
                |-- dim3
                |-- dim4
                |-- dim5
                `-- dim6    


The external file monitor_plane1_efield.h5
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The result of the near electric field calculation on the plane1 is stored in 
``/floatingType/Enear_field_plane1`` object.

The ``/floatingType/Enear_field_plane1`` arraySet
#################################################

The attribute of ``/floatingType/Enear_field_plane1`` is floatingType and
the value is arraySet. So ``/floatingType/Enear_field_plane1`` is an arraySet.


The arraySet is defined by :

* data, an HDF5 multidimensionnal dataset
* a “ds” group (dimension scale) that contains the HDF5 dataset representing data dimensions.

``/floatingType/Enear_field_plane1/data`` has 5 dimensions, so in 
``/floatingType/Enear_field_plane1/ds``, there are 5 children (dim1 to dim5).

.. warning::
  The names dim1 ... dimN is a Amelet HDF convention and are the only authorized names.
  The first dimension is the most internal loop of data.

The arraySet contains electric field value on a nodes group. In Amelet HDF this 
association is handled by a dim* child of the arraySet. The physicalNature of 
this dimension must be meshEntity and the meshEntity attribute give the name of 
the mesh support.

For this arrayset the mesh support is ``/mesh/sphere/surfaces/group/plane1``. So in this file
we found the arrayset and the mesh which supports the data.

So for ``/floatingType/Enear_field_plane1`` we have :

* data is a 5 dimensionnal complex dataset:

  * physcicalNature = ``electricField``
  * unit = ``voltPerMeter``
  * label = ``Nearfield``

* dim1:

  * physicalNature = ``meshEntity``
  * meshEntity = ``/mesh/sphere/surfaces/group/plane1``

* dim2:

  * physcicalNature = ``component``
  * label = ``FIELD_COMPONENT``
  * values are [ "Ex" , "Ey" , "Ez" ]

* dim3:

  * physcicalNature = ``frequency``
  * unit = ``hertz``
  * label = ``frequency``
  * values are [ 2.9e9 , 3.0e9 , 3.1e9 ]

* dim4:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Phi_i``
  * values are [ 0.0 ]

* dim5:

  * physcicalNature = ``angle``
  * unit = ``degree``
  * label = ``Theta_i``
  * values are [ 180.0 ]

So the monitor_plane1_efield.h5 looks like this :

::


    monitor_plane1_efield.h5
    |-- floatingType
    |   `-- Enear_field_plane1
    |       |-- data
    |       `-- ds
    |           |-- dim1
    |           |-- dim2
    |           |-- dim3
    |           |-- dim4
    |           |-- dim5
    `-- mesh
        `-- surfaces
            |-- group
            |   `-- plane1            
            `-- nodes

