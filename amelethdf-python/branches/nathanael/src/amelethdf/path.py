#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Define some class to define Amelet-HDF model.

"""

SEPARATOR = '/'
SELF = '/.'

def format(p):
    if len(p) > 1 and p[-1] == SEPARATOR:
        return format(p[:-1])
    else:
        return p
        

def split(p):
    """
    
        >>> split('/a/b/c')
        ('/a/b', 'c')
        
        >>> split('a/b/c')
        ('a/b', 'c')
        
        >>> split('a/b/c/')
        ('a/b/c', '')
        
        >>> split('a')
        ('', 'a')
        
        >>> split('/')
        ('/', '')
        
        >>> split('.')
        ('', '.')
        
        >>> split('/a')
        ('/', 'a')
        
    """
    s = list(splits(p))
    a, b = join(*s[:-1]), s[-1]
    if isabs(p) and not isabs(a):
        a = SEPARATOR + a
    return a, b
    
    

def join(*p):
    """
    
        >>> join('a', 'b')
        'a/b'
        >>> join('/', '')
        '/'
    
    """
    return normpath(SEPARATOR.join(p))

def isabs(s):
    """
    
        >>> isabs('') 
        False
        >>> isabs('/')
        True
        >>> isabs('//')
        True
        >>> isabs('/a')
        True
        >>> isabs('a')
        False
    
    """
    return len(s) > 0 and s[0] == SEPARATOR

def normpath(p):
    """
    
        >>> normpath('//')
        '/'
        >>> normpath('/a/a')
        '/a/a'
        >>> normpath('//a/a')
        '/a/a'
        >>> normpath('/a/a/')
        '/a/a'
        
        >>> normpath('/./')
        '/'
        >>> normpath('/a/a/.')
        '/a/a'
        
        
    """    
    s = p.replace(SELF, '')
    s = s.replace(SEPARATOR + SEPARATOR, SEPARATOR)
    if isabs(p) and not isabs(s):
        s = SEPARATOR + s
    else:
        if len(s) > 1 and s[-1] == SEPARATOR:
            s = s[:-1]
    return s

def splits(p):
    """
    
        >>> splits('/a/b/c')
        ['', 'a', 'b', 'c']
        
        >>> splits('a/b/c')
        ['a', 'b', 'c']
        
        >>> splits('a/b/c/')
        ['a', 'b', 'c', '']
        
        >>> splits('a')
        ['a']
        
        >>> splits('/')
        ['', '']
        
        
    """
    return p.split(SEPARATOR)
