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

#include "keysightosci.h"

KeysightOsci::KeysightOsci(int printlevel) : VISADevice(printlevel)
{

}

int KeysightOsci::StrToInt(std::string number)
{
    int value = 0;
    bool negative = false;
    for(unsigned int i = 0; i < number.length(); ++i)
    {
        if(number.c_str()[i] >= 48 && number.c_str()[i] <= 57)
            value = 10*value + number.c_str()[i]-48;
        else if(number.c_str()[i] == '-')
            negative = true;
    }

    if(negative)
        value = -value;

    if(printlevel >= DEBUG)
        std::cout << "Decoded \"" << number << "\" to " << value << std::endl;

    return value;
}

double KeysightOsci::StrToDouble(std::string number)
{
    long value = 0;
    int kommacorrection = -1;
    int exponent = 0;
    bool negative = false;
    bool negativeexponent = false;
    //extract base number:
    unsigned int i = 0;
    for(i = 0; i < number.length(); ++i)
    {
        if(number.c_str()[i] >= 48 && number.c_str()[i] <= 57)
        {
            value = 10*value + number.c_str()[i]-48;
            if(kommacorrection >= 0)
                ++kommacorrection;
        }
        else if(number.c_str()[i] == '-')
            negative = true;
        else if(number.c_str()[i] == '.')
            kommacorrection = 0;
        else if(number.c_str()[i] == 'E')
            break;
    }

    for(i = i+1; i < number.length(); ++i)
    {
        if(number.c_str()[i] >= 48 && number.c_str()[i] <= 57)
            exponent = 10*exponent + number.c_str()[i]-48;
        else if(number.c_str()[i] == '-')
            negativeexponent = true;
    }

    if(negative)
        value = -value;

    if(negativeexponent)
        exponent = -exponent;

    double result = value;
    result *= pow(10, exponent - kommacorrection);

    if(printlevel >= DEBUG)
        std::cout << "Decoded \"" << number << "\" to " << result << std::endl
                  << "  value: " << value << std::endl
                  << "  komma: " << kommacorrection << std::endl
                  << "  exponent: " << exponent << std::endl;

    return result;
}

bool KeysightOsci::SetAcquisitionState(bool run)
{
    if(instr == VI_NULL)
        return false;

    if(run)
        return Write("run") > 0;
    else
        return Write("stop") > 0;
}

bool KeysightOsci::SetChannel(int index)
{
    if(instr == VI_NULL || index < 1 || index > 4)
        return false;

    std::stringstream s("");
    s << "waveform:source chan" << index;

    if(Write(s.str()) == 0)
        return false;
    else
    {
        if(printlevel >= INFO)
            std::cout << "Set readout to Channel " << index << std::endl;
        return true;
    }
}


int KeysightOsci::ReadWaveFormSetup(bool restart)
{
    //no connection open:
    if(instr == VI_NULL)
        return -1;

    if(Write("stop") == 0)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not stop acquisition for getting parameters." << std::endl;
        return -2;
    }

    if(Write("wav:preamble?") == 0)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not request waveform parameters." << std::endl;
        return -2;
    }
    sleep(10);
    std::string parameters = Read(100);

    if(parameters.length() == 0)
    {
        if(printlevel >= ERRORS)
            std::cerr << "no answer from device. Quitting." << std::endl;
        return -3;
    }

    if(printlevel >= DEBUG)
        std::cout << "Waveform Parameters (raw): \"" << parameters << "\"" << std::endl;

    unsigned int min = 0;
    unsigned int max = 0;

    for(int i = 0; i < 10;++i)
    {
        min = max;
        max = parameters.find(',', min+1);

        int iresult = 0;
        double dresult = 0;

        if(i < 4 || i == 6 || i == 9)
            iresult = StrToInt(parameters.substr(min, max - min));
        else
            dresult = StrToDouble(parameters.substr(min, max - min));

        switch(i)
        {
        case(0):
            pntfrmt = iresult; break;
        case(1):
            pnttype = iresult; break;
        case(2):
            numpnts = iresult; break;
        case(3):
            avgbase = iresult; break;
        case(4):
            xincr = dresult; break;
        case(5):
            xorigin = dresult; break;
        case(6):
            xreference = iresult; break;
        case(7):
            yincr = dresult; break;
        case(8):
            yorigin = dresult; break;
        case(9):
            yreference = iresult; break;
        }
    }

    waveformsetupdone = true;

    if(restart)
    {
        if(Write("run") == 0)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Error: Could not restart acquisition after getting parameters." << std::endl;
            return -2;
        }
    }

    if(printlevel >= INFO)
        std::cout << "Waveform Parameters read successfully" << std::endl;

    return 0;
}

std::string KeysightOsci::ReadWaveForm(bool setup, WaitFunction waitmethod, int sleeptime)
{
    if(instr == VI_NULL)
        return "";

    if((!waveformsetupdone || setup) && ReadWaveFormSetup(false) < 0)
        return "";

    //request the waveform:
    if(Write("wav:data?") == 0)
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: Could not request waveform data" << std::endl;
        return "";
    }

    //wait for the response of the oscilloscope:
    std::string wfmstart = Read(2);
    int loopcounter = 0;
    while(wfmstart.length() == 0 && loopcounter++ < 60)
    {
        sleep(sleeptime);
        wfmstart = Read(2);
        if(waitmethod != nullptr)
            waitmethod();
    }

    //check for correct start of the data transmission:
    if(wfmstart.c_str()[0] != '#')
    {
        if(printlevel >= ERRORS)
            std::cerr << "Error: waveform start not as expected." << std::endl;
        return "";
    }

    //get the transmission length:
    int length = wfmstart.c_str()[1] - '0';
    wfmstart = Read(length);

    //create the read buffer:
    long bufferlength = 0;
    for(int i=0;i<length; ++i)
        bufferlength = bufferlength * 10 + (wfmstart.c_str()[i] - '0');

    //read the actual data:
    std::string wfm = Read(bufferlength);

    if(printlevel >= INFO)
        std::cout << "WaveForm transmitted: " << wfm.length() << " points." << std::endl;

    //add the timescale to the data:
    std::stringstream s("");
    s << xincr;
    wfm = s.str() + "\n" + wfm;

    return wfm;
}

double KeysightOsci::GetTimeScale(std::string data)
{
    if(data == "")
        return xincr;
    else
    {
        std::stringstream s("");
        s << data;
        double timescale;
        s >> timescale;
        return timescale;
    }
}

double KeysightOsci::GetDispTimeScale()
{
    if(instr == VI_NULL)
        return -1;

    return QueryF(":TIMebase:SCALe?\n");
}

bool KeysightOsci::SetDispTimeScale(double value)
{
    if(instr == VI_NULL || value <= 0)
        return false;

    std::stringstream s("");
    s << ":TIMebase:SCALe " << value << "\n";
    return Write(s.str());
}

double KeysightOsci::GetDispTimeOffset()
{
    if(instr == VI_NULL)
        return -1;

    return QueryF(":TIMebase:POSition?\n");
}

bool KeysightOsci::SetDispTimeOffset(double offset)
{
    if(instr == VI_NULL)
        return false;

    std::stringstream s("");
    s << ":TIMebase:POSition " << offset << "\n";
    return Write(s.str());
}

double KeysightOsci::GetVoltageScale(int channel)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return -1;

    std::stringstream s("");
    s << ":CHANnel" << channel << ":SCALe?\n";

    return QueryF(s.str());
}

bool KeysightOsci::SetVoltageScale(int channel, double scale)
{
    if(instr == VI_NULL || channel < 1 || channel > 4 || scale <= 0)
        return false;

    std::stringstream s("");
    s << ":CHANnel" << channel << ":SCALe " << scale << "\n";
    return Write(s.str());
}

double KeysightOsci::GetVoltageOffset(int channel)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return -1e10;

    std::stringstream sq("");
    sq << ":CHANnel" << channel << ":SCALe?\n";
    double yscale = QueryF(sq.str());

    std::stringstream s("");
    s << ":CHANnel" << channel << ":OFFSet?\n";

    return QueryF(s.str()) / yscale;
}

bool KeysightOsci::SetVoltageOffset(int channel, double offset)
{
    if(instr == VI_NULL || channel < 1 || channel > 4)
        return false;

    std::stringstream sq("");
    sq << ":CHANnel" << channel << ":SCALe?\n";
    double yscale = QueryF(sq.str());

    std::stringstream s("");
    s << ":CHANnel" << channel << ":OFFSet " << (offset * yscale) << "\n";
    return Write(s.str());
}

std::vector<double> KeysightOsci::DecodeWaveFormY(std::string data)
{
    std::vector<double> wfm;

    unsigned int start = data.find('\n');
    for(unsigned int i = start + 1; i < data.length(); ++i)
        wfm.push_back(((static_cast<double>(static_cast<unsigned char>(data.c_str()[i]))-yreference) * yincr + yorigin));

    return wfm;
}

std::vector<std::pair<double, double> > KeysightOsci::DecodeWaveFormXY(std::string data)
{
    std::vector<std::pair<double, double> > wfm;

    unsigned int start = data.find('\n');
    double time = 0;

    std::stringstream s("");
    s << data;
    double timescale;
    s >> timescale;

    for(unsigned int i = start + 1; i < data.length(); ++i)
    {
        wfm.push_back(std::make_pair(time, ((static_cast<double>(static_cast<unsigned char>(data.c_str()[i]))-yreference) * yincr + yorigin)));
        time += timescale;
    }

    return wfm;
}

bool KeysightOsci::SetupSingleTrigger()
{
    //make sure, that no old trigger activates the readout:
    QueryI("TER?");
    if(Write("SINGLE") == 0 && printlevel >= ERRORS)
    {
        std::cerr << "Error: Setting up the Single Trigger Mode failed." << std::endl;
        return false;
    }
    else
        return true;
}

bool KeysightOsci::WaitForSingleTrigger(WaitFunction waitmethod, int sleeptime, double timeout)
{
    int triggered = 0;
    int loopcounter = int(timeout * 1000) / (sleeptime + 10); //the query also takes time
    while(triggered == 0 && loopcounter >= 0)
    {
        sleep(sleeptime);
        triggered = QueryI("TER?");
        if(triggered == -1023*1024)
        {
            if(printlevel >= ERRORS)
                std::cerr << "Error: Querying trigger status failed. Aborting." << std::endl;
            return false;
        }
        if(waitmethod != nullptr)
            waitmethod();

        --loopcounter;
    }

    return loopcounter >= 0;
}

bool KeysightOsci::SaveCompressed(std::string filename, std::string data, bool extendedparameters)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);

    if(!f.is_open())
        return false;

    if(extendedparameters)
        f << "# Formula: (data - yreference) * yincrement + yorigin\n"
          << "# Timescale:  " << xincr << std::endl
          << "# YReference: " << yreference << std::endl
          << "# YIncrement: " << yincr << std::endl
          << "# YOrigin:    " << yorigin << std::endl;
    else
        f << "KEY;" << xincr << ";" << yreference << ";" << yincr << ";" << yorigin << ";";

    unsigned int start = data.find('\n');

    f << data.substr(start + 1);

    f.close();

    return true;
}
