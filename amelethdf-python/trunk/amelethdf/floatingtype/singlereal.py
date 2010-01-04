#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import Float, String

from amelethdf.floatingtype.simple import Simple

class SingleReal(Simple):
    floatingType = String("singleReal")
    value = Float(0.0)

