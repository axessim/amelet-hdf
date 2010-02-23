#!/usr/bin/env python
# -*- coding: utf-8 -*-

from unittest import TestCase, main
from amelethdf.model.model_db import ModelDataBase

__all__ = ['AA', 'BB', 'CC', 'DD']

class AA: pass
class BB: pass
class CC(BB): pass

class DD(CC): pass

class tests_model_data_bass(TestCase):
    def setUp(self):
        pass
    
    def tearDown(self):
        pass
    
    def test_load(self):
        db = ModelDataBase()
        db.load_class_model(AA)
        self.assertEqual(db.class_model, [AA, BB, CC, DD])
    
    def test_recorde(self):
        
        db = ModelDataBase()
        
        db.recorde(AA, '/')
        db.recorde(BB, '/b')
        db.recorde(CC, '/b')
        db.recorde(CC, '/c')
        
        self.assertEqual(db.get_klass('/'), [AA])
        self.assertEqual(db.get_klass('/b'), [BB, CC])
        
        self.assertEqual(db.get_xpath(AA), ['/'])
        self.assertEqual(db.get_xpath(BB), ['/b'])
        self.assertEqual(db.get_xpath(CC), ['/b', '/c'])
    
    def test_subklass_of(self):
        db = ModelDataBase()
        
        db.recorde(AA, '/')
        db.recorde(BB, '/b')
        db.recorde(CC, '/b')
        db.recorde(CC, '/c')
        db.recorde(DD, '/c/d')
        
        self.assertEqual(db.get_subklass_of(AA), [])
        self.assertEqual(db.get_subklass_of(BB), [CC, DD])
        self.assertEqual(db.get_subklass_of(CC), [DD])
        self.assertEqual(db.get_subklass_of(DD), [])


    def test_str(self):
        db = ModelDataBase()
        
        db.recorde(AA, '/')
        db.recorde(BB, '/b')
        db.recorde(CC, '/b')
        db.recorde(CC, '/c')
        db.recorde(DD, '/c/d')
        
        
        self.assertEqual(str(db), "/ -> AA\n"
                                  "/b -> BB CC\n" 
                                  "/c -> CC\n" 
                                  "/c/d -> DD")

if __name__ == "__main__":
    main()
