#include "ProjectDef.h"
#ifdef PMMMPU

void PMMMPUSetup();
void ZeroMPUCalibration();
float PMMMPUGetAngles();
float PMMMPUActualAngle();

void PMMMPUSetup()
{
    Wire.begin();
    Wire.beginTransmission(0x69);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void ZeroMPUCalibration()
{
    float CalibAngle = PMMMPUActualAngle();
    //PutCalibrateMPUAngleToEEprom(CalibAngle);
}

float PMMMPUGetAngles()
{
    double x;

    int16_t AcX, AcY, AcZ;
    int minVal = 265;
    int maxVal = 402;
    float CalibAngle = GetCalibrateMPUAngleToEEprom();
    Wire.beginTransmission(0x69);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x69, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);

    x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    // double y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    // double z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

    if (x > CalibAngle && x < 180)
    {
        x = (x - CalibAngle) + (x / 40);
    }

    else
    {
        x = -1 * ((360 + CalibAngle - x));
    }

    return x;
}

float PMMMPUActualAngle() // This Function Use Only To calibration
{
    int16_t AcX, AcY, AcZ;
    int minVal = 265;
    int maxVal = 402;
    float CalibAngle = GetCalibrateMPUAngleToEEprom();
    Wire.beginTransmission(0x69);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x69, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);

    double x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    // double y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    // double z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    if (abs(x) > 180)
    {
        x = x - 360;
    }

    return x;
}

#endif