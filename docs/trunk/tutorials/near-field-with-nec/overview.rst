Overview
========

This tutorial aims at showing how to compute the electric field alongside 
a dipole with the well known Nec2 software within the Quercy platform.

The use case is based upon the first example of Nec2 distribution.

A 0.5m length dipole is modeled with two linked points :

* the first point is at (x=0  y=0  z=-0.25 in meter)
* the second point is at (x=0  y=0  z=0.25 in meter)

The wire radius is 0.001m and finally the dipole is discretized in 7 segments.
The source is a voltage source (1V) located on the fourth segment.

A load serial impedance is also located on the fourth segment :

* The resistance is R = 10 Ohms
* The inductance is L = 3e-9 Henries
* The capacitance is C = 5.3e-11 Farads

A near electric fields calculation is requested on 15 points from the point of
coordinates (x = 0.001, y = 0, z = 0) and with a dz=0.1786m z step.

The frequency is 299.8 MHz.



Prerequisites
=============

Nec
---

The Nec product can be downloaded at http://www.si-list.net/swindex.html



Python
------

In order to introduce the python language and to show how it fits well with the 
task we will generate input.h5 
in python ( http://www.python.org  select at least the version 2.5.4 ). 

Two python modules must be present on the system :

* numpy : numpy is a module to handle multidimensional matrices (à la Matlab)
  numpy 1.3 can be found out at http://sourceforge.net/projects/numpy/files.
* pytables : pytables permits to read/write HDF5 file in a very easy way.
  pytables 2.1.2 can be found out at http://www.pytables.org/download/stable/

Python module version must match with the version of the python interpreter.



The Amelet-HDF input file
=========================

We are going the build an unstructured mesh.

The nodes
---------

The wire is made up 7 segments (length = 0.5/7 = 0.07143m)  
so there are 8 points. The wire is oriented toward the Z axis with
x=0m and y=0m.

* p1 = (x=0  y=0  z=-0.25)
* p2 = (x=0  y=0  z=-0.1785)
* p3 = (x=0  y=0  z=-0.1071)
* p4 = (x=0  y=0  z=-0.0357)
* p5 = (x=0  y=0  z=0.0357)
* p6 = (x=0  y=0  z=0.1071)
* p7 = (x=0  y=0  z=0.1785)
* p8 = (x=0  y=0  z=0.25)

Then 15 points for the near fields on the line (x=0.001  y=0) from z=0
with dz=0.1786 15th times :

* p9 =  (x=0.001  y=0  z=-0)
* p10 = (x=0.001  y=0  z=0.0179)
* p11 = (x=0.001  y=0  z=0.0357)
* p12 = (x=0.001  y=0  z=0.0536)
* p13 = (x=0.001  y=0  z=0.0714)
* p14 = (x=0.001  y=0  z=0.0893)
* p15 = (x=0.001  y=0  z=0.1072)
* p16 = (x=0.001  y=0  z=0.1250)
* p17 = (x=0.001  y=0  z=0.1429)
* p18 = (x=0.001  y=0  z=0.1607)
* p19 = (x=0.001  y=0  z=0.1786)
* p20 = (x=0.001  y=0  z=0.1965)
* p21 = (x=0.001  y=0  z=0.2143)
* p22 = (x=0.001  y=0  z=0.2322)
* p23 = (x=0.001  y=0  z=2.5004)

In python code :

.. code-block:: python

    # import the numpy module into the session
    import numpy as np
    # import the pytables module into the session
    import tables as t
    t.parameters.PYTABLES_SYS_ATTRS = False

    # Open in write mode the file "input.h5"
    input_h5 = t.openFile("input.h5", "w")
    # Add the FORMAT attribute
    input_h5.setNodeAttr("/", "FORMAT", "AMELETHDF")
    # Add the AMELETHDF_FORMAT_VERSION attribute
    input_h5.setNodeAttr("/", "AMELETHDF_FORMAT_VERSION", "1.0.0")

    # Create the "/mesh" category
    input_h5.createGroup("/", "mesh")

    # Create the "/mesh/wire_mesh" group mesh
    wire_mesh = input_h5.createGroup("/mesh", "wire_mesh")

    # Create the part1 of the wire_mesh mesh
    part1 = input_h5.createGroup(wire_mesh, "part1")
    part1._v_attrs["type"] = "unstructured"

    # Build the nodes array for the wire
    wire_nodes = np.array([[0., 0., r] for r in np.linspace(-0.25, 0.25, 7)], np.float32)

    # Build the nodes array for the output
    output_nodes = np.array([[0.001, 0., r] for r in np.linspace(0., 0.25, 15)], np.float32)

    # The complete nodes array
    nodes = np.concatenate((wire_nodes, output_nodes), axis=0)

    # write nodes in input.h5
    input_h5.createArray(part1, "nodes",  nodes)


::

    input.h5
    `-- mesh
        `-- wire_mesh
            `-- part1[@type=unstructured]
                `-- nodes

with ``input.h5:/mesh/wiremesh/part1/nodes`` :

::

    0.           0.          -0.25        
    0.           0.          -0.17857143  
    0.           0.          -0.10714286  
    0.           0.          -0.03571429  
    0.           0.           0.03571429  
    0.           0.           0.10714286  
    0.           0.           0.17857143  
    0.           0.           0.25                         
    0.001        0.           0.        
    0.001        0.           0.01785714
    0.001        0.           0.03571429
    0.001        0.           0.05357143
    0.001        0.           0.07142857
    0.001        0.           0.08928571
    0.001        0.           0.10714286
    0.001        0.           0.125     
    0.001        0.           0.14285714
    0.001        0.           0.16071429
    0.001        0.           0.17857143
    0.001        0.           0.19642857
    0.001        0.           0.21428571
    0.001        0.           0.23214286
    0.001        0.           0.25      
      


The element types
-----------------

All the 7 elements are segments, the element type is 1.

.. code-block:: python

    # write element type in input.h5
    input_h5.createArray(part1, "elementTypes",  np.ones(7, np.int32))

::

    input.h5
    `-- mesh
        `-- wire_mesh
            `-- part1[@type=unstructured]
                |-- elementTypes
                `-- nodes

with ``input.h5:/mesh/wiremesh/part1/elementTypes`` :

::

    1
    1
    1
    1
    1
    1
    1


The element nodes
-----------------

The element nodes are :

* s1 = [p1 p2]
* s2 = [p2 p3]
* s3 = [p3 p4]
* s4 = [p4 p5]
* s5 = [p5 p6]
* s6 = [p6 p7]
* s7 = [p7 p8]

.. code-block:: python

    # write element type in input.h5
    element_nodes = []
    [element_nodes.extend(i) for i in zip(range(0, 7), range(1, 8))]
    input_h5.createArray(part1, "elementNodes", element_nodes)


::

    input.h5
    `-- mesh/
        `-- wire_mesh
            `-- part1[@type=unstructured]
                |-- elementNodes
                |-- elementTypes
                `-- nodes

with ``input.h5:/mesh/wiremesh/part1/elementTypes`` :

::
    
    1
    2
    2
    3
    3
    4
    4
    5
    5
    6
    6
    7


Finally, the wire is a group that gathers all segments named 
``/mesh/wire_mesh/part1/group/wire`` :

::

    input.h5
    `-- mesh/
        `-- wire_mesh
            `-- part1[@type=unstructured]
                |-- elementNodes
                |-- elementTypes
                |-- nodes
                `-- group
                    `-- wire[@type=element
                             @entityType=edge]

with ``/mesh/wire_mesh/part1/group/wire`` :

::

    0
    1
    2
    3
    4
    5
    6


In python this can be done with :

.. code-block:: python

    # "/mesh/wire_mesh/part1/group" creation
    input_h5.createGroup(part1, "groupGroup")
    group = input_h5.createArray(part1, "group")

    # "/mesh/wire_mesh/part1/group/wire" dataset creation
    wire_group = input_h5.createArray(group, "wire", np.arange(0, 7))
    # "/mesh/wire_mesh/part1/group/output_nodes" has a @type = nodes
    wire_group.attrs.type = "element"
    wire_group.attrs.entityType = "edge"


The calculation request points
------------------------------

We create a node dataset which will contain ``output_nodes`` elements : 

.. code-block:: python

    # "/mesh/wire_mesh/part1/group/output_nodes" dataset creation
    output_group = input_h5.createArray(group, "output_nodes", np.arange(7, 22))
    # "/mesh/wire_mesh/part1/group/output_nodes" has a @type = nodes
    output_group.attrs.type = "node"


::

    input.h5
    `-- mesh/
        `-- wire_mesh/
            `-- part1[@type=unstructured]/
                |-- elementNodes
                |-- elementTypes
                |-- nodes
                `-- group/
                    |-- wire
                    `-- output_nodes
                    

Setting the wire radius
-----------------------

We saw in the overview the wire radius is 1e-3m.
The section 14.2.2. of Amelet-HDF 1.0.0 explains how to set the radius of
a wire.

The ``/label/predefinedLabels`` dataset contains Amelet-HDF predefined label. 
The ``wireRadius`` label can be used to set the radius :

:: 

    data.h5
    |-- label/
    |   `-- predefinedLabels
    |-- mesh/
    |   `-- wire_mesh
    |       `-- part1
    |           `-- group
    |               |-- wire
    |               `-- output_nodes
    `-- link
        `-- link_group
            `-- radius[@subject=/label/predefinedLabels
                       @subject_id=0
                       @object=/mesh/wire_mesh/part1/group/wire
                       @radius=1e-3]



Lets write this in the input file :


.. code-block:: python

    # Setting the wire radius
    # "/label" group creation
    input_h5.createGroup("/", "label")
    predefinedLabels = ["wireRadius"]
    # "/label/predefinedLabels" dataset creation
    input_h5.createArray("/label", "predefinedLabels", predefinedLabels)

    # "/link" group creation
    input_h5.createGroup("/", "link")
    # "/link" group creation
    input_h5.createGroup("/link", "link_group")

    # The radius link creation
    radius = input_h5.createGroup("/link/link_group", "radius")
    radius._v_attrs.subject = "/label/predefinedLabels"
    radius._v_attrs.subject_id = 0
    radius._v_attrs.object = "/mesh/wire_mesh/part1/group/wire"
    radius._v_attrs.radius = np.float32(1e-3)



The generator
-------------

First of all, we have to isolate the point where the generator is located.
The is done by creating an element in 
``/mesh/wire_mesh/part1/selectorOnMesh/elements`` 

:: 

    data.h5
    `-- mesh/
        `-- wire_mesh/
            `-- part1/
                |-- elementNodes
                |-- elementTypes
                |-- nodes
                |-- selectorOnMesh/
                |   `-- elements
                `-- group/
                    |-- wire
                    `-- output_nodes

where ``/mesh/wire_mesh/part1/selectorOnMesh/elements`` is :

================== ========== ===== ===== =====
shortName            index      v1   v2    v3
================== ========== ===== ===== =====
voltage_generator    4          -1    -1    -1
================== ========== ===== ===== =====


The equivalement python code is the following :

.. code-block:: python

    # Element for excitation location
    som = input_h5.createGroup(part1, "selectorOnMesh")
    class USelectorOnMeshElement(t.IsDescription):
      shortName = t.StringCol(30, pos=0)
      index     = t.Int32Col()
      v1        = t.Float32Col()
      v2        = t.Float32Col()
      v3        = t.Float32Col()
    some = input_h5.createTable(som, 'elements', USelectorOnMeshElement)
    usome = some.row 
    usome["shortName"] = "voltage_generator"
    usome["index"] = 4
    usome["v1"] = -1.
    usome["v2"] = -1.
    usome["v3"] = -1.
    usome.append()


Next a generator has to be created in ``/electromagneticSource`` named 
``voltage_source`` : 

::

    data.h5
    |-- electromagneticSource/
    |   `-- generator/
    |       `-- voltage_source[@type=voltage]
    |           `-- magnitude[@singleComplex=(1,0)]
    `-- mesh/

The magnitude is complex and equals 1+0j V.

.. code-block:: python

    # Voltage source creation
    input_h5.createGroup("/", "electromagneticSource")
    input_h5.createGroup("/electromagneticSource", "generator")
    vs = input_h5.createGroup("/electromagneticSource/generator", "voltage_source")
    vs._v_attrs.type = "voltage"
    mag = input_h5.createGroup(vs, "magnitude")
    mag._v_attrs.floatingType = "singleComplex"
    mag._v_attrs.value = np.complex64(complex(1, 0))


Then the generator is located on the ``voltage_generator`` segment thanks a
link :

::

    data.h5
    |-- electromagneticSource/
    |   `-- generator/
    |       `-- voltage_source[@type=voltage]
    |-- link/
    |   `-- link_group/
    |       `-- generator[@subject=/electromagneticSource/generator/voltage_source
    |                     @object=/mesh/wire_mesh/part1/selectorOnMesh/elements
    |                     @object_shortName=voltage_generator]
    `-- mesh/
        `-- selectorOnMesh/
            `-- elements


.. code-block:: python

    # The voltage source link creation
    generator = input_h5.createGroup("/link/link_group", "generator")
    generator._v_attrs.subject = "/electromagneticSource/generator/voltage_source"
    generator._v_attrs.object = "/mesh/wire_mesh/part1/selectorOnMesh/elements"
    generator._v_attrs.object_shortName = "voltage_generator"


The output requests
-------------------

We have seen at the beginning of the tutorial the current has to be 
computed alongside the wire on ``output_nodes`` elements :


::

    data.h5
    |-- label/
    |   `-- predefinedOutputRequest
    |-- outputRequest/
    |   `-- request_group/
    |       `-- near_field[@subject=/label/predefinedOutputRequests
    |                      @subject_id=0
    |                      @object=/mesh/wire_mesh/part1/group/output_nodes
    |                      @output=/floatingPoint/near_field]
    `-- mesh/
        `-- wire_mesh/
            `-- part1/
                |-- elementNodes
                |-- elementTypes
                |-- nodes
                |-- selectorOnMesh/
                |   `-- elements
                `-- group/
                    |-- wire
                    `-- output_nodes

where ``/label/predefinedOutputRequest`` is :

+---------------------+
| electricField       |
+---------------------+


.. code-block:: python

    # OutputRequest handling
    # "/label" group creation
    predefinedOutputRequests = ["electricField"]
    # "/label/predefinedOutputRequests" dataset creation
    input_h5.createArray("/label", "predefinedOutputRequests", predefinedOutputRequests)

    # "/outputRequest" group creation
    input_h5.createGroup("/", "outputRequest")
    input_h5.createGroup("/", "floatingType")
    # "/request_group" group creation
    input_h5.createGroup("/outputRequest", "request_group")

    # The output request creation
    near_field = input_h5.createGroup("/outputRequest/request_group", "near_field")
    near_field._v_attrs.subject = "/label/predefinedOutputRequests"
    near_field._v_attrs.subject_id = 0
    near_field._v_attrs.object = "/mesh/wire_mesh/part1/group/output_nodes"
    near_field._v_attrs.output = "/floatingPoint/near_field"


The simulation frequency
------------------------

# The frequency setting
input_h5.createGroup("/", "globalEnvironment")
freq = input_h5.createGroup("/globalEnvironment", "frequency")
freq._v_attrs.singleReal = 299.8



The simulation object
---------------------

The simulation object is relatively straightforward :

::

    data.h5
    |-- simulation/
    |   `-- simuNec[@module=nec
    |       |       @version=1.0.0]
    |       |-- inputs
    |       `-- outputs
    |-- globalEnvironment
    |   `-- frequency
    |-- label/
    |   `-- predefinedOutputRequest
    |-- outputRequest/
    |   `-- request_group/
    `-- mesh/
        `-- wire_mesh/

where ``/simulation/simuNec/inputs`` is :

+---------------------------------+
|``/mesh/wire_mesh``              |
+---------------------------------+
|``/globalEnvironment/link_group``|
+---------------------------------+
|``/link/link_group``             |
+---------------------------------+
|``/outputRequest/request_group`` |
+---------------------------------+


and  ``/simulation/simuNec/output`` is :

+---------------------------------+
|``/floatingType/near_field``     |
+---------------------------------+


This can be performed in python with the following sequence :

.. code-block:: python

    # "/simulation/simuNec" creation
    input_h5.createGroup("/", "simulation")
    simu = input_h5.createGroup("/simulation", "simuNec")
    # The entry point of the is /simulation/simuNec
    input_h5.root._v_attrs.entryPoint = "/simulation/simuNec"
    simu._v_attrs.module = "nec"
    simu._v_attrs.version = "1.0.0"
    # Simulation inputs
    inputs = []
    inputs.extend(("/mesh/wire_mesh", 
                   "/globalEnvironment",
                   "/link/link_group",
                   "/outputRequest/request_group"))
    input_h5.createArray(simu, "inputs", inputs)

    # Simulation inputs
    outputs = []
    outputs.append("/floatingType/near_field")
    input_h5.createArray(simu, "outputs", outputs)




Entire python source code
-------------------------

.. code-block:: python

    # import the numpy module into the session
    import numpy as np
    # import the pytables module into the session
    import tables as t
    t.parameters.PYTABLES_SYS_ATTRS = False

    # Open in write mode the file "input.h5"
    input_h5 = t.openFile("input.h5", "w")
    # Add the FORMAT attribute
    input_h5.setNodeAttr("/", "FORMAT", "AMELETHDF")
    # Add the AMELETHDF_FORMAT_VERSION attribute
    input_h5.setNodeAttr("/", "AMELETHDF_FORMAT_VERSION", "1.0.0")

    # Create the "/mesh" category
    input_h5.createGroup("/", "mesh")

    # Create the "/mesh/wire_mesh" group mesh
    wire_mesh = input_h5.createGroup("/mesh", "wire_mesh")

    # Create the part1 of the wire_mesh mesh
    part1 = input_h5.createGroup(wire_mesh, "part1")
    part1._v_attrs["type"] = "unstructured"

    # Build the nodes array for the wire
    wire_nodes = np.array([[0., 0., r] for r in np.linspace(-0.25, 0.25, 8)], np.float32)

    # Build the nodes array for the output
    output_nodes = np.array([[0.001, 0., r] for r in np.linspace(0., 0.25, 15)], np.float32)

    # The complete nodes array
    nodes = np.concatenate((wire_nodes, output_nodes), axis=0)

    # write nodes in input.h5
    input_h5.createArray(part1, "nodes",  nodes)

    # write element type in input.h5
    input_h5.createArray(part1, "elementTypes",  np.ones(7, np.int32))

    # write element nodes in input.h5
    element_nodes = []
    [element_nodes.extend(i) for i in zip(range(0, 7), range(1, 8))]
    input_h5.createArray(part1, "elementNodes", element_nodes)



    # mesh group init
    input_h5.createGroup(part1, "groupGroup")


    # wire group
    # "/mesh/wire_mesh/part1/group" creation
    group = input_h5.createGroup(part1, "group")


    # output_nodes group for near field calculation
    # "/mesh/wire_mesh/part1/group/output_nodes" dataset creation
    output_group = input_h5.createArray(group, "output_nodes", np.arange(7, 22))
    # "/mesh/wire_mesh/part1/group/output_nodes" has a @type = nodes
    output_group.attrs.type = "node"


    # "/mesh/wire_mesh/part1/group/wire" dataset creation
    wire_group = input_h5.createArray(group, "wire", np.arange(0, 7))
    # "/mesh/wire_mesh/part1/group/output_nodes" has a @type = nodes
    wire_group.attrs.type = "element"
    wire_group.attrs.entityType = "edge"


    # Element for excitation location
    som = input_h5.createGroup(part1, "selectorOnMesh")
    class USelectorOnMeshElement(t.IsDescription):
      shortName = t.StringCol(30, pos=0)
      index     = t.Int32Col()
      v1        = t.Float32Col()
      v2        = t.Float32Col()
      v3        = t.Float32Col()
    some = input_h5.createTable(som, 'elements', USelectorOnMeshElement)
    usome = some.row 
    usome["shortName"] = "voltage_generator"
    usome["index"] = 4
    usome["v1"] = -1.
    usome["v2"] = -1.
    usome["v3"] = -1.
    usome.append()


    # Setting the wire radius
    # "/label" group creation
    input_h5.createGroup("/", "label")
    predefinedLabels = ["wireRadius"]
    # "/label/predefinedLabels" dataset creation
    input_h5.createArray("/label", "predefinedLabels", predefinedLabels)

    # "/link" group creation
    input_h5.createGroup("/", "link")
    # "/link" group creation
    input_h5.createGroup("/link", "link_group")

    # The radius link creation
    radius = input_h5.createGroup("/link/link_group", "radius")
    radius._v_attrs.subject = "/label/predefinedLabels"
    radius._v_attrs.subject_id = 0
    radius._v_attrs.object = "/mesh/wire_mesh/part1/group/wire"
    radius._v_attrs.radius = np.float32(1e-3)


    # Voltage source creation
    input_h5.createGroup("/", "electromagneticSource")
    input_h5.createGroup("/electromagneticSource", "generator")
    vs = input_h5.createGroup("/electromagneticSource/generator", "voltage_source")
    vs._v_attrs.type = "voltage"
    mag = input_h5.createGroup(vs, "magnitude")
    mag._v_attrs.floatingType = "singleComplex"
    mag._v_attrs.value = np.complex64(complex(1, 0))

    # The voltage source link creation
    generator = input_h5.createGroup("/link/link_group", "generator")
    generator._v_attrs.subject = "/electromagneticSource/generator/voltage_source"
    generator._v_attrs.object = "/mesh/wire_mesh/part1/selectorOnMesh/elements"
    generator._v_attrs.object_shortName = "voltage_generator"


    # OutputRequest handling
    # "/label" group creation
    predefinedOutputRequests = ["electricField"]
    # "/label/predefinedOutputRequests" dataset creation
    input_h5.createArray("/label", "predefinedOutputRequests", predefinedOutputRequests)

    # "/outputRequest" group creation
    input_h5.createGroup("/", "outputRequest")
    input_h5.createGroup("/", "floatingType")
    # "/request_group" group creation
    input_h5.createGroup("/outputRequest", "request_group")

    # The output request creation
    near_field = input_h5.createGroup("/outputRequest/request_group", "near_field")
    near_field._v_attrs.subject = "/label/predefinedOutputRequests"
    near_field._v_attrs.subject_id = 0
    near_field._v_attrs.object = "/mesh/wire_mesh/part1/group/wire"
    near_field._v_attrs.output = "/floatingPoint/near_field"


    # "/simulation/simuNec" creation
    input_h5.createGroup("/", "simulation")
    simu = input_h5.createGroup("/simulation", "simuNec")
    # The entry point of the is /simulation/simuNec
    input_h5.root._v_attrs.entryPoint = "/simulation/simuNec"
    simu._v_attrs.module = "nec"
    simu._v_attrs.version = "1.0.0"
    # Simulation inputs
    inputs = []
    inputs.extend(("/mesh/wire_mesh", 
                   "/globalEnvironment",
                   "/link/link_group",
                   "/outputRequest/request_group"))
    input_h5.createArray(simu, "inputs", inputs)

    # Simulation inputs
    outputs = []
    outputs.append("/floatingType/near_field")
    input_h5.createArray(simu, "outputs", outputs)

    # physicalModel category
    input_h5.createGroup("/", "physicalModel")
    input_h5.createGroup("/physicalModel", "perfectElectricConductor")

    # The frequency setting
    input_h5.createGroup("/", "globalEnvironment")
    freq = input_h5.createGroup("/globalEnvironment", "frequency")
    freq._v_attrs.singleReal = 299.8

    input_h5.close()



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
Amelet-HDF intput file.



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

The SDK permits to create a runable project in a few steps :

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
with n segment. This concept does not exist in Amelet-HDF so we have to 
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

The second modification does not use the same free formatting string 
schema than C language. It is more common the write data in columns
as the original nec defines it.

.. --+++-----++++++++++----------++++++++++----------++++++++++----------+++++++++

    CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA
    CE
    GW  0    1     0.000     0.000       -0.250     0.000     0.000    -0.178    0.001
    GW  0    1     0.000     0.000       -0.178     0.000     0.000    -0.107    0.001
    GW  0    1     0.000     0.000       -0.107     0.000     0.000    -0.035    0.001
    GW  1    1     0.000     0.000       -0.035     0.000     0.000     0.035    0.001
    GW  0    1     0.000     0.000        0.035     0.000     0.000     0.107    0.001
    GW  0    1     0.000     0.000        0.107     0.000     0.000     0.178    0.001
    GW  0    1     0.000     0.000        0.178     0.000     0.000     0.250    0.001
    GE
    EX  0    1    1    0    1.
    XQ
    LD  0    1    1    1   10.     3.000E-09 5.300E-11
    PQ
    NE  0    1    1   15   .001      0         0          0.        0.      .01786
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

Nec input file creation
^^^^^^^^^^^^^^^^^^^^^^^

From the skeletal project, the first thing to do is the creation of the 
``input.nec`` file, add the following fortran code line after the reading
of ``output_folder`` :

.. code-block:: fortran

    ! create / open input.nec file
    open(unit=numnec, file=trim(output_folder)//'/'//inputnec, &
         form='formatted', status='replace')
    write(numnec, "(a40)"), "CM EXAMPLE 1.  CENTER FED LINEAR ANTENNA"
    write(numnec, "(a2)"), "CE"

``status`` is ``replace`` because a lot of tries will be performed.

Compile and run the project and compare ``test/reference`` and ``test/result``
folders. The two first line are identical.

The wire definition (GW)
------------------------

Nec take into account only segment elements so the structure is ``elementTypes``
elements. For each ``elementTypes`` elements we have to create a ``GW`` card.

But first of all, we have to know the radius of wires. All wires should be
linked the ``label#wireRadius`` label. So the strategy is to look for 
``wireRadius`` label link.



The Nec output file
===================

::

                               __________________________________________
                              |                                          |
                              |  NUMERICAL ELECTROMAGNETICS CODE (nec2c) |
                              |   Translated to 'C' in Double Precision  |
                              |__________________________________________|



                               ---------------- COMMENTS ----------------
                               EXAMPLE 1.  CENTER FED LINEAR ANTENNA
                              



                               -------- STRUCTURE SPECIFICATION --------
                                     COORDINATES MUST BE INPUT IN
                                     METERS OR BE SCALED TO METERS
                                     BEFORE STRUCTURE INPUT IS ENDED

  WIRE                                                                                 SEG FIRST  LAST  TAG
   No:        X1         Y1         Z1         X2         Y2         Z2       RADIUS   No:   SEG   SEG  No:
     1      0.0000     0.0000    -0.2500     0.0000     0.0000     0.2500     0.0010     7     1     7    0

     TOTAL SEGMENTS USED: 7   SEGMENTS IN A SYMMETRIC CELL: 7   SYMMETRY FLAG: 0


                               ---------- SEGMENTATION DATA ----------
                                        COORDINATES IN METERS
                            I+ AND I- INDICATE THE SEGMENTS BEFORE AND AFTER I

   SEG    COORDINATES OF SEGM CENTER     SEGM    ORIENTATION ANGLES    WIRE    CONNECTION DATA   TAG
   No:       X         Y         Z      LENGTH     ALPHA      BETA    RADIUS    I-     I    I+   NO:
     1    0.0000    0.0000   -0.2143    0.0714   90.0000    0.0000    0.0010     0     1     2     0
     2    0.0000    0.0000   -0.1429    0.0714   90.0000    0.0000    0.0010     1     2     3     0
     3    0.0000    0.0000   -0.0714    0.0714   90.0000    0.0000    0.0010     2     3     4     0
     4    0.0000    0.0000   -0.0000    0.0714   90.0000    0.0000    0.0010     3     4     5     0
     5    0.0000    0.0000    0.0714    0.0714   90.0000    0.0000    0.0010     4     5     6     0
     6    0.0000    0.0000    0.1429    0.0714   90.0000    0.0000    0.0010     5     6     7     0
     7    0.0000    0.0000    0.2143    0.0714   90.0000    0.0000    0.0010     6     7     0     0



  DATA CARD No:   1 EX   0     0     4     0  1.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00
  DATA CARD No:   2 XQ   0     0     0     0  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00


                               --------- FREQUENCY --------
                                FREQUENCY : 2.9980E+02 MHz
                                WAVELENGTH: 1.0000E+00 Mtr

                        APPROXIMATE INTEGRATION EMPLOYED FOR SEGMENTS 
                        THAT ARE MORE THAN 1.000 WAVELENGTHS APART


                          ------ STRUCTURE IMPEDANCE LOADING ------
                                 THIS STRUCTURE IS NOT LOADED


                            -------- ANTENNA ENVIRONMENT --------
                            FREE SPACE


                             ---------- MATRIX TIMING ----------
                               FILL: 0 msec  FACTOR: 0 msec


                        --------- ANTENNA INPUT PARAMETERS ---------
  TAG   SEG       VOLTAGE (VOLTS)         CURRENT (AMPS)         IMPEDANCE (OHMS)        ADMITTANCE (MHOS)     POWER
  No:   No:     REAL      IMAGINARY     REAL      IMAGINARY     REAL      IMAGINARY    REAL       IMAGINARY   (WATTS)
    0     4  1.0000E+00  0.0000E+00  9.2058E-03 -5.1547E-03  8.2698E+01  4.6306E+01  9.2058E-03 -5.1547E-03  4.6029E-03


                           -------- CURRENTS AND LOCATION --------
                                  DISTANCES IN WAVELENGTHS

   SEG  TAG    COORDINATES OF SEGM CENTER     SEGM    ------------- CURRENT (AMPS) -------------
   No:  No:       X         Y         Z      LENGTH     REAL      IMAGINARY    MAGN        PHASE
     1    0    0.0000    0.0000   -0.2143   0.07143  2.3592E-03 -1.6881E-03  2.9010E-03  -35.584
     2    0    0.0000    0.0000   -0.1429   0.07143  5.9998E-03 -4.0463E-03  7.2367E-03  -33.996
     3    0    0.0000    0.0000   -0.0714   0.07143  8.3711E-03 -5.1857E-03  9.8472E-03  -31.777
     4    0    0.0000    0.0000   -0.0000   0.07143  9.2058E-03 -5.1547E-03  1.0551E-02  -29.246
     5    0    0.0000    0.0000    0.0714   0.07143  8.3711E-03 -5.1857E-03  9.8472E-03  -31.777
     6    0    0.0000    0.0000    0.1429   0.07143  5.9998E-03 -4.0463E-03  7.2367E-03  -33.996
     7    0    0.0000    0.0000    0.2143   0.07143  2.3592E-03 -1.6881E-03  2.9010E-03  -35.584


                               ---------- POWER BUDGET ---------
                               INPUT POWER   =  4.6029E-03 Watts
                               RADIATED POWER=  4.6029E-03 Watts
                               STRUCTURE LOSS=  0.0000E+00 Watts
                               NETWORK LOSS  =  0.0000E+00 Watts
                               EFFICIENCY    =  100.00 Percent



  DATA CARD No:   3 LD   0     0     4     4  1.00000E+01  3.00000E-09  5.30000E-11  0.00000E+00  0.00000E+00  0.00000E+00
  DATA CARD No:   4 PQ   0     0     0     0  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00
  DATA CARD No:   5 NE   0     1     1    15  1.00000E-03  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  1.78600E-02


                          ------ STRUCTURE IMPEDANCE LOADING ------
  LOCATION        RESISTANCE  INDUCTANCE  CAPACITANCE     IMPEDANCE (OHMS)   CONDUCTIVITY  CIRCUIT
  ITAG FROM THRU     OHMS       HENRYS      FARADS       REAL     IMAGINARY   MHOS/METER      TYPE
          4    4  1.0000E+01  3.0000E-09  5.3000E-11                                        SERIES 


                            -------- ANTENNA ENVIRONMENT --------
                            FREE SPACE


                             ---------- MATRIX TIMING ----------
                               FILL: 0 msec  FACTOR: 0 msec


                        --------- ANTENNA INPUT PARAMETERS ---------
  TAG   SEG       VOLTAGE (VOLTS)         CURRENT (AMPS)         IMPEDANCE (OHMS)        ADMITTANCE (MHOS)     POWER
  No:   No:     REAL      IMAGINARY     REAL      IMAGINARY     REAL      IMAGINARY    REAL       IMAGINARY   (WATTS)
    0     4  1.0000E+00  0.0000E+00  8.9547E-03 -4.0515E-03  9.2698E+01  4.1941E+01  8.9547E-03 -4.0515E-03  4.4773E-03


                           -------- CURRENTS AND LOCATION --------
                                  DISTANCES IN WAVELENGTHS

   SEG  TAG    COORDINATES OF SEGM CENTER     SEGM    ------------- CURRENT (AMPS) -------------
   No:  No:       X         Y         Z      LENGTH     REAL      IMAGINARY    MAGN        PHASE
     1    0    0.0000    0.0000   -0.2143   0.07143  2.3241E-03 -1.3790E-03  2.7024E-03  -30.682
     2    0    0.0000    0.0000   -0.1429   0.07143  5.8908E-03 -3.2779E-03  6.7413E-03  -29.093
     3    0    0.0000    0.0000   -0.0714   0.07143  8.1824E-03 -4.1467E-03  9.1731E-03  -26.875
     4    0    0.0000    0.0000   -0.0000   0.07143  8.9547E-03 -4.0515E-03  9.8285E-03  -24.344
     5    0    0.0000    0.0000    0.0714   0.07143  8.1824E-03 -4.1467E-03  9.1731E-03  -26.875
     6    0    0.0000    0.0000    0.1429   0.07143  5.8908E-03 -3.2779E-03  6.7413E-03  -29.093
     7    0    0.0000    0.0000    0.2143   0.07143  2.3241E-03 -1.3790E-03  2.7024E-03  -30.682


                                  ------ CHARGE DENSITIES ------
                                     DISTANCES IN WAVELENGTHS

   SEG   TAG    COORDINATES OF SEG CENTER     SEG          CHARGE DENSITY (COULOMBS/METER)
   NO:   NO:     X         Y         Z       LENGTH     REAL      IMAGINARY     MAGN        PHASE
     1    0    0.0000    0.0000   -0.2143   0.07143  1.8292E-11  3.1761E-11  3.6652E-11    60.061
     2    0    0.0000    0.0000   -0.1429   0.07143  1.0429E-11  2.2040E-11  2.4383E-11    64.676
     3    0    0.0000    0.0000   -0.0714   0.07143  2.1140E-12  1.1638E-11  1.1829E-11    79.705
     4    0    0.0000    0.0000   -0.0000   0.07143  5.1684E-19  2.3814E-19  5.6906E-19    24.738
     5    0    0.0000    0.0000    0.0714   0.07143 -2.1140E-12 -1.1638E-11  1.1829E-11  -100.295
     6    0    0.0000    0.0000    0.1429   0.07143 -1.0429E-11 -2.2040E-11  2.4383E-11  -115.324
     7    0    0.0000    0.0000    0.2143   0.07143 -1.8292E-11 -3.1761E-11  3.6652E-11  -119.939


                               ---------- POWER BUDGET ---------
                               INPUT POWER   =  4.4773E-03 Watts
                               RADIATED POWER=  3.9943E-03 Watts
                               STRUCTURE LOSS=  4.8300E-04 Watts
                               NETWORK LOSS  =  0.0000E+00 Watts
                               EFFICIENCY    =   89.21 Percent


                             -------- NEAR ELECTRIC FIELDS --------
     ------- LOCATION -------     ------- EX ------    ------- EY ------    ------- EZ ------
      X         Y         Z       MAGNITUDE   PHASE    MAGNITUDE   PHASE    MAGNITUDE   PHASE
    METERS    METERS    METERS     VOLTS/M  DEGREES    VOLTS/M   DEGREES     VOLTS/M  DEGREES
    0.0010    0.0000    0.0000   1.0228E-05   24.74   0.0000E+00    0.00   1.3042E+01 -175.10
    0.0010    0.0000    0.0179   5.5442E+01  -66.31   0.0000E+00    0.00   1.2537E+01 -175.08
    0.0010    0.0000    0.0357   1.0968E+02  -67.15   0.0000E+00    0.00   6.7271E+00 -175.46
    0.0010    0.0000    0.0536   1.5608E+02  -88.85   0.0000E+00    0.00   8.4339E-01 -179.75
    0.0010    0.0000    0.0714   2.1267E+02 -100.30   0.0000E+00    0.00   4.2135E-04   -6.13
    0.0010    0.0000    0.0893   2.7147E+02 -106.86   0.0000E+00    0.00   3.4497E-01   -8.87
    0.0010    0.0000    0.1072   3.2920E+02 -111.08   0.0000E+00    0.00   2.8000E-01   22.83
    0.0010    0.0000    0.1250   3.8592E+02 -113.51   0.0000E+00    0.00   2.2076E-01   74.41
    0.0010    0.0000    0.1429   4.3835E+02 -115.33   0.0000E+00    0.00   3.0905E-04  -94.14
    0.0010    0.0000    0.1607   4.8563E+02 -116.77   0.0000E+00    0.00   2.1937E-01 -106.41
    0.0010    0.0000    0.1786   5.2800E+02 -117.97   0.0000E+00    0.00   1.9750E+00   57.57
    0.0010    0.0000    0.1965   5.9664E+02 -119.06   0.0000E+00    0.00   3.3113E+00   58.63
    0.0010    0.0000    0.2143   6.5880E+02 -119.94   0.0000E+00    0.00   9.9556E-03 -121.24
    0.0010    0.0000    0.2322   7.1246E+02 -120.67   0.0000E+00    0.00   1.0680E+01 -121.66
    0.0010    0.0000    0.2500   5.5195E+02 -121.29   0.0000E+00    0.00   3.8032E+02 -121.43



  DATA CARD No:   6 EN   0     0     0     0  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00

  TOTAL RUN TIME: 0 msec
