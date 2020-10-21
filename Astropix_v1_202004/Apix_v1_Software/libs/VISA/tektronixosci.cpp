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

#include "tektronixosci.h"

TektronixOsci::TektronixOsci(int printlevel) : VISADevice(), timestep(0), yoffset(0), yscale(0)
{
    this->printlevel = printlevel;
}

bool TektronixOsci::SetAcquisitionState(bool run)
{
    if(instr == VI_NULL)
        return false;

    if(run)
        return Write("acq:start") > 0;
    else
        return Write("acq:stop") > 0;
}

bool TektronixOsci::SetChannel(int index)
{
    if(instr == VI_NULL || index < 1 || index > 4)
        return false;

    std::stringstream s("");
    s << "data:source ch" << index;

    if(Write(s.str()) == 0)
        return false;
    else
    {
        if(printlevel >= INFO)
            std::cout << "Set readout to Channel " << index << std::endl;
        return true;
    }
}

int TektronixOsci::ReadWaveFormSetup(bool restart)
{
    (void) restart;

    if(instr == VI_NULL)
        return -1;

    if(printlevel >= INFO)
        std::cout << "Reading WaveForm Setup" << std::endl;

    ViUInt32 writtenchars;

    //--- set transmission format --
    status = viWrite(instr, ViBuf("DATA:ENCDG RIBINARY\n"),
                     20, &writtenchars);
    status = viWrite(instr, ViBuf("WFMOutpre:BYT_Nr 1\n"),
                     19, &writtenchars);

    //--- get the parameters to transform to time and voltage ---
    float timescale;
    status = viQueryf(instr, ViString("WFMOutpre:XINcr?\n"), ViString("%f"),
                      &timescale);
    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not read Timescale" << std::endl;
        return -2;
    }
    float tekyoffset;
    status = viQueryf(instr, ViString("WFMOutpre:YOFF?\n"), ViString("%f"),
                      &tekyoffset);
    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Coould not read y offset" << std::endl;
        return -3;
    }
    float ymult;
    status = viQueryf(instr, ViString("WFMOutpre:YMULT?\n"), ViString("%f"),
                      &ymult);
    if(status < VI_SUCCESS)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not read y multiplier" << std::endl;
        return -4;
    }

    timestep = double(timescale);
    yoffset  = double(tekyoffset);
    yscale   = double(ymult);

    waveformsetupdone = true;

    if(printlevel >= INFO)
        std::cout << "   ... done" << std::endl;

    return 0;
}

std::string TektronixOsci::ReadWaveForm(bool setup, WaitFunction waitmethod, int sleeptime)
{
    //check for open connection to instrument:
    if(instr == VI_NULL)
        return "";

    //check for set up waveform parameters and retrieve them if nevessary
    if((!waveformsetupdone || setup) && ReadWaveFormSetup() < 0)
        return "";

    //request the curve:
    if(Write("Curve?") != 7)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not write waveform request" << std::endl;
        return "";
    }

    //ViChar buffer[50];

    //status = viRead(instr, (ViBuf)buffer, 2, &readchars);
    std::string wfmstart = Read(2);
    int loopcounter = 0;
    while(wfmstart.length() == 0 && loopcounter++ < 60)
    {
        sleep(sleeptime);
        wfmstart = Read(2);
        if(waitmethod != nullptr)
            waitmethod();
    }

    if(wfmstart.c_str()[0] != '#')
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Waveform start not as expected" << std::endl;
        return "";
    }

    //get length of the waveform:
    int length = wfmstart.c_str()[1] - '0';
    //status = viRead(instr, (ViBuf)buffer, length, &readchars);
    wfmstart = Read(length);
    //convert length of the waveform to int:
    long bufferlength = 0;
    for(int i=0;i<length; ++i)
        bufferlength = bufferlength * 10 + (wfmstart.c_str()[i] - '0');

    //read the waveform data:
    std::string wfm = Read(bufferlength);

    if(printlevel >= INFO)
        std::cout << "Waveform received: " << wfm.length() << " points" << std::endl;

    //add the timescale at the beginning:
    std::stringstream s("");
    s << timestep;
    wfm = s.str() + "\n" + wfm;

    return wfm;
}

double TektronixOsci::GetTimeScale(std::string data)
{
    if(data == "")
        return timestep;
    else
    {
        std::stringstream s("");
        s << data;
        double timescale = 0;
        s >> timescale;
        return timescale;
    }
}

double TektronixOsci::GetDispTimeScale()
{
    if(instr == VI_NULL)
        return -1;

    return QueryF("HORizontal:MAIn:SCAle?\n");
}

bool TektronixOsci::SetDispTimeScale(double value)
{
    if(value < 0 || instr == VI_NULL)
        return false;

    std::stringstream s("");
    s << "HORizontal:MAIn:SCAle " << value << "\n";
    return Write(s.str());
}

double TektronixOsci::GetDispTimeOffset()
{
    if(instr == VI_NULL)
        return -1;

    double xscale = QueryF("HORizontal:MAIn:SCAle?\n");
    int    recordlength = QueryI("HORizontal:RECordlength?\n");

    double fraction = QueryI("HORizontal:POSition?\n");

    if(fraction >= 0)
        return (fraction - 50)/100. * recordlength * xscale;
    else
        return -2;
}

bool TektronixOsci::SetDispTimeOffset(double offset)
{
    if(instr == VI_NULL)
        return false;

    double xscale = QueryF("HORizontal:MAIn:SCAle?\n");
    int    recordlength = QueryI("HORizontal:RECordlength?\n");

    std::stringstream s("");
    s << "HORizontal:POSition " << ((offset/recordlength/xscale + 0.5)*100) << "\n";
    return Write(s.str());
}

double TektronixOsci::GetVoltageScale(int channel)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return -1;

    std::stringstream s("");
    s << "CH" << channel << ":SCAle?\n";
    return QueryF(s.str());
}

bool TektronixOsci::SetVoltageScale(int channel, double scale)
{
    if(instr == VI_NULL || channel < 1 || channel > 4 || scale <= 0)
        return false;

    std::stringstream s("");
    s << "CH" << channel << ":SCAle " << scale << "\n";
    return Write(s.str());

}

double TektronixOsci::GetVoltageOffset(int channel)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return -1;

    std::stringstream s("");
    s << "CH" << channel << ":POSition?\n";
    return QueryF(s.str());
}

bool TektronixOsci::SetVoltageOffset(int channel, double offset)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return false;

    std::stringstream s("");
    s << "CH" << channel << ":POSition " << offset << "\n";
    return Write(s.str());
}

std::vector<double> TektronixOsci::DecodeWaveFormY(std::string data)
{
    std::vector<double> wfm;

    unsigned int start = data.find('\n');
    for(unsigned int i = start + 1; i < data.length(); ++i)
        wfm.push_back((static_cast<double>(data[i])-yoffset) * yscale);

    return wfm;
}

std::vector<std::pair<double, double> > TektronixOsci::DecodeWaveFormXY(std::string data)
{
    std::vector<std::pair<double, double> > wfm;

    std::stringstream s("");
    s << data;
    double timescale;
    s >> timescale;

    unsigned int start = data.find('\n');
    double time = 0;
    for(unsigned int i = start + 1; i < data.length(); ++i)
    {
        wfm.push_back(std::make_pair(time, (static_cast<double>(data[i]) - yoffset) * yscale));
        time += timescale;
    }

    return wfm;
}

bool TektronixOsci::SetupSingleTrigger()
{
    if(Write("ACQ:StopA SEQ") == 0 || Write("ACQ:State 1") == 0)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not set up single trigger mode" << std::endl;
        return false;
    }

    return true;
}

bool TektronixOsci::WaitForSingleTrigger(WaitFunction waitmethod, int sleeptime, double timeout)
{
    int state = 1;
    int loopcounter = int(timeout * 1000) / (sleeptime + 10); //the query also takes time
    while(state != 0 && loopcounter >= 0)
    {
        sleep(sleeptime);
        state = QueryI("ACQ:State?");
        if(state == -1023*1024)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Error: Could not read trigger Information. Aborting." << std::endl;
            return false;
        }
        if(waitmethod != nullptr)
            waitmethod();

        --loopcounter;
    }

    return loopcounter >= 0;
}

bool TektronixOsci::SaveCompressed(std::string filename, std::string data, bool extendedparameters)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    if(!f.is_open())
        return false;

    if(extendedparameters)
        f << "# Formula: (data - offset) * scale\n"
          << "# Timescale: " << timestep << std::endl
          << "# YOffset:   " << yoffset << std::endl
          << "# YScale:    " << yscale << std::endl;
    else
        f << "TEK;" << timestep << ";" << yoffset << ";" << yscale << ";";

    unsigned int start = data.find('\n');

    f << data.substr(start + 1);

    f.close();

    return true;
}
