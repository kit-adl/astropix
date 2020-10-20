/*
 * ATLASPix3_SoftAndFirmware
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spinexys.h"

SPINexys::SPINexys(unsigned int buffersize)  : writeaddress(23), readaddress(24),
                clockdivaddress(22), clockdiv(10)
{
    identifier = "SPI Configuration";
    this->buffersize = buffersize;

    fm_config.AddFlag("WrFIFORst"  ,   1, false);
    fm_config.AddFlag("WrFIFOEmpty",   2, false);
    fm_config.AddFlag("WrFIFOFull" ,   4, false);
    fm_config.AddFlag("RdFIFORst"  ,   8, false);
    fm_config.AddFlag("RdFIFOEmpty",  16, false);
    fm_config.AddFlag("RdFIFOFull" ,  32, false);
    fm_config.AddFlag("ReadbackEn" ,  64, false);
    fm_config.AddFlag("SPIModuleEn", 128, false);

    fm_lines.AddFlag("Sin"      , Sin       , false);
    fm_lines.AddFlag("Ck1"      , Ck1       , false);
    fm_lines.AddFlag("Ck2"      , Ck2       , false);
    fm_lines.AddFlag("Rb"       , Rb        , false);
    fm_lines.AddFlag("LdDAC"    , LdDAC     , false);
    fm_lines.AddFlag("LdConfig" , LdConfig  , false);
    fm_lines.AddFlag("LdVDAC"   , LdVDAC    , false);
    fm_lines.AddFlag("LdTDAC"   , LdTDAC    , false);
    fm_lines.AddFlag("LdRow"    , LdRow     , false);
    fm_lines.AddFlag("LdColumn" , LdColumn  , false);
    fm_lines.AddFlag("WriteRAM0", WriteRAM0 , false);
    fm_lines.AddFlag("WriteRAM1", WriteRAM1 , false);
    fm_lines.AddFlag("WriteRAM2", WriteRAM2 , false);
    fm_lines.AddFlag("WriteRAM3", WriteRAM3 , false);
    fm_lines.AddFlag("WrRAMEn"  , WriteRAMEn, false);
    fm_lines.AddFlag("ResetTS"  , ResetTS   , false);
    fm_lines.AddFlag("Injection", Injection , false);
    fm_lines.AddFlag("SPCH"     , SPCH      , false);
}

std::string SPINexys::GetIdentifier() const
{
    return identifier;
}

void SPINexys::SetIdentifier(std::string name)
{
    identifier = name;
}

unsigned int SPINexys::GetBufferSize()
{
    return buffersize;
}

void SPINexys::SetBufferSize(unsigned int buffersize)
{
    this->buffersize = buffersize;
}

bool SPINexys::GetReadBackEnable()
{
    return fm_lines.GetFlag("Rb");
}

bool SPINexys::SetReadBackEnable(NexysIO* nexys, bool enable)
{
    fm_config.SetFlag("ReadbackEn", enable);

    if(nexys == nullptr || !nexys->is_open())
        return false;
    else
        return nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));
}

unsigned char SPINexys::GetWriteAddress()
{
    return writeaddress;
}

void SPINexys::SetWriteAddress(unsigned char address)
{
    writeaddress = address;
}

unsigned char SPINexys::GetReadAddress()
{
    return readaddress;
}

void SPINexys::SetReadAddress(unsigned char address)
{
    readaddress = address;
}

unsigned char SPINexys::GetClockDivAddress()
{
    return clockdivaddress;
}

void SPINexys::SetClockDivAddress(unsigned char address)
{
    clockdivaddress = address;
}

unsigned char SPINexys::GetConfigAddress()
{
    return configurationaddress;
}

void SPINexys::SetConfigAddress(unsigned char address)
{
    configurationaddress = address;
}

unsigned char SPINexys::GetClockDivider()
{
    return clockdiv;
}

bool SPINexys::SetClockDivider(NexysIO *nexys, unsigned char clockdiv)
{
    this->clockdiv = clockdiv;

    if(nexys == nullptr || !nexys->is_open())
        return false;
    else {
        return nexys->Write(clockdivaddress, clockdiv);
    }
}

bool SPINexys::GetSPIEnable()
{
    return !fm_config.GetFlag("SPIModuleEn");
}

bool SPINexys::SetSPIEnable(NexysIO *nexys, bool enable)
{
    fm_config.SetFlag("SPIModuleEn", (enable)?0:1);

    if(nexys == nullptr || !nexys->is_open())
        return false;
    else
        return nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));
}

bool SPINexys::Update(NexysIO *nexys)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool status = true;

    status &= SetSPIEnable(nexys, GetSPIEnable());
    status &= SetReadBackEnable(nexys, GetReadBackEnable());
    status &= SetClockDivider(nexys, GetClockDivider());

    return status;
}

bool SPINexys::ResetModule(NexysIO *nexys)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool result = true;

    result &= SetSPIEnable(nexys, false);
    result &= SetSPIEnable(nexys, true);

    return result;
}

bool SPINexys::ResetWriteFIFO(NexysIO *nexys)
{
    if(nexys == nullptr && !nexys->is_open())
        return false;

    bool result = true;
    fm_config.SetFlag("WrFIFORst", 1);
    result &= nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));
    fm_config.SetFlag("WrFIFORst", 0);
    result &= nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));

    return result;
}

bool SPINexys::ResetReadFIFO(NexysIO *nexys)
{
    if(nexys == nullptr && !nexys->is_open())
        return false;

    bool result = true;
    fm_config.SetFlag("RdFIFORst", 1);
    result &= nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));
    fm_config.SetFlag("RdFIFORst", 0);
    result &= nexys->Write(configurationaddress, byte(fm_config.GetConfiguration()));

    return result;
}

std::string SPINexys::Read(NexysIO *nexys, unsigned int numchars)
{
    if(nexys == nullptr || !nexys->is_open())
        return "";

    return nexys->Read(readaddress, int(numchars));
}

bool SPINexys::Write(NexysIO *nexys, int setting)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    return WriteNoNexysCheck(nexys, setting);
}

bool SPINexys::Write(NexysIO *nexys, std::vector<int> data, void* pb, WaitFunction waiter)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool waiting = true;
    int counter = 1000;
    while(waiting && --counter > 0)
    {
        int result = nexys->Read(configurationaddress, 1).c_str()[0];
        int compare = fm_config.GetFirstBitPosition("WrFIFOEmpty");
        if((result & compare) != 0)
            waiting = false;
    }

    bool success = true;

    counter = buffersize / 3;
    unsigned int i = 0;
    while(i < data.size())
    {
        if(counter > 0)
        {
            success &= WriteNoCheck(nexys, data[i]);
            ++i;
            --counter;
            if(pb != nullptr && (i % 5) == 4)
            {
#ifdef _useQT_SPI_
                (static_cast<QProgressBar*>(pb))->setValue((static_cast<QProgressBar*>(pb))->value() + 1); //update every 5 config words (i.e. 1 bit transmitted)
#endif
                if(waiter != nullptr)
                    waiter();

                nexys->Flush();
            }
        }
        else
        {
            int result = nexys->Read(configurationaddress, 1).c_str()[0];
            if((result & fm_config.GetFirstBitPosition("WrFIFOEmpty")) != 0)
                counter = buffersize / 3;
            else if((result & fm_config.GetFirstBitPosition("WrFIFOFull")) == 0)
                counter = 1;

            static int loopcount = 0;
            if(loopcount++ > 10)
            {
                loopcount = 0;
                if(waiter != nullptr)
                    waiter();
            }
        }
    }

    return success;
}

bool SPINexys::WriteASIC(NexysIO *nexys, std::vector<bool> data, int load, void* pb, WaitFunction waiter)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    std::vector<int> config;


    int value = fm_lines.GetConfiguration();

    value &= ~(Ck1 + Ck2 + LdDAC + LdConfig + LdVDAC + LdTDAC + LdRow + LdColumn + WriteRAM0
               + WriteRAM1 + WriteRAM2 + WriteRAM3 + WriteRAMEn);

    for(const auto it : data)
    {
        if(it)
            value |= Sin;
        else
            value &= ~Sin;
        config.push_back(value);
        config.push_back(value | Ck1);
        config.push_back(value);
        config.push_back(value | Ck2);
        config.push_back(value);
    }

    if(load != 0)
    {
        for(int i = 0; i < 4; ++i)
            config.push_back(value | load);
        config.push_back(value);
    }

    return Write(nexys,config, pb, waiter);
}

tinyxml2::XMLError SPINexys::LoadFromXMLElement(tinyxml2::XMLElement *root)
{
    if(root == nullptr)
        return tinyxml2::XML_ERROR_FILE_NOT_FOUND;

    const char* nam;
    nam = root->Attribute("name");
    if(nam != nullptr)
        identifier = std::string(nam);

    int value;
    value = root->IntAttribute("writeaddr");
    if(value > 0)
        writeaddress = byte(value);
    value = root->IntAttribute("readaddr");
    if(value > 0)
        readaddress = byte(value);
    value = root->IntAttribute("clockdivaddr");
    if(value > 0)
        clockdivaddress = byte(value);
    value = root->IntAttribute("clockdiv");
    if(value > 0)
        clockdiv = byte(value);
    value = root->IntAttribute("buffersize");
    if(value > 0)
        buffersize = static_cast<unsigned int>(value);
    value = root->IntAttribute("configaddr");
    if(value > 0)
        configurationaddress = byte(value);
    value = root->IntAttribute("config");
    if(value >= 0)
        fm_config.SetConfiguration(value);
    value = root->IntAttribute("lines");
    if(value >= 0)
        fm_lines.SetConfiguration(value);

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError SPINexys::LoadFromXMLFile(std::string filename, bool compressed)
{
    if(compressed)
    {
        std::fstream f;
        f.open(filename, std::ios::in);
        if(!f.is_open())
            return tinyxml2::XML_ERROR_FILE_NOT_FOUND;

        zip_file archive;
        archive.load(filename);
        filename = filename.substr(0, filename.rfind('.')) + ".xml";

        archive.extract(filename);
    }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    tinyxml2::XMLNode* config = getNode(&doc, "SPIConfig");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLElement *SPINexys::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string devicename)
{
    tinyxml2::XMLElement* rootnode = doc.NewElement("SPIConfig");

    if(devicename == "")
        rootnode->SetAttribute("name", identifier.c_str());
    else
        rootnode->SetAttribute("name", devicename.c_str());

    rootnode->SetAttribute("clockdiv", clockdiv);
    rootnode->SetAttribute("buffersize", buffersize);
    rootnode->SetAttribute("writeaddr", writeaddress);
    rootnode->SetAttribute("readaddr", readaddress);
    rootnode->SetAttribute("configaddr", configurationaddress);
    rootnode->SetAttribute("clockdivaddr", clockdivaddress);
    rootnode->SetAttribute("config", fm_config.GetConfiguration());
    rootnode->SetAttribute("lines", fm_lines.GetConfiguration());

    return rootnode;
}

tinyxml2::XMLError SPINexys::SaveToXMLFile(std::string filename, std::string devicename, bool compressed)
{
    std::string archivename = "";
    if(compressed)
    {
        archivename = filename;
        filename = filename.substr(0, filename.rfind('.')) + ".xml";
    }

    //create a new XML Document:
    tinyxml2::XMLDocument doc;

    //include the XML declaration:
    tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
    doc.LinkEndChild(dec);

    tinyxml2::XMLElement* node;

    node = SaveToXMLElement(doc, devicename);

    if(node != nullptr)
        doc.LinkEndChild(node);

    tinyxml2::XMLError error = doc.SaveFile(filename.c_str());

    if(compressed)
    {
        zip_file archive;
        archive.write(filename, filename);

        archive.save(archivename);

        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting compression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

bool SPINexys::WriteNoNexysCheck(NexysIO *nexys, int setting)
{
    bool notdone = true;
    bool success = true;
    int counter = 1000;
    while(notdone && --counter > 0)
    {
        int result = nexys->Read(configurationaddress, 1).c_str()[0];
        if((result & fm_config.GetFirstBitPosition("WrFIFOFull")) == 0)
        {
            success &= WriteNoCheck(nexys, setting);
            notdone = false;
        }
    }

    return success && !notdone;
}

bool SPINexys::WriteNoCheck(NexysIO *nexys, int setting)
{
    std::vector<byte> config;
    config.push_back(static_cast<byte>(setting >> 16));
    config.push_back(static_cast<byte>(setting >>  8));
    config.push_back(static_cast<byte>(setting      ));

    return nexys->Write(writeaddress, config, false);
}
