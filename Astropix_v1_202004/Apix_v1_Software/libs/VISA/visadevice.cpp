/*
 * Osci_SMU Readout Project
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu) and
 *					   Felix Ehrler (felix.ehrler@kit.edu)
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

#include "visadevice.h"

#if defined(__linux__)
#include <unistd.h>

void sleep(int ms)
{
    if(ms > 0)
        usleep(1000*ms);
}
#else
#include <windows.h>

void sleep(int ms)
{
    if(ms > 0)
        Sleep(static_cast<unsigned long>(ms));
}
#endif

VISADevice::VISADevice(int printlevel) : defaultRM(VI_NULL), instr(VI_NULL), status(VI_NULL), waveformsetupdone(false)
{
    this->printlevel = printlevel;
}

VISADevice::~VISADevice()
{
    if(instr != VI_NULL)
        close();
}

std::vector<std::string> VISADevice::ScanForDevices()
{
    std::vector<std::string> devicenames;
    if(instr != VI_NULL)
        return devicenames;

    //open the default manager handle:
    status=viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Could not open a session to the VISA Resource Manager!" << std::endl;
        return devicenames;
    }

    //get a list of all USB devices:
    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, ViString("USB?*INSTR"), &findList, &numInstrs,
                        instrResourceString);

    if (status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "An error occurred while searching resources." << std::endl;
        viClose(defaultRM);
        return devicenames;
    }

    resname.clear();

    for (unsigned int i=0; i<numInstrs; i++)
    {
        if (i > 0)
            viFindNext(findList, instrResourceString);

        status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);

        if (status < VI_SUCCESS)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Cannot open a session to device " << (i+1) << std::endl;
            continue;
        }

        //get the identification of the device:
        ViUInt32 writeCount;
        status = viWrite(instr, ViBuf("*IDN?\n"), ViUInt32(6), &writeCount);
        if (status < VI_SUCCESS)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Error writing to device " << (i+1) << std::endl;
            status = viClose(instr);
            continue;
        }

        //read back the device identification:
        ViUInt32 retCount;
        unsigned char buffer[100];
        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Error reading a response from device " << (i+1) << std::endl;
        }
        else
        {
            std::string name = std::string(reinterpret_cast<char*>(buffer));
            //std::string res  = std::string(instrResourceString);
            //resource.push_back(res); //instrResourceString);
            resname.push_back(name);
            if(printlevel >= INFO)
                std::cout << "Device " << resname.size() << ": \"" << name << "\"" << std::endl
                          << "    Resource: " << instrResourceString << std::endl;
        }
        status = viClose(instr);
    }


    //close the resource manager handle:
    status = viClose(defaultRM);

    return resname;
}

int VISADevice::GetNumDevicesFound()
{
    return int(resname.size());
}

bool VISADevice::is_open()
{
    return instr != VI_NULL;
}

bool VISADevice::open(unsigned int index)
{
    status = viOpenDefaultRM(&defaultRM);
    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error opening default Resource Manager" << std::endl;
        return false;
    }

    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, ViString("USB?*INSTR"), &findList, &numInstrs,
                        instrResourceString);

    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error getting Resource List" << std::endl;
        return false;
    }

    if(index > numInstrs)
        return false;

    //get the descriptor for the requested resource
    for(unsigned int i = 0; i < index; ++i)
        viFindNext(findList,instrResourceString);

    status = viOpen(defaultRM,instrResourceString,VI_NULL, VI_NULL,&instr);

    /*
     * this check was always failing by now, but the communication worked, hence skip it:
    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error opening device " << index << std::endl;
        return false;
    }
    */

    waveformsetupdone = false;

    if(printlevel >= INFO)
        std::cout << "Session opened" << std::endl;

    return true;
}

bool VISADevice::open(std::string description)
{
    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, ViString("USB?*INSTR"), &findList, &numInstrs,
                        instrResourceString);

    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error getting Resource List" << std::endl;
        return false;
    }


    //get the descriptor for the requested resource
    unsigned int i;
    for(i = 0; i < numInstrs; ++i)
    {
        if(description.compare(resname[i]) == 0)
            break;
        viFindNext(findList,instrResourceString);
    }

    if(i < numInstrs)
        status = viOpen(defaultRM,instrResourceString,VI_NULL, VI_NULL,&instr);
    else
        return false;

    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error opening resource \"" << instrResourceString << "\"" << std::endl;
        return false;
    }

    waveformsetupdone = false;

    if(printlevel >= INFO)
        std::cout << "Session opened" << std::endl;

    return true;
}

bool VISADevice::CheckResourceName(int index, std::string findstring)
{
    if(index < 0 || index >= int(resname.size()))
        return false;
    else
        return (resname[static_cast<unsigned int>(index)].find(findstring) != std::string::npos);
}

void VISADevice::close()
{
    viClose(instr);
    viClose(defaultRM);
    instr = VI_NULL;
    if(printlevel >= INFO)
        std::cout << "Session Closed." << std::endl;
}

int VISADevice::GetPrintLevel()
{
    return printlevel;
}

bool VISADevice::SetPrintLevel(int level)
{
    if(level >= NONE && level <= ALL)
    {
        printlevel = level;
        return true;
    }
    else
        return false;
}

std::string VISADevice::Read(int characters)
{
    //only read if a session is open:
    if(instr == VI_NULL)
        return "";

    if(printlevel >= READWRITE)
        std::cout << "read " << characters << " characters..." << std::endl;

    ViChar* buffer = new ViChar[static_cast<unsigned int>(characters)];
    ViUInt32 readchars;
    status = viRead(instr, ViBuf(buffer), static_cast<unsigned long>(characters), &readchars);

    std::string data = "";
    for(unsigned int i = 0; i < readchars; ++i)
        data = data + buffer[i];

    if(printlevel >= READWRITE)
        std::cout << readchars << " characters read: \"" << data << "\"" << std::endl;

    delete[] buffer;
    return data;
}

int VISADevice::Write(std::string data)
{
    //only try to write if a session is open:
    if(instr == VI_NULL)
        return 0;

    if(data.c_str()[data.length()-1] != '\n')
        data = data + std::string("\n");

    if(printlevel >= READWRITE)
        std::cout << "Write: \"" << data.substr(0,data.length()-1) << "\" (" << data.length() << " characters)" << std::endl;

    ViUInt32 writtenchars;
    status = viWrite(instr, ViBuf(data.c_str()), data.length(), &writtenchars);

    if(printlevel >= READWRITE)
        std::cout << "  ... " << writtenchars << " characters written." << std::endl;

    return int(writtenchars);
}

double VISADevice::QueryF(std::string query)
{
    if(instr == VI_NULL)
        return -1e10;

    if(query.c_str()[query.length()-1] != '\n')
        query = query + '\n';

    float fval;
    status = viQueryf(instr,ViString(query.c_str()),ViString("%f"), &fval);

    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error querying \"" << query << "\" for floating point number." << std::endl;
        return -1e10;
    }

    return double(fval);
}

int VISADevice::QueryI(std::string query)
{
    if(instr == VI_NULL)
        return -1023*1024;

    if(query.c_str()[query.length()-1] != '\n')
        query = query + '\n';

    int ival;
    status = viQueryf(instr, ViString(query.c_str()),ViString("%i"), &ival);

    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error querying \"" << query << "\" for integer number." << std::endl;
        return -1023*1024;
    }

    return ival;
}

bool VISADevice::SetAcquisitionState(bool run)
{
    (void) run;
    return false;
}

bool VISADevice::SetChannel(int index)
{
    (void) index;
    return false;
}

bool VISADevice::GetWaveFormSetupStatus()
{
    return waveformsetupdone;
}

int VISADevice::ReadWaveFormSetup(bool restart)
{
    (void) restart;
    return -1;
}

std::string VISADevice::ReadWaveForm(bool setup, WaitFunction waitmethod, int sleeptime)
{
    (void) setup;
    (void) waitmethod;
    (void) sleeptime;

    std::cout << "not implemented" << std::endl;

    return "";
}

double VISADevice::GetTimeScale(std::string data)
{
    (void) data;
    return -1;
}

double VISADevice::GetDispTimeScale()
{
    return -1;
}

bool VISADevice::SetDispTimeScale(double value)
{
    (void) value;
    return false;
}

double VISADevice::GetDispTimeOffset()
{
    return -1;
}

bool VISADevice::SetDispTimeOffset(double offset)
{
    (void) offset;
    return false;
}

double VISADevice::GetVoltageScale(int channel)
{
    (void) channel;
    return -1;
}

bool VISADevice::SetVoltageScale(int channel, double scale)
{
    (void) channel;
    (void) scale;
    return false;
}

double VISADevice::GetVoltageOffset(int channel)
{
    (void) channel;
    return -1e10;
}

bool VISADevice::SetVoltageOffset(int channel, double offset)
{
    (void) channel;
    (void) offset;
    return false;
}

std::vector<double> VISADevice::DecodeWaveFormY(std::string data)
{
    (void) data;
    return std::vector<double>();
}

std::vector<std::pair<double, double> > VISADevice::DecodeWaveFormXY(std::string data)
{
    (void) data;
    return std::vector<std::pair<double, double> >();
}

bool VISADevice::SetupSingleTrigger()
{
    return false;
}

bool VISADevice::WaitForSingleTrigger(WaitFunction waitmethod, int sleeptime, double timeout)
{
    //this suppresses the warnings for unused variable(s):
    (void) waitmethod;
    (void) sleeptime;
    (void) timeout;
    return false;
}

bool VISADevice::SaveToFile(std::string filename, std::vector<double> data, double timescale, bool compact)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);
    if(!f.is_open())
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not open \"" << filename << "\" for writing out the waveform." << std::endl;
        return false;
    }

    std::stringstream s("");

    if(compact)
    {
        s << "# Timescale: " << timescale << std::endl;
        for(auto& it : data)
            s << it << std::endl;
    }
    else
    {
        s << "# Time (s); Signal (V)" << std::endl;
        double time = 0;
        for(auto& it : data)
        {
            s << time << "\t" << it << std::endl;
            time += timescale;
        }
    }

    f << s.str();

    f.close();

    if(printlevel >= INFO)
        std::cout << "Saved waveform to \"" << filename << "\"." << std::endl;

    return true;
}

bool VISADevice::SaveToFile(std::string filename, std::vector<std::pair<double, double> > data, bool compact)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);
    if(!f.is_open())
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not open \"" << filename << "\" for writing out the waveform." << std::endl;
        return false;
    }

    std::stringstream s("");

    if(compact)
    {
        double timestep = (++data.begin())->first - data.front().first;
        s << "# Timescale: " << timestep << std::endl;
        for(auto& it : data)
        {
            s << it.second << std::endl;
        }
    }
    else
    {
        s << "# Time (s); Signal (V)" << std::endl;
        for(auto& it : data)
        {
            s << it.first << "\t" << it.second << std::endl;
        }
    }

    f << s.str();

    f.close();

    if(printlevel >= INFO)
        std::cout << "Saved waveform to \"" << filename << "\"" << std::endl;

    return true;
}

bool VISADevice::SaveCompressed(std::string filename, std::string data, bool extendedparameters)
{
    (void) filename;
    (void) data;
    (void) extendedparameters;
    return false;
}
