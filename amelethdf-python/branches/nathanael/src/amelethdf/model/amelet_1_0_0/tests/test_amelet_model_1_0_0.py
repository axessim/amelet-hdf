'''
Created on 10 fevr. 2010

@author: nathanael
'''



if __name__ == '__main__':
    from amelethdf.model.amelet_1_0_0.amelet import Amelet
    from amelethdf.model import load

    
    node = load(Amelet)
    print node.tostring()
    
    print '\n\n'
    
    print str(node)

