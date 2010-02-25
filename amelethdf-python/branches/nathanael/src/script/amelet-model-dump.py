#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 févr. 2010

@author: nathanael
'''

import sys
from optparse import OptionParser, OptionGroup

from amelethdf import VERSION
from amelethdf.model import DEFAULT_MODEL


USAGE = """
    %s [OPTIONS]
    """ % (sys.argv[0])
DESCRIPTION = """Dump the Amelet-HDF default model in string tree view."""



if __name__ == '__main__':
    parser = OptionParser(usage=USAGE,
                          description=DESCRIPTION,
                          version=VERSION)
    (options, args) = parser.parse_args()
    
    node = DEFAULT_MODEL
    print node.tostring()

