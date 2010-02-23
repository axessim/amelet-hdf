#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 22 févr. 2010

@author: nathanael
'''
import unittest
from tempfile import NamedTemporaryFile
from tables import openFile


from amelethdf.model.api \
    import (IsModel, Int, Float, Bool, Complex, String)
    
from amelethdf.model import load
from amelethdf.node.node_num import NumNode

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
        self.h5file = NamedTemporaryFile()
        self.h5 = openFile(self.h5file.name, 'w')
    
    def tearDown(self):
        self.h5.close()
        
    def test_can_set_attr(self):
        r = NumNode(self.h5, '/', '', load(SimpleContainer))
       
        
        self.assertTrue(r.can_set_attr('i', int()))
        self.assertTrue(r.can_set_attr('i', bool()))
        
        self.assertTrue(r.can_set_attr('i', numpy.int()))
        self.assertTrue(r.can_set_attr('i', numpy.int32()))
                
        self.assertFalse(r.can_set_attr('i', float()))
        self.assertFalse(r.can_set_attr('i', str()))
        self.assertFalse(r.can_set_attr('i', complex()))

    def test_can_set_data(self):
        r = NumNode(self.h5, '/', '', load(SimpleContainer))
        
        self.assertTrue(r.can_set_attr('c', 1))
        self.assertFalse(r.can_set_attr('r', '2'))
        self.assertFalse(r.can_set_attr('i', 0.2))

if __name__ == "__main__":
    unittest.main()
