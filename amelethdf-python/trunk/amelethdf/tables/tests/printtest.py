'''
Created on 18 sept. 2009

@author: cyril
'''

import unittest

import tables
tables.parameters.PYTABLES_SYS_ATTRS = False

import numpy

from amelethdf.floatingtype.singlereal import SingleReal
from amelethdf.floatingtype.singlecomplex import SingleComplex
from amelethdf.floatingtype.vector import Vector
from amelethdf.floatingtype.dataset import DataSet
from amelethdf.floatingtype.arrayset import ArraySet
from amelethdf.floatingtype.rationalfunction import GeneralRationalFraction
from amelethdf.mesh.mesh import Group, GroupGroup
from amelethdf.mesh.unstructuredmesh import UnstructuredMesh, \
                                         USelectorOnMeshElement, \
                                         USelectorOnMeshNode
from amelethdf.mesh.structuredmesh import StructuredMesh, \
                                       SSelectorOnMeshElement, \
                                       SSelectorOnMeshNode
from amelethdf.physicalmodel.volume import Volume
from amelethdf.simulation import Simulation
from amelethdf.link import Link


from amelethdf.tables.hdfprint import print_node, open_file

class Test(unittest.TestCase):
    def setUp(self):
        self.h5file = open_file("example1.h5", mode = "a",
                                entry_point = "/simulation/simu1")

    def tearDown(self):
        self.h5file.close()

    def test_print_simplereal(self):
        group =  self.h5file.createGroup("/", "simplereal")
        simple = SingleReal(name="simple")
        simple.label = "A single real"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.value = 12.
        print_node(self.h5file, simple, group)

    def test_print_simplecomplex(self):
        group =  self.h5file.createGroup("/", "simplecomplex")
        simple = SingleComplex(name = "simple")
        simple.label = "A single complex"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.value = 13 + 14j
        print_node(self.h5file, simple, group)

    def test_print_integer_vector(self):
        if "/vector" not in self.h5file:
            group =  self.h5file.createGroup("/", "vector")
        else:
            group = self.h5file.getNode("/vector")
        
        simple = Vector(name = "int_vect")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.values = numpy.array([1, 2, 3], dtype=numpy.int32)
        print_node(self.h5file, simple, group)

    def test_print_real_vector(self):
        if "/vector" not in self.h5file:
            group =  self.h5file.createGroup("/", "vector")
        else:
            group = self.h5file.getNode("/vector")
        
        simple = Vector(name = "real_vect")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.values = numpy.array([1, 2, 3], dtype=numpy.float32)
        print_node(self.h5file, simple, group)

    def test_print_complex_vector(self):
        if "/vector" not in self.h5file:
            group =  self.h5file.createGroup("/", "vector")
        else:
            group = self.h5file.getNode("/vector")
        
        simple = Vector(name = "comp_vect")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.values = numpy.array([1, 2, 3+6j], dtype=numpy.complex64)
        print_node(self.h5file, simple, group)

    def test_print_complex_dataset(self):
        group =  self.h5file.createGroup("/", "dataset")
        
        simple = DataSet(name = "comp_dataset")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.values = numpy.array([[1, 2, 3+0.7],[4+5j, 5, 6]],
                                    dtype=numpy.complex64)
        print_node(self.h5file, simple, group)

    def test_print_real_arrayset(self):
        if "/arrayset" not in self.h5file:
            group =  self.h5file.createGroup("/", "arrayset")
        else:
            group = self.h5file.getNode("/arrayset")
        
        simple = ArraySet(name = "arr_real")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.data = numpy.array([1., 2., 3.], 
                                  dtype=numpy.float32)
        simple.ds.dim1.values = numpy.array([4., 5., 6.], 
                                  dtype=numpy.float32)
        print_node(self.h5file, simple, group)

    def test_print_complex_arrayset(self):
        if "/arrayset" not in self.h5file:
            group =  self.h5file.createGroup("/", "arrayset")
        else:
            group = self.h5file.getNode("/arrayset")
        
        simple = ArraySet(name = "arr_comp")
        simple.label = "Oh, mon beau simple"
        simple.physical_nature = "length"
        simple.unit = "meter"
        simple.data = numpy.array([1.+0.5j, 2., 3.], dtype=numpy.complex64)
        simple.ds.dim1.values = numpy.array([4., 5., 6.], dtype=numpy.float32)
        print_node(self.h5file, simple, group)

    def test_print_unstructured_mesh(self):
        if "/mesh" not in self.h5file:
            group =  self.h5file.createGroup("/", "mesh")
        else:
            group = self.h5file.getNode("/mesh")
        
        little_board = UnstructuredMesh(name = "mesh1")
        little_board.nodes = numpy.array([[11., 12., 13.], 
                                          [21., 22., 23.],
                                          [31., 32., 33.],
                                          [41., 42., 43.],
                                          [51., 52., 53.],
                                          [61., 62., 63.],
                                          [71., 72., 73.]])
        little_board.elementTypes = numpy.array([1, 1, 1], dtype=numpy.int8) 
        little_board.elementNodes = numpy.array([1, 2, 3, 4, 5, 6])

        little_board.group["first_group"] =\
            Group(type = "node", values = numpy.array([1, 1, 1]))
        little_board.group["second_group"] =\
            Group(type = "node", values = numpy.array([1, 3, 2]))

        little_board.groupGroup["first_groupGroup"] =\
            GroupGroup(values = numpy.array(["first_group", "second_group"]))

        little_board.selectorOnMesh = None

        print_node(self.h5file, little_board, group)

    def test_print_unstructured_mesh_with_selector_on_mesh(self):
        if "/mesh" not in self.h5file:
            group =  self.h5file.createGroup("/", "mesh")
        else:
            group = self.h5file.getNode("/mesh")
        
        little_board = UnstructuredMesh(name = "mesh2")
        little_board.nodes = numpy.array([[11., 12., 13.], 
                                          [21., 22., 23.],
                                          [31., 32., 33.],
                                          [41., 42., 43.],
                                          [51., 52., 53.],
                                          [61., 62., 63.],
                                          [71., 72., 73.]])
        little_board.elementTypes = numpy.array([1, 1, 1], dtype=numpy.int8) 
        little_board.elementNodes = numpy.array([1, 2, 3, 4, 5, 6])

        little_board.group["first_group"] =\
            Group(type = "node", values = numpy.array([1, 1, 1]))
        little_board.group["second_group"] =\
            Group(type = "node", values = numpy.array([1, 3, 2]))

        little_board.groupGroup["first_groupGroup"] =\
            GroupGroup(values = numpy.array(["first_group", "second_group"]))

        # Selector on mesh nodes
        little_board.selectorOnMesh.nodes.append(
                 USelectorOnMeshNode(shortName="first_node", id=12))
        little_board.selectorOnMesh.nodes.append(
                 USelectorOnMeshNode(shortName="second_node", id=1))

        # Selector on mesh elements
        little_board.selectorOnMesh.elements.append(
                USelectorOnMeshElement(shortName="first_element",
                                       v1=0.5,
                                       v2=-1.,
                                       v3=-1.))

        print_node(self.h5file, little_board, group)


    def test_print_structured_mesh(self):
        if "/mesh" not in self.h5file:
            group =  self.h5file.createGroup("/", "mesh")
        else:
            group = self.h5file.getNode("/mesh")
        
        little_board = StructuredMesh(name = "mesh3")
        little_board.cartesianGrid.x.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 
        little_board.cartesianGrid.y.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 
        little_board.cartesianGrid.z.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 

        little_board.group["first_group"] =\
            Group(type = "node", values = numpy.array([[1, 1, 1],
                                                       [5, 5, 5]], 
                                                       dtype=numpy.int32))
        little_board.group["second_group"] =\
            Group(type = "element",\
                  values = numpy.array([10, 10, 10, 25, 25, 25]))

        little_board.groupGroup["first_groupGroup"] =\
            GroupGroup(values = numpy.array(["first_group", "second_group"]))

        little_board.selectorOnMesh = None

        print_node(self.h5file, little_board, group)

        

    def test_print_structured_mesh_with_selector_on_mesh(self):
        if "/mesh" not in self.h5file:
            group =  self.h5file.createGroup("/", "mesh")
        else:
            group = self.h5file.getNode("/mesh")
        
        little_board = StructuredMesh(name = "mesh4")
        little_board.cartesianGrid.x.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 
        little_board.cartesianGrid.y.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 
        little_board.cartesianGrid.z.values =\
            numpy.arange(1, 50, dtype=numpy.float32) 

        little_board.group["first_group"] =\
            Group(type = "node", values = numpy.array([[1, 1, 1],
                                                       [5, 5, 5]], 
                                                       dtype=numpy.int32))
        little_board.group["second_group"] =\
            Group(type = "element", entityType = "face",
                  values = numpy.array([10, 10, 10, 25, 25, 25]))

        little_board.groupGroup["first_groupGroup"] =\
            GroupGroup(values = numpy.array(["first_group", "second_group"]))

        # Selector on mesh nodes
        little_board.selectorOnMesh.nodes.append(
                 SSelectorOnMeshNode(shortName="first_node", 
                                     i=12, j=13, k=14))
        little_board.selectorOnMesh.nodes.append(
                 SSelectorOnMeshNode(shortName="second_node",
                                     i=15, j=16, k=17))

        # Selector on mesh elements
        little_board.selectorOnMesh.elements.append(
                SSelectorOnMeshElement(shortName="first_element",
                                       imin=12, jmin=12, kmin=12,
                                       imax=20, jmax=20, kmax=20))
        
        print_node(self.h5file, little_board, group)

    
    
    def test_print_volume(self):
        self.h5file.createGroup("/", "physicalModel")
        group =  self.h5file.createGroup("/physicalModel", "volume")

        vol1 = Volume(name = "diel1")
        vol1.permittivity = Vector(values=numpy.zeros(10.0))
        vol1.permeability = Vector(values=numpy.zeros(10.0))
        vol1.e_conductivity = Vector(values=numpy.zeros(10.0))
        vol1.h_conductivity = Vector(values=numpy.zeros(10.0))
        print_node(self.h5file, vol1, group)

    def test_print_generalrationalfunction(self):
        group =  self.h5file.createGroup("/", "rational")

        vol1 = GeneralRationalFraction(name = "diel1",
                                       values=numpy.array([[1+2j, 3+4j], 
                                                           [5+6j, 7+8j],
                                                           [0.5+0.66j, 7+8j],
                                                           [5+6j, 7.3+1.34j],
                                                           [5+6.76j, 4.7+8j],
                                                           [9+10j,11+12j]]))
        print_node(self.h5file, vol1, group)

    def test_print_simulation(self):
        group =  self.h5file.createGroup("/", "simulation")

        simu1 = Simulation(name = "simu1",
                           module = "null-module",
                           version = "0.0.0",
                           inputs=numpy.array(["/mesh/mesh1",
                                               "/mesh/mesh2",
                                               "/physicalModel/volume/diel1"]),
                           outputs=numpy.array(["/toto"]))
        print_node(self.h5file, simu1, group)
        
    def test_print_link(self):
        group =  self.h5file.createGroup("/", "link")
        group =  self.h5file.createGroup(group, "model_links")

        link1 = Link(name = "link1",
                     subject = "/physicalModel/volume/diel1", 
                     object = "/mesh/mesh1/group/group1",
                     object_id= 12)
        print_node(self.h5file, link1, group)


#    def test_print_rationalfunction(self):
#        group =  self.h5file.createGroup("/", "rational")
#
#        base = Base()
#        vol1 = GeneralRationalFraction()
#        base.diel1 = vol1
#        print_node(self.h5file, vol1, base, group)



if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()