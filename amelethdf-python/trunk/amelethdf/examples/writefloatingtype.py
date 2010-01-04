#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''
import numpy

import tables
tables.parameters.PYTABLES_SYS_ATTRS = False
from tables import openFile

from amelethdf.floatingtype.singlereal import SingleReal
from amelethdf.floatingtype.singlecomplex import SingleComplex
from amelethdf.floatingtype.vector import Vector
from amelethdf.floatingtype.dataset import DataSet
from amelethdf.floatingtype.arrayset import ArraySet

from amelethdf.tables.hdfprint import print_node

import sys
filename = sys.argv[1]
h5file = openFile(filename, mode = "w", title = "test")

# Create a sinpleReal
simple = SingleReal(name = "a_singleReal")
simple.label = "a single real"
simple.physicalNature = "length"
simple.unit = "meter"
simple.value = 12.0 
print_node(h5file, simple, h5file.root)


# Create a sinpleComplex
simple = SingleComplex(name = "a_singleComplex")
simple.label = "a single complex"
simple.physicalNature = "length"
simple.unit = "meter"
simple.value = 12.0+13.5j 
print_node(h5file, simple, h5file.root)


# Create a vector
simple = Vector(name = "a_vector")
simple.label = "a complex vector"
simple.physicalNature = "length"
simple.unit = "meter"
simple.values = numpy.array([1+0.5j, 2+1.5j, 3+2.5j,\
                             4+3.5j, 5+4.5j, 6+5.5j], 
                            dtype=numpy.complex64)
print_node(h5file, simple, h5file.root)


# Create a dataset
simple = DataSet(name = "a_dataset")
simple.label = "a complex dataset"
simple.physicalNature = "length"
simple.unit = "meter"
simple.values = numpy.array([[1+0.5j, 2+1.5j, 3+2.5j],\
                             [4+3.5j, 5+4.5j, 6+5.5j]], 
                            dtype=numpy.complex64)
print_node(h5file, simple, h5file.root)


# Create a arrayset
simple = ArraySet(name = "a_real_arrayset")
simple.label = "a real arrayset"
simple.physical_nature = "length"
simple.unit = "meter"
simple.data = numpy.array([1., 2., 3.], 
                          dtype=numpy.float32)
simple.ds.dim1.values = numpy.array([4., 5., 6.], 
                          dtype=numpy.float32)
print_node(h5file, simple, h5file.root)

h5file.close()

