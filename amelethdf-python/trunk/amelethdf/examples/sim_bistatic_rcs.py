#!/usr/bin/env
# -*- coding: utf-8 -*-'''

'''
Created on 17 sept. 2009

@author: Cyril Giraudon
'''
import numpy
import tables
tables.parameters.PYTABLES_SYS_ATTRS = False

from amelethdf import *

import sys
filename = sys.argv[1]
# Open the file
h5file = open_file(filename, mode = "w",
                   entry_point = "/simulation/bistatic_rcs")

# Simulation object
group =  h5file.createGroup("/", SIMULATION)
sim = Simulation(name = "bistatic_rcs",
                 module = "a_module", version = "1.0.0",
                 inputs = numpy.array(["/mesh/sphere", 
                                      "/physicalModel/perfectlyConducting",
                                      "/link/sphere_links",
                                      "/electromagneticSource/planeWave/x_plane_wave",
                                      "/outputRequest/field_requests",
                                      "/globalEnvironment/ge"]),
                 outputs = numpy.array(["/arraySet/far_field_theta_phi",
                                       "/extensionType/DataSet/near_field"]))
print_node(h5file, sim, group)

# Global environment
group =  h5file.createGroup("/", GLOBAL_ENVIRONMENT)
group =  h5file.createGroup(group, "ge")
data = Vector(name = "frequency",
              values = numpy.array([2.9E9, 3.0E9, 3.1E9], dtype=numpy.float32))
print_node(h5file, data, group)

# the mesh
group =  h5file.createGroup("/", MESH)
group =  h5file.createGroup(group, "sphere")
mesh = UnstructuredMesh(name = "surfaces")

# PEC
pec = h5file.createGroup("/", PHYSICAL_MODEL)
h5file.createGroup(pec, "perfectlyConducting")

# Plane wave
EMSource =  h5file.createGroup("/", ELECTROMAGNETIC_SOURCE)
PW = h5file.createGroup(EMSource, PLANE_WAVE)
XPW = Base(name = "x_plane_wave")
XPW.theta = 0.0
XPW.phi = 0.0
XPW.linearPolarization = 1.57
mag = SingleComplex(name="magnitude", label="magnitude",
                    physicalNature="electricField", unit="voltPerMeter",
                    value=1+0j)
XPW.magnitude = mag
print_node(h5file, XPW, PW)

# Links
group =  h5file.createGroup("/", LINK)
group =  h5file.createGroup(group, "sphere_links")
link = Link(name = "incident_field",
            subject = "/electromagneticSource/planeWave/x_plane_wave",
            object = "/mesh/sphere/surfaces")
print_node(h5file, link, group)
link = Link(name = "skin_material",
            subject = "/physicalModel/perfectlyConducting",
            object = "/mesh/sphere/surfaces/group/faceGroup")
print_node(h5file, link, group)

# Label for output requests
group =  h5file.createGroup("/", LABEL)
label_dataset = numpy.array(["far field", "near field"])
print_node(h5file, label_dataset, group, "output_request_label")

# Output requests
group =  h5file.createGroup("/", OUTPUT_REQUEST)
group =  h5file.createGroup(group, "field_request")
req = Link(name = "far_field_theta_phi",
           subject = "/label/output_request_label",
           subject_id = 1,
           object = "",
           theta_min = 0.0, theta_max = 3.14, nb_theta = 181,
           phi_min = 0, phi_max = 1.57, nb_phi = 2,
           output = "/arraySet/far_field_theta_phi")
print_node(h5file, req, group)
req = Link(name = "near_field_plane1",
           subject = "/label/output_request_label",
           subject_id = 0,
           object = "/mesh/sphere/surfaces/group/plane1",
           output = "/arraySet/near_field_plane1")
print_node(h5file, req, group)
req = Link(name = "near_field_plane2",
           subject = "/label/output_request_label",
           subject_id = 0,
           object = "/mesh/sphere/surfaces/group/plane2",
           output = "/arraySet/near_field_plane2")
print_node(h5file, req, group)
req = Link(name = "near_field_plane3",
           subject = "/label/output_request_label",
           subject_id = 0,
           object = "/mesh/sphere/surfaces/group/plane3",
           output = "/arraySet/near_field_plane3")
print_node(h5file, req, group)

# output arraySet
group =  h5file.createGroup("/", ARRAY_SET)
h5file.createGroup(group, "far_field_theta_phi")
h5file.createGroup(group, "near_field_plane1")
h5file.createGroup(group, "near_field_plane2")
h5file.createGroup(group, "near_field_plane3")

# ExtensionTypes
group =  h5file.createGroup("/", "extensionTypes")
group =  h5file.createGroup(group, "DataSet")
field = Base(name = "near_field")
field.linksDefinition = numpy.array([["/arraySet/near_field_plane1", ""],
                                     ["/arraySet/near_field_plane2", ""],
                                     ["/arraySet/near_field_plane3", ""]])
print_node(h5file, field, group)

# Close the file
h5file.close()

