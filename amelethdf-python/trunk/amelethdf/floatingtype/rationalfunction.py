#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

import numpy

from enthought.traits.api import Array, String, Any

from amelethdf.floatingtype.simple import Simple 
from amelethdf.floatingtype.vector import Vector 

class rationalFraction(Vector):
    floatingType = String("rationalFraction")
    values = Array(dtype=numpy.float32)

class GeneralRationalFraction(Vector):
    floatingType = String("generalRationalFraction")
    values = Array(dtype=numpy.complex64)

class rational(Simple):
    floatingType = String("rational")
    data = Array(Any)
    
