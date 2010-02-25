#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 25 févr. 2010

@author: nathanael
'''

import numpy

from amelethdf.model.api import IsModel

__all__ = ['IsAH5Model', 'ah5_int', 'ah5_float', 'ah5_complex', 'ah5_bool', 'ah5_str']


class EntryPointIndex(IsModel): pass

ah5_int = numpy.int32
ah5_float = numpy.float32
ah5_complex = numpy.complex64
ah5_bool = numpy.bool
ah5_str = numpy.str


