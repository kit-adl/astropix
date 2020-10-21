#include "cmdconfig.h"

CMDConfig::CMDConfig()
{
    //build the encoder map once and use a copy for CMDConfig instances:
    static std::vector<int> singleencode;
    if(singleencode.size() == 0)
    {
        singleencode.push_back(0b01101010); // 0
        singleencode.push_back(0b01101100); // 1
        singleencode.push_back(0b01110001); // 2
        singleencode.push_back(0b01110010); // 3
        singleencode.push_back(0b01110100); // 4
        singleencode.push_back(0b10001011); // 5
        singleencode.push_back(0b10001101); // 6
        singleencode.push_back(0b10001110); // 7
        singleencode.push_back(0b10010011); // 8
        singleencode.push_back(0b10010101); // 9
        singleencode.push_back(0b10010110); //10
        singleencode.push_back(0b10011001); //11
        singleencode.push_back(0b10011010); //12
        singleencode.push_back(0b10011100); //13
        singleencode.push_back(0b10100011); //14
        singleencode.push_back(0b10100101); //15
        singleencode.push_back(0b10100110); //16
        singleencode.push_back(0b10101001); //17
        singleencode.push_back(0b10101010); //18
        singleencode.push_back(0b10101100); //19
        singleencode.push_back(0b10110001); //20
        singleencode.push_back(0b10110010); //21
        singleencode.push_back(0b10110100); //22
        singleencode.push_back(0b11000011); //23
        singleencode.push_back(0b11000101); //24
        singleencode.push_back(0b11000110); //25
        singleencode.push_back(0b11001001); //26
        singleencode.push_back(0b11001010); //27
        singleencode.push_back(0b11001100); //28
        singleencode.push_back(0b11010001); //29
        singleencode.push_back(0b11010010); //30
        singleencode.push_back(0b11010100); //31
    }

    encoder = singleencode;

    static FlagManager configuration0;
    if(configuration0.GetNumFlags() == 0)
    {
        configuration0.AddFlag("sout",     0);
        configuration0.AddFlag("ck1",      0);
        configuration0.AddFlag("ck2",      0);
        configuration0.AddFlag("rb",       0);
        configuration0.AddFlag("lddac",    0);
        configuration0.AddFlag("ldconfig", 0);
        configuration0.AddFlag("ldvdac",   0);
        configuration0.AddFlag("ldtdac",   0);
        configuration0.AddFlag("ldrow",    0);
        configuration0.AddFlag("ldcolumn", 0);
    }
    config0 = configuration0;

    static FlagManager configuration1;
    if(configuration1.GetNumFlags() == 0)
    {
        configuration1.AddFlag("wrram0", 0);
        configuration1.AddFlag("wrram1", 0);
        configuration1.AddFlag("wrram2", 0);
        configuration1.AddFlag("wrram3", 0);
        configuration1.AddFlag("wrramen", 0);
        configuration1.AddFlag("injection", 0);
        configuration1.AddFlag("pchb", 0);
        configuration1.AddFlag("resetts", 0);
        configuration1.AddFlag("cmdsendconfig", 0);
        configuration1.AddFlag("selectconfig", 0);
    }
    config1 = configuration1;
}

bool CMDConfig::GetConfigBit(std::string name)
{
    if(config0.GetFlagWidth(name) > 0)
        return config0.GetFlag(name);
    else if(config1.GetFlagWidth(name) > 0)
        return config1.GetFlag(name);
    else
        return false;
}

bool CMDConfig::SetConfigBit(std::string name, bool value)
{
    if(config0.SetFlag(name, (value)?1:0) == false
            && config1.SetFlag(name, (value)?1:0) == false)
        return false;
    else
        return true;
}

int CMDConfig::GetTriggerCode(bool trigger_slot1, bool trigger_slot2, bool trigger_slot3, bool trigger_slot4)
{
    int trigger = ((trigger_slot1)?1:0) + ((trigger_slot2)?2:0) + ((trigger_slot3)?4:0) + ((trigger_slot4)?8:0);

    return GetTriggerCode(trigger);
}

int CMDConfig::GetTriggerCode(int triggerpattern)
{
    //for invalid triggers or no triggers send the empty sync_word:
    if(triggerpattern > 15 || triggerpattern <= 0)
        return syncword;

    static std::vector<int> triggercodes;
    if(triggercodes.size() == 0)
    {
        triggercodes.push_back(0b00101011); // 1
        triggercodes.push_back(0b00101101); // 2
        triggercodes.push_back(0b00101110); // 3
        triggercodes.push_back(0b00110011); // 4
        triggercodes.push_back(0b00110101); // 5
        triggercodes.push_back(0b00110110); // 6
        triggercodes.push_back(0b00111001); // 7
        triggercodes.push_back(0b00111010); // 8
        triggercodes.push_back(0b00111100); // 9
        triggercodes.push_back(0b01001011); //10
        triggercodes.push_back(0b01001101); //11
        triggercodes.push_back(0b01001110); //12
        triggercodes.push_back(0b01010011); //13
        triggercodes.push_back(0b01010101); //14
        triggercodes.push_back(0b01010110); //15
    }

    return triggercodes[uint(triggerpattern)];
}




std::vector<byte> CMDConfig::GenerateSetBitByteCode(int chipid, int configreg, int numsyncwords)
{
    std::vector<byte> cmd;

    if(configreg < 0 || configreg > 1 || chipid < 0 || chipid > 16)
        return cmd;

    //set bit header:
    cmd.push_back(setbit % 256);
    cmd.push_back(setbit / 256);

    //configuration register:
    cmd.push_back(byte(encoder[uint(configreg)]));
    //chip ID / broadcast:
    cmd.push_back(byte(encoder[uint(chipid)]));

    int value = 0;
    switch(configreg)
    {
    case(0):
        value = config0.GetConfiguration();
        break;
    case(1):
        value = config1.GetConfiguration();
        break;
    default:
        std::cerr << "This should not happen" << std::endl;
        return std::vector<byte>();
    }

    cmd.push_back(byte(encoder[uint(value % 32)]));
    cmd.push_back(byte(encoder[uint(value / 32)]));

    if(numsyncwords > 0) AddSyncWord(&cmd, numsyncwords);

    return cmd;
}

std::vector<byte> CMDConfig::GenerateTriggerCode(int triggerpattern, int triggertag)
{
    int trigger = GetTriggerCode(triggerpattern);
    std::vector<byte> data;
    data.push_back(trigger);
    data.push_back(encoder[uint(triggertag % 32)]);

    return data;
}

std::vector<byte> CMDConfig::GenerateRegWriteByteCode(int chipid, int bits)
{
    std::vector<byte> cmd;

    if(chipid < 0 || chipid > 16)
        return cmd;

    //RW register header:
    cmd.push_back(rwreg % 256);
    cmd.push_back(rwreg / 256);

    //address (not used):
    cmd.push_back(byte(encoder[uint(0)]));
    //chip ID / broadcast:
    cmd.push_back(byte(encoder[uint(chipid)]));

    //data:
    cmd.push_back(byte(encoder[uint(bits % 32)]));
    cmd.push_back(byte(encoder[uint(bits / 32)]));

    return cmd;
}

void CMDConfig::AddSyncWord(std::vector<byte>* vec, int numwords)
{
    if(vec == nullptr)
        return;

    static const byte synchigh = syncword / 256;
    static const byte synclow  = syncword % 256;

    for(int i = 0; i < numwords; ++i)
    {
        vec->push_back(synclow);
        vec->push_back(synchigh);
    }
}

void CMDConfig::AddRegWriteWord(std::vector<byte>* vec, int chipid, int pattern, int syncspacing)
{
    if(vec == nullptr)
        return;

    static std::vector<byte> onecmd;
    onecmd.clear();

    onecmd = GenerateRegWriteByteCode(chipid, pattern);
    vec->insert(vec->end(), onecmd.begin(), onecmd.end());

    AddSyncWord(vec, syncspacing);
}

std::vector<byte> CMDConfig::GenerateRegWriteByteCode(int chipid, std::vector<bool> bitstream,
                                                      int loadconfig, int syncspacing)
{
    std::vector<byte> cmd;

    //data can only be sent in blocks of 10 bits:
    unsigned int offset = 10 - (bitstream.size() % 10);
    unsigned int steps = bitstream.size() / 10 + ((offset != 10)?1:0);
    int bits = 0;

    for(unsigned int i = 0; i < steps; ++i)
    {


        //abcdefg block 5
        //offset = 5-2 = 3

        bits = 0;
        if(i == 0 && offset != 10)
        {
            //without offset it worked (i.e. the right data ends up in the registers),
            //  but a load signal is missing, whether this works too is still to be tested

            for(unsigned int index = offset; index < 10; ++index)// 3 4
                bits += (bitstream[index - offset])?(1 << (9 - index)):0; // [0] [1] <<1 <<0 xxxab
        }

        else if(i > 0 && offset != 10) {
            const int localoffset = i * 10 + 0 - offset;//ivan is this an error? NO localoffset = i * 10 - offset loc 2
            for(unsigned int index = 0; index < 10; ++index)//0 ... 4
                bits += (bitstream[localoffset + index])?(1 << (9 - index)):0;//[2] ... [6]    <<4 <<0  cdefg

        }


        else if (offset == 10)
        {
            const int localoffset = i * 10 + 10 - offset;//0 1 2
            for(unsigned int index = 0; index < 10; ++index)
                bits += (bitstream[localoffset + index])?(1 << (9 - index)):0;

        }
        AddRegWriteWord(&cmd, chipid, bits, syncspacing);
    }

    //add a load pulse in case something was requested:
    if(loadconfig != 0)
    {
        //AddSetBitCMD(&cmd, chipid, loadconfig, 16); //syncspacing); //ivan cmd
        AddSetBitCMD(&cmd, chipid, loadconfig, 1); //syncspacing);
        AddSetBitCMD(&cmd, chipid, 0         , 0);
    }

    return cmd;
}

void CMDConfig::AddSetBitCMD(std::vector<byte>* vec, int chipid, int pattern, int syncspacing)
{
    if(vec == nullptr)
        return;

    static std::vector<byte> onecmd;
    onecmd.clear();

    config0.SetConfiguration(pattern);
    onecmd = GenerateSetBitByteCode(chipid, 0);
    vec->insert(vec->end(), onecmd.begin(), onecmd.end());

    AddSyncWord(vec, syncspacing);
}

std::vector<byte> CMDConfig::GenerateManualRegWriteByteCode(int chipid, std::vector<bool> bitstream, int loadconfig, int syncspacing)
{
    std::vector<byte> cmd;

    //make sure that the state machine output for SIn is '0':
    std::vector<byte> clearsin = GenerateRegWriteByteCode(16, 0);
    cmd.insert(cmd.end(), clearsin.begin(), clearsin.end());
    AddSyncWord(&cmd, 60); //long sync spacing as a long command has to be executed

    int pattern = 0;
    for(auto it : bitstream)
    {
        pattern = (it)?1:0;                                   //SIn
        AddSetBitCMD(&cmd, chipid, pattern    , syncspacing);
        AddSetBitCMD(&cmd, chipid, pattern + 2, syncspacing); //Ck1
        AddSetBitCMD(&cmd, chipid, pattern    , syncspacing);
        AddSetBitCMD(&cmd, chipid, pattern + 4, syncspacing); //Ck2
        AddSetBitCMD(&cmd, chipid, pattern    , syncspacing);
    }

    //add a load pulse in case something was requested:
    if(loadconfig != 0)
    {
        AddSetBitCMD(&cmd, chipid, loadconfig, 4 * syncspacing);
        AddSetBitCMD(&cmd, chipid, 0         , 0              );
    }

    return cmd;
}
