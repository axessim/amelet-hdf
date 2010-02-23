#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''
import unittest
from tempfile import NamedTemporaryFile
from tables import openFile

from amelethdf.path import split

from amelethdf.node.node_hdf import HDFNode, HDFNodeError
from tables import IntAtom, FloatAtom, IsDescription, StringCol, IntCol

import numpy

class Test(unittest.TestCase):

    def setUp(self):
        self.h5file = NamedTemporaryFile()
        self.h5 = openFile(self.h5file.name, 'w')
    
    def tearDown(self):
        self.h5.close()
        
        
    def test_new_root(self):
        """Test create a new root.
        """
        r = HDFNode(self.h5, '/', '')
        self.assertEqual(r.name, '')
        self.assertEqual(r.path, '/')
        r.close()
        
    def test_to_string_methods(self):
        """Test to string methods.
        """
        r = HDFNode(self.h5, '/', '')
        self.assertEqual(str(r), "/ (RootGroup) ''")
        self.assertEqual(r.__repr__(), '<HDFNode object at />')
        self.assertEqual(r.tostring(), split(r.hdf.filename)[1])
        
        r = HDFNode(self.h5, '/', '')
        g1 = r._create_group('g1')
        self.assertEqual(r.tostring(), '%s\n`-- g1' % split(r.hdf.filename)[1])
        
    def test_create_group(self):
        """Test all methods to create group.
        """
        r = HDFNode(self.h5, '/', '')
        
        g1 = r._create_group('g1')
        self.assertEqual(g1.name, 'g1')
        self.assertEqual(g1.path, '/g1')
        self.assertEqual(len(r), 1)
        
        gc = r.get_node('/g1')
        self.assertEqual(gc, g1)
        
        g2 = r._create_group('g2')
        self.assertNotEqual(g1, g2)
        self.assertEqual(len(r), 2)
        
        g1g1 = g1._create_group('gg1')
        self.assertEqual(g1g1.name, 'gg1')
        self.assertEqual(g1g1.path, '/g1/gg1')
        self.assertEqual(len(g1), 1)
        
        
        r2 = g1g1.get_root()
        self.assertEqual(r2.name, '')
        self.assertEqual(r2.path, '/')
        self.assertEqual(r, r2)
        
    def test_get_node(self):
        """Test get node.
        """
        r = HDFNode(self.h5, '/', '')
        g1 = r._create_group('g1')
        g1g1 = g1._create_group('gg1')
        
        g1g1ca = r.get_node('/g1/gg1')
        g1g1cr = g1.get_node('gg1')
        self.assertEqual(g1g1, g1g1ca)
        self.assertEqual(g1g1, g1g1cr)
        
        self.assertRaises(HDFNodeError, r.get_node, '/k')
        
        
    def test_create_dataset_1(self):
        """Test create a dataset 1D
        """
        r = HDFNode(self.h5, '/', '')
        
        d = r._create_dataSet('d', IntAtom())
        d.hdfo.append([0, 1, 2, 3, 4, 5, 6])
        self.assertEqual(len(r), 1)
        self.assertEqual(len(d), 0)
        
        self.assertTrue(numpy.all(d.hdfo == numpy.arange(7)))
        self.assertEqual(d.hdfo.shape, (7,))
        
    def test_create_dataset_2(self):
        """Test create a dataset 2D
        """
        r = HDFNode(self.h5, '/', '')
        
        d = r._create_dataSet('d', FloatAtom(), (3, 0))
        d.hdfo.append(((0.1, 0.2, 0.3), (1.1, 1.2, 1.3), (2.1, 2.2, 2.3)))
        self.assertTrue(numpy.all(d.hdfo 
                                  == numpy.array(((0.1, 0.2, 0.3),
                                                  (1.1, 1.2, 1.3),
                                                  (2.1, 2.2, 2.3)))))
        self.assertEqual(d.hdfo.shape, (3, 3))
        
        
    def test_create_table(self):
        """Test create a table
        """
        r = HDFNode(self.h5, '/', '')
        
        class description(IsDescription):
            name = StringCol(16, pos=1)
            val = IntCol(pos=2)

        t = r._create_table('t', description)
        
        t.hdfo.append([("val-1           ", 2),
                       ("val 2           ", 23)])
        
        self.assertEqual(t.hdfo[0][0], 'val-1           ')
        self.assertEqual(t.hdfo[1][1], 23)
        
    
    def test_walk_iter(self):
        """Test `walk` method on all node of data tree.
        """
        r = HDFNode(self.h5, '/', '')
        
        g1 = r._create_group('g1')
        g2 = r._create_group('g2')
        
        g1g1 = g1._create_group('gg1')
        
        l = [i for i in r]
        self.assertEqual(l, [g2, g1])
        
        l = [i for i in r.walk()]
        self.assertEqual(l, [r, g2, g1, g1g1])
        
        

if __name__ == "__main__":
    unittest.main()
