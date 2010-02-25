'''
Created on 25 fevr. 2010

@author: nathanael
'''

from amelethdf.node import openAmelet, openHDF

__all__ = ['Dump']

class Dump:
    def __init__(self, file, force=False):
        self.file = file
        self.force = force
    
        
    def open(self):
        if self.force:
            self.root = openHDF(self.file)
        else:
            self.root = openAmelet(self.file)
        
    def close(self):
        self.root.close()
        
    def __str__(self):
        self.open()
        return self.root.tostring()
    
    def __del__(self):
        self.close()
