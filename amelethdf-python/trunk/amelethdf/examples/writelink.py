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

# Links
group =  h5file.createGroup("/", a.LINK)
group =  h5file.createGroup(group, "sphere_links")
link = a.Link(name = "incident_field",
            subject = "/electromagneticSource/planeWave/x_plane_wave",
            object = "/mesh/sphere/surfaces")
a.print_node(h5file, link, group)
link = a.Link(name = "skin_material",
            subject = "/physicalModel/perfectlyConducting",
            object = "/mesh/sphere/surfaces/group/faceGroup")
a.print_node(h5file, link, group)

h5file.close()
