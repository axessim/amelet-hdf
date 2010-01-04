#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import Array, String

from amelethdf.floatingtype.simple import Simple 

class DataSet(Simple):
    floatingType = String("dataSet")
    values = Array()

