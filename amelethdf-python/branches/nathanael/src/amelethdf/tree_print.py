#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 11 fevr. 2010

@author: nathanael
'''


__all__ = ['TreeLink']

TL_NON = ' '
TL_END = '`'
TL_NEW = '|'
TL_FOL = '|'


TTL_G = '--'
TTL_L = '*-'
TTL_E = '?-'
TTL_N = '  '

TL_TAG = ' '


class TreeLink:
    def __init__(self):
        self.tree_link = []
        
    def close_branch(self):
        self.tree_link = [(((e[0] not in (TL_END, TL_NON)) and TL_FOL) or TL_NON) + TTL_N + TL_TAG
                           for e in self.tree_link]
        
    def append(self, type, close=False):
        self.close_branch()
        self.tree_link.append(((close and TL_END) or TL_NEW) + type + TL_TAG)
    
    def close_end(self, type=None):
        end = self.tree_link[-1]
        if end[0] != TL_NON:
            new_end = TL_END + (type or end[1:3])
        else:
            new_end = TL_NON + TTL_N
        self.tree_link[-1] = new_end + TL_TAG
        
    def follow(self, type, close=False):
        self.tree_link[-1] = ((close and TL_END) or TL_NEW) + type + TL_TAG
        
    def down(self):
        self.tree_link.pop()
            
    def __str__(self):
        return ''.join(self.tree_link)
