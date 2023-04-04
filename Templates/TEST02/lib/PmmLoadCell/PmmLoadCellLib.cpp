// #include <PMMLoadCellLib.h>

// void PmmLoadCell :: PmmLoadCellSetup(uint8_t DoutPin, uint8_t SCKPin)
// {
//     PMMLoadCell.begin(DoutPin, SCKPin);
// }

// bool PmmLoadCell :: PmmLoadCellReady()
// {
    
//     return PMMLoadCell.is_ready();
// }

// void PmmLoadCell ::PmmLoadCellSetOffset(float Offset)
// {
//     PMMLoadCell.set_offset(Offset);
// }

// float PmmLoadCell ::PmmLoadCellGetOffset()
// {
//    return  PMMLoadCell.get_offset();
// }

// void PmmLoadCell ::PmmLoadCellSetScale(float Scale)
// {
//     PMMLoadCell.set_scale(Scale);
// }

// float PmmLoadCell ::PmmLoadCellGetScale()
// {
//     return PMMLoadCell.get_scale();
// }

// float PmmLoadCell ::PmmLoadCellGetRealRead(uint8_t NumberOfReads)
// {
//     return PMMLoadCell.read_average(NumberOfReads);
// }

// float PmmLoadCell :: PmmLoadCellGetScaleReading(uint8_t NumberOfReads)
// {
//     return PMMLoadCell.get_units(NumberOfReads);
// }

// void ADS1231Clock() // Internal function
// {
// digitalWrite(CLOCK, LOW);   
// digitalWrite(CLOCK, HIGH);
// delayMicroseconds(10);
// } 

// void PmmLoadCell :: PmmADS1231Setup()
// {
// pinMode(POWERDOWN, OUTPUT);  
// pinMode(DATAPIN, INPUT);
// pinMode(CLOCK, OUTPUT);
// pinMode(SPEED, OUTPUT);

// digitalWrite(POWERDOWN, HIGH);  //0-->  POWERDOWN(SLEEPING -NO CONVERSION) ,1-->WORKING MODE
// digitalWrite(SPEED, 0);//0-->  10SPS ,1-->80SPS
// digitalWrite(CLOCK, LOW);
// }

// uint32_t PmmLoadCell :: PmmADS1231ReadRowData() 
// {
// uint32_t  AdcRead=0;  
// digitalWrite(POWERDOWN, HIGH); //POWER ON
// while(digitalRead(DATAPIN));//WAIT UNTIL DATA IS READY
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X800000;} //24
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X400000;}//23
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X200000;}//22
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X100000;}//21
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X80000;} //20
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X40000;}//19
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X20000;}//18
// ADS1231Clock();
// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X10000;}//17
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X8000;} //16
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X4000;}//15
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X2000;}//14
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X1000;}//13
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X800;} //12
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X400;}//11
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X200;}//10
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X100;}//9
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X80;} //8
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X40;}//7
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X20;}//6
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X10;}//5
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X8;} //4
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X4;}//3
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X2;}//2
// ADS1231Clock();

// if(digitalRead(DATAPIN))
// {AdcRead=AdcRead+0X1;}//1
// ADS1231Clock();//TO PULL DATA READY TO HIGH STATE --PAGE No.14 ON ADS1231 DATASHEET

// digitalWrite(CLOCK, LOW);   
// digitalWrite(POWERDOWN, LOW);
// return AdcRead;
// }

// float PmmLoadCell :: PmmADS1231Caliberation(float RealScale)
// {
//   float Value=(float(PmmADS1231ReadRowData())/100000.0);
//   Value = RealScale/Value;
//   return Value;  
// }

// float PmmLoadCell :: PmmADS1231ReadScaleValue()
// {
// float Value=(float(PmmADS1231ReadRowData())/100000.0);
// return Value;
// }
