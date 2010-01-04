#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import String, Instance, List, Int, Float

from amelethdf.base import Base
from amelethdf.constants import ELEMENT_LENGTH
from amelethdf.mesh.mesh import Mesh
from amelethdf.floatingtype.vector import Vector

class CartesianGrid(Base):
    x = Instance(Vector())
    y = Instance(Vector())
    z = Instance(Vector())

    def _x_default(self):
        return Vector()
    def _y_default(self):
        return Vector()
    def _z_default(self):
        return Vector()


class SSelectorOnMeshNode(Base):
    shortName = String(ELEMENT_LENGTH)
    i         = Int()
    j         = Int()
    k         = Int()

class SSelectorOnMeshElement(Base):
    shortName = String(ELEMENT_LENGTH)
    imin      = Int()
    jmin      = Int()
    kmin      = Int()
    imax      = Int()
    jmax      = Int()
    kmax      = Int()
    v1        = Float(-1.0)
    v2        = Float(-1.0)
    v3        = Float(-1.0)


class SSelectorOnMesh(Base):
    nodes = List(SSelectorOnMeshNode)
    elements = List(SSelectorOnMeshElement)


class StructuredMesh(Mesh):
    type = String("structured")
    cartesianGrid = Instance(CartesianGrid())
    selectorOnMesh = Instance(SSelectorOnMesh)
    
    def _cartesianGrid_default(self):
        return CartesianGrid()

    def _selectorOnMesh_default(self):
        return SSelectorOnMesh()
