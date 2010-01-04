'''
Created on 21 octobre 2009

@author: Cyril Giraudon
'''
import tables
tables.parameters.PYTABLES_SYS_ATTRS = False
from tables import openFile

import amelethdf as a 

import sys
filename = sys.argv[1]
h5file = openFile(filename, mode = "w", title = "test")


# Plane wave
EMSource =  h5file.createGroup("/", a.ELECTROMAGNETIC_SOURCE)
PW = h5file.createGroup(EMSource, a.PLANE_WAVE)
XPW = a.Base(name = "x_plane_wave")
XPW.theta = 0.0
XPW.phi = 0.0
XPW.linearPolarization = 1.57
mag = a.SingleComplex(name="magnitude", label="magnitude",
                    physicalNature="electricField", unit="voltPerMeter",
                    value=1+0j)
XPW.magnitude = mag
a.print_node(h5file, XPW, PW)

h5file.close()
