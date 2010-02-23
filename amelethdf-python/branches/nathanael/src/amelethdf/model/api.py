#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 9 fevr. 2010

@author: nathanael
'''

__all__ = ('Enum', 'Dict', 'List', 'ReadOnly', 'Tuple', 'Set',
           'Complex', 'Float', 'Bool', 'String', 'Int', 'Array',
           'Any', 'Instance',
           'IsModel', 'IsGroup', 'IsTable', 'IsArray', 'Node',
           'Group', 'DataSet', 'Table',
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

class Node(IsModel): pass

class Group(Node): pass

class DataSet(Node):
    type = Any

class Table(Node):
    pass

class TableObject(IsModel): pass

