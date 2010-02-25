#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from amelethdf.model.api import IsModel, Dict, UserName, Enum, Instance, String, PathWhere


from floatingtype.simple import FloatingType
from comon_amelet import EntryPointIndex

__all__ = ['Amelet']

import floatingtype
__all__ += floatingtype.__all__
from floatingtype import *

import simulation
__all__ += simulation.__all__
from simulation import *

import mesh
__all__ += mesh.__all__
from mesh import *

class Amelet(IsModel):
    AMELETHDF_FORMAT_VERSION = String("1.0.0")
    FORMAT = String('AMELETHDF')
    
    entryPoint = PathWhere(EntryPointIndex)
    
    floatingType = Dict(UserName, Instance(FloatingType))
    simulation = Dict(UserName, Instance(Simulation))
    mesh = Dict(UserName, Instance(MeshGroup))
    
    
