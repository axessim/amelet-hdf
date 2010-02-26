#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''


from ..model import DEFAULT_MODEL

from node_model import ModelNode, openMNode

__all__ = ['NumNode', 'openNNode',
           'NTYPE_GROUP', 'NTYPE_DATASET', 'NTYPE_TABLE']


def openNNode(path, mode='r', model=DEFAULT_MODEL, **kw):
    hdf = openMNode(path, mode, **kw)
    return NumNode(hdf.hdf, '/', '', model)

NTYPE_GROUP = 1
NTYPE_DATASET = 2
NTYPE_TABLE = 3

class NumNode(ModelNode):
    
    #---------------------------------------------------------------------------
    # Over definition of ModelNode
    #---------------------------------------------------------------------------
    #
    # This over definition use the `model` to check the numerical value
    #
    
    def can_set_attr(self, name, value):
        return (super(NumNode, self).can_set_attr(name, value)
                and self.model.is_legale_value(name, value))
        
    @property
    def ntype(self):
        return self.model.mtype / 100
