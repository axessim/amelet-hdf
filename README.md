﻿## Introduction

The purpose of the Amelet-HDF Specification is to provide a standard for
recording and recovering computer data of electromagnetic simulations.

A few well established data formats already exist, we can quote :

* CGNS (CFD General Notation System, http://cgns.sourceforge.net/) is
  recommended for computational fluid dynamics problems (CFD). That's why 
  a lot of electromagnetic concepts are missing
* SILO (a mesh and field I/O library and scientific database
  https://wci.llnl.gov/codes/silo/)
* MED (in french "Modélisation et échanges de Données"
  http://www.code-aster.org/outils/med/), it is the SALOME's data format
  (http://www.salome-platform.org/home/presentation/overview/). Like CGNS,
  SALOME provides tools for CFD simulation and aren't adapted to the
  electromagnetism domain
* netCDF (http://www.unidata.ucar.edu/software/netcdf/)

Besides, Amelet-HDF Specification is closely related to the CuToo Platform.

CuToo is a software platform which aims at providing a universal and
collaborative simulation environment. To achieve this objective, CuToo:

* Offers tools to integrate scientific software products in a homogeneous
  environment:

  * Provide a common description format (Amelet-HDF)
  * Handle pre and post processing tasks
  * Managing software execution

* Manage computation workflow
* Provide knowledge management capabilities
* Manage authentications and permissions
* Handle remote visualizations

CGNS, SILO and MED specifications give a standard way of describing physical
models, mesh definition and many physical concepts. Software tool that can
express their native data in CGNS vocabulary are good "customers" for the
specification, for data that can not be correctly expressed CGNS offers a
"user variable" notion. But for other software handling complex structures
stranger to the specification, there is no way to use the existing format,
even by part.

CuToo expresses data in the form of objects named "InfoType", it is possible
to add new InfoType to the platform and this InfoType have to be converted
into an equivalent concepts into Amelet-HDF. So, Amelet-HDF must be sufficiently
flexible to express unknown data coming from the new InfoTypes. This can not be
done with CGNS or MED.

Basically, Amelet HDF specification can express all sort of electromagnetic data,
the most important are :

* Mesh (unstructured or structured);
* Numerical array data;
* Material models
* Network and transmission line;
* Electromagnetic sources

This document covers all the aspects of the Amelet HDF specification.

Note: Amelet HDF is largely inspired from the Amelet project
(http://www.predit.prd.fr/predit3/syntheseProjet.fo?inCde=22740)