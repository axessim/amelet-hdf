#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from simple import FloatingType
from amelethdf.model.api import String, Array, Float, Int

__all__ = ['Vector', 'LinearListOfReal1', 'LinearListOfReal2',
           'LogarithmListOfReal', 'PerDecadeListOfReal', 'LinearListOfInteger2',
           'Interval']

class Vector(FloatingType):
    floatingType = String("vector")
    values = Array()

class LinearListOfReal1(FloatingType):
    floatingType = String("linearListOfReal1")
    first = Float
    last = Float
    number_of_values = Int

class LinearListOfReal2(FloatingType):
    floatingType = String("linearListOfReal2")
    first = Float
    step = Float
    number_of_values = Int

class LogarithmListOfReal(FloatingType):
    floatingType = String("logarithmListOfReal")
    first = Float
    last = Float
    number_of_values = Int

class PerDecadeListOfReal(FloatingType):
    floatingType = String("perDecadeListOfReal")
    first = Float
    number_of_decades = Int
    number_of_values_per_decade = Int

class LinearListOfInteger2(FloatingType):
    floatingType = String("linearListOfInteger2")
    first = Float
    step = Float
    number_of_values = Int

class Interval(FloatingType):
    floatingType = String("LinearListOfReal1")
    first = Float
    last = Float
