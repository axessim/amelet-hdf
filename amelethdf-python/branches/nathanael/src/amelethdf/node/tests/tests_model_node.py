#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''
import unittest

from tempfile import NamedTemporaryFile
from tables import openFile

from amelethdf.node.node_model \
    import (pathof, split_xname, DEFAULT_MODEL, ModelNode, ModelNodeError, HDFNodeError)
    
from amelethdf.model.api \
    import (Enum, Dict, List, ReadOnly, Tuple, Set,
        Complex, Float, Bool, String, Int, Array,
        HasTraits, BaseStr, UserName,
        Any, Instance,
        UserName, DataSet, Group, Table, IsModel, TableObject)
from amelethdf.model.node import load

__all__ = ['AA', 'AAA', 'AAB', 'SimpleContainer', 'ConstContainer',
           'ComplexContainer']


class AA(IsModel):
    """first model class
    """
    c = String
    
class AAA(AA):
    """first model sub-class  
    """
    c = String('a')
    
class AAB(AA):
    """second model sub-class
    """
    c = String('b')

class SimpleContainer(IsModel):
    """model class with simple data
    """
    i = Int
    f = Float
    b = Bool
    c = Complex
    s = String
    
class ConstContainer(IsModel):
    """model class with constants  
    """
    i = Int(1)
    f = Float(1.0)
    b = Bool(True)
    c = Complex(complex(0.1, 0.1))
    s = String('string')
    
class ComplexContainer(IsModel):
    """genetic model class 
    """
    const = String('MODEL')
    
    # data set
    array = Array
    
    # 
    subContainer = Instance(SimpleContainer)
    
    # 
    list_of = Dict(UserName, Instance(ConstContainer))
    # 
    list_indexed = List(Instance(AA))
    
    #
    table_t = List(Tuple(Instance(String), Instance(Int)))

                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
class Test_tools(unittest.TestCase):
    def test_path_of_xpath(self):
        """Test the function to conv xpath to hdf path.
        """
        self.assertEqual(pathof('/a/b/c'), '/a/b/c')
        self.assertEqual(pathof('/a/b()/c'), '/a/b/c')
        self.assertEqual(pathof('/a/b()/c()'), '/a/b/c')
        self.assertEqual(pathof('/a/b(a=j)/c'), '/a/b/c')
        
        self.assertEqual(pathof('/a/b/c/'), '/a/b/c')
        self.assertEqual(pathof('/a/b/c[]'), '/a/b/c')
        self.assertEqual(pathof('/a/b/c{}'), '/a/b/c')
        
    def test_split_xname(self):
        """Test the function that split xname (get the select node informations).
        """
        name, d = split_xname('name')
        self.assertEqual(name, 'name')
        self.assertEqual(d, {})
        
        name, d = split_xname('name(a=5)')
        self.assertEqual(name, 'name')
        self.assertEqual(d, {'a':'5'})
        
        name, d = split_xname('name(a=5, b=a)')
        self.assertEqual(name, 'name')
        self.assertEqual(d, {'a':'5', 'b':'a'})
        
        
        
        
        
        
        
        
        

class Test(unittest.TestCase):
    def setUp(self):
        self.h5file = NamedTemporaryFile()
        self.h5 = openFile(self.h5file.name, 'w')
    
    def tearDown(self):
        self.h5.close()
        
    def test_new_node(self):
        r = ModelNode(self.h5, '/', '', DEFAULT_MODEL)
        self.assertEqual(r.name, '')
        self.assertEqual(r.path, '/')
        self.assertEqual(r.xpath, '/')
        
    def test_name_of_sub_elements(self):
        r = ModelNode(self.h5, '/', '', load(SimpleContainer))
        self.assertEqual(r.attrs_name, [])
        self.assertEqual(r.children_name, [])
        self.assertEqual(len(r), 0)
        
        self.assertEqual(r.model.attrs_name, ['c', 'b', 'f', 'i', 's'])
        self.assertEqual(r.model.children_name, [])
        self.assertEqual(r.model.consts, [])
        
        #
        #
        #
        r = ModelNode(self.h5, '/', '', load(ConstContainer))
        self.assertEqual(r.model.consts, [('c', (0.1 + 0.1j)),
                                          ('b', True),
                                          ('f', 1.0),
                                          ('i', 1),
                                          ('s', 'string')])
        self.assertEqual(r.model.children_name, [])
        self.assertEqual(r.model.attrs_name, [])
        
        self.assertEqual(r.children_name, [])
        self.assertEqual(r.attrs_name, [])
        self.assertEqual(len(r), 0)
        
        
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        self.assertEqual(r.model.attrs_name, [])
        self.assertEqual(r.model.children_name, ['list_indexed',
                                                 'list_of',
                                                 'table_t',
                                                 'subContainer',
                                                 'array'])
        self.assertEqual(r.model.consts, [('const', 'MODEL')])
        
        self.assertEqual(r.attrs_name, [])
        self.assertEqual(r.children_name, [])
        self.assertEqual(len(r), 0)
        
        
        
    def test_has_already(self):
        r = ModelNode(self.h5, '/', '', load(SimpleContainer))
        
        r.hdfo._f_setAttr('i', 1)
        r.hdfo._f_setAttr('a', 'a')
        
        self.assertTrue(r.has_already_attr('i'))
        self.assertTrue(r.has_already_attr('i', testalsomodel=False))
        
        self.assertFalse(r.has_already_attr('a'))
        self.assertTrue(r.has_already_attr('a', testalsomodel=False))
        
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        r._create_group('list_of')
        
        self.assertTrue(r.has_already_child('list_of'))
        self.assertTrue(r.has_already_child('list_of', testalsomodel=False))
        self.assertEqual(len(r), 1)
        
        self.assertFalse(r.has_already_child('a'))
        self.assertFalse(r.has_already_child('a', testalsomodel=False))
        
        
    def test_can_create_child(self):
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        self.assertTrue(r.can_create_child('list_of'))
        self.assertFalse(r.can_create_child('a'))
    
    def test_can_set_data(self):
        """Test can set data
        
        Don't test the value just attribute's name 
        """
        r = ModelNode(self.h5, '/', '', load(SimpleContainer))
        
        self.assertTrue(r.can_set_attr('c', 1))
        self.assertTrue(r.can_set_attr('i', 0.2))
        
        self.assertFalse(r.can_set_attr('r', '2'))
        
    def test_can_set_const_data(self):
        
        r = ModelNode(self.h5, '/', '', load(ConstContainer))
        self.assertFalse(r.can_set_attr('c', 1))
        self.assertFalse(r.can_set_attr('r', '2'))
        self.assertFalse(r.can_set_attr('i', 0.2))
        self.assertTrue(r.can_set_attr('i', 1))
        
    def test_get_set_data(self):
        r = ModelNode(self.h5, '/', '', load(SimpleContainer))
        
        # get/set attr (data) on model
        self.assertFalse(r.has_already_attr('i'))
        r.set_attr('i', 1)
        self.assertEqual(r.attrs_name, ['i'])
        self.assertEqual(r.get_attr('i'), 1)
        self.assertTrue(r.has_already_attr('i'))
        
        # get/set attr (data) over the model
        self.assertRaises(ModelNodeError, r.set_attr, 'tt', 3)
        super(ModelNode, r).set_attr('tt', 3)
        self.assertTrue(r.has_already_attr('tt', testalsomodel=False))
        self.assertRaises(ModelNodeError, r.get_attr, 'tt')
        self.assertEqual(super(ModelNode, r).get_attr('tt'), 3)
        
    def test_get_set_child(self):
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        
        self.assertRaises(ModelNodeError, r.set_child, 'trucc')
        self.assertRaises(HDFNodeError, r.get_child, 'subContainer(tt=2)')
        
        
        self.assertRaises(HDFNodeError, r.get_child, 'subContainer')
        
        child = r.set_child('subContainer')
        self.assertEqual(child.name, 'subContainer')
        self.assertEqual(child.path, '/subContainer')
        self.assertTrue(isinstance(child, ModelNode))
        
        self.assertEqual(r._create_group('a'), None)
        self.assertRaises(ModelNodeError, r.set_child, 'a')
        
        
    def test_get_model(self):
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        
        m = r.get_model('list_of')
        self.assertEqual(m.xpath, '/complexContainer/list_of')
        
        r._create_group('list_of')
        c = r.get_child('list_of')
        self.assertEqual(c.model.xpath, '/complexContainer/list_of')
        
    def test_children_list_of(self):
        r = ModelNode(self.h5, '/', '', load(ComplexContainer))
        
        r._create_group('list_of')
        list_of = r.get_child('list_of')
        print list_of, list_of.model, list_of.model.children_name
        

if __name__ == "__main__":
    unittest.main()
