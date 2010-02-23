#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from simple import FloatingType
from amelethdf.model.api import Array, String

__all__ = ['DataSet']

class DataSet(FloatingType):
    floatingtype = String('dataset') 
    values = Array
