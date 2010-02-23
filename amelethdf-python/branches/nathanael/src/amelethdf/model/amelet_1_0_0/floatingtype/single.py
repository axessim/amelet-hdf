#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from simple import FloatingType
from amelethdf.model.api import String, Int, Complex, Float

__all__ = ['SingleComplex', 'SingleInterger', 'SingleReal', 'SingleString']


class SingleComplex(FloatingType):
    floatingType = String('singleComplex')
    value = Complex
    
    def __complex__(self):
        return self.value
    
    
class SingleInterger(FloatingType):
    floatingType = String('singleInteger')
    value = Int
    
    def __int__(self):
        return self.value
        
    
class SingleReal(FloatingType):
    floatingType = String('singleReal')
    value = Float
    
    def __float__(self):
        return self.value
    
    
class SingleString(FloatingType):
    floatingType = String('singleString')
    value = String
    
    def __str__(self):
        return self.value
    
