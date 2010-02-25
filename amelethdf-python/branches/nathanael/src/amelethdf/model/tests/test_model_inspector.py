#!/usr/bin/env python
# -*- coding: utf-8 -*-

from unittest import TestCase, main


 


from amelethdf.model.api \
    import (Enum, Dict, List, ReadOnly, Tuple, Set,
        Complex, Float, Bool, String, Int, Array,
        HasTraits, BaseStr, UserName,
        Any, Instance,
        UserName, IsModel)
    
from amelethdf.model import model_inspector as minspect
from amelethdf.model.model_inspector import load
    
    
__all__ = ['AA', 'AAA', 'AAB', 'SimpleContainer', 'ConstContainer',
           'ComplexContainer', 'EnumContainer']
    
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
    
    
class EnumContainer(IsModel):
    """
    """
    value = Instance(AA)
    

class tests_model_inspector(TestCase):
    def setUp(self):
        pass
    
    def tearDown(self):
        pass
    
    def test_to_string_line(self):
        node = load(SimpleContainer)
        self.assertEqual(node._DrawingTree__tostring_line(),
                         'simpleContainer(@c, @b, @f, @i, @s)/')
        
        node = load(ConstContainer)
        self.assertEqual(node._DrawingTree__tostring_line(),
                         'constContainer(#c=(0.1+0.1j), #b=True, #f=1.0, #i=1, #s=string)/')
        
        node = load(ComplexContainer)
        child = node.get_child('array')
        self.assertEqual(child._DrawingTree__tostring_line(), 'array{}')
        
        child = node.get_child('table_t')
        self.assertEqual(child._DrawingTree__tostring_line(), 'table_t[]')
        
        node = load(EnumContainer)
        child = node.get_child('value')
        self.assertEqual(child._DrawingTree__tostring_line(), 'value(@c)/')
        
    def test_to_string(self):
        node = load(ComplexContainer)
        
        self.assertEqual(node.tostring(),
"""complexContainer(#const=MODEL)/
|-- list_indexed/
|   `*- aA(@c)/
|       |?- aAA(#c=a)/
|       `?- aAB(#c=b)/
|-- list_of/
|   `*- $user_name(#c=(0.1+0.1j), #b=True, #f=1.0, #i=1, #s=string)/
|-- table_t[]
|-- subContainer(@c, @b, @f, @i, @s)/
`-- array{}""")
    
        node = load(EnumContainer)
        self.assertEqual(node.tostring(),
"""enumContainer/
`-- value(@c)/
    |?- aAA(#c=a)/
    `?- aAB(#c=b)/""")
    
    
    def test_simple_container(self):
        """Test simple container class model.
        """
        node = load(SimpleContainer)
        self.assertEqual(node.attrs_name, ['c', 'b', 'f', 'i', 's'])
        self.assertEqual(node.consts_name, [])
        self.assertEqual(node.children_name, [])
        
    def test_simple_const_container(self):
        """Test simple constant container class model.
        """
        node = load(ConstContainer)
        self.assertEqual(node.attrs_name, [])
        self.assertEqual(node.consts_name, ['c', 'b', 'f', 'i', 's'])
        self.assertEqual(node.children_name, [])
        
    def test_children_container(self):
        """Test the capacity of model to contain children.
        """
        node = load(ComplexContainer)
        self.assertEqual(node.attrs_name, [])
        self.assertEqual(node.consts_name, ['const'])
        self.assertEqual(node.children_name,
                         ['list_indexed', 'list_of', 'table_t',
                          'subContainer', 'array'])
        
        list_of = node.get_child('list_of')
        self.assertEqual(list_of.children_name, ['$user_name'])
        
        
    def test_node_type(self):
        """Test the hdf node type (group, data-set or table)
        """
        node = load(SimpleContainer)
        self.assertEqual(node.mtype, minspect.InstanceOf_Inspector.MTYPE)
        
        node = load(ConstContainer)
        self.assertEqual(node.mtype, minspect.InstanceOf_Inspector.MTYPE)
        
        node = load(ComplexContainer)
        self.assertEqual(node.mtype, minspect.InstanceOf_Inspector.MTYPE)
        
        child = node.get_child('array')
        self.assertEqual(child.mtype, minspect.Array_Inspector.MTYPE)
        
        child = node.get_child('subContainer')
        self.assertEqual(child.mtype, minspect.InstanceOf_Inspector.MTYPE)
        
        child = node.get_child('list_of')
        self.assertEqual(child.mtype, minspect.ListOf_Inspector.MTYPE)
        
        child = node.get_child('list_indexed')
        self.assertEqual(child.mtype, minspect.IListOf_Inspector.MTYPE)
        
        child = node.get_child('table_t')
        self.assertEqual(child.mtype, minspect.TableTupled_Inspector.MTYPE)
        
        node = load(EnumContainer)
        self.assertEqual(node.mtype, minspect.InstanceOf_Inspector.MTYPE)
        
        child = node.get_child('value')
        self.assertEqual(child.mtype, minspect.SubClassOf_Inspector.MTYPE)
        
        
if __name__ == "__main__":
    main()
