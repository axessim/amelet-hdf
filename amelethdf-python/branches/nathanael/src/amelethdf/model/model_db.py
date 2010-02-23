#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 12 févr. 2010

@author: nathanael
'''

__all__ = ['ModelDataBase']

ALL_VAR_NAME = '__all__'

class ModelDataBase:
    def __init__(self):
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
    def _recorde(db, key, data):
        recorde = None
        if not db.has_key(key):
            recorde = []
            db.setdefault(key, recorde)
        else:
            recorde = db.get(key)
            
        if data not in recorde:
            recorde.append(data)
            
        
        
    def recorde(self, klass, xpath):
        if klass not in self.class_model:
            self.class_model.append(klass)
        self._recorde(self.klass_db, xpath, klass)
        self._recorde(self.xpath_db, klass, xpath)
    
    def get_klass(self, xpath):
        return self.klass_db.get(xpath, None)
    
    def get_xpath(self, klass):
        return self.xpath_db.get(klass, None)
    
    def get_subklass_of(self, klass):
        return [slass 
                for slass in self.class_model 
                if issubclass(slass, klass) and slass != klass]
        
    def __str__(self):
        l = [ (str(xpath), ' '.join([klass.__name__ for klass in klass_l])) 
             for xpath, klass_l in self.klass_db.items()]
        l.sort()
        l = [a + ' -> ' + b for a, b in l]
        return '\n'.join(l)
        
             
