#ifndef CMDCONFIG_H
#define CMDCONFIG_H

#include <vector>
#include "flagmanager.h"

typedef unsigned int  uint;
typedef unsigned char byte;

class CMDConfig
{
public:
    CMDConfig();

    static const int syncword = 0b1000000101111110;
    static const int rwreg    = 0b0110011001100110;
    static const int setbit   = 0b0110010101100101;

    enum loads {
        lddac    =  16,
        ldconfig =  32,
        ldvdac   =  64,
        ldtdac   = 128,
        ldrow    = 256,
        ldcolumn = 512
    };

    bool GetConfigBit(std::string name);
    bool SetConfigBit(std::string name, bool value);

    int GetTriggerCode(bool trigger_slot1, bool trigger_slot2, bool trigger_slot3, bool trigger_slot4);
    int GetTriggerCode(int triggerpattern);

    std::vector<byte> GenerateSetBitByteCode(int chipid, int configreg, int numsyncwords = 0);
    std::vector<byte> GenerateTriggerCode(int triggerpattern, int triggertag);
    std::vector<byte> GenerateRegWriteByteCode(int chipid, int bits);
    std::vector<byte> GenerateRegWriteByteCode(int chipid, std::vector<bool> bitstream,
                                               int loadconfig, int syncspacing = 50);
    std::vector<byte> GenerateManualRegWriteByteCode(int chipid, std::vector<bool> bitstream,
                                                     int loadconfig, const int syncspacing = 4);

private:
    std::vector<int> encoder;
    FlagManager config0;
    FlagManager config1;
    void AddSyncWord(std::vector<byte> *vec, int numwords);
    void AddSetBitCMD(std::vector<byte> *vec, int chipid, int pattern, int syncspacing);
    void AddRegWriteWord(std::vector<byte> *vec, int chipid, int pattern, int syncspacing);
};

#endif // CMDCONFIG_H
