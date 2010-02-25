#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''
import unittest

from amelethdf.path import join, split, isabs, normpath, splits, format

class TestPast(unittest.TestCase):

    def test_format(self):
        self.assertEqual(format(''), '')
        self.assertEqual(format('/A/A'), '/A/A')
        self.assertEqual(format('/A/A/'), '/A/A')

    def test_join(self):
        self.assertEqual(join('a', 'b'), 'a/b')
        self.assertEqual(join('/', ''), '/')
        
    def test_split(self):
        self.assertEqual(split('/a/b/c'), ('/a/b', 'c'))
        self.assertEqual(split('a/b/c'), ('a/b', 'c'))
        self.assertEqual(split('a/b/c/'), ('a/b/c', ''))
        self.assertEqual(split('a'), ('', 'a'))
        self.assertEqual(split('/'), ('/', ''))
        self.assertEqual(split('.'), ('', '.'))
        self.assertEqual(split('/a'), ('/', 'a'))
        
    def test_isabs(self):
        self.assertFalse(isabs(''))
        self.assertTrue(isabs('/'))
        self.assertTrue(isabs('//'))
        self.assertTrue(isabs('/a'))
        self.assertFalse(isabs('a'))
        
    def test_normpath(self):
        self.assertEqual(normpath('//'), '/')
        self.assertEqual(normpath('/a/a'), '/a/a')
        self.assertEqual(normpath('//a/a'), '/a/a')
        self.assertEqual(normpath('/a/a/'), '/a/a')
        self.assertEqual(normpath('/./'), '/')
        self.assertEqual(normpath('/a/a/.'), '/a/a')
        self.assertEqual(normpath('//.'), '/')
        
        self.assertEqual(normpath('/path/where.my/is'), '/path/where.my/is')
        
    def test_splits(self):
        self.assertEqual(splits('/a/b/c'), ['', 'a', 'b', 'c'])
        self.assertEqual(splits('a/b/c'), ['a', 'b', 'c'])
        self.assertEqual(splits('a/b/c/'), ['a', 'b', 'c', ''])
        self.assertEqual(splits('a'), ['a'])
        self.assertEqual(splits('/'), ['', ''])

if __name__ == "__main__":
    unittest.main()
