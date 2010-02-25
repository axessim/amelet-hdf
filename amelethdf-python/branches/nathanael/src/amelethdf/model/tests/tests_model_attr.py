#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 24 févr. 2010

@author: nathanael
'''
import unittest

from amelethdf.model.api \
    import (Enum, Dict, List, ReadOnly, Tuple, Set,
        Complex, Float, Bool, String, Int, Array,
        HasTraits, BaseStr, UserName,
        Any, Instance,
        UserName, IsModel)
    
from amelethdf.model.model_inspector import load

__all__ = ['SimpleContainer']

class SimpleContainer(IsModel):
    """model class with simple data
    """
    i = Int
    f = Float
    b = Bool
    c = Complex
    s = String

class Test(unittest.TestCase):

    def setUp(self):
        pass
    def tearDown(self):
        pass
        
    def test_type_attr(self):
        node = load(SimpleContainer)
        
        self.assertEqual(node.get_attr_type('i'), int)
        self.assertEqual(node.get_attr_type('f'), float)
        self.assertEqual(node.get_attr_type('b'), bool)
        self.assertEqual(node.get_attr_type('c'), complex)
        self.assertEqual(node.get_attr_type('s'), str)


if __name__ == "__main__":
    unittest.main()
