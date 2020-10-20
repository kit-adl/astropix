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

#include "lasersetup.h"

LaserSetup::LaserSetup() : lasersleep(150), stagesleep(100)
{
    for(int i = 0; i < 3; ++i)
    {
        initialised[i] = false;
        position[i] = 0;
        minposition[i] = 0;
        maxposition[i] = (i != 2)?25:100;
    }

    laser.setBaudRate(QSerialPort::Baud19200);
    laser.setDataBits(QSerialPort::Data8);
    laser.setStopBits(QSerialPort::OneStop);
    laser.setParity(QSerialPort::NoParity);

    stages.setBaudRate(QSerialPort::Baud9600);
    stages.setDataBits(QSerialPort::Data8);
    stages.setStopBits(QSerialPort::OneStop);
    stages.setParity(QSerialPort::NoParity);
}

bool LaserSetup::ConfigureLaserPort(std::string portname)
{
    laser.setPortName(QString(portname.c_str()));

    return true;
}

bool LaserSetup::ConfigureStagePort(std::string portname, int baudrate)
{
    stages.setBaudRate(baudrate);
    stages.setPortName(QString(portname.c_str()));

    return true;
}

bool LaserSetup::IdentifyPorts()
{
    std::vector<std::string> portlist = GetPortList();

    //test for laser driver with "version?" command and with "1 0 *IDN?" for stages

    laser.close();
    stages.close();

    for(auto& it : portlist)
    {
        std::cout << "Testing port \"" << it << "\" ..." << std::endl;

        if(!laser.isOpen())
        {
            laser.setPortName(it.c_str());
            laser.open(QSerialPort::ReadWrite);
            if(laser.isOpen())
            {
                laser.clearError();
                laser.clear();
                WriteLaser("version?", 5);
                std::string answer = ReadLaser(1000);
                if(answer.find("PiLas") == std::string::npos)
                    laser.close();
                else
                {
                    std::cout << "Laser Port found: \"" << it << "\"" << std::endl;
                    continue;
                }
            }
        }

        if(!stages.isOpen())
        {
            stages.setPortName(it.c_str());
            stages.open(QSerialPort::ReadWrite);
            if(stages.isOpen())
            {
                stages.clearError();
                stages.clear();
                WriteStages("1 0 *IDN?");
                std::string answer = ReadStages(200);
                if(answer.find("Physik Instrumente") == std::string::npos)
                    stages.close();
                else
                {
                    std::cout << "Stage Port found: \"" << it << "\"" << std::endl;
                //    continue;
                }
            }
        }
    }

    if(stages.isOpen() && laser.isOpen())
        return true;
    else
        return false;
}

bool LaserSetup::Laser_is_open()
{
    return laser.isOpen();
}

bool LaserSetup::Stages_is_open()
{
    return stages.isOpen();
}

bool LaserSetup::is_open()
{
    return (laser.isOpen() && stages.isOpen());
}

bool LaserSetup::OpenLaserPort()
{
    if(laser.isOpen())
        return true;
    else
        return laser.open(QSerialPort::ReadWrite);
}

bool LaserSetup::CloseLaserPort()
{
    laser.close();
    return true;
}

bool LaserSetup::ActivateLaser(bool onoff)
{
    if(!laser.isOpen())
        return false;

    std::stringstream s("");
    s << "ld=" << ((onoff)?1:0);

    if(WriteLaser(s.str()))
    {
        std::string answer = ReadLaser(30);
        if(answer.compare("done") == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool LaserSetup::GetLaserState()
{
    if(!laser.isOpen())
        return false;

    if(!WriteLaser("ld?"))
        return false;
    else
    {
        std::string answer = ReadLaser(30);
        //std::cout << "Laser Status: \"" << answer << "\" => extracted: \""
        //          << answer.substr(23,2) << "\"" << std::endl;
        if(answer.substr(23,2).compare("on") == 0)
            return true;
        else
            return false;
    }

}

double LaserSetup::GetTuneValue()
{
    if(!laser.isOpen())
        return -1;

    if(!WriteLaser("tune?"))
        return -1;
    else
    {
        std::string answer = ReadLaser(100);

        double tunevalue = 0.;
        std::stringstream s("");
        s << answer;
        s >> answer >> answer >> tunevalue;

        return tunevalue;
    }
}

bool LaserSetup::SetTuneValue(double tune)
{
    if(!laser.isOpen() || tune > 100 || tune < 0)
        return false;

    std::stringstream s("");
    s << "tune=" << int(10*tune);
    if(!WriteLaser(s.str()))
        return false;
    else
    {
        std::string answer = ReadLaser(6);
        if(answer.substr(0,4).compare("done") == 0)
            return true;
        else
            return false;
    }
}

int LaserSetup::GetTriggerFrequency()
{
    if(!laser.isOpen())
        return -1;

    if(!WriteLaser("f?"))
        return -1;
    else
    {
        std::string answer = ReadLaser(100);
        int freq;
        std::stringstream s("");
        s << answer;
        s >> answer >> answer >> freq;

        return freq;
    }
}

bool LaserSetup::SetTriggerFrequency(int freq)
{
    if(!laser.isOpen() || freq < 25)
        return false;

    std::stringstream s("");
    s << "f=" << freq;
    if(!WriteLaser(s.str()))
        return false;
    else
    {
        std::string answer = ReadLaser(6);
        return (answer.substr(0,4).compare("done") == 0);
    }
}

int LaserSetup::GetTriggerEdge()
{
    if(!Laser_is_open())
        return -1;

    if(!WriteLaser("te?"))
        return -1;
    else
    {
        std::string answer = ReadLaser(100);
        std::stringstream s("");
        s << answer;
        s >> answer >> answer >> answer;

        if(answer.compare("rising") == 0)
            return rising;
        else if(answer.compare("falling") == 0)
            return falling;
        else
            return -1;
    }
}

bool LaserSetup::SetTriggerEdge(unsigned int edge)
{
    if(!Laser_is_open())
        return false;

    if(edge == rising)
    {
        if(!WriteLaser("te=1"))
            return false;
        else
        {
            std::string answer = ReadLaser(20);
            return (answer.find("done") != std::string::npos);
        }
    }
    else if(edge == falling)
    {
        if(!WriteLaser("te=0"))
            return false;
        else
        {
            std::string answer = ReadLaser(40);
            return (answer.find("done") != std::string::npos);
        }
    }
    else
        return false;
}

int LaserSetup::GetTriggerSource()
{
    if(!Laser_is_open())
        return -1;

    if(!WriteLaser("ts?"))
        return -1;
    else
    {
        std::string answer = ReadLaser(40);
        std::stringstream s("");
        s << answer;
        s >> answer >> answer >> answer;

        if(answer.compare("ext.") == 0)
        {
            s >> answer;
            answer = std::string("ext. ") + answer;
        }

        if(answer.find("internal") != std::string::npos)
            return internal;
        else if(answer.find("ext. adjustable") != std::string::npos)
            return extAdj;
        else if(answer.find("ext. TTL") != std::string::npos)
            return extTTL;
        else
            return -1;
    }
}

bool LaserSetup::SetTriggerSource(unsigned int source)
{
    if(!Laser_is_open())
        return false;

    if(source > 2) //three options for trigger settings
        return false;

    std::stringstream s("");
    s << "ts=" << source;

    if(!WriteLaser(s.str()))
        return false;
    else
    {
        std::string answer = ReadLaser(20);
        return (answer.find("done") != std::string::npos);
    }
}

double LaserSetup::GetTriggerLevel()
{
    if(!Laser_is_open())
        return -1e10;

    if(!WriteLaser("tl?"))
        return -1e10;
    else
    {
        double threshold = -1e10;
        std::string answer = ReadLaser(40);
        std::stringstream s("");
        s << answer;
        s >> answer >> answer >> threshold >> answer;
        if(answer.compare("V") == 0)
            return threshold;
        else if(answer.compare("mV") == 0)
            return threshold / 1000.;
        else
            return -1e10;
    }
}

bool LaserSetup::SetTriggerLevel(double level)
{
    if(!Laser_is_open())
        return false;
    if(level < -4.8 || level > 4.8)
        return false;

    std::stringstream s("");
    s << "tl=" << int(level*1000);
    if(!WriteLaser(s.str()))
        return false;
    else
    {
        std::string answer = ReadLaser(10);
        return (answer.find("done") != std::string::npos);
    }
}

bool LaserSetup::WriteLaser(std::string command, double extendsleep)
{
    if(!laser.isOpen())
        return false;

    //add line feed if it is not present at the end:
    if(command.c_str()[command.length()-1] != 13)
        command = command + char(13);

    unsigned int sentbytes = laser.write(command.c_str()); //, command.length());
    laser.flush();

    if(extendsleep > 0)
    {
        Timing::Sleep(lasersleep * extendsleep);
        QApplication::processEvents();
    }

    return sentbytes == command.length();
}

std::string LaserSetup::ReadLaser(unsigned int chars)
{
    if(!laser.isOpen())
        return "";

    char* readdata = new char[chars];
    /*unsigned int readchars =*/ laser.read(readdata, chars);

    //if(readchars != chars)
    //    std::cout << "Laser Read Timeout: read only " << readchars << " instead of " << chars << std::endl;

    std::string data = std::string(readdata);

    delete readdata;

    return data;
}

int LaserSetup::GetLaserSleep()
{
    return lasersleep;
}

void LaserSetup::SetLaserSleep(unsigned int ms)
{
    lasersleep = ms;
}

bool LaserSetup::OpenStagePort()
{
    if(stages.isOpen())
        return true;
    else
        return stages.open(QSerialPort::ReadWrite);
}

bool LaserSetup::CloseStagePort()
{
    stages.close();
    return true;
}

bool LaserSetup::InitialiseStages(int referencepoint)
{
    if(!stages.isOpen())
        return false;

    //get axis ranges:
    for(int i = 1; i <= 3; ++i)
    {
        GetMinPosition(i, true);
        GetMaxPosition(i, true);
    }

    //turn on motors:
    for(int i = 1; i <= 3; ++i)
        TurnOnMotor(i);

    //reference the axes:
    for(int i = 1; i <= 3; ++i)
        ReferenceAxis(i, referencepoint);

    //get the current positions:
    for(int i = 1; i <= 3; ++i)
        GetPosition(i, true);

    return true;
}

bool LaserSetup::InitialiseStage(int axis, int referencepoint)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return false;

    //get axis ranges:
    GetMinPosition(axis, true);
    GetMaxPosition(axis, true);

    //turn on motors:
    TurnOnMotor(axis);

    //reference the axes:
    ReferenceAxis(axis, referencepoint);

    //get the current positions:
    GetPosition(axis, true);

    return true;
}

bool LaserSetup::GetInitStatus(int axis)
{
    if(axis > 3)
        return (initialised[0] && initialised[1] && initialised[2]);
    else if(axis < 1)
        return false;
    else
        return initialised[axis-1];
}

bool LaserSetup::TurnOnMotor(int axis)
{
    if(axis > 3 || axis < 1)
        return false;

    std::stringstream s("");
    s << axis << " 0 SVO 1 1"; //turn on motor 1 of this stage driver (for C663 there is only one)
    return WriteStages(s.str());
}

bool LaserSetup::TurnOffMotor(int axis)
{
    if(axis > 3 || axis < 1)
        return false;

    initialised[axis-1] = false;

    std::stringstream s("");
    s << axis << " 0 SVO 1 0"; //turn off motor 1 of this stage driver (for C663 there is only one)
    return WriteStages(s.str());

}

bool LaserSetup::ReferenceAxis(int axis, int referencepoint)
{
    if(axis > 3 || axis < 1)
        return false;

    std::string refstring = "";
    switch(referencepoint)
    {
    case(negLimit):
        refstring = "FNL";
        break;
    case(posLimit):
        refstring = "FPL";
        break;
    case(refPos):
        refstring = "FRF";
        break;
    default:
        return false;
    }

    std::stringstream s("");
    s << axis << " 0 " << refstring;


    if(!WriteStages(s.str()))
        return false;

    initialised[axis-1] = true;

    return true;
}

bool LaserSetup::SetMinPosition(int axis, double minvalue)
{
    if(axis > 3 || axis < 1)
        return false;
    else
    {
        minposition[axis-1] = minvalue;
        return true;
    }
}

double LaserSetup::GetMinPosition(int axis, bool onhardware)
{
    if(axis > 3 || axis < 1)
        return -1e10;

    if(!onhardware)
        return minposition[axis-1];
    else
    {
        std::stringstream s("");
        s << axis << " 0 TMN?";

        if(!WriteStages(s.str()))
            return -1e10;
        std::string answer = ReadStages(15);
        if(answer.substr(0,3).compare(s.str().substr(0,3)) != 0)
            return -1e10;
        else
        {
            std::stringstream s("");
            s << answer.substr(6);
            s >> minposition[axis-1];
            return minposition[axis-1];
        }
    }
}

bool LaserSetup::SetMaxPosition(int axis, double maxvalue)
{
    if(axis > 3 || axis < 1)
        return false;
    else
    {
        maxposition[axis-1] = maxvalue;
        return true;
    }
}

double LaserSetup::GetMaxPosition(int axis, bool onhardware)
{
    if(axis > 3 || axis < 1)
        return -1e10;

    if(!onhardware)
        return maxposition[axis-1];
    else
    {
        std::stringstream s("");
        s << axis << " 0 TMX?";

        if(!WriteStages(s.str()))
            return -1e10;
        std::string answer = ReadStages(15);
        if(answer.c_str()[0] != s.str().c_str()[2] || answer.c_str()[2] != s.str().c_str()[0])
            return -1e10;
        else
        {
            std::stringstream s("");
            s << answer.substr(6);
            s >> maxposition[axis-1];
            return maxposition[axis-1];
        }
    }
}

double LaserSetup::GetPosition(int axis, bool onhardware)
{
    if(axis > 3 || axis < 1)
        return -1e10;

    if(!initialised[axis-1])
        return -1e10;

    if(!onhardware)
        return position[axis-1];
    else
    {
        std::stringstream s("");
        s << axis << " 0 POS?";

        if(!WriteStages(s.str()))
            return -1e10;
        std::string answer = ReadStages(15);
        if(answer.c_str()[0] != s.str().c_str()[2] || answer.c_str()[2] != s.str().c_str()[0])
            return -1e10;
        else
        {
            std::stringstream s("");
            s << answer.substr(6);
            s >> position[axis-1];
            return position[axis-1];
        }
    }

}

double LaserSetup::GetSpeed(int axis)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return -1e10;

    std::stringstream s("");
    s << axis << " 0 VEL? 1";
    if(!WriteStages(s.str()))
        return -1e10;
    else
    {
        std::string answer = ReadStages(40);
        if(answer.c_str()[0] != '0' || answer.c_str()[2] != axis+48)
            return -1e10;
        else
        {
            std::stringstream s("");
            s << answer.substr(6);
            double result = -1e10;
            s >> result;

            return result;
        }
    }
}

bool LaserSetup::SetSpeed(int axis, double speed)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return false;

    if(speed < 0)
        return false;

    std::stringstream s("");
    s << axis << " 0 VEL 1 " << speed;
    return WriteStages(s.str());
}

double LaserSetup::Move(int axis, double length, double backlashcompensation)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return -1e10;

    if(!initialised[axis-1])
        return -1e10;

    if((backlashcompensation > 0 && length < 0)
            || (backlashcompensation < 0 && length > 0))
        length -= backlashcompensation;

    position[axis-1] += length;
    if(length > 0 && position[axis-1] > maxposition[axis-1])
        position[axis-1] = maxposition[axis-1];
    else if(length < 0 && position[axis-1] < minposition[axis-1])
        position[axis-1] = minposition[axis-1];

    std::stringstream s("");
    s << axis << " 0 MOV 1 " << position[axis-1];
    if(!WriteStages(s.str()))
        return -1e10;
    else
    {
        position[axis-1] += backlashcompensation;
        if(position[axis-1] > maxposition[axis-1])
            position[axis-1] = maxposition[axis-1];
        else if(position[axis-1] < minposition[axis-1])
            position[axis-1] = minposition[axis-1];

        std::stringstream s("");
        s << axis << " 0 MOV 1 " << position[axis-1];
        WaitforAxes(axis);

        if(!WriteStages(s.str()))
            return -1e10;
    }


    return position[axis-1];
}

double LaserSetup::MoveAbsolute(int axis, double position, double backlashcompensation)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return -1e10;

    if(!initialised[axis-1])
        return -1e10;

    bool tolerancecorrection = false;
    if((backlashcompensation > 0 && this->position[axis-1] > position)
            || (backlashcompensation < 0 && this->position[axis-1] < position))
    {
        position -= backlashcompensation;
        tolerancecorrection = true;
    }

    if(position > maxposition[axis-1])
        this->position[axis-1] = maxposition[axis-1];
    else if(position < minposition[axis-1])
        this->position[axis-1] = minposition[axis-1];
    else
        this->position[axis-1] = position;

    std::stringstream s("");
    s << axis << " 0 MOV 1 " << this->position[axis-1];
    std::cout << "Movement: \"" << s.str() << "\"" << std::endl;
    if(!WriteStages(s.str()))
        return -1e10;
    else if(tolerancecorrection)
    {
        position += backlashcompensation;
        if(position > maxposition[axis-1])
            this->position[axis-1] = maxposition[axis-1];
        else if(position < minposition[axis-1])
            this->position[axis-1] = minposition[axis-1];
        else
            this->position[axis-1] = position;

        std::stringstream s("");
        s << axis << " 0 MOV 1 " << this->position[axis-1];
        WaitforAxes(axis);

        if(!WriteStages(s.str()))
            return -1e10;
    }

    return this->position[axis-1];
}

bool LaserSetup::AxisOnTarget(int axis)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return false;

    std::stringstream s("");
    s << axis << " 0 ONT?";
    if(!WriteStages(s.str()))
        return false;
    else
    {
        std::string answer = ReadStages(8);
        return (answer.substr(4,3).compare("1=1") == 0);
    }
}

void LaserSetup::WaitforAxes(int axis)
{
    if(axis < 1)
        return;

    bool xdone = (axis == x || axis > 3);
    bool ydone = (axis == y || axis > 3);
    bool zdone = (axis == z || axis > 3);

    while(xdone || ydone || zdone)
    {
        if(xdone && AxisOnTarget(x))
            xdone = false;
        if(ydone && AxisOnTarget(y))
            ydone = false;
        if(zdone && AxisOnTarget(z))
            zdone = false;
        Timing::Sleep(5);
        QApplication::processEvents();
    }
    return;
}

void LaserSetup::StopAxis(int axis)
{
    if(!stages.isOpen() || axis > 3 || axis < 1)
        return;

    std::stringstream s("");
    s << axis << " 0 STP";
    WriteStages(s.str(), 0);
}

bool LaserSetup::WriteStages(std::string command, double extendsleep)
{
    if(!stages.isOpen())
        return false;

    //add line feed if it is not present at the end:
    if(command.c_str()[command.length()-1] != 10)
        command = command + char(10);

    unsigned int sentbytes = uint(stages.write(command.c_str(), command.length()));
    stages.flush();

    if(extendsleep > 0)
    {
        Timing::Sleep(stagesleep * extendsleep);
        QApplication::processEvents();
    }

    return sentbytes == command.length();
}

std::string LaserSetup::ReadStages(unsigned int chars)
{
    if(!stages.isOpen())
        return "";

    char* readdata = new char[chars];
    /*unsigned int readchars =*/ stages.read(readdata, chars);

    //if(readchars != chars)
    //    std::cout << "Stages Read Timeout: read only " << readchars << " instead of " << chars << std::endl;

    std::string data = std::string(readdata);


    delete[] readdata;

    return data;
}

int LaserSetup::GetStageSleep()
{
    return stagesleep;
}

void LaserSetup::SetStageSleep(int ms)
{
    stagesleep = ms;
}

template <size_t N, class T>
std::array<T,N> make_array(const T &v) {
    std::array<T,N> ret;
    ret.fill(v);
    return ret;
}

std::array<double,3> LaserSetup::Scan2D(bool* running, int xaxis, double xstart, double xend,
                                             int yaxis, double ystart, double yend,
                                             double deltax, double deltay,
                                             int measuredelay, bool laseroff,
                                             double (*measure)(double, double, double, void *), void* mainwindow,
                                             std::string filename, std::string measureunit, bool zickzack)
{
    if(!laser.isOpen() || !stages.isOpen() || measure == nullptr || deltax == 0 || deltay == 0)
        return make_array<3>(-1.);

    if(xaxis > 3 || xaxis < 1 || yaxis > 3 || yaxis < 1)
        return make_array<3>(-1.);

    //restrict scan area to accessible area:
    if(xstart < minposition[xaxis-1])
        xstart = minposition[xaxis-1];
    else if(xstart > maxposition[xaxis-1])
        xstart = maxposition[xaxis-1];

    if(xend < minposition[xaxis-1])
        xend = minposition[xaxis-1];
    else if(xend > maxposition[xaxis-1])
        xend = maxposition[xaxis-1];

    if(ystart < minposition[yaxis-1])
        ystart = minposition[yaxis-1];
    else if(ystart > maxposition[yaxis-1])
        ystart = maxposition[yaxis-1];

    if(yend < minposition[yaxis-1])
        yend = minposition[yaxis-1];
    else if(yend > maxposition[yaxis-1])
        yend = maxposition[yaxis-1];

    //adapt the deltas to the measurement direction
    if((xstart < xend) ^ (deltax > 0))
        deltax = -deltax;

    if((ystart < yend) ^ (deltay > 0))
        deltay = -deltay;

    if(laseroff)
        ActivateLaser(false);

    //go to start position:
    MoveAbsolute(xaxis, xstart+((deltax < 0)?0.02:-0.02));
    MoveAbsolute(yaxis, ystart+((deltay < 0)?0.02:-0.02));
    WaitforAxes(all);
    MoveAbsolute(xaxis, xstart);
    MoveAbsolute(yaxis, ystart);
    WaitforAxes(all);

    int localz = 6 - xaxis - yaxis;

    //measurement loop:

    double bestx = xstart;
    double besty = ystart;
    double bestz = GetPosition(localz);
    double bestval = -1e10;

    double currentx = xstart;
    double currenty = ystart;

    std::fstream f;
    if(filename != "")
    {
        f.open(filename.c_str(), std::ios::out | std::ios::app);
        if(!f.is_open())
            std::cout << "Could not open output file \"" << filename << "\"" << std::endl;
        else
        {
            std::string xlabel = "";
            switch(xaxis)
            {
            case(x):xlabel = "x"; break;
            case(y):xlabel = "y"; break;
            case(z):xlabel = "z"; break;
            default:xlabel = "undefined"; break;
            }
            std::string ylabel = "";
            switch(yaxis)
            {
            case(x):ylabel = "x"; break;
            case(y):ylabel = "y"; break;
            case(z):ylabel = "z"; break;
            default:ylabel = "undefined"; break;
            }
            std::string zlabel = "";
            switch(localz)
            {
            case(x):zlabel = "x"; break;
            case(y):zlabel = "y"; break;
            case(z):zlabel = "z"; break;
            default:zlabel = "undefined"; break;
            }


            f << "# Position " << xlabel << "; Position " << ylabel << "; Position " << zlabel << "; Measure\n"
              << "# in mm; in mm; in mm; in " << measureunit << std::endl;
        }
    }

    std::cout << "Scan2D: start actual scan" << std::endl;

    bool done = false;
    while(!done && *running)
    {
        //turn on laser:
        ActivateLaser(true);

        //measure position:
        std::cout << "Current Position: " << currentx << " " << currenty << std::endl;
        double result = measure(currentx, currenty, GetPosition(localz), mainwindow);

        //compare with best result so far:
        if(result > bestval)
        {
            bestx = currentx;
            besty = currenty;
            bestz = GetPosition(localz);
            bestval = result;
        }

        //save new measurement:
        if(f.is_open())
            f << currentx << " " << currenty << " " << GetPosition(localz) << " " << result << std::endl;

        //turn laser off:
        if(laseroff)
            ActivateLaser(false);

        //move to next measurement position:
        if(((deltax > 0) && (currentx < xstart || currentx < xend))
                || (((deltax < 0) && (currentx > xstart || currentx > xend))))
        {
            currentx += deltax;
            Move(xaxis, deltax);    //motor command
        }
        //zick-zack scanning:
        else if(zickzack)
        {
            deltax = -deltax;
            if(((deltay > 0) && (currenty < ystart || currenty < yend))
                    || ((deltay < 0) && (currenty > ystart || currenty > yend)))
            {
                currenty += deltay;
                Move(yaxis, deltay);    //motor command
            }
            else
                done = true;
        }
        //alway the same direction scanning:
        else
        {
            if(((deltay > 0) && (currenty < ystart || currenty < yend))
                    || ((deltay < 0) && (currenty > ystart || currenty > yend)))
            {
                currentx = xstart;
                currenty += deltay;
                MoveAbsolute(xaxis, xstart+((deltax < 0)?0.02:-0.02));
                    //move to the start with overshoot to compensate for direction change tolerance
                WaitforAxes(xaxis);
                MoveAbsolute(xaxis, xstart);    //move to position
                Move(yaxis, deltay);            //move to next row of measurement
            }
            else
                done = true;
        }


        //wait for axes to reach target position:
        WaitforAxes(all);

        Timing::Sleep(measuredelay);
    }

    //turn off laser:
    ActivateLaser(false);

    //add best value note to output file:
    f << "# Best value:" << std::endl << bestx << " " << besty << " " << bestz << " " << bestval << std::endl;

    f.close();

    std::array<double, 3> result;
    result[0] = bestx;
    result[1] = besty;
    result[2] = bestz;

    return result; //std::array<double,3>(bestx, besty, bestz);
}

std::vector<std::string> LaserSetup::GetPortList()
{
    QList<QSerialPortInfo> tempports = QSerialPortInfo::availablePorts();
    std::vector<std::string> ports;
    for (QList<QSerialPortInfo>::Iterator it = tempports.begin(); it != tempports.end(); ++it)
        ports.push_back(it->portName().toStdString());

    return ports;
}
