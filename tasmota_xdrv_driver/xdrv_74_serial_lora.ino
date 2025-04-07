#ifdef USE_LORA_UART
#define XDRV_104 104 // Định danh driver trong hệ thống Tasmota

#include "LoRa_E32.h"
#include "HardwareSerial.h"

//#define Lora_Uart_E32_433_RX 7 // RX của ESP32 (gắn với TX của Lora_Uart)
//#define Lora_Uart_E32_433_TX 6 // TX của ESP32 (gắn với RX của Lora_Uart)
HardwareSerial *Lora_UartSerial = nullptr;
LoRa_E32 *Lora_Uart = nullptr;

bool Lora_Uart_e32 = false;
struct Pkg
{
    uint8_t id;
    char payload[64];
};
const char Lora_UartE32Commands[] PROGMEM = "|" // No Prefix
                                       "SendLora_Uart|"
                                       "e32test|"
                                       "e32testset";
void (*const Lora_UartE32Command[])(void) PROGMEM = {
    &CmdSendLora_Uart,
    &e32testCommand, &e32testsetCommand};

void CmdSendLora_Uart(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("Nothing to transmit"));
        ResponseCmndDone();
        return;
    }
    char *tran = XdrvMailbox.data;
    AddLog(LOG_LEVEL_INFO, PSTR("Transmit data: %s"), tran);
    ResponseStatus rs = Lora_Uart->sendMessage(tran);
    AddLog(LOG_LEVEL_INFO, rs.getResponseDescription().c_str());
    ResponseCmndDone();
}
void e32testCommand(void)
{
    Lora_UartE32PrintInfomation();
    ResponseCmndDone();
}
void e32testsetCommand(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Custom1 Command Executed!"));
    ResponseCmndDone();
}
void Lora_UartE32Config(uint8_t channel = 20, uint8_t addrHigh = 0x01, uint8_t addrLow = 0x02, uint8_t baudRate = 3, uint8_t fixedTransmission = 0)
{
    Configuration configuration;
    configuration.ADDH = addrHigh;                              // Địa chỉ cao
    configuration.ADDL = addrLow;                               // Địa chỉ thấp
    configuration.CHAN = channel;                               // Kênh truyền
    configuration.SPED.uartParity = 0;                          // 8N1
    configuration.SPED.uartBaudRate = baudRate;                 // Tốc độ baud UART
    configuration.SPED.airDataRate = 2;                         // Tốc độ truyền không khí mặc định
    configuration.OPTION.fixedTransmission = fixedTransmission; // Chế độ Transparent
    configuration.OPTION.ioDriveMode = 1;
    configuration.OPTION.wirelessWakeupTime = 3; // Wakeup Time mặc định
    configuration.OPTION.fec = 1;
    configuration.OPTION.transmissionPower = 0;
    Lora_Uart->setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
}
void printParameters(struct Configuration configuration)
{
    AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));

    AddLog(LOG_LEVEL_INFO, PSTR("HEAD BIN: %d %d %X"), configuration.HEAD, configuration.HEAD, configuration.HEAD);
    AddLog(LOG_LEVEL_INFO, PSTR("AddH BIN: %d"), configuration.ADDH);
    AddLog(LOG_LEVEL_INFO, PSTR("AddL BIN: %d"), configuration.ADDL);
    AddLog(LOG_LEVEL_INFO, PSTR("Chan BIN: %d -> %s"), configuration.CHAN, configuration.getChannelDescription().c_str());

    AddLog(LOG_LEVEL_INFO, PSTR("SpeedParityBit BIN    : %d -> %s"),
           configuration.SPED.uartParity, configuration.SPED.getUARTParityDescription().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("SpeedUARTDataRate BIN : %d -> %s"),
           configuration.SPED.uartBaudRate, configuration.SPED.getUARTBaudRate().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("SpeedAirDataRate BIN  : %d -> %s"),
           configuration.SPED.airDataRate, configuration.SPED.getAirDataRate().c_str());

    AddLog(LOG_LEVEL_INFO, PSTR("OptionTrans BIN       : %d -> %s"),
           configuration.OPTION.fixedTransmission, configuration.OPTION.getFixedTransmissionDescription().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("OptionPullup BIN      : %d -> %s"),
           configuration.OPTION.ioDriveMode, configuration.OPTION.getIODroveModeDescription().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("OptionWakeup BIN      : %d -> %s"),
           configuration.OPTION.wirelessWakeupTime, configuration.OPTION.getWirelessWakeUPTimeDescription().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("OptionFEC BIN         : %d -> %s"),
           configuration.OPTION.fec, configuration.OPTION.getFECDescription().c_str());
    AddLog(LOG_LEVEL_INFO, PSTR("OptionPower BIN       : %d -> %s"),
           configuration.OPTION.transmissionPower, configuration.OPTION.getTransmissionPowerDescription().c_str());

    AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));
}

void printModuleInformation(struct ModuleInformation moduleInformation)
{
    AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));

    AddLog(LOG_LEVEL_INFO, PSTR("HEAD BIN: %d %d %X"), moduleInformation.HEAD, moduleInformation.HEAD, moduleInformation.HEAD);
    AddLog(LOG_LEVEL_INFO, PSTR("Freq.: %X"), moduleInformation.frequency);
    AddLog(LOG_LEVEL_INFO, PSTR("Version  : %X"), moduleInformation.version);
    AddLog(LOG_LEVEL_INFO, PSTR("Features : %X"), moduleInformation.features);

    AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));
}
void Lora_UartE32Init()
{
    if (!PinUsed(GPIO_LORA_RX) || !PinUsed(GPIO_LORA_TX))
        return;

#if CONFIG_IDF_TARGET_ESP32S3
    pinMode(Pin(GPIO_LORA_RX), OUTPUT);
    digitalWrite(Pin(GPIO_LORA_RX), HIGH);
    sleep(1);
#endif // CONFIG_IDF_TARGET_ESP32S3
    Lora_UartSerial = new HardwareSerial(1);
    Lora_Uart = new LoRa_E32(Pin(GPIO_LORA_TX), Pin(GPIO_LORA_RX), Lora_UartSerial, UART_BPS_RATE_9600, SERIAL_8N1);
    if (!Lora_Uart)
        return;
    Lora_Uart_e32 = Lora_Uart->begin();
    if (Lora_Uart_e32)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("Lora_Uart E32 Initialized successfully"));
    }
    else
        AddLog(LOG_LEVEL_INFO, PSTR("Lora_Uart E32 Initialized failed"));
    Lora_UartE32Config();
}
void Lora_UartE32Processing()
{
    if (!Lora_Uart_e32)
        return;

    // Kiểm tra có tin nhắn từ Lora_Uart
    if (Lora_Uart->available() > 1)
    {
        ResponseContainer rc = Lora_Uart->receiveMessageUntil('!');
        if (rc.status.code == 1)
        {
            AddLog(LOG_LEVEL_INFO, PSTR("Receive Mess: "));
            AddLog(LOG_LEVEL_INFO, rc.data.c_str());
        }
        else
        {
            AddLog(LOG_LEVEL_INFO, PSTR("ERROR!"));
        }
    }
    // if(Lora_Uart->available() > 1){
    //   ResponseStructContainer rc = Lora_Uart->receiveMessage(sizeof(Pkg));
    //   if (rc.status.code == 1)
    //     {
    //       Pkg pkg;
    //       memcpy(&pkg, rc.data, sizeof(Pkg));
    //       rc.close();
    //       AddLog(LOG_LEVEL_INFO, PSTR("ID: %u, Payload: %s"), pkg.id, pkg.payload);
    //       // AddLog(LOG_LEVEL_INFO, rc.data.c_str());
    //     }
    //     else
    //     {
    //       AddLog(LOG_LEVEL_INFO, PSTR("ERROR!"));
    //     }
    // }
}
void Lora_UartE32PrintInfomation()
{
    if (!Lora_Uart)
        return;
    Configuration configuration;
    ModuleInformation moduleInformation;
    ResponseStructContainer rc;
    rc = Lora_Uart->getConfiguration();
    if (!rc.data)
        return;
    memcpy(&configuration, rc.data, sizeof(Configuration));
    rc.close();
    rc = Lora_Uart->getModuleInformation();
    if (!rc.data)
        return;
    memcpy(&moduleInformation, rc.data, sizeof(ModuleInformation));
    rc.close();
    printParameters(configuration);
    printModuleInformation(moduleInformation);
}
bool Xdrv104(uint32_t function)
{

    bool result = false;

    if (FUNC_INIT == function)
    {
        // AddLog(LOG_LEVEL_INFO, PSTR("INIT"));
        Lora_UartE32Init();
    }
    else if (Lora_Uart_e32)
    {

        switch (function)
        {
            //    Select suitable interval for polling your function
        // case FUNC_EVERY_SECOND:
        //   AddLog(LOG_LEVEL_INFO, PSTR("EVERY SECOND"));
        //   break;
        case FUNC_EVERY_250_MSECOND:
            Lora_UartE32Processing();
            break;
        case FUNC_COMMAND:
            result = DecodeCommand(Lora_UartE32Commands, Lora_UartE32Command);
            break;
            // case FUNC_EVERY_SECOND:
            //   break;
            //    case FUNC_EVERY_200_MSECOND:
            //    case FUNC_EVERY_100_MSECOND:
        }
    }

    return result;
}
#endif // USE_Lora_Uart_E32_433