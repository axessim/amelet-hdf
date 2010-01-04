#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

import tables
tables.parameters.PYTABLES_SYS_ATTRS = False

from tables import openFile


import numpy
from types import IntType, ComplexType, FloatType, StringType

from amelethdf.floatingtype.simple import Simple
from amelethdf.floatingtype.vector import Vector
from amelethdf.floatingtype.dataset import DataSet
from amelethdf.mesh.mesh import Group, GroupGroup

from amelethdf.tables import hdftables

class AmeletFile():
    pass

def print_root_attribute(h5file, entry_point = None):
    attrs = h5file.root._v_attrs
    if "FORMAT" not in attrs:
        h5file.setNodeAttr("/", "FORMAT", "AMELETHDF")
    if "AMELETHDF_FORMAT_VERSION" not in attrs:
        h5file.setNodeAttr("/", "AMELETHDF_FORMAT_VERSION", "1.0.0")
    if entry_point and "entry_point" not in attrs:
        h5file.setNodeAttr("/", "entry_point", entry_point)

def open_file(file_name, mode, entry_point = None):
    h5file = openFile(file_name, mode = mode, title = "Simulation test")
    print_root_attribute(h5file, entry_point)
    return h5file


def print_node(h5file, node, hdf_parent, name = None):
    if node is None:
        print "\nNone node ...."
        return
    print "\n\nEnter in print_node", node, ", Name : ", name, ", type ", type(node) 
    hdf_attrs = None
    if not name:
        name = node.name
    if isinstance(node, Vector) or\
       isinstance(node, DataSet) or\
       isinstance(node, GroupGroup) or\
       isinstance(node, Group):
        print "A new vector or dataset"
        hdf_node = h5file.createArray(hdf_parent, name, node.values)
        hdf_attrs = hdf_node.attrs
    elif isinstance(node, Simple):
        print "A new vector or Simple"
        hdf_node = h5file.createGroup(hdf_parent, name)
        hdf_attrs = hdf_node._v_attrs
    elif isinstance(node, numpy.ndarray):
        print "A new vector or array"
        if isinstance(hdf_parent, tables.Group):
            hdf_node = h5file.createArray(hdf_parent, name, node)
            hdf_attrs = hdf_node.attrs
        return
    elif hasattr(node, "items"):
        print "A new Dict ..."
        hdf_node = h5file.createGroup(hdf_parent, name)
        hdf_attrs = hdf_node._v_attrs
        for item in node.items():
            print "item : ", item[0]
            print_node(h5file, item[1], hdf_node, item[0])
    elif hasattr(node, "append"):
        print "A new List ...", node.__class__
        if node:
            klass = getattr(hdftables, node[0].__class__.__name__)
            hdf_node = h5file.createTable(hdf_parent, name, klass)
            selector = hdf_node.row
            for item in node:
                print "item : ", item.__class__
                for name, value in item.trait_get().items():
                    if name is "name": continue
                    print "attr : ", name
                    selector[name] = value
                selector.append()
    else:
        hdf_node = h5file.createGroup(hdf_parent, name)
        hdf_attrs = hdf_node._v_attrs

    if hasattr(node, "trait_get"): 
        for attr in node.trait_get():
            if attr is "name": continue
            print "attr :", attr, " of :", name
            if not attr.startswith("__") and attr is not None:
                attr_obj = getattr(node, attr)
                print "attr type :", type(attr_obj), ", value : ", attr_obj
                if type(attr_obj) in [IntType, FloatType, StringType]:
                    hdf_attrs[attr] = attr_obj
                elif type(attr_obj) is FloatType:
                    hdf_attrs[attr] = numpy.float32(attr_obj)
                elif type(attr_obj) is ComplexType:
                    hdf_attrs[attr] = numpy.complex64(attr_obj)
                else:
                    if attr is not "values":
                        name = attr
                    print_node(h5file, attr_obj, hdf_node, name)


