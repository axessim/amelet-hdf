#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 9 fevr. 2010

@author: nathanael
'''

__all__ = ('Enum', 'Dict', 'List', 'ReadOnly', 'Tuple', 'Set',
           'Complex', 'Float', 'Bool', 'String', 'Int', 'Array',
           'Any', 'Instance',
           'IsModel', 'IsGroup', 'IsTable', 'IsArray',
           'UserName',)

from enthought.traits.api \
    import (Enum, Dict, List, ReadOnly, Tuple, Set,
        Complex, Float, Bool, String, Int, Array,
        HasTraits, BaseStr,
        Any, Instance)
    

class UserName(BaseStr):
    pass

class IsModel(HasTraits): pass
class IsGroup(IsModel): pass
class IsTable(IsModel): pass
class IsArray(IsModel): pass

def PathWhere(type, **metadata):
    #
    # At this time just a string by in the future can by some this else :   
    #  * Instance(type, ispath=True, **metadata) (using Traits metadata)
    #  * or WeakRef(type, **metadata)            (or WeakRef pattern)
    #
    return String


