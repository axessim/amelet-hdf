#!/usr/bin/env python 
'''
Created on 24 nov. 2009

@author: Cyril Giraudon
'''
import sys, os
import shutil

from tables import openFile

if __name__ == '__main__':
    # input.h5 path
    input_file_name = sys.argv[1]
    # workingDir path
    working_dir = output_dir_name = sys.argv[2]

    h5input = openFile(input_file_name, "r")
    
    print "Format : ", h5input.root._v_attrs.FORMAT
    print "Format version : ", h5input.root._v_attrs.AMELETHDF_FORMAT_VERSION
    
    entry_point = h5input.root._v_attrs.entry_point[0].strip()
    print "Entry point : ", entry_point

    simulation = h5input.getNode(entry_point)
    input = simulation.inputs[0].strip()
    output = simulation.outputs[0].strip()
    
    print "Input : ", input
    print "Output : ", output
    
    input_file = h5input.getNode(input).attached_files[0][0]
    print "Input file : ", input_file
    
    input_dir = os.path.dirname(input_file_name)
    print "inputDir : ", input_dir
    print "workingDir : ", working_dir

    # copy input file in working dir
    shutil.copy(os.path.join(input_dir, input_file), working_dir)

    # Generate the run.sh script
    runsh_name = os.path.join(working_dir, "run.sh")
    runsh = open(runsh_name, "w")
    path_linux = simulation.parameter._v_attrs.path_linux[0].strip()
    runsh.write(path_linux + " " + os.path.join(working_dir, input_file + "\n"))
    runsh.close() 
    
    h5input.close()
    