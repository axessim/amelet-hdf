#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 23 fevr. 2010

@author: nathanael
'''

from ..comon_amelet import ah5_int, ah5_float
from all import Mesh, Group, GroupGroup

from amelethdf.model.api import \
    (String, Array, Dict, UserName, Instance, List)

__all__ = ['UGroup', 'UMesh']

class UGroup(Group):
    array = Array(shape=(None,), dtype=ah5_int)
    
class UGroupGroup(GroupGroup):
    data = List(Instance(UGroup))

class UMesh(Mesh):
    type = String('unstructured') 
    
    nodes = Array(shape=(None, None), dtype=ah5_float)
    elementTypes = Array(shape=(None,), dtype=ah5_int)
    elementNodes = Array(Shape=(None,), dtype=ah5_int)

    group = Dict(UserName, Instance(UGroup))
    groupGroup = Dict(UserName, Instance(UGroupGroup))
