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
    """Base class for define a user name.
    
    At this time it's just an alias of 'Str' 
    """

class IsModel(HasTraits):
    """Base class for all model class
    """
class IsGroup(IsModel):
    """For the Group model class
    """
class IsTable(IsModel):
    """For the Table model class
    """
class IsArray(IsModel):
    """For the Array model class
    """

def PathWhere(type, **metadata):
    """Pattern to define data path where an model class
    
        >>> class A(IsModel): pass
        
        >>> class B(IsModel):
        ...     a = Instance(A)
        ...     b = PathWhere(A)
        
        The hdf tree can be::
        
            b(@b=/a/a)/
            `-- a
        
    
    """
    #
    # At this time just a string by in the future can by some this else :   
    #  * Instance(type, ispath=True, **metadata) (using Traits metadata)
    #  * or WeakRef(type, **metadata)            (or WeakRef pattern)
    #
    return String


