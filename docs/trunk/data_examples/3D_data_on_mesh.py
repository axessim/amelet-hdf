#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
import tables
 
filters = tables.Filters(complevel=5, complib='zlib')

nodes=[[0.0,0.0,0.0],
       [0.0,1.0,0.0],
       [1.0,0.0,0.0],
       [1.0,1.0,0.0],
       [2.0,0.0,0.0],
       [2.0,1.0,0.0]]
eltnodes=[0,1,2,
          2,1,3,
          2,3,4,
          3,5,4]
elttypes=[11,
          11,
          11,
          11]
           
#write unstructured mesh - 4 triangles
h5file = tables.openFile("3Ddata_on_triangles.h5",'w',PYTABLES_SYS_ATTRS=False)
h5file.setNodeAttr('/','FORMAT','AMELETHDF')
h5file.setNodeAttr('/','AMELETHDF_FORMAT_VERSION','1.0.0')

mesh_id = h5file.createGroup(h5file.root,'mesh')
trianglesMesh_id = h5file.createGroup(mesh_id,'trianglesMesh')
tmesh_id = h5file.createGroup(trianglesMesh_id,'tmesh')
h5file.setNodeAttr(tmesh_id,'type','unstructured')

eltnodes_arr = h5file.createCArray(tmesh_id,"elementNodes",
                                   tables.Int32Atom(),numpy.shape(eltnodes),
                                   filters=filters)
i = 0
for array in eltnodes_arr:
    eltnodes_arr[i] = eltnodes[i]
    i+=1

elttypes_arr = h5file.createCArray(tmesh_id,"elementTypes",
                                   tables.Int8Atom(),numpy.shape(elttypes),
                                   filters=filters)

nodes_arr = h5file.createCArray(tmesh_id,"nodes",
                                tables.Float32Atom(),numpy.shape(nodes),
                                filters=filters)
i=0
for array in nodes_arr:
    nodes_arr[i] = nodes[i]
    i+=1

i = 0
for array in elttypes_arr:
    elttypes_arr[i] = elttypes[i]
    i+=1

group_id = h5file.createGroup(tmesh_id,'group')
group_arr = h5file.createCArray(group_id,"triangles",
                                   tables.Int32Atom(),numpy.shape(elttypes),
                                   filters=filters)
i=0
for array in group_arr:
    group_arr[i]=i
    i+=1
h5file.setNodeAttr(group_arr,'type','element')
h5file.setNodeAttr(group_arr,'entityType','face')



#write data
ft_id = h5file.createGroup(h5file.root,'floatingType')
ars_id = h5file.createGroup(ft_id,'data3D')
h5file.setNodeAttr(ars_id,'floatingType','arraySet')
h5file.setNodeAttr(ars_id,'label','Data on triangles')
data_arr = h5file.createCArray(ars_id,'data',
                                   tables.Float64Atom(),numpy.shape(elttypes),
                                   filters=filters)
i = 0
for array in data_arr:
    data_arr[i] = numpy.float64(i)
    i+=1
h5file.setNodeAttr(data_arr,'label','Current on element')
h5file.setNodeAttr(data_arr,'physicalNature','electricCurrent')
h5file.setNodeAttr(data_arr,'unit','ampere')

ds_id = h5file.createGroup(ars_id,'ds')
dim1_arr = h5file.createCArray(ds_id,'dim1',
                               tables.StringAtom(42),(1,),
                               filters=filters)
dim1_arr[0]='/mesh/trianglesMesh/tmesh/group/triangles'
h5file.setNodeAttr(dim1_arr,'label','mesh elements')
h5file.setNodeAttr(dim1_arr,'physicalNature','meshEntity')
h5file.close()
