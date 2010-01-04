#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import Instance, Array, String

from amelethdf.base import Base
from amelethdf.floatingtype.simple import Simple
from amelethdf.floatingtype.vector import Vector 

class DS(Base):
    dim1 = Instance(Vector)

    def _dim1_default(self):
        return Vector()

class ArraySet(Simple):
    floatingType = String("arraySet")
    data = Array()
    ds = Instance(DS)
       
    def _ds_default(self):
        return DS()
