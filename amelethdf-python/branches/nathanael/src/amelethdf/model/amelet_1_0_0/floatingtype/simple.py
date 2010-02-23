#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 10 févr. 2010

@author: nathanael
'''

from amelethdf.model.api import IsModel, String

__all__ = ['FloatingType']

class FloatingType(IsModel):
    floatingType = String
    label = String
    physicalNature = String
    unit = String
    comment = String


