#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 15 févr. 2010

@author: nathanael
'''

from tables import openFile

from amelethdf.path import join, isabs, splits, split
from amelethdf.tree_print import TreeLink, TTL_G


__all__ = ['openHDF', 'HDFNode', 'HDFNodeError']


class HDFNodeError(Exception): pass

def openHDF(path, mode='r', PYTABLES_SYS_ATTRS=False, **kw):
    hdf = openFile(path, mode, PYTABLES_SYS_ATTRS=PYTABLES_SYS_ATTRS, **kw)
    return HDFNode(hdf, '/', '')



class HDFNode(object):
    def __init__(self, hdf, where, name):
        self.hdf = hdf
        self.__where = where
        self.__name = name
        
    
    
    
    def __eq__(self, other):
        return self.path == other.path
    
    #---------------------------------------------------------------------------
    #--- To string methods
    #---------------------------------------------------------------------------
    def __str__(self):
        """Return the default string view of this node.
        """
        return str(self.get_hdfo())
    
    def tostring(self):
        """Return the string tree view of this object.
        """
        return self._tostring()
    
    def __repr__(self):
        return "<%s object at %s>" % (self.__class__.__name__, self.path)
    
    
    #---------------------------------------------------------------------------
    #--- File meta data an tools
    #---------------------------------------------------------------------------
    def close(self):
        """Close the file
        """
        self.hdf.close()
        
    @property
    def isopen(self):
        return self.hdf.isopen
    
    @property
    def filename(self):
        return self.hdf.filename
    
    
    #---------------------------------------------------------------------------
    #---  PyTables access
    #---------------------------------------------------------------------------
    def get_hdfo(self):
        """Return the hdf object of this node.
        """
        if self.__where == '/' and self.name == '':
            return self.hdf.root
        else:
            return self.hdf.getNode(self.path)
    hdfo = property(get_hdfo)
        
        
    #---------------------------------------------------------------------------
    #--- HDF Node property
    #---------------------------------------------------------------------------
    @property
    def name(self):
        """Return the name of this data node.
        """
        return self.__name 
        
    @property
    def path(self):
        """Return the path of this node on the HDF File.
        """
        return join(self.__where, self.__name)
    
    #---------------------------------------------------------------------------
    #--- Tree methods
    #---------------------------------------------------------------------------
    def get_root(self):
        """Return the root node of this tree.
        """
        return self.__class__(self.hdf, '/', '')
    
    def get_node(self, path):
        """Get a children node of this node.
        """
        if isabs(path):
            node = self.get_root()
        else:
            node = self
            
        for name in splits(path):
            if name != '':
                node = node.get_child(name)
            
        return node
    
    #---------------------------------------------------------------------------
    #--- Attributes methods
    #---------------------------------------------------------------------------
    
    def can_set_attr(self, name, val):
        return True

    @property
    def attrs_name(self):
        """Return attributes list name.
        """
        return self.hdfo._v_attrs._v_attrnamesuser

    def has_already_attr(self, name):
        """Has already attribute named `name`
        """
        return  hasattr(self.hdfo._v_attrs, name)
    
    def set_attr(self, name, val):
        """Set the attribute `name` at value `val`
        
        Wrapper of PyTables method
        """
        return self.hdfo._f_setAttr(name, val)
    
    def get_attr(self, name):
        """Get the value of the attribute `name`
        
        Wrapper of PyTables method
        """
        return self.hdfo._f_getAttr(name)
    
    def del_attr(self, name):
        """Delete the attribute `name` 
        
        Wrapper of PyTables method
        """
        return self.hdfo._f_delAttr(name)
    
    
    #---------------------------------------------------------------------------
    #--- Children methods
    #---------------------------------------------------------------------------
    def can_create_child(self, name):
        return True
    
    def has_already_child(self, name):
        """has already child the child named `name`
        """
        return name in self.hdfo._v_children.keys()
    
    @property
    def children_name(self):
        """Return children list name.
        """
        return self.hdfo._v_children.keys()
        
    def get_child(self, name, **kw):
        """Set a new child at this node.
        """
        if self.has_already_child(name):
            return self._build_child(name, **kw)
        else:
            raise HDFNodeError("Invalid children '%s' for the node '%s'" % (name, str(self)))
        
    #---------------------------------------------------------------------------
    #--- Iterator on Children
    #---------------------------------------------------------------------------
    def __len__(self):
        """Return the numbre of children.
        """
        if hasattr(self.hdfo, '_v_children'):
            return len(self.hdfo._v_children.keys())
        else:
            return 0
        
    def __iter__(self):
        """iteration on the children
        """
        if hasattr(self.hdfo, '_v_children'):
            for child_name in self.hdfo._v_children.keys():
                yield self.get_child(child_name)
        
    
    def walk(self):
        """Visit all tree
        """
        yield self
        if len(self) > 0:
            for child in self:
                for sub in child.walk():
                    yield sub
            
    
    
    #---------------------------------------------------------------------------
    #--- Internal API
    #---------------------------------------------------------------------------
    
    def _hdf_node(self):
        return HDFNode(self.hdf, self.__where, self.__name)
    
    def _build_child(self, name):
        """Internal method to build child.
        
        Prefer it at __class__ to build a child.
        """
        return self.__class__(self.hdf, self.path, name)
        
    def _tostring(self, tree_link=TreeLink()):
        """Internal API
        
        Build a string tree view of this object
        """
        if self == self.get_root():
            name = split(self.hdf.filename)[1]
        else:
            name = self.name
            
        s = str(tree_link) + name + self._tostring_attrs()
        
        tree_link.append(TTL_G)
        
        i = 0
        for item in self:
            i += 1
            tree_link.follow(TTL_G, len(self) == i)
            s += '\n' + item._tostring(tree_link)
        tree_link.down()
        
        return s
    
    def _tostring_attrs(self):
        """Internal API
        
        Build a string view of this node's attributes
        """
        s = ''
        
        i = 0
        attrs_name = self.attrs_name
        if len(attrs_name) > 0:
            s += '('
            for attr_name in attrs_name:
                s += "%s%s=%s" % ('@', attr_name, str(self.get_attr(attr_name)))
                i += 1
                if i != len(attrs_name):
                    s += ', '
            s += ')'
            
        return s
    
    def _create_group(self, name, **kw):
        """Internal API
        
        Create a new group with the given `name` as son of this.
        """
        if self.can_create_child(name):
            self.hdf.createGroup(self.path, name)
            return self._build_child(name, **kw)
    
    
    def _create_table(self, name, description, **kw):
        """Internal API
        
        Create a new table with the given `name` as son of this.
        """
        if self.can_create_child(name):
            self.hdf.createTable(self.path, name, description)
            return self._build_child(name, **kw)
    
        
    def _create_dataSet(self, name, dtype, shape=(0,), **kw):
        """Internal API
        
        Create a new DataSet with the given `name` as son of this.
        
        This DataSet is an homogeneous dataset of type `dtype`.
        
        The shape of this dataset is given by `shape`, and the last dim of this 
        dataset can be enlarged along on last of its dimensions. 
        """
        if self.can_create_child(name):
            self.hdf.createEArray(self.path, name, dtype, shape)
            return self._build_child(name, **kw)

