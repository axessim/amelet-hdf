#!/usr/bin/env python 
'''
Created on 24 nov. 2009

@author: Cyril Giraudon
'''
import sys, os
import shutil

from tables import openFile

if __name__ == '__main__':
    # workingDir
    working_dir = sys.argv[1]
    # output file
    output_dir_name = sys.argv[2]

    input_dir = os.path.join(os.path.dirname(working_dir), "inputDir")
    output_dir = os.path.join(os.path.dirname(working_dir), "outputDir")
    print "inputDir : ", input_dir
    print "workingDir : ", working_dir
    print "outputDir : ", output_dir

    # Copy inputDir / input.h5 in outputDir / output.h5
    shutil.copy(os.path.join(input_dir, "input.h5"), output_dir_name)
    
    # Create outputDir / output.h5
    h5output = openFile(output_dir_name, "a")
    entry_point = h5output.root._v_attrs.entry_point[0].strip()
    
    simulation = h5output.getNode(entry_point)
    input = simulation.inputs[0].strip()
    output = simulation.outputs[0].strip()
    
    print "Input : ", input
    print "Output : ", output
    
    input_file = h5output.getNode(input).attached_files[0][0]
    print "Input file : ", input_file
    output_file = h5output.getNode(output).attached_files[0][0]
    print "Output file : ", output_file
    
    shutil.copy(os.path.join(working_dir, input_file),
                os.path.join(output_dir, output_file)) 
    
    
    h5output.close()
