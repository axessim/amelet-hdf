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


