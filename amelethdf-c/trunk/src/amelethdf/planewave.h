#include <complex.h>

#include "floatingtype.h"

#ifndef PLANEWAVE_H
#define PLANEWAVE_H

#define C_PLANE_WAVE "planeWave"
#define F_MAGNITUDE "magnitude"
#define A_THETA "theta"
#define A_PHI "phi"
#define A_LINEAR_POLARIZATION "linearPolarization"
#define A_ELLIPTICAL_POLARIZATION_ETHETA "ellipticalPolarizationETheta"
#define A_ELLIPTICAL_POLARIZATION_EPHI "ellipticalPolarizationEPhi"

#define LINEAR 1
#define ELLIPTIC 2

typedef struct
{
        float theta;
        float phi;
        float linear_polarization;
        int polarization;
        complex float elliptical_polarization_etheta;
        complex float elliptical_polarization_ephi;
        floatingtype_t magnitude;
} planewave_t;

planewave_t read_planewave(hid_t file_id, const char* path);
planewave_t clear_content_planewave(planewave_t pw);
int islinear(planewave_t pw);
int iselliptic(planewave_t pw);

#endif // PLANEWAVE_H
