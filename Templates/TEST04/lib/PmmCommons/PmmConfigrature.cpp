#include "PmmConfigrature.h"

struct PmmGeneralSettings PMMGENERALSETTINGS;
struct PmmModBusSettings PMMMODBUSSETTINGS;
struct PmmTCPUDPSettings PMMTCPUDPSETTINGS;
struct PmmSerialSettings PMMSERIALSETTINGS;
struct PmmIOPins PMMIOPINS;
struct PmmExtensionsSettings PMMEXTENSIONSSETTINGS;

char *strings[128]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;
string values[128];

typedef struct tcp_udp_settings
{
    int ip_address01 = 192;
    int ip_address02 = 168;
    int ip_address03 = 1;
    int ip_address04 = 100;
    int port = 502;
    int net_mask01 = 255;
    int net_mask02 = 255;
    int net_mask03 = 255;
    int net_mask04 = 255;
    int default_gateway01 = 192;
    int default_gateway02 = 168;
    int default_gateway03 = 1;
    int default_gateway04 = 50;
    int mac_address01 = 10;
    int mac_address02 = 20;
    int mac_address03 = 30;
    int mac_address04 = 40;
    int mac_address05 = 50;
    int preferred_dns_server01 = 8;
    int preferred_dns_server02 = 8;
    int preferred_dns_server03 = 8;
    int preferred_dns_server04 = 8;
    int alternate_dns_server01 = 8;
    int alternate_dns_server02 = 8;
    int alternate_dns_server03 = 0;
    int alternate_dns_server04 = 0;
    int connection_timeout_tcp = 5000;
    int max_retry_tcp = 5;
    int udp_port_one = 90;
    int udp_port_two = 91;
    int udp_port_three = 92;
    int udp_port_four = 93;
} tcp_udp_settings;

typedef struct serial_connector
{
    int enabled = 1;
    int type = 2;
} serial_connector;

typedef struct rtu_settings
{
    int ComName;
    int baudRate = 9600;
    int dataBits = 8;
    int stopBits = 1;
    int parity = 4;
    int interface = 2;
    int connection_timeout_rtu = 3000;
    int max_retry_rtu = 5;
} rtu_settings;

typedef struct left_io_pins
{
    int pin1 = 1;
    int pin2 = 1;
    int pin3 = 1;
    int pin4 = 1;
    int pin5 = 1;
    int pin6 = 1;
    int pin7 = 1;
    int pin8 = 1;
    int pin9 = 2;
    int pin10 = 3;
    int pin11 = 3;
    int pin12 = 3;
} left_io_pins;

typedef struct right_io_pins
{
    int pin13 = 4;
    int pin14 = 4;
    int pin15 = 4;
    int pin16 = 4;
    int pin17 = 4;
    int pin18 = 4;
    int pin19 = 4;
    int pin20 = 5;
    int pin21 = 5;
    int pin22 = 8;
    int pin23 = 7;
    int pin24 = 6;
} right_io_pins;

typedef struct Product
{
    int firmware_version01 = 1;
    int firmware_version02 = 0;
    int firmware_version03 = 0;
    int firmware_version04 = 1;
    int serial_number = 5000;
    int hardware_version01 = 1;
    int hardware_version02 = 0;
    int hardware_version03 = 0;
    int hardware_version04 = 1;
    int software_version01 = 1;
    int software_version02 = 0;
    int software_version03 = 0;
    int software_version04 = 1;
    int cpu_type = 3;
    int connection_type = 1;
    int ethernet = 1;
    int fiber = 1;
    int switchh = 1;
    int rtc_external = 0;
    int rtc_internal = 1;
    int eprom = 1;
    int internal_flash = 1;
    int external_flash = 0;
    int i2c = 1;
    struct left_io_pins left_io_pins;
    struct right_io_pins right_io_pins;
    struct serial_connector serial1;
    struct serial_connector serial2;
    struct serial_connector serial3;
    struct serial_connector serial4;
    struct tcp_udp_settings tcp_udp_settings;
    struct rtu_settings rtu_settings;
    int product_family = 20;
    int enclosure_type = 100;
    int other_support_name = 10;
    int arduino_support = 20;
    int platform_io_support = 1;
    int other_support = 10;
    int slave_i2c_address = 4001;
    int board1 = 1;
    int board1_address1 = 1;
    int board1_address2 = 101;
    int board2 = 2;
    int board2_address1 = 201;
    int board2_address2 = 301;
    int board3 = 3;
    int board3_address1 = 401;
    int board3_address2 = 501;
    int board4 = 4;
    int board4_address1 = 601;
    int board4_address2 = 701;
    int usb_com = 1;
    int ice_only = 1;
    int settingPanel = 1;

    int EthernetAndUSB = 0;
    int IsUSB = 1;
    int SettingsROM = 100;
} Product;

FlashStorage(my_flash_store, Product);

FlashStorage(my_flash_store2, tcp_udp_settings);

void SetTCPSettings(string Message)
{
    PmmStringToArray(Message);

    tcp_udp_settings tcpudp;
    // TCP Settings

    tcpudp.ip_address01 = stoi(values[0]);
    tcpudp.ip_address02 = stoi(values[1]);
    tcpudp.ip_address03 = stoi(values[2]);
    tcpudp.ip_address04 = stoi(values[3]);

    tcpudp.net_mask01 = stoi(values[4]);
    tcpudp.net_mask02 = stoi(values[5]);
    tcpudp.net_mask03 = stoi(values[6]);
    tcpudp.net_mask04 = stoi(values[7]);

    tcpudp.preferred_dns_server01 = stoi(values[8]);
    tcpudp.preferred_dns_server02 = stoi(values[9]);
    tcpudp.preferred_dns_server03 = stoi(values[10]);
    tcpudp.preferred_dns_server04 = stoi(values[11]);

    tcpudp.alternate_dns_server01 = stoi(values[12]);
    tcpudp.alternate_dns_server02 = stoi(values[13]);
    tcpudp.alternate_dns_server03 = stoi(values[14]);
    tcpudp.alternate_dns_server04 = stoi(values[15]);

    tcpudp.default_gateway01 = stoi(values[16]);
    tcpudp.default_gateway02 = stoi(values[17]);
    tcpudp.default_gateway03 = stoi(values[18]);
    tcpudp.default_gateway04 = stoi(values[20]);

    tcpudp.mac_address01 = stoi(values[21]);
    tcpudp.mac_address02 = stoi(values[22]);
    tcpudp.mac_address03 = stoi(values[23]);
    tcpudp.mac_address04 = stoi(values[24]);
    tcpudp.mac_address05 = stoi(values[25]);

    tcpudp.connection_timeout_tcp = stoi(values[26]);
    tcpudp.max_retry_tcp = stoi(values[27]);
    tcpudp.udp_port_one = stoi(values[28]);
    tcpudp.udp_port_two = stoi(values[29]);
    tcpudp.udp_port_three = stoi(values[30]);
    tcpudp.udp_port_four = stoi(values[31]);

    // // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(tcpudp);
    my_flash_store2.write (tcpudp);
}

String GetTCPSettings()
{
    tcp_udp_settings tcpudp;
    // tcpudp = my_flash_store.read();

    String settings = "";

    settings = String(settings + String(tcpudp.ip_address01));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.ip_address02));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.ip_address03));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.ip_address04));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.net_mask01));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.net_mask02));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.net_mask03));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.net_mask04));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.preferred_dns_server01));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.preferred_dns_server02));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.preferred_dns_server03));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.preferred_dns_server04));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.default_gateway01));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.default_gateway02));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.default_gateway03));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.default_gateway04));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.mac_address01));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.mac_address02));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.mac_address03));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.mac_address04));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.mac_address05));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.connection_timeout_tcp));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.max_retry_tcp));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.udp_port_one));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.udp_port_two));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.udp_port_three));
    settings = String(settings + ",");
    settings = String(settings + String(tcpudp.udp_port_four));

    // String settings = "";
    // settings = "1919";
    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.ip_address);

    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.net_mask);
    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.preferred_dns_server);
    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.alternate_dns_server);
    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.default_gateway);
    // settings = String(settings + ",");
    // settings = String(settings + tcpudp.mac_address);
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.connection_timeout_tcp));
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.max_retry_tcp));
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.udp_port_one));
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.udp_port_two));
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.udp_port_three));
    // settings = String(settings + ",");
    // settings = String(settings + String(tcpudp.udp_port_four));

    SerialUSB.println(settings);
    return settings;
}

void PmmStringToArray(string input)
{

    // int length = input.length();

    // declaring character array (+1 for null terminator)
    char *char_array = new char[128];

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

    SerialUSB.println(index);

    for (int n = 0; n < (index); n++)
    {
        string s(strings[n]);
        values[n] = s;

        SerialUSB.print(n);
        SerialUSB.print(" : ");
        SerialUSB.println(s.c_str());
    }
}

void PMMWriteIntoFlashAllSettings(string Message)
{
    // PmmStringToArray(Message);
    SerialUSB.println(Message.c_str());
    Product product;

#pragma region
    // //  General Info
    // PMMGENERALSETTINGS.FirmwareVersion = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    // PMMGENERALSETTINGS.HardwareVersion = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    // PMMGENERALSETTINGS.HardwareVersion = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    // PMMGENERALSETTINGS.SerialNumber = stoi(values[12]);
    // PMMGENERALSETTINGS.CPUType = values[13];
    // PMMGENERALSETTINGS.ProductFamily = values[14];
    // PMMGENERALSETTINGS.EnclosureType = values[15];
    // PMMGENERALSETTINGS.ArduinoSupport = (values[16] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.PlatformIOSupport = (values[17] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.OtherSupportName = values[18];

    // // RTU Setting
    // PMMSERIALSETTINGS.PortOneName = values[19];
    // PMMSERIALSETTINGS.PortOneBaudRate = stoi(values[20]);
    // PMMSERIALSETTINGS.PortOneDataBits = stoi(values[21]);
    // PMMSERIALSETTINGS.PortOneStopBits = stoi(values[22]);
    // PMMSERIALSETTINGS.PortOneParity = values[23];
    // PMMSERIALSETTINGS.PortOneInterface = (values[24] == "1" ? "true" : "false");
    // PMMSERIALSETTINGS.PortOneConnTimeOut = stoi(values[25]);
    // PMMSERIALSETTINGS.PortOneMaxRetries = stoi(values[26]);

    // // TCP Settings
    // PMMTCPUDPSETTINGS.IPAddressEthOne = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    // PMMTCPUDPSETTINGS.SubnetMaskEthOne = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    // PMMTCPUDPSETTINGS.PreferredDNSServer = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    // PMMTCPUDPSETTINGS.AlternateDNSServer = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    // PMMTCPUDPSETTINGS.GatewayEthOne = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    // PMMTCPUDPSETTINGS.MacAddressEthOne = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    // PMMTCPUDPSETTINGS.TimeOutConnEthOne = stoi(values[52]);
    // PMMTCPUDPSETTINGS.MaxRetriesEthOne = stoi(values[53]);
    // PMMTCPUDPSETTINGS.UDPPortOne = stoi(values[54]);
    // PMMTCPUDPSETTINGS.UDPPortTwo = stoi(values[55]);
    // PMMTCPUDPSETTINGS.UDPPortThree = stoi(values[56]);
    // PMMTCPUDPSETTINGS.UDPPortFour = stoi(values[57]);

    // // Options
    // PMMGENERALSETTINGS.HasEthernet = (values[58] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasFiber = (values[59] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasSwitch = (values[60] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasExternalRTC = (values[61] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasInternalRTC = (values[62] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasEEprom = (values[63] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasInternalFlash = (values[64] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasExternalFlash = (values[65] == "1" ? "true" : "false");

    // PMMSERIALSETTINGS.SerialOneEnabled = (values[66] == "1" ? "true" : "false");
    // PMMSERIALSETTINGS.SerialOneType = values[67];
    // PMMSERIALSETTINGS.SerialTowEnabled = (values[68] == "1" ? "true" : "false");
    // PMMSERIALSETTINGS.SerialTowType = values[69];
    // PMMSERIALSETTINGS.SerialThreeEnabled = (values[69] == "1" ? "true" : "false");
    // PMMSERIALSETTINGS.SerialThreeType = values[71];
    // PMMSERIALSETTINGS.SerialFourEnabled = (values[70] == "1" ? "true" : "false");
    // PMMSERIALSETTINGS.SerialFourType = values[73];

    // // Extension Boards
    // PMMEXTENSIONSSETTINGS.I2C = (values[74] == "1" ? "true" : "false");
    // PMMEXTENSIONSSETTINGS.SlaveI2CAddress = values[75];
    // PMMEXTENSIONSSETTINGS.BoardOne = values[76];
    // PMMEXTENSIONSSETTINGS.BoardOneAddress1 = values[77];
    // PMMEXTENSIONSSETTINGS.BoardOneAddress2 = values[78];
    // PMMEXTENSIONSSETTINGS.BoardTwo = values[79];
    // PMMEXTENSIONSSETTINGS.BoardTowAddress1 = values[80];
    // PMMEXTENSIONSSETTINGS.BoardTowAddress2 = values[81];
    // PMMEXTENSIONSSETTINGS.BoardThree = values[82];
    // PMMEXTENSIONSSETTINGS.BoardThreeAddress1 = values[83];
    // PMMEXTENSIONSSETTINGS.BoardThreeAddress2 = values[84];
    // PMMEXTENSIONSSETTINGS.BoardFour = values[85];
    // PMMEXTENSIONSSETTINGS.BoardFourAddress1 = values[86];
    // PMMEXTENSIONSSETTINGS.BoardFourAddress2 = values[87];

    // // Left Pins
    // PMMIOPINS.Pin01 = values[88];
    // PMMIOPINS.Pin02 = values[89];
    // PMMIOPINS.Pin03 = values[90];
    // PMMIOPINS.Pin04 = values[91];
    // PMMIOPINS.Pin05 = values[92];
    // PMMIOPINS.Pin06 = values[93];
    // PMMIOPINS.Pin07 = values[94];
    // PMMIOPINS.Pin08 = values[95];
    // PMMIOPINS.Pin09 = values[96];
    // PMMIOPINS.Pin10 = values[97];
    // PMMIOPINS.Pin11 = values[98];
    // PMMIOPINS.Pin12 = values[99];

    // // Right Pins
    // PMMIOPINS.Pin13 = values[100];
    // PMMIOPINS.Pin14 = values[101];
    // PMMIOPINS.Pin15 = values[102];
    // PMMIOPINS.Pin16 = values[103];
    // PMMIOPINS.Pin17 = values[104];
    // PMMIOPINS.Pin18 = values[105];
    // PMMIOPINS.Pin19 = values[106];
    // PMMIOPINS.Pin20 = values[107];
    // PMMIOPINS.Pin21 = values[108];
    // PMMIOPINS.Pin22 = values[109];
    // PMMIOPINS.Pin23 = values[110];
    // PMMIOPINS.Pin24 = values[111];
#pragma endregion

    // General Info
    // product.firmware_version01 = stoi(values[0]);
    // product.firmware_version02 = stoi(values[1]);
    // product.firmware_version03 = stoi(values[2]);
    // product.firmware_version04 = stoi(values[3]);
    // product.software_version01 = stoi(values[4]);
    // product.software_version02 = stoi(values[5]);
    // product.software_version03 = stoi(values[6]);
    // product.software_version04 = stoi(values[7]);
    // product.hardware_version01 = stoi(values[8]);
    // product.hardware_version02 = stoi(values[9]);
    // product.hardware_version03 = stoi(values[10]);
    // product.hardware_version04 = stoi(values[11]);
    // product.serial_number = stoi(values[12]);
    // product.cpu_type = stoi(values[13]);
    // product.product_family = stoi(values[14]);
    // product.enclosure_type = stoi(values[15]);
    // product.arduino_support = stoi(values[16]);
    // product.platform_io_support = stoi(values[17]);
    // product.other_support_name = stoi(values[18]);

    // // RTU Setting
    // product.rtu_settings.ComName = stoi(values[19]);
    // product.rtu_settings.baudRate = stoi(values[20]);
    // product.rtu_settings.dataBits = stoi(values[21]);
    // product.rtu_settings.stopBits = stoi(values[22]);
    // product.rtu_settings.parity = stoi(values[23]);
    // product.rtu_settings.interface = stoi(values[24]);
    // product.rtu_settings.connection_timeout_rtu = stoi(values[25]);
    // product.rtu_settings.max_retry_rtu = stoi(values[26]);

    // // TCP Settings
    // product.tcp_udp_settings.ip_address01 = stoi(values[27]);
    // product.tcp_udp_settings.ip_address02 = stoi(values[28]);
    // product.tcp_udp_settings.ip_address03 = stoi(values[29]);
    // product.tcp_udp_settings.ip_address04 = stoi(values[30]);

    // product.tcp_udp_settings.net_mask01 = stoi(values[31]);
    // product.tcp_udp_settings.net_mask02 = stoi(values[32]);
    // product.tcp_udp_settings.net_mask03 = stoi(values[33]);
    // product.tcp_udp_settings.net_mask04 = stoi(values[34]);

    // product.tcp_udp_settings.preferred_dns_server01 = stoi(values[35]);
    // product.tcp_udp_settings.preferred_dns_server02 = stoi(values[36]);
    // product.tcp_udp_settings.preferred_dns_server03 = stoi(values[37]);
    // product.tcp_udp_settings.preferred_dns_server04 = stoi(values[38]);

    // product.tcp_udp_settings.alternate_dns_server01 = stoi(values[39]);
    // product.tcp_udp_settings.alternate_dns_server02 = stoi(values[40]);
    // product.tcp_udp_settings.alternate_dns_server03 = stoi(values[41]);
    // product.tcp_udp_settings.alternate_dns_server04 = stoi(values[42]);

    // product.tcp_udp_settings.default_gateway01 = stoi(values[43]);
    // product.tcp_udp_settings.default_gateway02 = stoi(values[44]);
    // product.tcp_udp_settings.default_gateway03 = stoi(values[45]);
    // product.tcp_udp_settings.default_gateway04 = stoi(values[46]);

    // product.tcp_udp_settings.mac_address01 = stoi(values[47]);
    // product.tcp_udp_settings.mac_address02 = stoi(values[48]);
    // product.tcp_udp_settings.mac_address03 = stoi(values[49]);
    // product.tcp_udp_settings.mac_address04 = stoi(values[50]);
    // product.tcp_udp_settings.mac_address05 = stoi(values[51]);

    // product.tcp_udp_settings.connection_timeout_tcp = stoi(values[52]);
    // product.tcp_udp_settings.max_retry_tcp = stoi(values[53]);
    // product.tcp_udp_settings.udp_port_one = stoi(values[54]);
    // product.tcp_udp_settings.udp_port_two = stoi(values[55]);
    // product.tcp_udp_settings.udp_port_three = stoi(values[56]);
    // product.tcp_udp_settings.udp_port_four = stoi(values[57]);

    // // Options
    // product.ethernet = stoi(values[58]);
    // product.fiber = stoi(values[59]);
    // product.switchh = stoi(values[60]);
    // product.rtc_external = stoi(values[61]);
    // product.rtc_internal = stoi(values[62]);
    // product.eprom = stoi(values[63]);
    // product.internal_flash = stoi(values[64]);
    // product.external_flash = stoi(values[65]);

    // // Serail Connectors
    // product.serial1.enabled = stoi(values[66]);
    // product.serial1.type = stoi(values[67]);
    // product.serial2.enabled = stoi(values[68]);
    // product.serial2.type = stoi(values[69]);
    // product.serial3.enabled = stoi(values[69]);
    // product.serial3.type = stoi(values[71]);
    // product.serial4.enabled = stoi(values[70]);
    // product.serial4.type = stoi(values[73]);

    // // Extension Boards
    // product.i2c = stoi(values[74]);
    // product.slave_i2c_address = stoi(values[75]);
    // product.board1 = stoi(values[76]);
    // product.board1_address1 = stoi(values[77]);
    // product.board1_address2 = stoi(values[78]);
    // product.board2 = stoi(values[79]);
    // product.board2_address1 = stoi(values[80]);
    // product.board2_address2 = stoi(values[81]);
    // product.board3 = stoi(values[82]);
    // product.board3_address1 = stoi(values[83]);
    // product.board3_address2 = stoi(values[84]);
    // product.board4 = stoi(values[85]);
    // product.board4_address1 = stoi(values[86]);
    // product.board4_address2 = stoi(values[87]);

    // // Left Pins
    // product.left_io_pins.pin1 = stoi(values[88]);
    // product.left_io_pins.pin2 = stoi(values[89]);
    // product.left_io_pins.pin3 = stoi(values[90]);
    // product.left_io_pins.pin4 = stoi(values[91]);
    // product.left_io_pins.pin5 = stoi(values[92]);
    // product.left_io_pins.pin6 = stoi(values[93]);
    // product.left_io_pins.pin7 = stoi(values[94]);
    // product.left_io_pins.pin8 = stoi(values[95]);
    // product.left_io_pins.pin9 = stoi(values[96]);
    // product.left_io_pins.pin10 = stoi(values[97]);
    // product.left_io_pins.pin11 = stoi(values[98]);
    // product.left_io_pins.pin12 = stoi(values[99]);

    // // Right Pins
    // product.right_io_pins.pin13 = stoi(values[100]);
    // product.right_io_pins.pin14 = stoi(values[101]);
    // product.right_io_pins.pin15 = stoi(values[102]);
    // product.right_io_pins.pin16 = stoi(values[103]);
    // product.right_io_pins.pin17 = stoi(values[104]);
    // product.right_io_pins.pin18 = stoi(values[105]);
    // product.right_io_pins.pin19 = stoi(values[106]);
    // product.right_io_pins.pin20 = stoi(values[107]);
    // product.right_io_pins.pin21 = stoi(values[108]);
    // product.right_io_pins.pin22 = stoi(values[109]);
    // product.right_io_pins.pin23 = stoi(values[110]);
    // product.right_io_pins.pin24 = stoi(values[111]);

    // product.usb_com = stoi(values[112]);
    // product.ice_only = stoi(values[113]);
    // product.settingPanel = stoi(values[114]);
    // product.EthernetAndUSB = stoi(values[115]);
    // product.IsUSB = stoi(values[116]);
    // product.SettingsROM = stoi(values[117]);

    // ...and finally save everything into "my_flash_store"
    my_flash_store.write(product);

    SerialUSB.println("Done");
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

    // // General Info
    // product.firmware_version = values[0] + "." + values[1] + "." + values[2] + "." + values[3];
    // product.software_version = values[4] + "." + values[5] + "." + values[6] + "." + values[7];
    // product.hardware_version = values[8] + "." + values[9] + "." + values[10] + "." + values[11];
    // product.serial_number = values[12];
    // product.cpu_type = values[13];
    // product.product_family = values[14];
    // product.enclosure_type = values[15];
    // product.arduino_support = (values[16] == "1" ? "true" : "false");
    // product.platform_io_support = (values[17] == "1" ? "true" : "false");
    // product.other_support_name = values[18];

    // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(product);
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

    // // RTU Setting
    // product.rtu_settings.ComName = values[19];
    // product.rtu_settings.baudRate = stoi(values[20]);
    // product.rtu_settings.dataBits = stoi(values[21]);
    // product.rtu_settings.stopBits = stoi(values[22]);
    // product.rtu_settings.parity = stoi(values[23]);
    // product.rtu_settings.interface = values[24];
    // product.rtu_settings.connection_timeout_rtu = stoi(values[25]);
    // product.rtu_settings.max_retry_rtu = stoi(values[26]);

    // // Serail Connectors
    // product.serial1.enabled = (values[66] == "1" ? "true" : "false");
    // product.serial1.type = values[67];
    // product.serial2.enabled = (values[68] == "1" ? "true" : "false");
    // product.serial2.type = values[69];
    // product.serial3.enabled = (values[69] == "1" ? "true" : "false");
    // product.serial3.type = values[71];
    // product.serial4.enabled = (values[70] == "1" ? "true" : "false");
    // product.serial4.type = values[73];

    // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(product);
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
    // product.tcp_udp_settings.ip_address = values[27] + "." + values[28] + "." + values[29] + "." + values[30];
    // product.tcp_udp_settings.net_mask = values[31] + "." + values[32] + "." + values[33] + "." + values[34];
    // product.tcp_udp_settings.preferred_dns_server = values[35] + "." + values[36] + "." + values[37] + "." + values[38];
    // product.tcp_udp_settings.alternate_dns_server = values[39] + "." + values[40] + "." + values[41] + "." + values[42];
    // product.tcp_udp_settings.default_gateway = values[43] + "." + values[44] + "." + values[45] + "." + values[46];
    // product.tcp_udp_settings.mac_address = values[47] + ":" + values[48] + ":" + values[49] + ":" + values[50] + ":" + values[51];
    // product.tcp_udp_settings.connection_timeout_tcp = stoi(values[52]);
    // product.tcp_udp_settings.max_retry_tcp = stoi(values[53]);
    // product.tcp_udp_settings.udp_port_one = stoi(values[54]);
    // product.tcp_udp_settings.udp_port_two = stoi(values[55]);
    // product.tcp_udp_settings.udp_port_three = stoi(values[56]);
    // product.tcp_udp_settings.udp_port_four = stoi(values[57]);

    // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(product);
}

void PMMWriteIntoFlashOptionsAndPinsSettings(string Message)
{
    // Product product;

    // PmmStringToArray(Message);
    // // Options
    // PMMGENERALSETTINGS.HasEthernet = (values[58] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasFiber = (values[59] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasSwitch = (values[60] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasExternalRTC = (values[61] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasInternalRTC = (values[62] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasEEprom = (values[63] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasInternalFlash = (values[64] == "1" ? "true" : "false");
    // PMMGENERALSETTINGS.HasExternalFlash = (values[65] == "1" ? "true" : "false");

    // // Extension Boards
    // PMMEXTENSIONSSETTINGS.I2C = (values[74] == "1" ? "true" : "false");
    // PMMEXTENSIONSSETTINGS.SlaveI2CAddress = values[75];
    // PMMEXTENSIONSSETTINGS.BoardOne = values[76];
    // PMMEXTENSIONSSETTINGS.BoardOneAddress1 = values[77];
    // PMMEXTENSIONSSETTINGS.BoardOneAddress2 = values[78];
    // PMMEXTENSIONSSETTINGS.BoardTwo = values[79];
    // PMMEXTENSIONSSETTINGS.BoardTowAddress1 = values[80];
    // PMMEXTENSIONSSETTINGS.BoardTowAddress2 = values[81];
    // PMMEXTENSIONSSETTINGS.BoardThree = values[82];
    // PMMEXTENSIONSSETTINGS.BoardThreeAddress1 = values[83];
    // PMMEXTENSIONSSETTINGS.BoardThreeAddress2 = values[84];
    // PMMEXTENSIONSSETTINGS.BoardFour = values[85];
    // PMMEXTENSIONSSETTINGS.BoardFourAddress1 = values[86];
    // PMMEXTENSIONSSETTINGS.BoardFourAddress2 = values[87];

    // // Left Pins
    // PMMIOPINS.Pin01 = values[88];
    // PMMIOPINS.Pin02 = values[89];
    // PMMIOPINS.Pin03 = values[90];
    // PMMIOPINS.Pin04 = values[91];
    // PMMIOPINS.Pin05 = values[92];
    // PMMIOPINS.Pin06 = values[93];
    // PMMIOPINS.Pin07 = values[94];
    // PMMIOPINS.Pin08 = values[95];
    // PMMIOPINS.Pin09 = values[96];
    // PMMIOPINS.Pin10 = values[97];
    // PMMIOPINS.Pin11 = values[98];
    // PMMIOPINS.Pin12 = values[99];

    // // Right Pins
    // PMMIOPINS.Pin13 = values[100];
    // PMMIOPINS.Pin14 = values[101];
    // PMMIOPINS.Pin15 = values[102];
    // PMMIOPINS.Pin16 = values[103];
    // PMMIOPINS.Pin17 = values[104];
    // PMMIOPINS.Pin18 = values[105];
    // PMMIOPINS.Pin19 = values[106];
    // PMMIOPINS.Pin20 = values[107];
    // PMMIOPINS.Pin21 = values[108];
    // PMMIOPINS.Pin22 = values[109];
    // PMMIOPINS.Pin23 = values[110];
    // PMMIOPINS.Pin24 = values[111];

    // // Options
    // product.ethernet = (values[58] == "1" ? "true" : "false");
    // product.fiber = (values[59] == "1" ? "true" : "false");
    // product.switchh = (values[60] == "1" ? "true" : "false");
    // product.rtc_external = (values[61] == "1" ? "true" : "false");
    // product.rtc_internal = (values[62] == "1" ? "true" : "false");
    // product.eprom = (values[63] == "1" ? "true" : "false");
    // product.internal_flash = (values[64] == "1" ? "true" : "false");
    // product.external_flash = (values[65] == "1" ? "true" : "false");

    // // Extension Boards
    // product.i2c = (values[74] == "1" ? "true" : "false");
    // product.slave_i2c_address = values[75];
    // product.board1 = values[76];
    // product.board1_address1 = values[77];
    // product.board1_address2 = values[78];
    // product.board2 = values[79];
    // product.board2_address1 = values[80];
    // product.board2_address2 = values[81];
    // product.board3 = values[82];
    // product.board3_address1 = values[83];
    // product.board3_address2 = values[84];
    // product.board4 = values[85];
    // product.board4_address1 = values[86];
    // product.board4_address2 = values[87];

    // // Left Pins
    // product.left_io_pins.pin1 = values[88];
    // product.left_io_pins.pin2 = values[89];
    // product.left_io_pins.pin3 = values[90];
    // product.left_io_pins.pin4 = values[91];
    // product.left_io_pins.pin5 = values[92];
    // product.left_io_pins.pin6 = values[93];
    // product.left_io_pins.pin7 = values[94];
    // product.left_io_pins.pin8 = values[95];
    // product.left_io_pins.pin9 = values[96];
    // product.left_io_pins.pin10 = values[97];
    // product.left_io_pins.pin11 = values[98];
    // product.left_io_pins.pin12 = values[99];

    // // Right Pins
    // product.right_io_pins.pin13 = values[100];
    // product.right_io_pins.pin14 = values[101];
    // product.right_io_pins.pin15 = values[102];
    // product.right_io_pins.pin16 = values[103];
    // product.right_io_pins.pin17 = values[104];
    // product.right_io_pins.pin18 = values[105];
    // product.right_io_pins.pin19 = values[106];
    // product.right_io_pins.pin20 = values[107];
    // product.right_io_pins.pin21 = values[108];
    // product.right_io_pins.pin22 = values[109];
    // product.right_io_pins.pin23 = values[110];
    // product.right_io_pins.pin24 = values[111];

    // ...and finally save everything into "my_flash_store"
    // my_flash_store.write(product);
}

String PMMReadFromFlashAllSettings()
{
    SerialUSB.println("PMM");
    Product product;
    product = my_flash_store.read();

    String settings = "";

    settings = String(settings + String(product.firmware_version01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.firmware_version01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.firmware_version02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.firmware_version03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.firmware_version04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.software_version01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.software_version02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.software_version03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.software_version04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.hardware_version01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.hardware_version02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.hardware_version03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.hardware_version04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial_number));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.cpu_type));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.product_family));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.enclosure_type));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.arduino_support));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.platform_io_support));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.other_support_name));
    // settings = String(settings + ",");
    // // RTU Setting
    // settings = String(settings + String(product.rtu_settings.ComName));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.baudRate));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.dataBits));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.stopBits));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.parity));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.interface));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.connection_timeout_rtu));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtu_settings.max_retry_rtu));
    // settings = String(settings + ",");
    // TCP Settings
    // settings = String(settings + String(product.tcp_udp_settings.ip_address01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.ip_address02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.ip_address03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.ip_address04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.net_mask01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.net_mask02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.net_mask03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.net_mask04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.preferred_dns_server01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.preferred_dns_server02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.preferred_dns_server03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.preferred_dns_server04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.alternate_dns_server01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.alternate_dns_server02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.alternate_dns_server03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.alternate_dns_server04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.default_gateway01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.default_gateway02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.default_gateway03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.default_gateway04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.mac_address01));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.mac_address02));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.mac_address03));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.mac_address04));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.mac_address05));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.connection_timeout_tcp));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.max_retry_tcp));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.udp_port_one));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.udp_port_two));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.udp_port_three));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.tcp_udp_settings.udp_port_four));
    // settings = String(settings + ",");
    // // Options
    // settings = String(settings + String(product.ethernet));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.fiber));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.switchh));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtc_external));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.rtc_internal));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.eprom));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.internal_flash));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.external_flash));
    // settings = String(settings + ",");
    // // Serail Connectors
    // settings = String(settings + String(product.serial1.enabled));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial1.type));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial2.enabled));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial2.type));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial3.enabled));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial3.type));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial4.enabled));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.serial4.type));
    // settings = String(settings + ",");
    // // Extension Boards
    // settings = String(settings + String(product.i2c));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.slave_i2c_address));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board1_address1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board1_address2));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board2));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board2_address1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board2_address2));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board3));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board3_address1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board3_address2));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board4));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board4_address1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.board4_address2));
    // settings = String(settings + ",");

    // // Left Pins
    // settings = String(settings + String(product.left_io_pins.pin1));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin2));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin3));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin4));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin5));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin6));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin7));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin8));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin9));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin10));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin11));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.left_io_pins.pin12));
    // settings = String(settings + ",");

    // // Right Pins
    // settings = String(settings + String(product.right_io_pins.pin13));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin14));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin15));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin16));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin17));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin18));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin19));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin20));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin21));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin22));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin23));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.right_io_pins.pin24));
    // settings = String(settings + ",");

    // settings = String(settings + String(product.usb_com));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.ice_only));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.settingPanel));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.EthernetAndUSB));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.IsUSB));
    // settings = String(settings + ",");
    // settings = String(settings + String(product.SettingsROM));
    // settings = String(settings + ",END");

    SerialUSB.println(settings);
    SerialUSB.println("ISRAR");
    return settings;
}

string PMMIsAlive()
{
    return "PMMAlive";
}
