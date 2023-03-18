#include "ProjectDef.h"

#ifdef PMMBNO

PmmBNO055 bno = PmmBNO055(55);



extern float xAxisCalibration, yAxisCalibration, zAxisCalibration;
//extern void PMMMBNOSetup();
extern void PMMBNOGetCoordination(float &xAxis, float &yAxis, float &zAxis);
//extern void PMMBNCalibrate();

string PMMMBNOSetup()
{
    string result = "";
    bool BNOFound = bno.begin();

    if (!BNOFound)
    {
        result = "Faild Initalize PMMMBNO";
    }
    else
    {
        bno.setExtCrystalUse(false);
        result = "Initalize PMMMBNO Successfully";
    }

    return result;
}

void PMMBNOGetCoordination(float &xAxis, float &yAxis, float &zAxis)
{
    sensors_event_t event;
    bno.getEvent(&event);
    xAxis = event.orientation.x + xAxisCalibration;
    yAxis = event.orientation.y + yAxisCalibration;
    zAxis = event.orientation.z + zAxisCalibration;
}


string PMMBNCalibrate()
{    
    float currentxAxis, currentyAxis, currentzAxis;

    xAxisCalibration = 0;
    yAxisCalibration = 0;
    zAxisCalibration = 0;

    PMMBNOGetCoordination(currentxAxis, currentyAxis, currentzAxis);

    xAxisCalibration = -1 * currentxAxis;
    yAxisCalibration = -1 * currentyAxis;
    zAxisCalibration = -1 * currentzAxis;

    return "BNO Calibrated";
}

#endif