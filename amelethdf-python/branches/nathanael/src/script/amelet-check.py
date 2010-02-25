#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 16 févr. 2010

@author: nathanael
'''

import sys

from optparse import OptionParser, OptionGroup

from amelethdf.tools.check import Check
from amelethdf import VERSION

USAGE = """
    %s [OPTIONS] amelet-file
    """ % (sys.argv[0])
DESCRIPTION = """Check the Amelet-HDF file give."""


if __name__ == '__main__':
    parser = OptionParser(usage=USAGE,
                          description=DESCRIPTION,
                          version=VERSION)
    parser.add_option("-i",
                      dest="h5file",
                      metavar="FILE",
                      help="Amelet-HDF file.")
    
    (options, args) = parser.parse_args()
    
    if len(args) >= 1 :
        h5file = args[0]
    elif options.h5file != None:
        h5file = options.h5file
    else:
        parser.error("the Amelet-HDF file is not pointed out.")
    
    print Check(h5file)
