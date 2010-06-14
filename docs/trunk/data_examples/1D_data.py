#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
import tables
 
filters = tables.Filters(complevel=5, complib='zlib')
data=[]
xTime=[]
xFreq=[]
xt=0.
xf=0.
for i in range(1000):
    data.append(numpy.sin((i/1000.0)*2.0*numpy.pi))
    xTime.append(xt)
    xFreq.append(xf)
    xt+=1e-9
    xf+=1.0e6

dataAtom = tables.Float64Atom()
dimAtom=tables.Float32Atom()
shape = numpy.shape(data)
files=['1Dtime.h5','1Dfreq.h5']

for f in files:
    h5file = tables.openFile(f,'w',PYTABLES_SYS_ATTRS=False)
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
    if(f=='1Dtime.h5'):
        dim_arr=h5file.createCArray(ds_id,"dim1",dimAtom,shape,filters=filters)
        i=0
        for array in dim_arr:
            dim_arr[i]=xTime[i]
            i+=1
        h5file.setNodeAttr(dim_arr,'physicalNature','time')
        h5file.setNodeAttr(dim_arr,'label','Time')
        h5file.setNodeAttr(dim_arr,'unit','second')
    elif(f=='1Dfreq.h5'):
        dim_arr=h5file.createCArray(ds_id,"dim1",dimAtom,shape,filters=filters)
        i=0
        for array in dim_arr:
            dim_arr[i]=xFreq[i]
            i+=1
        h5file.setNodeAttr(dim_arr,'physicalNature','frequency')
        h5file.setNodeAttr(dim_arr,'label','Frequency')
        h5file.setNodeAttr(dim_arr,'unit','hertz')
    h5file.close()
