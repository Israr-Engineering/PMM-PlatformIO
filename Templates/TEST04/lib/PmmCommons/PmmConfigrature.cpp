#include "PmmConfigrature.h"

struct PmmGeneralSettings PMMGENERALSETTINGS;
struct PmmModBusSettings PMMMODBUSSETTINGS;
struct PmmTCPUDPSettings PMMTCPUDPSETTINGS;
struct PmmSerialSettings PMMSERIALSETTINGS;
struct PmmIOPins PMMIOPINS;
struct PmmExtensionsSettings PMMEXTENSIONSSETTINGS;

char *strings[120]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
string values[120];

typedef struct tcp_udp_settings
{
    string ip_address;
    int port;
    string net_mask;
    string default_gateway;
    string mac_address;
    string preferred_dns_server;
    string alternate_dns_server;
    int connection_timeout_tcp;
    int max_retry_tcp;
    int udp_port_one;
    int udp_port_two;
    int udp_port_three;
    int udp_port_four;
} tcp_udp_settings;

typedef struct serial_connector
{
    bool enabled;
    string type;
} serial_connector;

typedef struct rtu_settings
{
    string ComName;
    int32_t baudRate;
    int32_t dataBits;
    int32_t stopBits;
    int32_t parity;
    string interface;
    int32_t connection_timeout_rtu;
    int32_t max_retry_rtu;
} rtu_settings;

typedef struct left_io_pins
{
    string pin1;
    string pin2;
    string pin3;
    string pin4;
    string pin5;
    string pin6;
    string pin7;
    string pin8;
    string pin9;
    string pin10;
    string pin11;
    string pin12;
} left_io_pins;

typedef struct right_io_pins
{
    string pin13;
    string pin14;
    string pin15;
    string pin16;
    string pin17;
    string pin18;
    string pin19;
    string pin20;
    string pin21;
    string pin22;
    string pin23;
    string pin24;
} right_io_pins;

typedef struct Product
{
    // string name[128];
    string firmware_version;
    string serial_number;
    string hardware_version;
    string software_version;
    string cpu_type;
    // string description[128];
    // string created_by;
    string settings_rom;
    string connection_type;
    bool ethernet;
    bool fiber;
    bool switchh;
    bool rtc_external;
    bool rtc_internal;
    bool eprom;
    bool internal_flash;
    bool external_flash;
    bool i2c;
    struct left_io_pins left_io_pins;
    struct right_io_pins right_io_pins;
    struct serial_connector serial1;
    struct serial_connector serial2;
    struct serial_connector serial3;
    struct serial_connector serial4;
    struct tcp_udp_settings tcp_udp_settings;
    struct rtu_settings rtu_settings;
    string product_family;
    string enclosure_type;
    string other_support_name;
    bool arduino_support;
    bool platform_io_support;
    bool other_support;
    string slave_i2c_address;
    string board1;
    string board1_address1;
    string board1_address2;
    string board2;
    string board2_address1;
    string board2_address2;
    string board3;
    string board3_address1;
    string board3_address2;
    string board4;
    string board4_address1;
    string board4_address2;
    bool usb_com;
    bool ice_only;
    string settingPanel;
} Product;

FlashStorage(my_flash_store, Product);

// std::vector<string> PMMStringToArray(string csvStr, char delimiter)
// {
//     vector<string> arr;
//     istringstream iss(csvStr);
//     string token;
//     while (getline(iss, token, delimiter))
//     {
//         arr.push_back(token);
//     }
//     return arr;
// }

void PmmStringToArray(string input)
{

    int length = input.length();

    // declaring character array (+1 for null terminator)
    char *char_array = new char[length + 1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, input.c_str());

    byte index = 0;
    ptr = strtok(char_array, ","); // delimiter
    while (ptr != NULL)
    {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, ",");
    }

    for (int n = 0; n < 120; n++)
    {
        string s(strings[n]);
        values[n] = s;
    }
}

void PMMWriteIntoFlashAllSettings(string Message)
{
    // std::vector<string> values = PMMStringToArray(Message,',');

    PmmStringToArray(Message);

    // for (int n = 0; n < 120; n++)
    // {
    //     SerialUSB.print(n);
    //     SerialUSB.print(" : ");
    //     SerialUSB.println((values[n]).c_str());
    // }

    Product product;

    //product = my_flash_store.read();
    //SerialUSB.println((PMMGENERALSETTINGS.FirmwareVersion).c_str());
    // General Info
    PMMGENERALSETTINGS.FirmwareVersion = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    PMMGENERALSETTINGS.HardwareVersion = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    PMMGENERALSETTINGS.HardwareVersion = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    PMMGENERALSETTINGS.SerialNumber = stoi(values[12]);
    PMMGENERALSETTINGS.CPUType = values[13];
    PMMGENERALSETTINGS.ProductFamily = values[14];
    PMMGENERALSETTINGS.EnclosureType = values[15];
    PMMGENERALSETTINGS.ArduinoSupport = (values[16] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.PlatformIOSupport = (values[17] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.OtherSupportName = values[18];

    // RTU Setting
    PMMSERIALSETTINGS.PortOneName = values[19];
    PMMSERIALSETTINGS.PortOneBaudRate = stoi(values[20]);
    PMMSERIALSETTINGS.PortOneDataBits = stoi(values[21]);
    PMMSERIALSETTINGS.PortOneStopBits = stoi(values[22]);
    PMMSERIALSETTINGS.PortOneParity = values[23];
    PMMSERIALSETTINGS.PortOneInterface = (values[24] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.PortOneConnTimeOut = stoi(values[25]);
    PMMSERIALSETTINGS.PortOneMaxRetries = stoi(values[26]);

    // TCP Settings
    PMMTCPUDPSETTINGS.IPAddressEthOne = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    PMMTCPUDPSETTINGS.SubnetMaskEthOne = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    PMMTCPUDPSETTINGS.PreferredDNSServer = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    PMMTCPUDPSETTINGS.AlternateDNSServer = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    PMMTCPUDPSETTINGS.GatewayEthOne = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    PMMTCPUDPSETTINGS.MacAddressEthOne = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    PMMTCPUDPSETTINGS.TimeOutConnEthOne = stoi(values[52]);
    PMMTCPUDPSETTINGS.MaxRetriesEthOne = stoi(values[53]);
    PMMTCPUDPSETTINGS.UDPPortOne = stoi(values[54]);
    PMMTCPUDPSETTINGS.UDPPortTwo = stoi(values[55]);
    PMMTCPUDPSETTINGS.UDPPortThree = stoi(values[56]);
    PMMTCPUDPSETTINGS.UDPPortFour = stoi(values[57]);

    // Options
    PMMGENERALSETTINGS.HasEthernet = (values[58] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasFiber = (values[59] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasSwitch = (values[60] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasExternalRTC = (values[61] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasInternalRTC = (values[62] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasEEprom = (values[63] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasInternalFlash = (values[64] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasExternalFlash = (values[65] == "1" ? "true" : "false");

    PMMSERIALSETTINGS.SerialOneEnabled = (values[66] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialOneType = values[67];
    PMMSERIALSETTINGS.SerialTowEnabled = (values[68] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialTowType = values[69];
    PMMSERIALSETTINGS.SerialThreeEnabled = (values[69] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialThreeType = values[71];
    PMMSERIALSETTINGS.SerialFourEnabled = (values[70] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialFourType = values[73];

    // Extension Boards
    PMMEXTENSIONSSETTINGS.I2C = (values[74] == "1" ? "true" : "false");
    PMMEXTENSIONSSETTINGS.SlaveI2CAddress = values[75];
    PMMEXTENSIONSSETTINGS.BoardOne = values[76];
    PMMEXTENSIONSSETTINGS.BoardOneAddress1 = values[77];
    PMMEXTENSIONSSETTINGS.BoardOneAddress2 = values[78];
    PMMEXTENSIONSSETTINGS.BoardTwo = values[79];
    PMMEXTENSIONSSETTINGS.BoardTowAddress1 = values[80];
    PMMEXTENSIONSSETTINGS.BoardTowAddress2 = values[81];
    PMMEXTENSIONSSETTINGS.BoardThree = values[82];
    PMMEXTENSIONSSETTINGS.BoardThreeAddress1 = values[83];
    PMMEXTENSIONSSETTINGS.BoardThreeAddress2 = values[84];
    PMMEXTENSIONSSETTINGS.BoardFour = values[85];
    PMMEXTENSIONSSETTINGS.BoardFourAddress1 = values[86];
    PMMEXTENSIONSSETTINGS.BoardFourAddress2 = values[87];

    // Left Pins
    PMMIOPINS.Pin01 = values[88];
    PMMIOPINS.Pin02 = values[89];
    PMMIOPINS.Pin03 = values[90];
    PMMIOPINS.Pin04 = values[91];
    PMMIOPINS.Pin05 = values[92];
    PMMIOPINS.Pin06 = values[93];
    PMMIOPINS.Pin07 = values[94];
    PMMIOPINS.Pin08 = values[95];
    PMMIOPINS.Pin09 = values[96];
    PMMIOPINS.Pin10 = values[97];
    PMMIOPINS.Pin11 = values[98];
    PMMIOPINS.Pin12 = values[99];

    // Right Pins
    PMMIOPINS.Pin13 = values[100];
    PMMIOPINS.Pin14 = values[101];
    PMMIOPINS.Pin15 = values[102];
    PMMIOPINS.Pin16 = values[103];
    PMMIOPINS.Pin17 = values[104];
    PMMIOPINS.Pin18 = values[105];
    PMMIOPINS.Pin19 = values[106];
    PMMIOPINS.Pin20 = values[107];
    PMMIOPINS.Pin21 = values[108];
    PMMIOPINS.Pin22 = values[109];
    PMMIOPINS.Pin23 = values[110];
    PMMIOPINS.Pin24 = values[111];

    // General Info
    product.firmware_version = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    product.software_version = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    product.hardware_version = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    product.serial_number = values[12];
    product.cpu_type = values[13];
    product.product_family = values[14];
    product.enclosure_type = values[15];
    product.arduino_support = (values[16] == "1" ? "true" : "false");
    product.platform_io_support = (values[17] == "1" ? "true" : "false");
    product.other_support_name = values[18];

    // RTU Setting
    product.rtu_settings.ComName = values[19];
    product.rtu_settings.baudRate = stoi(values[20]);
    product.rtu_settings.dataBits = stoi(values[21]);
    product.rtu_settings.stopBits = stoi(values[22]);
    product.rtu_settings.parity = stoi(values[23]);
    product.rtu_settings.interface = values[24];
    product.rtu_settings.connection_timeout_rtu = stoi(values[25]);
    product.rtu_settings.max_retry_rtu = stoi(values[26]);

    // TCP Settings
    product.tcp_udp_settings.ip_address = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    product.tcp_udp_settings.net_mask = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    product.tcp_udp_settings.preferred_dns_server = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    product.tcp_udp_settings.alternate_dns_server = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    product.tcp_udp_settings.default_gateway = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    product.tcp_udp_settings.mac_address = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    product.tcp_udp_settings.connection_timeout_tcp = stoi(values[52]);
    product.tcp_udp_settings.max_retry_tcp = stoi(values[53]);
    product.tcp_udp_settings.udp_port_one = stoi(values[54]);
    product.tcp_udp_settings.udp_port_two = stoi(values[55]);
    product.tcp_udp_settings.udp_port_three = stoi(values[56]);
    product.tcp_udp_settings.udp_port_four = stoi(values[57]);

    // Options
    product.ethernet = (values[58] == "1" ? "true" : "false");
    product.fiber = (values[59] == "1" ? "true" : "false");
    product.switchh = (values[60] == "1" ? "true" : "false");
    product.rtc_external = (values[61] == "1" ? "true" : "false");
    product.rtc_internal = (values[62] == "1" ? "true" : "false");
    product.eprom = (values[63] == "1" ? "true" : "false");
    product.internal_flash = (values[64] == "1" ? "true" : "false");
    product.external_flash = (values[65] == "1" ? "true" : "false");

    // Serail Connectors
    product.serial1.enabled = (values[66] == "1" ? "true" : "false");
    product.serial1.type = values[67];
    product.serial2.enabled = (values[68] == "1" ? "true" : "false");
    product.serial2.type = values[69];
    product.serial3.enabled = (values[69] == "1" ? "true" : "false");
    product.serial3.type = values[71];
    product.serial4.enabled = (values[70] == "1" ? "true" : "false");
    product.serial4.type = values[73];

    // Extension Boards
    product.i2c = (values[74] == "1" ? "true" : "false");
    product.slave_i2c_address = values[75];
    product.board1 = values[76];
    product.board1_address1 = values[77];
    product.board1_address2 = values[78];
    product.board2 = values[79];
    product.board2_address1 = values[80];
    product.board2_address2 = values[81];
    product.board3 = values[82];
    product.board3_address1 = values[83];
    product.board3_address2 = values[84];
    product.board4 = values[85];
    product.board4_address1 = values[86];
    product.board4_address2 = values[87];

    // Left Pins
    product.left_io_pins.pin1 = values[88];
    product.left_io_pins.pin2 = values[89];
    product.left_io_pins.pin3 = values[90];
    product.left_io_pins.pin4 = values[91];
    product.left_io_pins.pin5 = values[92];
    product.left_io_pins.pin6 = values[93];
    product.left_io_pins.pin7 = values[94];
    product.left_io_pins.pin8 = values[95];
    product.left_io_pins.pin9 = values[96];
    product.left_io_pins.pin10 = values[97];
    product.left_io_pins.pin11 = values[98];
    product.left_io_pins.pin12 = values[99];

    // Right Pins
    product.right_io_pins.pin13 = values[100];
    product.right_io_pins.pin14 = values[101];
    product.right_io_pins.pin15 = values[102];
    product.right_io_pins.pin16 = values[103];
    product.right_io_pins.pin17 = values[104];
    product.right_io_pins.pin18 = values[105];
    product.right_io_pins.pin19 = values[106];
    product.right_io_pins.pin20 = values[107];
    product.right_io_pins.pin21 = values[108];
    product.right_io_pins.pin22 = values[109];
    product.right_io_pins.pin23 = values[110];
    product.right_io_pins.pin24 = values[111];

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);
}

void PMMWriteIntoFlashGeneralSettings(string Message)
{
    Product product;

    PmmStringToArray(Message);

    // General Info
    PMMGENERALSETTINGS.FirmwareVersion = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    PMMGENERALSETTINGS.HardwareVersion = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    PMMGENERALSETTINGS.HardwareVersion = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    PMMGENERALSETTINGS.SerialNumber = stoi(values[12]);
    PMMGENERALSETTINGS.CPUType = values[13];
    PMMGENERALSETTINGS.ProductFamily = values[14];
    PMMGENERALSETTINGS.EnclosureType = values[15];
    PMMGENERALSETTINGS.ArduinoSupport = (values[16] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.PlatformIOSupport = (values[17] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.OtherSupportName = values[18];

    // General Info
    product.firmware_version = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    product.software_version = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    product.hardware_version = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    product.serial_number = values[12];
    product.cpu_type = values[13];
    product.product_family = values[14];
    product.enclosure_type = values[15];
    product.arduino_support = (values[16] == "1" ? "true" : "false");
    product.platform_io_support = (values[17] == "1" ? "true" : "false");
    product.other_support_name = values[18];

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);
}

void PMMWriteIntoFlashSerialSettings(string Message)
{
   Product product;

    PmmStringToArray(Message);

    // RTU Setting
    PMMSERIALSETTINGS.PortOneName = values[19];
    PMMSERIALSETTINGS.PortOneBaudRate = stoi(values[20]);
    PMMSERIALSETTINGS.PortOneDataBits = stoi(values[21]);
    PMMSERIALSETTINGS.PortOneStopBits = stoi(values[22]);
    PMMSERIALSETTINGS.PortOneParity = values[23];
    PMMSERIALSETTINGS.PortOneInterface = (values[24] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.PortOneConnTimeOut = stoi(values[25]);
    PMMSERIALSETTINGS.PortOneMaxRetries = stoi(values[26]);

    PMMSERIALSETTINGS.SerialOneEnabled = (values[66] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialOneType = values[67];
    PMMSERIALSETTINGS.SerialTowEnabled = (values[68] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialTowType = values[69];
    PMMSERIALSETTINGS.SerialThreeEnabled = (values[69] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialThreeType = values[71];
    PMMSERIALSETTINGS.SerialFourEnabled = (values[70] == "1" ? "true" : "false");
    PMMSERIALSETTINGS.SerialFourType = values[73];

    // RTU Setting
    product.rtu_settings.ComName = values[19];
    product.rtu_settings.baudRate = stoi(values[20]);
    product.rtu_settings.dataBits = stoi(values[21]);
    product.rtu_settings.stopBits = stoi(values[22]);
    product.rtu_settings.parity = stoi(values[23]);
    product.rtu_settings.interface = values[24];
    product.rtu_settings.connection_timeout_rtu = stoi(values[25]);
    product.rtu_settings.max_retry_rtu = stoi(values[26]);

    // Serail Connectors
    product.serial1.enabled = (values[66] == "1" ? "true" : "false");
    product.serial1.type = values[67];
    product.serial2.enabled = (values[68] == "1" ? "true" : "false");
    product.serial2.type = values[69];
    product.serial3.enabled = (values[69] == "1" ? "true" : "false");
    product.serial3.type = values[71];
    product.serial4.enabled = (values[70] == "1" ? "true" : "false");
    product.serial4.type = values[73];

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);
}

void PMMWriteIntoFlashTCPSettings(string Message)
{
   Product product;

    PmmStringToArray(Message);
    // TCP Settings
    PMMTCPUDPSETTINGS.IPAddressEthOne = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    PMMTCPUDPSETTINGS.SubnetMaskEthOne = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    PMMTCPUDPSETTINGS.PreferredDNSServer = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    PMMTCPUDPSETTINGS.AlternateDNSServer = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    PMMTCPUDPSETTINGS.GatewayEthOne = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    PMMTCPUDPSETTINGS.MacAddressEthOne = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    PMMTCPUDPSETTINGS.TimeOutConnEthOne = stoi(values[52]);
    PMMTCPUDPSETTINGS.MaxRetriesEthOne = stoi(values[53]);
    PMMTCPUDPSETTINGS.UDPPortOne = stoi(values[54]);
    PMMTCPUDPSETTINGS.UDPPortTwo = stoi(values[55]);
    PMMTCPUDPSETTINGS.UDPPortThree = stoi(values[56]);
    PMMTCPUDPSETTINGS.UDPPortFour = stoi(values[57]);

    // TCP Settings
    product.tcp_udp_settings.ip_address = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    product.tcp_udp_settings.net_mask = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    product.tcp_udp_settings.preferred_dns_server = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    product.tcp_udp_settings.alternate_dns_server = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    product.tcp_udp_settings.default_gateway = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    product.tcp_udp_settings.mac_address = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    product.tcp_udp_settings.connection_timeout_tcp = stoi(values[52]);
    product.tcp_udp_settings.max_retry_tcp = stoi(values[53]);
    product.tcp_udp_settings.udp_port_one = stoi(values[54]);
    product.tcp_udp_settings.udp_port_two = stoi(values[55]);
    product.tcp_udp_settings.udp_port_three = stoi(values[56]);
    product.tcp_udp_settings.udp_port_four = stoi(values[57]);

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);
}

void PMMWriteIntoFlashOptionsAndPinsSettings(string Message)
{
    Product product;

    PmmStringToArray(Message);
    // Options
    PMMGENERALSETTINGS.HasEthernet = (values[58] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasFiber = (values[59] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasSwitch = (values[60] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasExternalRTC = (values[61] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasInternalRTC = (values[62] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasEEprom = (values[63] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasInternalFlash = (values[64] == "1" ? "true" : "false");
    PMMGENERALSETTINGS.HasExternalFlash = (values[65] == "1" ? "true" : "false");

    // Extension Boards
    PMMEXTENSIONSSETTINGS.I2C = (values[74] == "1" ? "true" : "false");
    PMMEXTENSIONSSETTINGS.SlaveI2CAddress = values[75];
    PMMEXTENSIONSSETTINGS.BoardOne = values[76];
    PMMEXTENSIONSSETTINGS.BoardOneAddress1 = values[77];
    PMMEXTENSIONSSETTINGS.BoardOneAddress2 = values[78];
    PMMEXTENSIONSSETTINGS.BoardTwo = values[79];
    PMMEXTENSIONSSETTINGS.BoardTowAddress1 = values[80];
    PMMEXTENSIONSSETTINGS.BoardTowAddress2 = values[81];
    PMMEXTENSIONSSETTINGS.BoardThree = values[82];
    PMMEXTENSIONSSETTINGS.BoardThreeAddress1 = values[83];
    PMMEXTENSIONSSETTINGS.BoardThreeAddress2 = values[84];
    PMMEXTENSIONSSETTINGS.BoardFour = values[85];
    PMMEXTENSIONSSETTINGS.BoardFourAddress1 = values[86];
    PMMEXTENSIONSSETTINGS.BoardFourAddress2 = values[87];

    // Left Pins
    PMMIOPINS.Pin01 = values[88];
    PMMIOPINS.Pin02 = values[89];
    PMMIOPINS.Pin03 = values[90];
    PMMIOPINS.Pin04 = values[91];
    PMMIOPINS.Pin05 = values[92];
    PMMIOPINS.Pin06 = values[93];
    PMMIOPINS.Pin07 = values[94];
    PMMIOPINS.Pin08 = values[95];
    PMMIOPINS.Pin09 = values[96];
    PMMIOPINS.Pin10 = values[97];
    PMMIOPINS.Pin11 = values[98];
    PMMIOPINS.Pin12 = values[99];

    // Right Pins
    PMMIOPINS.Pin13 = values[100];
    PMMIOPINS.Pin14 = values[101];
    PMMIOPINS.Pin15 = values[102];
    PMMIOPINS.Pin16 = values[103];
    PMMIOPINS.Pin17 = values[104];
    PMMIOPINS.Pin18 = values[105];
    PMMIOPINS.Pin19 = values[106];
    PMMIOPINS.Pin20 = values[107];
    PMMIOPINS.Pin21 = values[108];
    PMMIOPINS.Pin22 = values[109];
    PMMIOPINS.Pin23 = values[110];
    PMMIOPINS.Pin24 = values[111];

    // Options
    product.ethernet = (values[58] == "1" ? "true" : "false");
    product.fiber = (values[59] == "1" ? "true" : "false");
    product.switchh = (values[60] == "1" ? "true" : "false");
    product.rtc_external = (values[61] == "1" ? "true" : "false");
    product.rtc_internal = (values[62] == "1" ? "true" : "false");
    product.eprom = (values[63] == "1" ? "true" : "false");
    product.internal_flash = (values[64] == "1" ? "true" : "false");
    product.external_flash = (values[65] == "1" ? "true" : "false");

    // Extension Boards
    product.i2c = (values[74] == "1" ? "true" : "false");
    product.slave_i2c_address = values[75];
    product.board1 = values[76];
    product.board1_address1 = values[77];
    product.board1_address2 = values[78];
    product.board2 = values[79];
    product.board2_address1 = values[80];
    product.board2_address2 = values[81];
    product.board3 = values[82];
    product.board3_address1 = values[83];
    product.board3_address2 = values[84];
    product.board4 = values[85];
    product.board4_address1 = values[86];
    product.board4_address2 = values[87];

    // Left Pins
    product.left_io_pins.pin1 = values[88];
    product.left_io_pins.pin2 = values[89];
    product.left_io_pins.pin3 = values[90];
    product.left_io_pins.pin4 = values[91];
    product.left_io_pins.pin5 = values[92];
    product.left_io_pins.pin6 = values[93];
    product.left_io_pins.pin7 = values[94];
    product.left_io_pins.pin8 = values[95];
    product.left_io_pins.pin9 = values[96];
    product.left_io_pins.pin10 = values[97];
    product.left_io_pins.pin11 = values[98];
    product.left_io_pins.pin12 = values[99];

    // Right Pins
    product.right_io_pins.pin13 = values[100];
    product.right_io_pins.pin14 = values[101];
    product.right_io_pins.pin15 = values[102];
    product.right_io_pins.pin16 = values[103];
    product.right_io_pins.pin17 = values[104];
    product.right_io_pins.pin18 = values[105];
    product.right_io_pins.pin19 = values[106];
    product.right_io_pins.pin20 = values[107];
    product.right_io_pins.pin21 = values[108];
    product.right_io_pins.pin22 = values[109];
    product.right_io_pins.pin23 = values[110];
    product.right_io_pins.pin24 = values[111];

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);
}

string PMMReadFromFlashAllSettings()
{
    Product product;
    
    //SerialUSB.println("GET 1");
    product = my_flash_store.read();
    //SerialUSB.println("GET 2");
    string settings = product.firmware_version;
    settings = settings + "," + product.software_version;
    settings = settings + "," + product.hardware_version;
    settings = settings + "," + product.serial_number;
    settings = settings + "," + product.cpu_type;
    settings = settings + "," + product.product_family;
    settings = settings + "," + product.enclosure_type;
    settings = settings + "," + std::to_string(product.arduino_support);
    settings = settings + "," + std::to_string(product.platform_io_support);
    settings = settings + "," + product.other_support_name;

    // RTU Setting
    settings = settings + "," + product.rtu_settings.ComName;
    settings = settings + "," + std::to_string(product.rtu_settings.baudRate);
    settings = settings + "," + std::to_string(product.rtu_settings.dataBits);
    settings = settings + "," + std::to_string(product.rtu_settings.stopBits);
    settings = settings + "," + std::to_string(product.rtu_settings.parity);
    settings = settings + "," + product.rtu_settings.interface;
    settings = settings + "," + std::to_string(product.rtu_settings.connection_timeout_rtu);
    settings = settings + "," + std::to_string(product.rtu_settings.max_retry_rtu);

    // TCP Settings
    settings = settings + "," + product.tcp_udp_settings.ip_address;
    settings = settings + "," + product.tcp_udp_settings.net_mask;
    settings = settings + "," + product.tcp_udp_settings.preferred_dns_server;
    settings = settings + "," + product.tcp_udp_settings.alternate_dns_server;
    settings = settings + "," + product.tcp_udp_settings.default_gateway;
    settings = settings + "," + product.tcp_udp_settings.mac_address;
    settings = settings + "," + std::to_string(product.tcp_udp_settings.connection_timeout_tcp);
    settings = settings + "," + std::to_string(product.tcp_udp_settings.max_retry_tcp);
    settings = settings + "," + std::to_string(product.tcp_udp_settings.udp_port_one);
    settings = settings + "," + std::to_string(product.tcp_udp_settings.udp_port_two);
    settings = settings + "," + std::to_string(product.tcp_udp_settings.udp_port_three);
    settings = settings + "," + std::to_string(product.tcp_udp_settings.udp_port_four);

    // Options
    settings = settings + "," + std::to_string(product.ethernet);
    settings = settings + "," + std::to_string(product.fiber);
    settings = settings + "," + std::to_string(product.switchh);
    settings = settings + "," + std::to_string(product.rtc_external);
    settings = settings + "," + std::to_string(product.rtc_internal);
    settings = settings + "," + std::to_string(product.eprom);
    settings = settings + "," + std::to_string(product.internal_flash);
    settings = settings + "," + std::to_string(product.external_flash);

    // Serail Connectors
    settings = settings + "," + std::to_string(product.serial1.enabled);
    settings = settings + "," + product.serial1.type;
    settings = settings + "," + std::to_string(product.serial2.enabled);
    settings = settings + "," + product.serial2.type;
    settings = settings + "," + std::to_string(product.serial3.enabled);
    settings = settings + "," + product.serial3.type;
    settings = settings + "," + std::to_string(product.serial4.enabled);
    settings = settings + "," + product.serial4.type;

    // Extension Boards
    settings = settings + "," + std::to_string(product.i2c);
    settings = settings + "," + product.slave_i2c_address;
    settings = settings + "," + product.board1;
    settings = settings + "," + product.board1_address1;
    settings = settings + "," + product.board1_address2;
    settings = settings + "," + product.board2;
    settings = settings + "," + product.board2_address1;
    settings = settings + "," + product.board2_address2;
    settings = settings + "," + product.board3;
    settings = settings + "," + product.board3_address1;
    settings = settings + "," + product.board3_address2;
    settings = settings + "," + product.board4;
    settings = settings + "," + product.board4_address1;
    settings = settings + "," + product.board4_address2;

    // Left Pins
    settings = settings + "," + product.left_io_pins.pin1;
    settings = settings + "," + product.left_io_pins.pin2;
    settings = settings + "," + product.left_io_pins.pin3;
    settings = settings + "," + product.left_io_pins.pin4;
    settings = settings + "," + product.left_io_pins.pin5;
    settings = settings + "," + product.left_io_pins.pin6;
    settings = settings + "," + product.left_io_pins.pin7;
    settings = settings + "," + product.left_io_pins.pin8;
    settings = settings + "," + product.left_io_pins.pin9;
    settings = settings + "," + product.left_io_pins.pin10;
    settings = settings + "," + product.left_io_pins.pin11;
    settings = settings + "," + product.left_io_pins.pin12;

    // Right Pins
    settings = settings + "," + product.right_io_pins.pin13;
    settings = settings + "," + product.right_io_pins.pin14;
    settings = settings + "," + product.right_io_pins.pin15;
    settings = settings + "," + product.right_io_pins.pin16;
    settings = settings + "," + product.right_io_pins.pin17;
    settings = settings + "," + product.right_io_pins.pin18;
    settings = settings + "," + product.right_io_pins.pin19;
    settings = settings + "," + product.right_io_pins.pin20;
    settings = settings + "," + product.right_io_pins.pin21;
    settings = settings + "," + product.right_io_pins.pin22;
    settings = settings + "," + product.right_io_pins.pin23;
    settings = settings + "," + product.right_io_pins.pin24;

    //SerialUSB.println("GET 3");
    return settings;
}

string PMMIsAlive()
{
    return "PMMAlive";
}