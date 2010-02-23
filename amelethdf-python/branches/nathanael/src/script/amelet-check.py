#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 16 févr. 2010

@author: nathanael
'''

import sys

from optparse import OptionParser, OptionGroup

from amelethdf import openAmelet, openHDF, VERSION, HDFNode, Node

USAGE = """
    %s [OPTIONS] amelet-file
    """ % (sys.argv[0])
DESCRIPTION = """Check the Amelet-HDF file give."""


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
                check.append("%s Invalid attr name '%s'" % (self.node_r.path, name))
            else:
                val = self.node_r.get_attr(name)
                if not self.node_c.can_set_attr(name, val):
                    check.append("%s Invalid value for attr name '%s'" % (self.node_r.path, name))
        return check
                
    
    def check(self):
        check = []
        if not self.check_node():
            check.append("%s Invalid child named '%s'" % (self.parent.node_r.path, self.node_r.name))
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
        self.root = CheckNode(openHDF(self.file), openAmelet(self.file))
        
        
     
    def __str__(self):
        return '\n'.join(self.root.check())

    def __del__(self):
        self.root.close()


if __name__ == '__main__':
    parser = OptionParser(usage=USAGE,
                          description=DESCRIPTION,
                          version=VERSION)
    parser.add_option("-i",
                      dest="h5file",
                      metavar="FILE",
                      help="Amelet-HDF file.")
    
    
    
    
    (options, args) = parser.parse_args()
    
    if len(args) >= 1 :
        h5file = args[0]
    elif options.h5file != None:
        h5file = options.h5file
    else:
        parser.error("the Amelet-HDF file is not pointed out.")
    
    print Check(h5file)
