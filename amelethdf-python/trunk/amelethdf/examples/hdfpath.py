#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''
import tables
tables.parameters.PYTABLES_SYS_ATTRS = False

from tables import openFile

import sys

filename = sys.argv[1]
h5file = openFile(filename, mode = "w", title = "test")

h5file.createGroup("/", "a_group")
h5file.createArray("/", "an_array", [1, 2, 3])

h5file.close()


