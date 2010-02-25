#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 22 févr. 2010

@author: nathanael
'''
import unittest


from amelethdf.model.api import \
    (IsModel, Int, Float, Bool, Complex, String)
from amelethdf.model.model_inspector import load

import numpy

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
        
    def test_int(self):
        m = load(SimpleContainer)
        
        self.assertTrue(m.is_legale_value('i', int()))
        self.assertTrue(m.is_legale_value('i', bool()))
        
        self.assertTrue(m.is_legale_value('i', numpy.int()))
        self.assertTrue(m.is_legale_value('i', numpy.int32()))
                
        self.assertFalse(m.is_legale_value('i', float()))
        self.assertFalse(m.is_legale_value('i', str()))
        self.assertFalse(m.is_legale_value('i', complex()))
        
        

if __name__ == "__main__":
    unittest.main()
