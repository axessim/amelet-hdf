#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

import numpy

from enthought.traits.api import Dict, Array, String

from amelethdf.base import Base

class Group(Base):
    type = String("node")
    values = Array(dtype=numpy.int32) 

class GroupGroup(Base):
    values = Array() 
    
class Mesh(Base):
    group = Dict(key_trait = String, value_trait = Group) 
    groupGroup = Dict(key_trait = String, value_trait = GroupGroup) 
