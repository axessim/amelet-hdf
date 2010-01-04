#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import String

from amelethdf.base import Base

class Link(Base):
    subject = String("")
    object = String("")

