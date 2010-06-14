#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
import tables
 
filters = tables.Filters(complevel=5, complib='zlib')

x1 = numpy.arange(-200., 201.,2)/50.
x2 = numpy.arange(-200., 201.)/50. 
xx = numpy.outer(x1, x2)
data = numpy.sinc(xx)

dataAtom = tables.Float64Atom()
dimAtom=tables.Float32Atom()
shape = numpy.shape(data)
shapex1 = numpy.shape(x1)
shapex2 = numpy.shape(x2)
h5file = tables.openFile("2Ddata.h5",'w',PYTABLES_SYS_ATTRS=False)
h5file.setNodeAttr('/','FORMAT','AMELETHDF')
h5file.setNodeAttr('/','AMELETHDF_FORMAT_VERSION','1.0.0')

ft_id = h5file.createGroup(h5file.root,'floatingType')
ars_id = h5file.createGroup(ft_id,'data1D')
h5file.setNodeAttr(ars_id,"floatingType","arraySet")
# Write data
data_arr=h5file.createCArray(ars_id,"data",dataAtom,shape,filters=filters)
i=0
for array in data_arr:
    data_arr[i]=data[i]
    i+=1
h5file.setNodeAttr(data_arr,'label','Voltage')
h5file.setNodeAttr(data_arr,'physicalNature','voltage')
h5file.setNodeAttr(data_arr,'unit','volt')
h5file.flush()
# Write dim1
ds_id=h5file.createGroup(ars_id,"ds")
dim_arr=h5file.createCArray(ds_id,"dim1",dimAtom,shapex2,filters=filters)
i=0
for array in dim_arr:
    dim_arr[i]=x2[i]
    i+=1
h5file.setNodeAttr(dim_arr,'label','X2')
dim_arr=h5file.createCArray(ds_id,"dim2",dimAtom,shapex1,filters=filters)
i=0
for array in dim_arr:
    dim_arr[i]=x1[i]
    i+=1
h5file.setNodeAttr(dim_arr,'label','X1')
h5file.close()
