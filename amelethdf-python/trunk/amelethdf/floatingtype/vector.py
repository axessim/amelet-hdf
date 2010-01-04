#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import Array, Int, Float, String

from amelethdf.floatingtype.simple import Simple 

class Vector(Simple):
    floatingType = String("vector")
    values = Array()

class LinearListOfReal1(Simple):
    floatingType = String("linearListOfReal1")
    first = Float(0.0)
    last = Float(0.0)
    number_of_values = Int(0)

class LinearListOfReal2(Simple):
    floatingType = String("linearListOfReal2")
    first = Float(0.0)
    step = Float(0.0)
    number_of_values = Int(0) 

class LogarithmListOfReal(Simple):
    floatingType = String("logarithmListOfReal")
    first = Float(0.0)
    last = Float(0.0)
    number_of_values = 0

class PerDecadeListOfReal(Simple):
    floatingType = String("perDecadeListOfReal")
    first = Float(0.0)
    number_of_decades = Int(0)
    number_Of_values_per_decade = Int(0)

class LinearListOfInteger2(Simple):
    floatingType = String("linearListOfInteger2")
    first = Float(0.0)
    step = Float(0.0)
    number_of_values = Int(0)

class Interval(LinearListOfReal1):
    floatingType = String("LinearListOfReal1")
    first = Float(0.0)
    last = Float(0.0) 
    number_of_values = None

