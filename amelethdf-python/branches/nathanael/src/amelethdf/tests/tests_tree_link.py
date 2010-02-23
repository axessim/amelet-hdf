'''
Created on 11 fevr. 2010

@author: nathanael
'''
import unittest

from amelethdf.tree_print import TreeLink

class Test(unittest.TestCase):


    def test_tree_link_1(self):
        tl = TreeLink()
        
        self.assertEqual(str(tl), '')
        
        tl.append('*-')
        self.assertEqual(str(tl), '|*- ')
        
        tl.append('--')
        self.assertEqual(str(tl), '|   |-- ')
        
        tl.close_end()
        self.assertEqual(str(tl), '|   `-- ')
        
        tl.down()
        self.assertEqual(str(tl), '|   ')
        
        tl.close_end('--')
        self.assertEqual(str(tl), '`-- ')
        
    def test_tree_link_2(self):
        tl = TreeLink()
        
        self.assertEqual(str(tl), '')
        
        tl.append('*-', True)
        self.assertEqual(str(tl), '`*- ')
        
        tl.append('--')
        self.assertEqual(str(tl), '    |-- ')
        
        tl.close_end()
        self.assertEqual(str(tl), '    `-- ')
        
        tl.down()
        self.assertEqual(str(tl), '    ')
        
        tl.close_end('--')
        self.assertEqual(str(tl), '    ')
    

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()
