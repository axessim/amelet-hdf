#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 fevr. 2010

@author: nathanael
'''

from amelethdf.node import openAmelet, openHDF, NTYPE_DATASET

__all__ = ['Check']


ERR_CONST_TYPE = "'%s' Invalid type for const name '%s': TRAC: %s"
ERR_CONST_VALUE = "'%s' Invalid const '%s' value expected '%s' read '%s'"
ERR_ATTR_NAME = "'%s' Invalid attr name '%s' expected %s"
ERR_ATTR_TYPE = "'%s' Invalid type for attr name '%s': TRAC: %s"
ERR_ATTR_VALUE = "'%s' Invalid value for attr name '%s'"
ERR_CHILD_NAME = "'%s' Invalid child named '%s' expected %s"
ERR_LEAF_ARRAY = "'%s' Invalid DataSet '%s' property read {%s}, expected %s"

class CheckNode:
    def __init__(self, node_r, node_c, parent=None):
        self.node_r = node_r # node used to read
        self.node_c = node_c # node used to check the model
        
        self.parent = parent
        
    def close(self):
        self.node_r.close()
        self.node_c.close()
        
    def check_node(self):
        if self.parent is None:
            return []
        else:
            if (self.parent.node_c.can_create_child(self.node_r.name) 
                and self.node_c != None):
                if self.node_c.ntype == NTYPE_DATASET:
                    
                    def build_property(array):
                        property_list = ['dtype', 'shape']
                        l = ["%s=%s" % (name, str(getattr(array, name))) 
                             for name in property_list]
                        return ', '.join(l)
                    
                    array = self.node_r.hdfo[:]
                    if not (self.parent.node_c.model.\
                                is_legale_value(self.node_r.name, array)
                            and self.node_c.model.is_legale_array(array)):
                        return [ERR_LEAF_ARRAY % (self.parent.node_r.path,
                                                  self.node_r.name,
                                                  build_property(self.node_r.hdfo[:]),
                                                  self.node_c.model._DrawingTree__tostring_type())]
                    
                return []
            else:
                return [ERR_CHILD_NAME % (self.parent.node_r.path,
                                          self.node_r.name,
                                          self.parent.node_c.children_name)]
            
    def check_attrs(self):
        check = []
        
        for name in self.node_r.attrs_name:
            if name not in self.node_c.model.attrs_name:
                if name in self.node_c.model.consts_name:
                    val = self.node_r.get_attr(name)
                    val_type = self.node_c.model.get_attr_type(name)
                    
                    try:
                        val = val_type(val)
                    except Exception, e:
                        check.append(ERR_CONST_TYPE % (self.node_r.path,
                                                       name,
                                                       str(e)))
                    
                    if val != self.node_c.model.get_const(name):
                        check.append(ERR_CONST_VALUE % 
                                     (self.node_r.path, name,
                                      str(self.node_c.model.get_const(name)),
                                      str(val)))
                    
                else:
                    check.append(ERR_ATTR_NAME % (self.node_r.path,
                                                  name,
                                                  str(self.node_c.model.attrs_name)))
            else:
                val = self.node_r.get_attr(name)
                val_type = self.node_c.model.get_attr_type(name)
                
                try:
                    val = val_type(val)
                except Exception, e:
                    check.append(ERR_ATTR_TYPE % (self.node_r.path, name, str(e)))
                
                if not self.node_c.can_set_attr(name, val):
                    check.append(ERR_ATTR_VALUE % (self.node_r.path, name))
        return check
                
    
    def check(self):
        check = self.check_node()
        
        if not check:
            check.extend(self.check_attrs())
            for child in self:
                check.extend(child.check())
                
        return check
        
    
    def __iter__(self):
        for child_r in self.node_r:
            if not self.node_c.has_already_child(child_r.name):
                child_c = None
            else:
                child_c = self.node_c.get_child(child_r.name)
                
            yield CheckNode(child_r, child_c, self)

class Check:
    def __init__(self, file):
        self.file = file
        self.root = CheckNode(openHDF(self.file),
                              openAmelet(self.file))
        
     
    def __str__(self):
        return '\n'.join(self.root.check())

    def __del__(self):
        self.root.close()
