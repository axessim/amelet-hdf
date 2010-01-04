#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import Array, String

from amelethdf.base import Base

class Simulation(Base):
    module = String("null-module")
    version = String("1.0.0")
    inputs = Array()
    outputs = Array()
