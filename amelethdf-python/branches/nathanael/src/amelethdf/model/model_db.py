#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 12 févr. 2010

@author: nathanael
'''

__all__ = ['ModelDataBase']

ALL_VAR_NAME = '__all__'

class ModelDataBase:
    """Define a data base of model class define in model.
    
    This data base can be used to look for a class model get its path ...
    """
    
    def __init__(self):
        """Init the data base
        """
        self.klass_db = {}
        self.xpath_db = {}
        self.class_model = []
        
    def load_class_model(self, klass):
        """Load all class used in the model.
        
        klass is the root class of this model.
        
        In the same module we must found a variable '__all_model_class__'
        that define the name of all class used in the model.
        And all this class are imported in this module.
        
        """
        module = __import__(klass.__module__, globals(), locals(),
                            [ALL_VAR_NAME], -1)
    
        if hasattr(module, ALL_VAR_NAME):
            all_model_class = getattr(module, ALL_VAR_NAME)
        else:
            raise Exception("Can't load model's class set global variable "
                            "'%s' in module '%s'." % (
                            ALL_VAR_NAME, klass.__module__))
        
        self.class_model = [getattr(module, name) for name in all_model_class]
        
        
    @staticmethod
    def _record(db, key, data):
        """Function to record a data on dict
        
            >>> db = {}
            >>> ModelDataBase._record(db, 'key', 'data')
            >>> db['key']
            ['data']
            
            >>> ModelDataBase._record(db, 'key', 'data2')
            >>> db['key']
            ['data', 'data2']
        
        """
        recorde = None
        if not db.has_key(key):
            recorde = []
            db.setdefault(key, recorde)
        else:
            recorde = db.get(key)
            
        if data not in recorde:
            recorde.append(data)
            
        
        
    def recorde(self, klass, xpath):
        """Recode a new model class
        
        @param klass: model class
        @param xpath: path of this model class on the model
        """
        if klass not in self.class_model:
            self.class_model.append(klass)
        self._record(self.klass_db, xpath, klass)
        self._record(self.xpath_db, klass, xpath)
    
    def get_klass(self, xpath):
        """Return all model class of this model path.
        """
        return self.klass_db.get(xpath, None)
    
    def get_xpath(self, klass):
        """Return all path on model of this model class.
        """
        return self.xpath_db.get(klass, None)
    
    def get_subklass_of(self, klass):
        """Return all sub model class recorded of the given model class.
        """
        return [slass 
                for slass in self.class_model 
                if issubclass(slass, klass) and slass != klass]
        
    def __str__(self):
        """Sort string view of this data base.
        """
        l = [ (str(xpath), ' '.join([klass.__name__ for klass in klass_l])) 
             for xpath, klass_l in self.klass_db.items()]
        l.sort()
        l = [a + ' -> ' + b for a, b in l]
        return '\n'.join(l)
        
             
