#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''

from enthought.traits.api import HasTraits, String

class Base(HasTraits):
    name = String("default_name")
    
    def get_children_name(self, children):
        for attr in self.__dict__:
            if self.__dict__[attr] is children:
                return attr
            