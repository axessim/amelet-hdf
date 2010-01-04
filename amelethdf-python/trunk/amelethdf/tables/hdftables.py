#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 24 sept. 2009

@author: Cyril Giraudon
'''

from tables import StringCol, Int32Col, Float32Col, IsDescription

from amelethdf.constants import ELEMENT_LENGTH

class USelectorOnMeshNode(IsDescription):
    shortName = StringCol(ELEMENT_LENGTH, pos=0)
    id        = Int32Col()

class USelectorOnMeshElement(IsDescription):
    shortName = StringCol(ELEMENT_LENGTH, pos=0)
    id        = Int32Col()
    v1        = Float32Col()
    v2        = Float32Col()
    v3        = Float32Col()

class SSelectorOnMeshNode(IsDescription):
    shortName = StringCol(ELEMENT_LENGTH, pos=0)
    i         = Int32Col()
    j         = Int32Col()
    k         = Int32Col()

class SSelectorOnMeshElement(IsDescription):
    shortName = StringCol(ELEMENT_LENGTH, pos=0)
    imin      = Int32Col(pos=1)
    jmin      = Int32Col(pos=2)
    kmin      = Int32Col(pos=3)
    imax      = Int32Col(pos=4)
    jmax      = Int32Col(pos=5)
    kmax      = Int32Col(pos=6)
    v1        = Float32Col()
    v2        = Float32Col()
    v3        = Float32Col()
