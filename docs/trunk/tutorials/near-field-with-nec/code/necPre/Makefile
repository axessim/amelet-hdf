# ============================================================================
# Name        : Makefile
# Author      : Cyril Giraudon
# Version     :
# Copyright   : Your copyright notice
# Description : Makefile for input wrapper in Fortran
# ============================================================================

.PHONY: all clean

# Change this line if you are using a different Fortran compiler
FORTRAN_COMPILER = ifort
COMPILER_OPTIONS= -O2 -g -c
HDF5_ROOT=/home/cyril/outils/hdf5/local-intel-1.8.4
AMELET_ROOT=/home/cyril/affaires/cutoo-sdk/build/amelethdf-fortran/local

AMELET_MOD=$(AMELET_ROOT)/include
AMELET_LIB=$(AMELET_ROOT)/lib/libamelethdff.a
HDF5_LIB=$(HDF5_ROOT)/lib/libhdf5hl_fortran.a \
     	 $(HDF5_ROOT)/lib/libhdf5_hl.a \
		 $(HDF5_ROOT)/lib/libhdf5_fortran.a \
		 $(HDF5_ROOT)/lib/libhdf5.a

all: bin/necPre.o
	$(FORTRAN_COMPILER) -o bin/necPre \
		bin/necPre.o $(AMELET_LIB) $(HDF5_LIB) -lz

bin/necPre.o: src/necPre.f90
	$(FORTRAN_COMPILER) $(COMPILER_OPTIONS) \
		-I $(HDF5_ROOT)/include -I $(AMELET_MOD) \
		src/necPre.f90 \
		-o bin/necPre.o \

clean:
	rm -f bin/*

