#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 fevr. 2010

@author: nathanael
'''

from amelethdf.node import openAmelet, openHDF

__all__ = ['Check']

class CheckNode:
    def __init__(self, node_r, node_c, parent=None):
        self.node_r = node_r
        self.node_c = node_c
        
        self.parent = parent
        
    def close(self):
        self.node_r.close()
        self.node_c.close()
        
    def check_node(self):
        if self.parent is None:
            return True
        else:
            return (self.parent.node_c.can_create_child(self.node_r.name) 
                    and self.node_c != None) 
            
    def check_attrs(self):
        check = []
        
        for name in self.node_r.attrs_name:
            if name not in self.node_c.attrs_name:
                check.append("'%s' Invalid attr name '%s'" % (self.node_r.path, name))
            else:
                val = self.node_r.get_attr(name)

                val_type = self.node_c.model.get_attr_type(name)
                
                try:
                    val = val_type(val)
                except Exception, e:
                    check.append("'%s' Invalid type for attr name '%s': TRAC: %s" % (self.node_r.path, name, str(e)))
                
                if not self.node_c.can_set_attr(name, val):
                    check.append("'%s' Invalid value for attr name '%s'" % (self.node_r.path, name))
        return check
                
    
    def check(self):
        check = []
        if not self.check_node():
            check.append("'%s' Invalid child named '%s'" % (self.parent.node_r.path, self.node_r.name))
        else:
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
