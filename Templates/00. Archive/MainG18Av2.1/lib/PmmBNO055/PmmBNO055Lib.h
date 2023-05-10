//#include "ProjectDef.h"


#ifdef PMMBNO

extern float xAxisCalibration, yAxisCalibration, zAxisCalibration;
extern string PMMMBNOSetup();
extern void PMMBNOGetCoordination(float &xAxis, float &yAxis, float &zAxis);
extern string PMMBNCalibrate();

#endif