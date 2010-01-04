'''
Created on 18 sept. 2009

@author: Cyril Giraudon
'''
import numpy

import tables
tables.parameters.PYTABLES_SYS_ATTRS = False
from tables import openFile

from amelethdf.floatingtype.vector import Vector
from amelethdf.floatingtype.arrayset import ArraySet

from amelethdf.tables.hdfprint import print_node

import sys
filename = sys.argv[1]
h5file = openFile(filename, mode = "w", title = "test")


# Real arrayset_t
simple = ArraySet(name = "a_real_arrayset2")
simple.label = "a real arrayset2"
simple.physical_nature = "length"
simple.unit = "meter"
simple.data = numpy.array([1., 2., 3.], 
                          dtype=numpy.float32)
simple.ds.dim1.values = numpy.array([4., 5., 6.], 
                          dtype=numpy.float32)
print_node(h5file, simple, h5file.root)


# Complex arrayset_t 
simple = ArraySet(name = "a_complex_arrayset2")
simple.label = "a complex arrayset2"
simple.physical_nature = "length"
simple.unit = "meter"
simple.data = numpy.array([1.+0.5j, 2., 3.], dtype=numpy.complex64)
simple.ds.dim1.values = numpy.array([4., 5., 6.], dtype=numpy.float32)
print_node(h5file, simple, h5file.root)

# Real arrayset2_t
simple = ArraySet(name = "a_real_arrayset")
simple.label = "a real arrayset"
simple.physical_nature = "length"
simple.unit = "meter"
simple.data = numpy.array([[1., 2., 3.], [4., 5., 6.]], 
                          dtype=numpy.float32)
simple.ds.dim1.values = numpy.array([7., 8., 9.], 
                          dtype=numpy.float32)
simple.ds.dim2 = Vector(values = numpy.array([10., 11.], 
                                 dtype=numpy.float32),
                        label = "the temperature",
                        physicalNature = "temperature",
                        unit = "celcius",
                        comment = "no comment")
print_node(h5file, simple, h5file.root)

h5file.close()
