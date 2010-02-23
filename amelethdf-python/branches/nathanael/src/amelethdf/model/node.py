'''
Created on 9 fevr. 2010

@author: nathanael
'''


__all__ = ['ModelInspector', 'load']

from api import (Complex, Float, Bool, String, Int, UserName,
                 Group, DataSet, Table, TableObject,
                 Array, Dict, List, Enum, Tuple,
                 Instance, IsModel, IsArray, IsTable, IsGroup)

from amelethdf import path as apath
from model_db import ModelDataBase

from amelethdf.tree_print import TreeLink, TTL_E, TTL_G, TTL_L


ATTR_TYPE = (Complex, Float, Bool, String, Int, UserName)


ATTR = '@'
CONS = '#'
ATTR_SEP = ', '
S_ATTRS = '('
E_ATTRS = ')'

GROUP_TAG = '/'
S_TAB = '['
E_TAB = ']'
S_DATA = '{'
E_DATA = '}'

USER_NAME = "$user_name"


def make_hdfname(name):
    """Make a hdf name in well cast.
    
        >>> make_hdfname('ClassName')
        'className'
    """
    return name[0].lower() + name[1:]


class ModelError(Exception):
    """Base exception for model.
    """
    
    
MTYPE_LIST_OF = 1
MTYPE_ILIST_OF = 2
MTYPE_ENUM = 3
MTYPE_TABLE_OF = 4
MTYPE_DATASET = 5
MTYPE_GROUP = 6
MTYPE_ISGROUP = 7

def load(model):
    node = ModelInspector(model)
    node._load()
    return node

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


class TraitsInspector(object):
    def __init__(self, model=None, name=None, _db=None, _xpath='/'):
        if not _db:
            _db = ModelDataBase()
        self._db = _db
        
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
            raise ModelError('Invalid model %s' % str(model))
        
    #---------------------------------------------------------------------------
    # All methods associated to string view of instance.
    #---------------------------------------------------------------------------
    def __str__(self):
        """Return the string view of this instance.
        """
        return self.__repr__()
    
    def __repr__(self):
        return "<%s of %s>" % (self.__class__.__name__, self.xpath)
    
    def tostring(self):
        """Return a tree string of this model node.
        """
        return self._tostring()
        
    #---------------------------------------------------------------------------
    # All property
    #---------------------------------------------------------------------------
    @property
    def mtype(self):
        """Return the type of model node.
        
        mtype is in:
            * MTYPE_LIST_OF
            * MTYPE_ILIST_OF
            * MTYPE_TABLE_OF
            * MTYPE_ENUM
            * MTYPE_DATASET
            * MTYPE_GROUP
            
        """
        if isinstance(self.tlass, Dict):
            return MTYPE_LIST_OF
        elif (isinstance(self.tlass, List)
              and isinstance(self.tlass.item_trait._CTrait__get_trait_type(), Instance)
              and not isinstance(self.tlass.item_trait._CTrait__get_trait_type().klass, TableObject)):
            return MTYPE_ILIST_OF
        elif (isinstance(self.tlass, List) 
               and isinstance(self.tlass.item_trait._CTrait__get_trait_type(), Tuple)):
            return MTYPE_TABLE_OF
        elif (isinstance(self.tlass, Array)):
            return MTYPE_DATASET
        elif (hasattr(self, 'klass') 
              and issubclass(self.klass, IsModel)
              and len(self._db.get_subklass_of(self.klass)) > 0):
            return MTYPE_ENUM
        else:
            return MTYPE_GROUP

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
        
    def get_child(self, name):
        """Get the model node of the child
        """
        child = self.tlass.get(name)
        return self._build_child(traitmodel2model(child), name)
    
    #---------------------------------------------------------------------------
    #
    #---------------------------------------------------------------------------
    def is_legale_value(self, name, value):
        """Check if can set the attribute `name` with the `value`. 
        """
        try:
            return isinstance(value, self.tlass.get(name).get_validate()[1])
        except Exception, e:
            print 'Warning: `is legal value` not implemented of ', self.tlass.get(name).get_validate()
            return True
            
                

    #---------------------------------------------------------------------------
    # All internal methods
    #---------------------------------------------------------------------------
    def _load(self):
        """Load the model
        
        Note: use this method just one time, when you open class root of our 
        model.
        """
        #if hasattr(self, 'klass'): 
        self._db.load_class_model(self.klass)
        def _load(node):
            for item in node.iter_children():
                _load(item)
        _load(self)
    
    
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

        


class ModelInspector(TraitsInspector):
    """Base to requested the class model.
    """
        
    #---------------------------------------------------------------------------
    # All property
    #---------------------------------------------------------------------------
    @property
    def mtype(self):
        """Return the type of model node.
        
        mtype is in:
            * MTYPE_LIST_OF
            * MTYPE_ILIST_OF
            * MTYPE_TABLE_OF
            * MTYPE_ENUM
            * MTYPE_DATASET
            * MTYPE_GROUP
            
        """
        if isinstance(self.tlass, Dict):
            return MTYPE_LIST_OF
        elif (isinstance(self.tlass, List)
              and isinstance(self.tlass.item_trait._CTrait__get_trait_type(), Instance)
              and not isinstance(self.tlass.item_trait._CTrait__get_trait_type().klass, TableObject)):
            return MTYPE_ILIST_OF
        elif ((isinstance(self.tlass, List) 
               and isinstance(self.tlass.item_trait._CTrait__get_trait_type(), Tuple))
              or (hasattr(self, 'klass') 
                  and issubclass(self.klass, IsTable))):
            return MTYPE_TABLE_OF
        elif (isinstance(self.tlass, Array) 
              or (hasattr(self, 'klass') 
                  and issubclass(self.klass, IsArray))):
            return MTYPE_DATASET
        elif (hasattr(self, 'klass') 
              and issubclass(self.klass, IsModel)
              and len(self._db.get_subklass_of(self.klass)) > 0):
            return MTYPE_ENUM
        elif (hasattr(self, 'klass') 
              and issubclass(self.klass, IsGroup)):
            return MTYPE_ISGROUP
        else:
            return MTYPE_GROUP


    @property
    def children_name(self):
        """Return children name
        """
        mtype = self.mtype
        if mtype == MTYPE_LIST_OF:
            return self._get_list_of_children_name()
        elif mtype == MTYPE_ISGROUP:
            return self._get_isgroup_children_name()
        else:
            return self._get_items_name(only_children)
    
    def iter_children(self):
        """Rerun a list of sub model node.
        """
        mtype = self.mtype
        
        if mtype == MTYPE_LIST_OF:
            return [self._get_list_of_children()]
            
        elif mtype == MTYPE_ILIST_OF:
            return [self._get_indexed_list_of_children()]
            
        elif mtype == MTYPE_ENUM:
            return self._iter_enum()
            
        elif mtype in (MTYPE_DATASET, MTYPE_TABLE_OF):
            return []
        elif mtype == MTYPE_ISGROUP:
            return self._get_isgroup_children()
        else:
            return [self.get_child(child_name) 
                    for child_name in self.children_name]
                
        return []
    
    def get_child(self, name, select=None):
        """Get the model node of the child
        """
        mtype = self.mtype
        
        if mtype == MTYPE_LIST_OF:
            return self._get_list_of_children()
        elif mtype in (MTYPE_TABLE_OF, MTYPE_DATASET):
            return self.tlass.get('data')
        elif mtype == MTYPE_ISGROUP:
            if self.has_children_user_name() and not select:
                raise ModelError("Try to get unnamed child without select")
            return self._get_isgroup_child(name, select)
        else:
            return super(ModelInspector, self).get_child(name)
        
        
    
        
        
        
    #---------------------------------------------------------------------------
    # Internal method to get children model node.
    #---------------------------------------------------------------------------
    def _get_isgroup_children_name(self):
        """Return 'list of children name' node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_ISGROUP'
        """
        if self.mtype == MTYPE_ISGROUP:
            childdren_name = []
            for child in super(ModelInspector, self).iter_children():
                childdren_name.extend(child.children_name)
            return childdren_name
    
    def _get_isgroup_children(self):
        """Return 'list of children' node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_ISGROUP'
        """
        if self.mtype == MTYPE_ISGROUP:
            children = []
            for child in super(ModelInspector, self).iter_children():
                children.extend(child.iter_children())
            return children
        
    def _get_isgroup_child(self, name, select):
        """Return child node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_ISGROUP'
        """
        
        if self.mtype == MTYPE_ISGROUP:
            #
            # try to get for all natural children 
            #
            for child in super(ModelInspector, self).iter_children():
                if name in child.children_name:
                    # if match name
                    return child.get_child(name)
                elif child.has_children_user_name():
                    # if the children has user name use `select` to fount it
                    # on all children
                    for child_name in super(ModelInspector, self).children_name:
                        # Get all children 
                        sub_child = super(ModelInspector, self).get_child(child_name)
                        sub_child = sub_child._get_list_of_children()
                        # if the child is model call get this friend 
                        # (model sub class) and test it.
                        if hasattr(sub_child, 'klass'):
                            for item in self._db.get_subklass_of(sub_child.klass):
                                sub_child_friend = self._build_child(item, None) 
                                if dict(sub_child_friend.consts) == select:
                                    return sub_child_friend
                

    def _get_list_of_children(self):
        """Return 'list of children' node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_LIST_OF'
        """
        if self.mtype == MTYPE_LIST_OF: 
            return self._build_child(self.tlass.value_trait._CTrait__get_trait_type(), USER_NAME)
            
    def _get_list_of_children_name(self):
        """Return 'list of children' node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_LIST_OF'
        """
        if self.mtype == MTYPE_LIST_OF: 
            return [USER_NAME]
            
    def _get_indexed_list_of_children(self):
        """Return 'indexed list of children' node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_ILIST_OF'
        """
        if self.mtype == MTYPE_ILIST_OF: 
            return self._build_child(self.tlass.item_trait._CTrait__get_trait_type(), None)
        
    def _iter_enum(self):
        """Return a list of node model of this enum node model.
        
        NOTE: Restrict to model node of type is 'MTYPE_ENUM'
        """
        if self.mtype == MTYPE_ENUM:
            return [self._build_child(item, None) 
                    for item in self._db.get_subklass_of(self.klass)]


    
        
        
        
        
    #---------------------------------------------------------------------------
    # Methods to build string tree view
    #---------------------------------------------------------------------------
    def _tostring(self, tree_link=TreeLink()):
        """Internal method to build tree string of this model node.
        """
        s = self._tostring_line(tree_link=tree_link)
        
        CON = {MTYPE_LIST_OF:TTL_L,
               MTYPE_ILIST_OF:TTL_L,
               MTYPE_ENUM:TTL_E}
        
        ttl = CON.get(self.mtype, TTL_G)
        
        tree_link.append(ttl)
        items = self.iter_children()
        i = 0
        for item in items:
            i += 1
            tree_link.follow(ttl, len(items) == i)
            s += '\n' + item._tostring(tree_link)
        tree_link.down()
                
        return s
    
    def _tostring_line(self, tree_link=None):
        """Internal method that return model node string. 
        """
        s = ""
        
        if tree_link:    
            s += str(tree_link) + self.name
        else:
            s += self.name
            
        s += self._tostring_attrs() + self._tostring_type()
        
        return s 
            
    def _tostring_attrs(self):
        """Internal method that build string view of model attributes.
        """
        s = ''
        
        consts_name = self.consts_name
        if len(consts_name) != 0:
            s += S_ATTRS
            i = 0
            for const_name in consts_name:
                s += "%s%s=%s" % (CONS, const_name, str(self.tlass.get(const_name).default))
                i += 1
                if i != len(consts_name):
                    s += ATTR_SEP
                
        attrs_name = self.attrs_name
        if len(attrs_name) != 0:
            if len(consts_name) == 0:
                s += S_ATTRS
            else:
                s += ATTR_SEP
            i = 0
            for attr_name in attrs_name:
                s += ATTR + attr_name
                i += 1
                if i == len(attrs_name):
                    s += E_ATTRS
                else:
                    s += ATTR_SEP
        else:
            if len(consts_name) != 0:
                s += E_ATTRS
                
        return s
    
    def _tostring_type(self):
        """Internal method to build string view model type
        """
        s = ''
        
        mtype = self.mtype
        if mtype in (MTYPE_GROUP, MTYPE_LIST_OF, MTYPE_ILIST_OF, MTYPE_ENUM):
            s += GROUP_TAG
        elif mtype == MTYPE_DATASET:
            if hasattr(self.tlass, 'shape'):
                s += S_DATA + E_DATA
            else:
                s += S_DATA + E_DATA
                
        elif mtype == MTYPE_TABLE_OF:
            s += S_TAB + E_TAB
            
        return s
            
