#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 16 févr. 2010

@author: nathanael
'''

import sys

from optparse import OptionParser, OptionGroup

from amelethdf import VERSION
from amelethdf.tools import Dump

USAGE = """
    %s [OPTIONS] amelet-file
    """ % (sys.argv[0])
DESCRIPTION = """Dump the Amelet-HDF file give in to string tree view."""










if __name__ == '__main__':
    parser = OptionParser(usage=USAGE,
                          description=DESCRIPTION,
                          version=VERSION)
    parser.add_option("-i",
                      dest="h5file",
                      metavar="FILE",
                      help="Amelet-HDF file.")
    
    dump_opts = OptionGroup(parser, 'dump options', '')
    dump_opts.add_option("--force",
                         dest="force",
                         action="store_const",
                         const=True,
                         default=False,
                         help="Force to dump all data record in the given HDF5.")
    parser.add_option_group(dump_opts)
    
    
    (options, args) = parser.parse_args()
    
    if len(args) >= 1 :
        h5file = args[0]
    elif options.h5file != None:
        h5file = options.h5file
    else:
        parser.error("the Amelet-HDF file is not pointed out.")
        
    print Dump(h5file, options.force)
