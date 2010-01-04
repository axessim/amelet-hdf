#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

import numpy
from enthought.traits.api import Array, String, Int, Float, List, Instance

from amelethdf.base import Base
from amelethdf.mesh.mesh import Mesh
from amelethdf.constants import ELEMENT_LENGTH


class USelectorOnMeshNode(Base):
    shortName = String(ELEMENT_LENGTH)
    id        = Int()

class USelectorOnMeshElement(Base):
    shortName = String(ELEMENT_LENGTH)
    id        = Int()
    v1        = Float()
    v2        = Float()
    v3        = Float()

class USelectorOnMesh(Base):
    nodes = List(USelectorOnMeshNode)
    elements = List(USelectorOnMeshElement)

class UnstructuredMesh(Mesh):
    type = String("unstructured")
    nodes = Array(dtype=numpy.float32)
    elementTypes = Array(dtype=numpy.int8)
    elementNodes = Array(dtype=numpy.int32)
    selectorOnMesh = Instance(USelectorOnMesh)

    def _selectorOnMesh_default(self):
        return USelectorOnMesh()

