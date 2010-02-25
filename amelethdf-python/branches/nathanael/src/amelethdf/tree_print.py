#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 11 fevr. 2010

@author: nathanael
'''


__all__ = ['TreeLink']

TL_NON = ' '
TL_END = '`'
TL_NEW = '|'
TL_FOL = '|'


TTL_G = '--'
TTL_L = '*-'
TTL_E = '?-'
TTL_N = '  '

TL_TAG = ' '


GROUP_TAG = '/'
S_TAB = '['
E_TAB = ']'
S_DATA = '{'
E_DATA = '}'


ATTR1 = '@'
ATTR2 = '#'
ATTR_SEP = ', '
ATTR_ASIGN = '='
S_ATTRS = '('
E_ATTRS = ')'


class TreeLink:
    def __init__(self):
        self.tree_link = []
        
    def close_branch(self):
        self.tree_link = [(((e[0] not in (TL_END, TL_NON)) and TL_FOL) or TL_NON) + TTL_N + TL_TAG
                           for e in self.tree_link]
        
    def append(self, type, close=False):
        self.close_branch()
        self.tree_link.append(((close and TL_END) or TL_NEW) + type + TL_TAG)
    
    def close_end(self, type=None):
        end = self.tree_link[-1]
        if end[0] != TL_NON:
            new_end = TL_END + (type or end[1:3])
        else:
            new_end = TL_NON + TTL_N
        self.tree_link[-1] = new_end + TL_TAG
        
    def follow(self, type, close=False):
        self.tree_link[-1] = ((close and TL_END) or TL_NEW) + type + TL_TAG
        
    def down(self):
        self.tree_link.pop()
            
    def __str__(self):
        return ''.join(self.tree_link)
    
    
    
    
class DrawingTree(object):
    """
    
    Friend of Tree 
    """
    
    def __get_children_type(self):
        return TTL_G
    
    def __get_attrs_name(self):
        return self.attrs_name
    
    def __get_attr_value(self, name):
        return None
    
    def __get_attr_type(self, name):
        return ATTR1
    
    def tostring(self):
        """Return a tree string of this model node.
        """
        return self.__tostring()
    
    #---------------------------------------------------------------------------
    # Methods to build string tree view
    #---------------------------------------------------------------------------
    def __tostring(self, tree_link=TreeLink()):
        """Internal method to build tree string of this model node.
        """
        s = self.__tostring_line(tree_link=tree_link)
        
        bt = self.__get_children_type()
        
        
        tree_link.append(bt)
        items = self.iter_children()
        i = 0
        for item in items:
            i += 1
            tree_link.follow(bt, len(items) == i)
            s += '\n' + item.__tostring(tree_link)
        tree_link.down()
                
        return s
    
    def __tostring_line(self, tree_link=None):
        """Internal method that return model node string. 
        """
        s = ""
        
        if tree_link:    
            s += str(tree_link) + self.name
        else:
            s += self.name
            
        s += self.__tostring_attrs() + self.__tostring_type()
        
        return s 
            
    def __tostring_attrs(self):
        """Internal method that build string view of model attributes.
        """
        
        attrs = []
        for attr_name in self.__get_attrs_name():
            val = self.__get_attr_value(attr_name)
            attr = "%s%s" % (self.__get_attr_type(attr_name), attr_name)
            
            if val != None:
                attr = "%s%s%s" % (attr, ATTR_ASIGN, val)
            attrs.append(attr)
                
        if attrs:    
            return "%s%s%s" % (S_ATTRS, ATTR_SEP.join(attrs), E_ATTRS)
        else:
            return ""
    
    
    def __tostring_type(self):
        """Internal method to build string view model type
        """
        return GROUP_TAG
    
