#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 févr. 2010

@author: nathanael
'''

from api import List, Tuple, IsGroup, Dict, Instance, IsModel, Array

from trait_inspector import TraitInspector, InspectorError, USER_NAME

from .. import tree_print


__all__ = ['ModelInspectorError', 'load']

class ModelInspectorError(InspectorError):
    """Base exception for model inspector.
    """
    

class BaseModelInspector(TraitInspector):
    """Base class for all model inspector.
    """
    # List of Model Inspector recorded
    ModelInspectors = []
    # Model inspector type 
    MTYPE = None
    
    def __init__(self, trait_inspector):
        super(BaseModelInspector, self).__init__(trait_inspector.model,
                                                 trait_inspector.name,
                                                 trait_inspector._db,
                                                 trait_inspector.xpath)
    
    #---------------------------------------------------------------------------
    #--- Public methods
    #---------------------------------------------------------------------------
    
    @property
    def mtype(self):
        """Return the type of this model inspector.
        """
        return self.MTYPE 
    
    @staticmethod
    def is_model_for(inspector):
        """Test of this class model inspector is valid for the given inspector
        """
        return False
    
    @classmethod
    def _build_model_inspector(cls, inspector):
        """Try to found in recording model inspector the first valid inspector
        and init it. 
        """
        for m_inspector in cls.ModelInspectors:
            if m_inspector.is_model_for(inspector):
                return m_inspector.init_by_copy(inspector)
        raise ModelInspectorError("Any Model inspector found for node '%s'"\
                                  % (inspector.xpath))

    #---------------------------------------------------------------------------
    #--- Over define TraitInspector
    #---------------------------------------------------------------------------
    def _build_child(self, model, name, **kw):
        """
        """
        inspector = TraitInspector(model, name, self._db, self.xpath)
        return self._build_model_inspector(inspector)
        
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
        
    def _DrawingTree__get_attrs_name(self):
        return self.attrs_name + self.consts_name
    
    def _DrawingTree__get_attr_value(self, name):
        if name in self.consts_name:
            return self.get_const(name)
        
    def _DrawingTree__get_attr_type(self, name):
        if name in self.consts_name:
            return tree_print.ATTR2
        else:
            return tree_print.ATTR1
        
    
    
    
    
    
    
    
    
        
class AttrModelInspector(BaseModelInspector):
    """Base model inspector for attribute 
    """

class LeafModelInspector(BaseModelInspector):
    """For all element that are tree leaf.
    """
    MTYPE = 0
    
    #---------------------------------------------------------------------------
    #--- Over define children methods of TraitInspector
    #---------------------------------------------------------------------------
    @property
    def children_name(self):
        return []
        
    def has_children_user_name(self):
        return False

    def iter_children(self):
        return []
        
    def get_child(self, name, **kw):
        raise ModelInspectorError("Is leaf can't get any children.")



class GroupModelInspector(BaseModelInspector):
    """For all element that are tree group.
    """
    MTYPE = 100
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__tostring_type(self):
        return tree_print.GROUP_TAG
    
    
class ArrayModelInspector(LeafModelInspector):
    """For all element that are leaf array.
    """
    MTYPE = 200
    
    
    #---------------------------------------------------------------------------
    #--- Public methods
    #---------------------------------------------------------------------------
    
    @property
    def dtype(self):
        """return the dtype of this model array
        """
        return self.tlass.dtype
    
    @property
    def shape(self):
        """return the shape of this model array
        """
        return self.tlass.shape
    
    def is_legale_array(self, array):
        """Test if the given array is valid
        
        Most strong that 'is_legale_value' on the parent model inspector. 
        Test if the array is valid in this state.
        """
        return self.dtype != None or array.dtype == self.dtype
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__tostring_type(self):
        l = []
        
        if not self.dtype is None:
            l.append('dtype=%s' % str(self.dtype))
        if self.shape:
            l.append('shape=%s' % str(self.shape))
                
        return  tree_print.S_DATA + ', '.join(l) + tree_print.E_DATA

    
class TableModelInspector(LeafModelInspector):
    """For all element that are leaf table.
    """
    MTYPE = 300
    
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__tostring_type(self):
        return tree_print.S_TAB + tree_print.E_TAB


class TableTupled_Inspector(TableModelInspector):
    """This class implement the table model inspector for traits shape: 
    
        * List(Tuple( ... ))
    
    """
    MTYPE = TableModelInspector.MTYPE + 1
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------
    
    @staticmethod
    def is_model_for(trait_inspector):
        return (isinstance(trait_inspector.tlass, List) 
                and isinstance(trait_inspector.tlass.item_trait._CTrait__get_trait_type(), Tuple))


class IsGroup_Inspector(GroupModelInspector):
    """ 
    
    """
    MTYPE = GroupModelInspector.MTYPE + 1
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return (trait_inspector.has_klass() 
                and issubclass(trait_inspector.klass, IsGroup))
    
    
    #---------------------------------------------------------------------------
    #--- Over define children methods of TraitInspector
    #---------------------------------------------------------------------------
    
    def has_children_user_name(self):
        return True
    
    @property
    def children_name(self):
        childdren_name = []
        for child in super(IsGroup_Inspector, self).iter_children():
            childdren_name.extend(child.children_name)
        return childdren_name
    
    def iter_children(self):
        children = []
        for child in super(IsGroup_Inspector, self).iter_children():
            children.extend(child.iter_children())
        return children
    
    def get_child(self, name, select, **kw):
        if self.has_children_user_name() and select == None:
            raise ModelInspectorError("Try to get unnamed child without select attribute.")

        #
        # try to get for all natural children 
        #
        for child in super(IsGroup_Inspector, self).iter_children():
            if name in child.children_name:
                # if match name
                return child.get_child(name)
            elif child.has_children_user_name():
                # if the children has user name use `select` to fount it
                # on all children
                
                print self, name, select
                
                for child_name in super(IsGroup_Inspector, self).children_name:
                    print 'child_name', child_name
                    # Get all children 
                    child_ = super(IsGroup_Inspector, self).get_child(child_name)
                    for sub_child in child_.iter_children():
                        # if the child is model call get this friend 
                        # (model sub class) and test it.
                        if hasattr(sub_child, 'klass'):
                            for item in self._db.get_subklass_of(sub_child.klass):
                                sub_child_friend = self._build_child(item, None) 
                                if dict(sub_child_friend.consts) == select:
                                    return sub_child_friend
        raise ModelInspectorError("No found child model '%s' of '%s'" % (name, str(self)))
        
        
        
class ListOf_Inspector(GroupModelInspector):
    """This class implement the model inspector for traits shape: 
    
        * Dict(UserName, Any)
    
    A 'list of' have alone children model inspector
    """
    MTYPE = GroupModelInspector.MTYPE + 2
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return isinstance(trait_inspector.tlass, Dict)
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__get_children_type(self):
        return tree_print.TTL_L
    
    #---------------------------------------------------------------------------
    # Over define children methods of TraitInspector
    #---------------------------------------------------------------------------
    
    @property
    def children_name(self):
        return [USER_NAME]
        
    def has_children_user_name(self):
        return True

    def iter_children(self):
        return [self.get_child(None)]
        
    def get_child(self, name, **kw):
        return self._build_child(self.tlass.value_trait._CTrait__get_trait_type(),
                                 USER_NAME)
    
    
    
class IListOf_Inspector(GroupModelInspector):
    """This class implement the model inspector for traits shape: 
    
        * List(Instance(`MyModelClass`))
    
    A 'list of' have alone children model inspector
    """
    MTYPE = GroupModelInspector.MTYPE + 3
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return (isinstance(trait_inspector.tlass, List)
                and isinstance(trait_inspector.tlass.item_trait._CTrait__get_trait_type(), Instance))
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__get_children_type(self):
        return tree_print.TTL_L
    
    #---------------------------------------------------------------------------
    # Over define children methods of TraitInspector
    #---------------------------------------------------------------------------
    
    def iter_children(self):
        return [self.get_child(None)]

    def get_child(self, name, **kw):
        return self._build_child(self.tlass.item_trait._CTrait__get_trait_type(),
                                 None)
        
class InstanceOf_Inspector(GroupModelInspector):
    """This class implement the model inspector for traits shape: 
    
        * Instance(`MyModelClass`) for MyModelClass haven't any sub class on the model
    
    The children this model inspector are all sub class of `MyModelClass` 
    recording in 'Model data base' (see: module 'model_db')
    """
    MTYPE = GroupModelInspector.MTYPE + 4
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return (not isinstance(trait_inspector.tlass, Instance)
                and trait_inspector.has_klass() 
                and issubclass(trait_inspector.klass, IsModel)
                and len(trait_inspector._db.get_subklass_of(trait_inspector.klass)) == 0)
        
    
class SubClassOf_Inspector(GroupModelInspector):
    """This class implement the model inspector for traits shape: 
    
        * Instance(`MyModelClass`) for MyModelClass have some sub class on the model
    
    The children this model inspector are all sub class of `MyModelClass` 
    recording in 'Model data base' (see: module 'model_db')
    """
    MTYPE = GroupModelInspector.MTYPE + 5
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return (not isinstance(trait_inspector.tlass, Instance)
                and trait_inspector.has_klass() 
                and issubclass(trait_inspector.klass, IsModel)
                and len(trait_inspector._db.get_subklass_of(trait_inspector.klass)) > 0)
    
    #---------------------------------------------------------------------------
    #--- Over define DrawingTree
    #---------------------------------------------------------------------------
    
    def _DrawingTree__get_children_type(self):
        return tree_print.TTL_E
   
        
    #---------------------------------------------------------------------------
    # Over define children methods of TraitInspector
    #---------------------------------------------------------------------------
    def iter_children(self):
        return [self._build_child(item, None) 
                for item in self._db.get_subklass_of(self.klass)]
        
        
class Array_Inspector(ArrayModelInspector):
    """This class implement the model inspector for trait shape:
    
        * Array
    
    """
    MTYPE = ArrayModelInspector.MTYPE + 1
    
    #---------------------------------------------------------------------------
    #--- Implement TableModelInspector
    #---------------------------------------------------------------------------.
    
    @staticmethod
    def is_model_for(trait_inspector):
        return isinstance(trait_inspector.tlass, Array)


#
# Record all model inspectors
#
BaseModelInspector.ModelInspectors = [IListOf_Inspector,
                                      ListOf_Inspector,
                                      IsGroup_Inspector,
                                      TableTupled_Inspector,
                                      Array_Inspector,
                                      InstanceOf_Inspector,
                                      SubClassOf_Inspector, ]
    
def load(model):
    """Load the model with for root of this model the given model class.
    
    """
    node = BaseModelInspector._build_model_inspector(TraitInspector(model))
    node._load()
    return node
