# ============================================================================
# Name        : Makefile
# Author      : Cyril Giraudon
# Version     :
# Copyright   : Your copyright notice
# Description : Makefile for output wrapper in Fortran
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

all: bin/necPost.o
	$(FORTRAN_COMPILER) -o bin/necPost \
		bin/necPost.o $(AMELET_LIB) $(HDF5_LIB) -lz

bin/necPost.o: src/necPost.f90
	$(FORTRAN_COMPILER) $(COMPILER_OPTIONS) \
		-I $(HDF5_ROOT)/include -I $(AMELET_MOD) \
		src/necPost.f90 \
		-o bin/necPost.o

clean:
	rm -f bin/*
