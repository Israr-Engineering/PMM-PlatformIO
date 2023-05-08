
typedef struct PMMIO
{
    // // represent descret InputStatus in modbus
    //int InputsByte[120];
    // // image for internal use
    //int InputsBytePrev[120];
    // // represent coils bits in modbus
    // bit OutputsByte[1920];
    // // image for internal use
    // bit OutputsBytePrev[1920];

    // represent Input registers in modbus
    byte Inputs[64];
    // image for internal use
    //int InputsPrev[120];
    // represent hodling registers in modbus
    byte Outputs[64];
    // image for internal use
    //int OutputsPrev[120];

} PMMIO;

PMMIO PmmIO;


int BytesToInt(byte IntLowByte, byte IntHighByte)
{
    int tmpInt = IntHighByte;
    tmpInt = (tmpInt << 8) | IntLowByte;

    return tmpInt;
}
