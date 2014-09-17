#ifndef AH5_CATEGORY_H
#define AH5_CATEGORY_H

#include <hdf5.h>
#include <ah5_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AH5_FILE_A_FORMAT                "FORMAT"
#define AH5_FILE_FORMAT                  "AMELETHDF"
#define AH5_FILE_A_VERSION               "AMELETHDF_FORMAT_VERSION"
#define AH5_FILE_DEFAULT_VERSION         "1.5.4"
  
// remove first char
#define AH5_CATEGORY_NAME(name) ((name) + 1)

#define AH5_C_ELECTROMAGNETIC_SOURCE    "/electromagneticSource"
#define AH5_C_EXCHANGE_SURFACE          "/exchangeSurface"
#define AH5_C_EXTERNAL_ELEMENT          "/externalElement"
#define AH5_C_GLOBAL_ENVIRONMENT        "/globalEnvironment"
#define AH5_C_LABEL                     "/label"
#define AH5_C_LINK                      "/link"
#define AH5_C_LOCALIZATION_SYSTEM       "/localizationSystem"
#define AH5_C_MESH                      "/mesh"
#define AH5_C_OUTPUT_REQUEST            "/outputRequest"
#define AH5_C_PHYSICAL_MODEL            "/physicalModel"
#define AH5_C_SIMULATION                "/simulation"
#define AH5_C_FLOATING_TYPE             "/floatingType"


/* ah5_general.h */
#define AH5_A_ENTRY_POINT               "entryPoint"
#define AH5_ABSOLUTE_PATH_LENGTH        111  // length in C (incl. '\0')
#define AH5_ELEMENT_NAME_LENGTH         31
#define AH5_ATTR_LENGTH                 31
#define AH5_TABLE_FIELD_NAME_LENGTH     31
#define AH5_V_INVALID                   "INVALID"
#define AH5_TRUE                        1
#define AH5_FALSE                       0
#define AH5_NATIVE_CHAR                 H5T_STD_I8LE   // NATIVE Amelet-HDF data type
#define AH5_NATIVE_INT                  H5T_STD_I32LE
#define AH5_NATIVE_FLOAT                H5T_IEEE_F32LE
#define AH5_NATIVE_STRING               H5T_FORTRAN_S1


/* ah5_emsource.h */
#define AH5_A_X                         "x"
#define AH5_A_Y                         "y"
#define AH5_A_Z                         "z"
#define AH5_A_XO                        "xo"
#define AH5_A_YO                        "yo"
#define AH5_A_ZO                        "zo"
#define AH5_A_THETA                     "theta"
#define AH5_A_PHI                       "phi"
#define AH5_A_TYPE                      "type"
#define AH5_A_WIRE_RADIUS               "wireRadius"
#define AH5_A_EXCHANGE_SURFACE          "exchangeSurface"
#define AH5_G_PLANE_WAVE                "/planeWave"
#define AH5_G_SPHERICAL_WAVE            "/sphericalWave"
#define AH5_G_GENERATOR                 "/generator"
#define AH5_G_DIPOLE                    "/dipole"
#define AH5_G_ANTENNA                   "/antenna"
#define AH5_G_SOURCE_ON_MESH            "/sourceOnMesh"
#define AH5_G_INNER_IMPEDANCE           "/innerImpedance"
#define AH5_G_MAGNITUDE                 "/magnitude"
#define AH5_G_INPUT_IMPEDANCE           "/inputImpedance"
#define AH5_G_LOAD_IMPEDANCE            "/loadImpedance"
#define AH5_G_FEEDER_IMPEDANCE          "/feederImpedance"
#define AH5_G_MODEL                     "/model"
#define AH5_G_GAIN                      "/gain"
#define AH5_G_EFFECTIVE_AREA            "/effectiveArea"
#define AH5_G_FAR_FIELD                 "/farField"
#define AH5_G_PARABOLIC_REFLECTOR       "/parabolicReflector"
#define AH5_V_VOLTAGE                   "voltage"
#define AH5_V_CURRENT                   "current"
#define AH5_V_POWER                     "power"
#define AH5_V_POWER_DENSITY             "powerDensity"
#define AH5_V_ELECTRIC                  "electric"
#define AH5_V_MAGNETIC                  "magnetic"
#define AH5_V_GAIN                      "gain"
#define AH5_V_EFFECTIVE_AREA            "effectiveArea"
#define AH5_V_FAR_FIELD                 "farField"
#define AH5_V_RECTANGULAR_HORN          "rectangularHorn"
#define AH5_V_CIRCULAR_HORN             "circularHorn"
#define AH5_V_LOG_PERIODIC              "logPeriodic"
#define AH5_V_WHIP                      "whip"
#define AH5_V_GENERIC                   "generic"
#define AH5_V_EXCHANGE_SURFACE          "exchangeSurface"


/* ah5_exsurf.h */
#define AH5_A_TYPE          "type"
#define AH5_A_NATURE        "nature"
#define AH5_V_RECIPROCITY   "reciprocity"
#define AH5_V_HUYGENS       "huygens"
#define AH5_V_GAUSS         "gauss"
#define AH5_V_OUTSIDE       "outside"
#define AH5_V_INSIDE        "inside"


/* ah5_extelt.h */
#define AH5_EE_INTERNAL_NAME(id)        3*id
#define AH5_EE_EXTERNAL_FILE_NAME(id)   3*id + 1
#define AH5_EE_EXTERNAL_NAME(id)        3*id + 2


/* ah5_fltype.h */
#define AH5_A_FLOATING_TYPE                 "floatingType"
#define AH5_A_LABEL                         "label"
#define AH5_A_PHYSICAL_NATURE               "physicalNature"
#define AH5_A_UNIT                          "unit"
#define AH5_A_COMMENT                       "comment"
#define AH5_A_VALUE                         "value"
#define AH5_A_FIRST                         "first"
#define AH5_A_LAST                          "last"
#define AH5_A_NUMBER_OF_VALUES              "numberOfValues"
#define AH5_A_STEP                          "step"
#define AH5_A_NUMBER_OF_DECADES             "numberOfDecades"
#define AH5_A_NUMBER_OF_VALUES_PER_DECADE   "numberOfValuesPerDecade"
#define AH5_F_TYPE                          "type"
#define AH5_F_A                             "A"
#define AH5_F_B                             "B"
#define AH5_F_F                             "F"
#define AH5_G_DATA                          "/data"
#define AH5_G_DS                            "/ds"
#define AH5_G_FUNCTION                      "/function"
#define AH5_V_SINGLE_INTEGER                "singleInteger"
#define AH5_V_SINGLE_REAL                   "singleReal"
#define AH5_V_SINGLE_COMPLEX                "singleComplex"
#define AH5_V_SINGLE_STRING                 "singleString"
#define AH5_V_VECTOR                        "vector"
#define AH5_V_LINEARLISTOFREAL1             "linearListOfReal1"
#define AH5_V_LINEARLISTOFREAL2             "linearListOfReal2"
#define AH5_V_LOGARITHMLISTOFREAL           "logarithmListOfReal"
#define AH5_V_PERDECADELISTOFREAL           "perDecadeListOfReal"
#define AH5_V_LINEARLISTOFINTEGER2          "linearListOfInteger2"
#define AH5_V_RATIONAL_FUNCTION             "rationalFunction"
#define AH5_V_GENERAL_RATIONAL_FUNCTION     "generalRationalFunction"
#define AH5_V_RATIONAL                      "rational"
#define AH5_V_DATASET                       "dataSet"
#define AH5_V_ARRAYSET                      "arraySet"


/* ah5_globenv.h */
#define AH5_G_FREQUENCY         "/frequency"
#define AH5_G_TIME              "/time"
#define AH5_G_LIMIT_CONDITIONS  "/limitConditions"


/* ah5_link.h */
#define AH5_A_SUBJECT           "subject"
#define AH5_A_OBJECT            "object"


/* ah5_locsys.h */
#define AH5_A_TYPE              "type"
#define AH5_A_RANK              "rank"
#define AH5_A_DIMENSION         "dimension"
#define AH5_V_SCALE             "scale"
#define AH5_V_ROTATION          "rotation"
#define AH5_V_TRANSLATION       "translation"


/* ah5_mesh.h */
#define AH5_A_TYPE              "type"
#define AH5_A_ENTITY_TYPE       "entityType"
#define AH5_A_MESH1             "mesh1"
#define AH5_A_MESH2             "mesh2"
#define AH5_G_NODES             "/nodes"
#define AH5_G_ELEMENT_TYPES     "/elementTypes"
#define AH5_G_ELEMENT_NODES     "/elementNodes"
#define AH5_G_GROUP             "/group"
#define AH5_G_GROUPGROUP        "/groupGroup"
#define AH5_G_SELECTOR_ON_MESH  "/selectorOnMesh"
#define AH5_G_CARTESIAN_GRID    "/cartesianGrid"
#define AH5_G_NORMAL            "/normal"
#define AH5_G_X                 "/x"
#define AH5_G_Y                 "/y"
#define AH5_G_Z                 "/z"
#define AH5_G_MESH_LINK         "/meshLink"
#define AH5_V_POINT_IN_ELEMENT  "pointInElement"
#define AH5_V_NODE              "node"
#define AH5_V_EDGE              "edge"
#define AH5_V_FACE              "face"
#define AH5_V_VOLUME            "volume"
#define AH5_V_UNSTRUCTURED      "unstructured"
#define AH5_V_STRUCTURED        "structured"
#define AH5_F_IMIN              "imin"
#define AH5_F_IMAX              "imax"
#define AH5_F_JMIN              "jmin"
#define AH5_F_JMAX              "jmax"
#define AH5_F_KMIN              "kmin"
#define AH5_F_KMAX              "kmax"
#define AH5_F_V1                "v1"
#define AH5_F_V2                "v2"
#define AH5_F_V3                "v3"
#define AH5_V_ELEMENT           "element"


/* ah5_outreq.h */
#define AH5_A_SUBJECT           "subject"
#define AH5_A_SUBJECT_ID        "subject_id"
#define AH5_A_OBJECT            "object"
#define AH5_A_OUTPUT            "output"


/* ah5_phmodel.h */
#define AH5_G_RELATIVE_PERMITTIVITY             "/relativePermittivity"
#define AH5_G_RELATIVE_PERMEABILITY             "/relativePermeability"
#define AH5_G_ELECTRIC_CONDUCTIVITY             "/electricConductivity"
#define AH5_G_MAGNETIC_CONDUCTIVITY             "/magneticConductivity"
#define AH5_G_VOLUME                            "/volume"
#define AH5_G_VACUUM                            "/vacuum"
#define AH5_G_PERFECT_ELECTRIC_CONDUCTOR        "/perfectElectricConductor"
#define AH5_G_PERFECT_MAGNETIC_CONDUCTOR        "/perfectMagneticConductor"
#define AH5_G_SURFACE                           "/surface"
#define AH5_G_INTERFACE                         "/interface"
#define AH5_G_LIST_OF_FUNCTIONS                 "/listOfFunctions"
#define AH5_A_F_MIN                             "frequency_validity_min"
#define AH5_A_F_MAX                             "frequency_validity_max"
#define AH5_A_TYPE                              "type"
#define AH5_A_PHYSICAL_MODEL                    "physicalModel"
#define AH5_A_THICKNESS                         "thickness"
#define AH5_A_ZS                                "Zs"
#define AH5_A_ZT                                "Zt"
#define AH5_A_ZS1                               "Zs1"
#define AH5_A_ZT1                               "Zt1"
#define AH5_A_ZS2                               "Zs2"
#define AH5_A_ZT2                               "Zt2"
#define AH5_A_MEDIUM1                           "medium1"
#define AH5_A_MEDIUM2                           "medium2"
#define AH5_A_ER_LIMIT                          "epsilonLimit"
#define AH5_A_ER_STATIC                         "epsilonStatic"
#define AH5_V_THIN_DIELECTRIC_LAYER             "thinDielectricLayer"
#define AH5_V_SIBC                              "SIBC"
#define AH5_V_ZS                                "Zs"
#define AH5_V_ZSZT                              "ZsZt"
#define AH5_V_ZSZT2                             "ZsZt2"
#define AH5_V_DEBYE                             "debye"
#define AH5_V_LORENTZ                           "lorentz"

/* ah5_simulation.h */
#define AH5_A_MODULE    "module"
#define AH5_A_VERSION   "version"
#define AH5_G_INPUTS    "/inputs"
#define AH5_G_OUTPUTS   "/outputs"
#define AH5_G_PARAMETER "/parameter"

#ifdef __cplusplus
}
#endif

#endif // AH5_CATEGORY_H
