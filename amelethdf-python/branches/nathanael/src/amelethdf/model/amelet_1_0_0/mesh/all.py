#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 23 fevr. 2010

@author: nathanael
'''

from amelethdf.model.api import \
    (IsModel, IsArray, IsGroup, Dict, UserName, Instance, String,
     List, Tuple)

__all__ = ['MeshGroup', 'GroupGroup']
    
class Group(IsArray):
    type = String
    entityType = String
    
class GroupGroup(IsArray):
    data = List(Instance(Group)) 
    
class Mesh(IsModel):
    type = String
    
    groupGroup = Dict(UserName, Instance(GroupGroup))
    

class MeshGroup(IsGroup):
    children = Dict(UserName, Instance(Mesh))
