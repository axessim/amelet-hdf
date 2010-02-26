#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 févr. 2010

@author: nathanael
'''




from api import (Complex, Float, Bool, String, Int, UserName,
                 Array, Dict, List, Enum, Tuple,
                 Instance, IsModel, IsArray, IsTable, IsGroup)

from amelethdf import path as apath
from model_db import ModelDataBase

from amelethdf.tree_print import TreeLink, TTL_E, TTL_G, TTL_L


ATTR_TYPE = (Complex, Float, Bool, String, Int, UserName)




USER_NAME = "$user_name"


def make_hdfname(name):
    """Make a hdf name in well cast.
    
        >>> make_hdfname('ClassName')
        'className'
    """
    return name[0].lower() + name[1:]


def traitmodel2model(model):
    child = model.trait_type
    return isinstance(child, Instance) and child.tlass or child

def only_children(model):
    """To get only children
    """
    return not model.is_trait_type(ATTR_TYPE)

def only_attrs(model):
    """To get only attributes
    """
    return model.is_trait_type(ATTR_TYPE) and not model.default


def only_consts(model):
    """To get only constants
    """
    return model.is_trait_type(ATTR_TYPE) and model.default


class InspectorError(Exception):
    """Base exception for model.
    """
    

from ..tree_print import DrawingTree


class WithDataBase:
    def __init__(self, db=None):
        if not db:
            db = ModelDataBase()
        self._db = db
        
        

class TraitInspector(DrawingTree, WithDataBase):
    def __init__(self, model=None, name=None, _model_db=None, _xpath='/'):
        WithDataBase.__init__(self, _model_db)
        
        self.model = model
        
        if not name:    
            if isinstance(model, Instance):
                name = make_hdfname(model.klass.__name__)
            else:
                name = make_hdfname(model.__name__)
        
        self.name = name
        self.xpath = apath.join(_xpath, name)
        
        if isinstance(model, (Array, Dict, List, Enum)):
            self.tlass = model
            
        elif isinstance(model, Instance):
            if hasattr(model.klass, '__class_traits__'):
                self._db.recorde(model.klass, self.xpath)
                self.tlass = model.klass.__class_traits__
                self.klass = model.klass
                
        elif hasattr(model, '__class_traits__'):
            self._db.recorde(model, self.xpath)
            self.tlass = model.__class_traits__
            self.klass = model
            
        else:
            raise InspectorError('Invalid model %s' % str(model))
        
    @classmethod
    def init_by_copy(cls, inspector):
        """Build a new instance by copy of given inspector

        

        """
        inited = cls.__new__(cls)
        inited.model = inspector.model
        inited.name = inspector.name
        inited.xpath = inspector.xpath
        inited.tlass = inspector.tlass
        inited._db = inspector._db
        if inspector.has_klass():
            inited.klass = inspector.klass
        return inited 
        
    
    
    def has_klass(self):
        return hasattr(self, 'klass')
    
    
        
    #---------------------------------------------------------------------------
    # All methods associated to string view of instance.
    #---------------------------------------------------------------------------
    def __str__(self):
        """Return the string view of this instance.
        """
        return self.__repr__()
    
    def __repr__(self):
        return "<%s of %s>" % (self.__class__.__name__, self.xpath)
    
    
    #---------------------------------------------------------------------------
    # All property
    #---------------------------------------------------------------------------
    @property
    def ttype(self):
        """Return the type of model node.
        """
        return type(self.tlass)
    
    #---------------------------------------------------------------------------
    #--- Attributes
    #---------------------------------------------------------------------------
    @property
    def attrs_name(self):
        """Return attributes name
        """
        return self._get_items_name(only_attrs)
    
    def has_attrs_user_name(self):
        for name in self.attrs_name:
            if name == USER_NAME:
                return True
        return False
    
    def get_attr_type(self, name):
        """Return the right type of the attribute `name`. If any attribute of 
        this name raise an error.
        
        """
        return type(self.tlass.get(name).default)
    
    #---------------------------------------------------------------------------
    #
    #---------------------------------------------------------------------------
    def is_legale_value(self, name, value):
        """Check if can set the attribute `name` with the `value`. 
        """
        try:
            self.tlass.get(name).validate(self.klass, name, value)
            return True
        except Exception, e:
            print 'Warning: illegal value `%s` ' % str(e)
            return False
 
    
    #---------------------------------------------------------------------------
    #--- Const
    #---------------------------------------------------------------------------
        
    @property
    def consts_name(self):
        """Return constants name
        """
        return self._get_items_name(only_consts)
    
    def get_const(self, name):
        return self.tlass.get(name).default
    
    @property
    def consts(self):
        """Return constants list (name and value)
        """
        return [(name, self.get_const(name))
                 for name in self.consts_name]


    #---------------------------------------------------------------------------
    # Children
    #---------------------------------------------------------------------------
    @property
    def children_name(self):
        """Return children name
        """
        return self._get_items_name(only_children)
        
    def has_children_user_name(self):
        for name in self.children_name:
            if name == USER_NAME:
                return True
        return False

    def iter_children(self):
        """Rerun a list of sub model node.
        """
        return [self._build_child(traitmodel2model(model), name)
                for name, model in self._tlass_items(only_children)]
        
    def get_child(self, name, **kw):
        """Get the model node of the child
        """
        child = self.tlass.get(name)
        return self._build_child(traitmodel2model(child), name)

    #---------------------------------------------------------------------------
    # All internal methods
    #---------------------------------------------------------------------------
    def _load(self):
        """Load the model
        
        Note: use this method just one time, when you open class root of our 
        model.
        """
        if self.has_klass():
            self._db.load_class_model(self.klass)
    
    
    def _build_child(self, model, name, **kw):
        """Internal method to build child.
        """
        return self.__class__(model,
                              name=name,
                              _db=self._db,
                              _xpath=self.xpath,
                              ** kw)
        
    def _tlass_items(self, with_=lambda model: True):
        if hasattr(self.tlass, 'items') and self.tlass.items:
            return [(name, model) 
                    for name, model in self.tlass.items()
                    if (name.find('trait_') == -1 
                        and name.find('_items') == -1 
                        and with_(model))]
        else:
            return []

                
    def _get_items_name(self, with_=lambda model: True):
        """Generic method to get sub element of this.
        
        @param with_: Test to keep or not the given sub-element. 
        """
        return [name for name, model in self._tlass_items(with_)]
