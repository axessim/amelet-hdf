#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 23 fevr. 2010

@author: nathanael
'''


from ..floatingtype.vector import Vector
from all import Mesh, Group, GroupGroup

from amelethdf.model.api import \
    (IsModel, String, Array, Dict, UserName, Instance, List)

__all__ = ['SGroup', 'CartesianGrid', 'SMesh']

class SGroup(Group):
    array = Array(shape=(None, None), dtype=int)
    
class SGroupGroup(GroupGroup):
    data = List(Instance(SGroup))
    
class CartesianGrid(IsModel):
    x = Instance(Vector)
    y = Instance(Vector)
    z = Instance(Vector)

class SMesh(Mesh):
    type = String('structured') 
    
    cartesianGrid = Instance(CartesianGrid)
    
    group = Dict(UserName, Instance(SGroup))
    groupGroup = Dict(UserName, Instance(SGroupGroup))

    
