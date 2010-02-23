#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from simple import FloatingType
from amelethdf.model.api import IsModel, Array, Instance, String, Dict

__all__ = ['ArraySet']


class ArraySet(FloatingType):
    flatingtype = String('arrayset')
    
    data = Array
    ds = Dict(String(regex='dim[0-9]+'), Array(shape=(None,)))
    
