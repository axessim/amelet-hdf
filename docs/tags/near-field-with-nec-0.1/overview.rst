Overview
========


This tutorial aims at showing how to compute the electric field alongside 
a dipole with the well known Nec software within the Quercy platform.

We don't intend to integrate Nec completely, the code that will be written
is adapted to a simple use case.


The use case
^^^^^^^^^^^^

The use case is based upon the first example of the Nec distribution.

A 0.5m length dipole is modeled with two linked points :

* the first point is at (x=0  y=0  z=-0.25 in meter)
* the second point is at (x=0  y=0  z=0.25 in meter)

The wire radius is 0.001m and finally the dipole is discretized in 7 segments.
The source is a voltage source (1V) located on the fourth segment.


.. image:: images/wire.png


A load serial impedance is also located on the fourth segment :

* The resistance is R = 10 Ohms
* The inductance is L = 3e-9 Henries
* The capacitance is C = 5.3e-11 Farads

A near electric fields calculation is requested on 15 points from the point of
coordinates (x = 0.001, y = 0, z = 0) and with a dz=0.1786m z step.

The frequency is 299.8 MHz.

