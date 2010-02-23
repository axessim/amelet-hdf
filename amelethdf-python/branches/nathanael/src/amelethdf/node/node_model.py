#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''

from ..model import DEFAULT_MODEL
from ..path import join, isabs, splits

from node_hdf import HDFNode, HDFNodeError, openHDF


__all__ = ['openMNode', 'ModelNode', 'ModelNodeError', 'HDFNodeError']


class ModelNodeError(HDFNodeError): pass


def pathof(xpath):
    """Convert xpath to hdf path
    """
    def remove_end(s, e):
        if s[-1] == e:
            s = s[:-1]
        return s
    
    def remove_block(s, s_b, e_b):
        i = s.find(s_b)
        while i != -1:
            j = s.find(e_b, i)
            s = s[:i] + s[j + 1:]
            i = s.find(s_b, i)
        return s

    
    return remove_end(remove_block(remove_block(remove_block(
            xpath, '(', ')'), '{', '}'), '[', ']'), '/')
    
def split_xname(xname):
    i = xname.find('(')
    select = {}
    if i >= 0:
        name = xname[:i]
        j = xname.find(')', i)
        
        select = dict([s.split('=')
                       for s in xname[i + 1:j].split(', ')])
    else:
        name = xname
        
    return name, select
    
def openMNode(path, mode='r', model=DEFAULT_MODEL, **kw):
    hdf = openHDF(path, mode, **kw)
    return ModelNode(hdf.hdf, '/', '', model)

class ModelNode(HDFNode):
    def __init__(self, hdf, xwhere, name, model_root, model_node=None):
        super(ModelNode, self).__init__(hdf, pathof(xwhere), name)
        
        self.__xwhere = xwhere
        self.model_root = model_root
        self.model = model_node or model_root
        
    #---------------------------------------------------------------------------
    #--- Model node property
    #---------------------------------------------------------------------------
    @property
    def xname(self):
        """Return the xnmae of this node.
        """
        select = ', '.join(["%s=%s" % (name, str(self.model.get_const(name))) 
                            for name in self.model.consts_name])
        xname = self.name
        if select:
            xname += '(' + select + ')' + self.model._tostring_type()
        return xname
        
    @property
    def xpath(self):
        """Return the xpath of this node.
        
        The xpath is a path an tree model its shape is the following:
        
            xpath: '/group_name/*/node_selected(#const_name=const_val){}'
        """
        return join(self.__xwhere, self.xname)
    
    #---------------------------------------------------------------------------
    #--- Model access
    #---------------------------------------------------------------------------
    def get_model(self, xname, select={}, mtype=None):
        """Return the model of child `name` selected by `select`
        
        Note: select is a dict of constants values.
        """
        name, xselect = split_xname(xname)
        select.update(xselect)
        if self.can_create_child(name):
            return self.model.get_child(name, select)
        else:
            raise ModelNodeError('%s is node a children of %s' % (xname, self))
            
    #---------------------------------------------------------------------------
    #--- Tree methods
    #---------------------------------------------------------------------------
    def set_path(self, xpath):
        """Set the new path on this node.
        """
        if isabs(xpath):
            node = self.get_root()
        else:
            node = self
            
        for xname in splits(xpath):
            if xname != '':
                if node.has_already_child(xname):
                    node = node.get_child(xname)
                else:
                    node = node.set_child(xname)
            
        return node
    
    def get_path(self, xpath):
        """Get the node define by this `xpath`
        """
        return self.get_node(xpath)
    
    #---------------------------------------------------------------------------
    #--- Children methods
    #---------------------------------------------------------------------------
    # Implementation of `set_child` relatively to the model type node.
    def set_child(self, xname):
        """Set the new child with for name `xname`.
        """
        name, selecte = split_xname(xname)
        if self.can_create_child(name):
            child = self._create_group(name)
            for name, val in selecte:
                child.set_attr(name, val)
            return child
        else:
            raise ModelNodeError('%s is node a children of %s' % (xname, self))
         
    
    #---------------------------------------------------------------------------
    #--- Over definition of HDFNode 
    #---------------------------------------------------------------------------
    #
    # This over definition use the `model` to check the actions  
    #
    
    #
    # Tree methods
    def get_root(self):
        return self.__class__(self.hdf, '/', '', self.model_root)
    
    # 
    # Attributes methods
    def has_already_attr(self, name, testalsomodel=True):
        """Has already attribute named `name`
        
        @param testalsomodel: Test is the attr's `name` is on the model.
        """
        if testalsomodel:
            return (name in self.model.attrs_name
                    and super(ModelNode, self).has_already_attr(name))
        else:
            return super(ModelNode, self).has_already_attr(name)
        
    def can_set_attr(self, name, val):
        return ((name in self.model.attrs_name) 
                or ((name, val) in self.model.consts))
        
    def get_attr(self, name):
        if ((name in self.model.attrs_name or self.model.consts_name)
            and name in self.attrs_name):
            return super(ModelNode, self).get_attr(name)
        else:
            raise ModelNodeError("%s is not an data of %s" % (name, self))
        
    def set_attr(self, name, val):
        if self.can_set_attr(name, val):
            return super(ModelNode, self).set_attr(name, val)
        else:
            raise ModelNodeError("%s is not set data %s in %s" % 
                                 (name, str(val), self))
            
            
    # 
    # Children methods
    def has_already_child(self, name, testalsomodel=True):
        """Has already child named `name`
        
        @param testalsomodel: Test is the child `name` is on the model.
        """
        if testalsomodel:
            return (self.can_create_child(name) 
                    and super(ModelNode, self).has_already_child(name))
        else:
            return super(ModelNode, self).has_already_child(name)
        
   
    def can_create_child(self, name):
        return (name in self.model.children_name 
                or self.model.has_children_user_name())
    

    def get_child(self, xname, select={}, mtype=None):
        name, xselect = split_xname(xname)
        xselect.update(select)
        
        node = HDFNode(self.hdf, self._HDFNode__where, self._HDFNode__name)
        node = node.get_child(name)
        
        for attr_name in node.attrs_name:
            xselect.setdefault(attr_name, node.get_attr(attr_name))
        
        return super(ModelNode, self).get_child(name,
                model=self.get_model(name, xselect, mtype))
        
    #
    # Iterator on Children
    def __len__(self):
        if hasattr(self.hdfo, '_v_children'):
            i = 0
            model_children_name = self.model.children_name
            for name in self.children_name:
                if name in model_children_name:
                    i += 1
            return i
        else:
            return 0
    
    def __iter__(self):
        if hasattr(self.hdfo, '_v_children'):
            model_children_name = self.model.children_name
            for child_name in self.children_name:
                if child_name in model_children_name:
                    yield self.get_child(child_name)
                    
                    
    def _build_child(self, name, model=None):
        """Internal method to build child.
        
        Prefer it at __class__ to build a child.
        
        Give `model` information of node to node.
        """
        if model:
            return self.__class__(self.hdf, self.xpath, name,
                                  self.model_root, model)
        else:
            return self.__class__(self.hdf, self.xpath, name,
                                  self.model_root, self.get_model(name))
