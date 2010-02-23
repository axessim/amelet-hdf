#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 11 févr. 2010

@author: nathanael
'''

from amelethdf.model.api \
    import (IsModel, Tuple, List, Instance, Dict, Enum,
            UserName, String, Int, Float, Complex, Array)

__all__ = ['Simulation']

class Parameter(IsModel):
    simple_attrs = Dict(UserName, Enum(Int, Float, String, Complex))
    complex_attrs = Dict(UserName, Enum(Array))

class Simulation(IsModel):
    module = String
    version = String
    inputs = List(Tuple(Instance(IsModel),))
    outputs = List(Tuple(Instance(IsModel),))
    #parameter = Instance(Parameter)
    

