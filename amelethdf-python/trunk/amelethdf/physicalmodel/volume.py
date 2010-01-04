#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''
from enthought.traits.api import Instance

from amelethdf.base import Base
from amelethdf.floatingtype.simple import Simple

class Volume(Base):
    permittivity = Instance(Simple)
    permeability = Instance(Simple)
    e_conductivity = Instance(Simple)
    h_conductivity = Instance(Simple)

