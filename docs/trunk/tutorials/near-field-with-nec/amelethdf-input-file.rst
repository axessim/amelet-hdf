The Amelet-HDF input file
=========================

First of all we are going the build an unstructured mesh.

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
voltage_generator    3          -1    -1    -1
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
    usome["index"] = 3
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


Generator's inner impedance
---------------------------

A load serial impedance is also located on the fourth segment :

* The resistance is R = 10 Ohms
* The inductance is L = 3e-9 Henries
* The capacitance is C = 5.3e-11 Farads

Amelet-HDF provides the RLC models :

.. code-block: python

    # Load source creation
    input_h5.createGroup("/physicalModel", "multiport")
    input_h5.createGroup("/physicalModel/multiport", "RLC")

    r = input_h5.createGroup("/physicalModel/multiport/", "resistance")
    r._v_attrs.physicalNature = "resistance"
    r._v_attrs.nbPort = 1
    r._v_attrs.floatingType = "singleReal"
    r._v_attrs.value = np.float32(10.)

    l = input_h5.createGroup("/physicalModel/multiport/", "inductance")
    l._v_attrs.physicalNature = "inductance"
    l._v_attrs.nbPort = 1
    l._v_attrs.floatingType = "singleReal"
    l._v_attrs.value = np.float32(3e-9)

    c = input_h5.createGroup("/physicalModel/multiport/", "capacitance")
    c._v_attrs.physicalNature = "capacitance"
    c._v_attrs.nbPort = 1
    c._v_attrs.floatingType = "singleReal"
    c._v_attrs.value = np.float32(5.3e-11)

    ld = input_h5.createGroup("/physicalModel/multiport/RLC", "generator_load")
    ld._v_attrs.type = np.int32(1)
    ld._v_attrs.R = "/physicalModel/multiport/resistance"
    ld._v_attrs.L = "/physicalModel/multiport/inductance"
    ld._v_attrs.C = "/physicalModel/multiport/capacitance"


Then the impedance is located on the ``voltage_generator`` segment thanks a
link :

::

    data.h5
    |-- electromagneticSource/
    |   `-- generator/
    |       `-- voltage_source[@type=voltage]
    |-- link/
    |   `-- link_group/
    |       `-- generator[@subject=/physicalModel/multiport/RLC/generator_load
    |                     @object=/mesh/wire_mesh/part1/selectorOnMesh/elements
    |                     @object_shortName=voltage_generator]
    `-- mesh/
        `-- selectorOnMesh/
            `-- elements



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
    |                      @output=/floatingType/near_field]
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

.. code-block:: python

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




Complete python source code
---------------------------

Finally, the entire python code is reported hereafter :

.. literalinclude:: tests/build_simulation.py
   :linenos:

