#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 23 fevr. 2010

@author: nathanael
'''

from all import Mesh, Group, GroupGroup

from amelethdf.model.api import \
    (String, Array, Dict, UserName, Instance, List)

__all__ = ['UGroup', 'UMesh']

class UGroup(Group):
    array = Array(shape=(None,), dtype=int)
    
class UGroupGroup(GroupGroup):
    data = List(Instance(UGroup))

class UMesh(Mesh):
    type = String('unstructured') 
    
    nodes = Array(shape=(None, None), dtype=float)
    elementTypes = Array(shape=(None,), dtype=int)
    elementNodes = Array(Shape=(None,), dtype=int)

    group = Dict(UserName, Instance(UGroup))
    groupGroup = Dict(UserName, Instance(UGroupGroup))
