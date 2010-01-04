#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''
import tables
tables.parameters.PYTABLES_SYS_ATTRS = False

from tables import openFile

h5file = openFile("example1.h5", mode = "w", title = "Simulation test")

c_label =  h5file.createGroup("/", "label")
c_simulation = h5file.createGroup("/", "simulation")
c_external_element = h5file.createGroup("/", "externalElement")

c_physical_model = h5file.createGroup("/", "physicalModel")
h5file.createGroup(c_physical_model, "volume")
h5file.createGroup(c_physical_model, "interface")
h5file.createGroup(c_physical_model, "multilayer")
h5file.createGroup(c_physical_model, "anisotropic")
c_multiport = h5file.createGroup(c_physical_model, "multiport")
h5file.createGroup(c_multiport, "sParameter")
h5file.createGroup(c_multiport, "distributed")
h5file.createGroup(c_physical_model, "aperture")
h5file.createGroup(c_physical_model, "slot")

c_mesh = h5file.createGroup("/", "mesh")
c_network = h5file.createGroup("/", "network")
c_transmission_line = h5file.createGroup("/", "transmissionLine")
c_link = h5file.createGroup("/", "link")
c_extension_type = h5file.createGroup("/", "extensionType")

h5file.close()


