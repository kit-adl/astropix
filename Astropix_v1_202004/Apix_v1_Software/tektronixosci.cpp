#include "tektronixosci.h"

TektronixOsci::TektronixOsci() : defaultRM(VI_NULL), instr(VI_NULL)
{

}

std::vector<std::string> TektronixOsci::ScanForDevices()
{
    std::vector<std::string> devicenames;
    if(instr != VI_NULL)
        return devicenames;

    //open the default manager handle:
    status=viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
    {
        std::cout << "Could not open a session to the VISA Resource Manager!" << std::endl;
        return devicenames;
    }

    //get a list of all USB devices:
    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, (ViString)"USB?*INSTR", &findList, &numInstrs,
                        instrResourceString);

    if (status < VI_SUCCESS)
    {
       std::cout << "An error occurred while searching resources." << std::endl;
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
           std::cout << "Cannot open a session to device " << (i+1) << std::endl;
           continue;
        }

        //get the identification of the device:
        ViUInt32 writeCount;
        status = viWrite(instr, (ViBuf)"*IDN?\n", (ViUInt32)6, &writeCount);
        if (status < VI_SUCCESS)
        {
            std::cout << "Error writing to device " << (i+1) << std::endl;
            status = viClose(instr);
            continue;
        }

        //read back the device identification:
        ViUInt32 retCount;
        unsigned char buffer[100];
        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
            std::cout << "Error reading a response from device " << (i+1) << std::endl;
        else
        {
            std::string name = std::string(reinterpret_cast<char*>(buffer));
            //std::string res  = std::string(instrResourceString);
            //resource.push_back(res); //instrResourceString);
            resname.push_back(name);
            std::cout << "Device " << resname.size() << ": \"" << name << "\"" << std::endl
                      << "    Resource: " << instrResourceString << std::endl;
        }
        status = viClose(instr);
    }


    //close the resource manager handle:
    status = viClose(defaultRM);

    return resname;
}

int TektronixOsci::GetNumDevicesFound()
{
    return resname.size();
}

bool TektronixOsci::open(unsigned int index)
{
    //if(instr != VI_NULL)
    //    return false;

    status = viOpenDefaultRM(&defaultRM);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error opening default Resource Manager" << std::endl;
        return false;
    }

    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, (ViString)"USB?*INSTR", &findList, &numInstrs,
                        instrResourceString);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting Resource List" << std::endl;
        return false;
    }

    if(index > numInstrs)
        return false;

    //get the descriptor for the requested resource
    for(unsigned int i=0;i < index; ++i)
        viFindNext(findList, instrResourceString);

    status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error opening device " << index << std::endl;
        return false;
    }

    return true;
}

/*bool TektronixOsci::open(char* resourceString)
{
    //check for opened hardware manager:
    if(defaultRM == VI_NULL)
    {
        status=viOpenDefaultRM(&defaultRM);
        if(status < VI_SUCCESS)
        {
            std::cout << "Error: Could not open resource manager!" << std::endl;
            return false;
        }
    }

    //close an already opened device:
    if(instr != VI_NULL)
        viClose(instr);
    //open the new device:
    status = viOpen(defaultRM, resourceString, VI_NULL, VI_NULL, &instr);

    if(status < VI_SUCCESS)
        std::cout << "Error: Could not open resource \"" << resourceString << "\"" << std::endl;

    return status;
}*/

bool TektronixOsci::open(std::string description)
{
    if(instr != VI_NULL || defaultRM == VI_NULL)
        return false;


    ViUInt32 numInstrs;
    ViFindList findList;
    status = viFindRsrc(defaultRM, (ViString)"USB?*INSTR", &findList, &numInstrs,
                        instrResourceString);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting Resource List" << std::endl;
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
        std::cout << "Error opening resource \"" << instrResourceString << "\"" << std::endl;
        return false;
    }

    return true;
}

bool TektronixOsci::is_open()
{
    return (instr != VI_NULL);
}

void TektronixOsci::close()
{
    viClose(instr);
    instr = VI_NULL;
    viClose(defaultRM);
    std::cout << "Session Closed." << std::endl;
}

std::string TektronixOsci::Read(int characters)
{
    //only read when a session is open:
    if(instr == VI_NULL)
        return "";

    std::cout << "read " << characters << " characters..." << std::endl;

    ViChar* buffer = new ViChar[characters];
    ViUInt32 readchars;
    status = viRead(instr, (ViBuf)buffer, characters, &readchars);

    std::string data = std::string(reinterpret_cast<char*>(buffer));
    data = data.substr(0, readchars);
    std::cout << readchars << " characters read: \"" << data << "\"" << std::endl;

    delete buffer;
    return data;
}

int TektronixOsci::Write(std::string data)
{
    //only try to write when a session is open:
    if(instr == VI_NULL)
        return 0;

    if(data.c_str()[data.length()-1] != '\n')
        data = data + std::string("\n");

    std::cout << "Write: \"" << data.substr(0,data.length()-1) << "\" (" << data.length() << " characters)" << std::endl;

    ViUInt32 writtenchars;
    status = viWrite(instr, (ViBuf)(data.c_str()), data.length(), &writtenchars);

    std::cout << "  ... " << writtenchars << " characters written." << std::endl;

    return writtenchars;
}

double TektronixOsci::QueryF(std::string query)
{
    if(instr == VI_NULL)
        return -1e10;

    float fval;
    status = viQueryf(instr,(ViString)query.c_str(),(ViString)"%f", &fval);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error querying \"" << query << "\"" << std::endl;
        return -1e10;
    }

    return fval;
}

int TektronixOsci::QueryI(std::string query)
{
    if(instr == VI_NULL)
        return -1023*1024;

    int ival;
    status = viQueryf(instr,(ViString)query.c_str(),(ViString)"%i", &ival);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error querying \"" << query << "\"" << std::endl;
        return -1023*1024;
    }

    return ival;
}

bool TektronixOsci::SetUpWaveFormRead(std::string channel, int start, int stop)
{
    if(instr == VI_NULL)
        return false;

    bool result = true;
    result &= (Write(std::string("data:Source ") + channel) > 0);
    std::stringstream s("");
    s << "data:start " << start;
    result &= (Write(s.str()) > 0);
    s.str("");
    s << "data:stop " << stop;
    result &= (Write(s.str()) > 0);

    return result;
}

bool TektronixOsci::SetupDataEncoding()
{
    if(instr == VI_NULL)
        return false;

    ViUInt32 writtenchars;

    //--- set transmission format --
    status = viWrite(instr, (ViBuf)"DATA:ENCDG RIBINARY\n", 20, &writtenchars);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error: Could not send data transmission format." << std::endl;
        return false;
    }

    status = viWrite(instr, (ViBuf)"WFMOutpre:BYT_Nr 1\n", 19, &writtenchars);

    if(status < VI_SUCCESS)
    {
        std::cout << "Error: Could not send data transmission precision." << std::endl;
        return false;
    }

    return true;
}

double TektronixOsci::GetTimeStep()
{
    if(instr == VI_NULL)
        return -1;

    float timescale;
    status = viQueryf(instr, (ViString)"WFMOutpre:XINcr?\n", (ViString)"%f", &timescale);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting timescale" << std::endl;
        return -1;
    }
    else
        return timescale;
}

double TektronixOsci::GetYOffset()
{
    if(instr == VI_NULL)
        return -1e10;

    float yoffset;
    status = viQueryf(instr, (ViString)"WFMOutpre:YOFF?\n", (ViString)"%f",
                      &yoffset);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting yoffset" << std::endl;
        return -1e10;
    }
    else
        return yoffset;
}

double TektronixOsci::GetYMultiplier()
{
    if(instr == VI_NULL)
        return -1e10;

    float ymult;
    status = viQueryf(instr, (ViString)"WFMOutpre:YMULT?\n", (ViString)"%f",
                      &ymult);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting Y Multiplier" << std::endl;
        return -1e10;
    }

    return ymult;
}

double TektronixOsci::GetChannelOffset(std::string channel)
{
    if(instr == VI_NULL)
        return -1e10;

    float choffset;
    status = viQueryf(instr, (ViString)((channel + ":offset?\n").c_str()), (ViString)"%f", &choffset);
    if(status < VI_SUCCESS)
    {
        std::cout << "Error getting channel offset" << std::endl;
        return -1e10;
    }

    return choffset;
}

std::string TektronixOsci::GetWaveForm()
{
    if(instr == VI_NULL)
        return "";

    ViUInt32 writtenchars;
    ViUInt32 readchars;

    //request the curve:
    status = viWrite(instr, (ViBuf)"CURVE?\n", 7, &writtenchars);

    //read the start:
    ViChar buffer[50];
    status = viRead(instr, (ViBuf)buffer, 2, &readchars);

    //the first character of the waveform is a '#':
    if(buffer[0] != '#')
    {
        std::cout << "Error The data does not represent a waveform" << std::endl;
        return "";
    }
    //the second byte gives the length of the following length in bytes:
    int length = buffer[1] - '0';

    //read the length of the waveform to read:
    status = viRead(instr, (ViBuf)buffer, length, &readchars);
    //convert the ASCI number to an integer number:
    long bufferlength = 0;
    for(int i=0;i<length; ++i)
        bufferlength = bufferlength * 10 + (buffer[i] - '0');

    //a buffer for the waveform data:
    ViChar* waveform = new ViChar[bufferlength];

    status = viRead(instr, (ViBuf)waveform, bufferlength, &readchars);

    std::string data;

    for(unsigned int i=0;i<readchars;++i)
        data = data + waveform[i];

    //data = data.substr(0,readchars);

    std::cout << "Waveform transmitted: " << readchars << " Points." << std::endl;

    return data;
}

bool TektronixOsci::DecodeAndSaveWaveForm(double timestep, double yoffset, double ymultiplier,
                                          std::string data, double dataoffset, std::string filename)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);
    if(!f.is_open())
    {
        std::cout << "Error: Could not open Output file \"" << filename << "\"" << std::endl;
        return false;
    }

    f << "# Timestep: " << timestep << std::endl;

    std::stringstream s("");

    int length = data.length();

    for(int i = 0; i < length; ++i)
    {
        s << ((static_cast<double>(data.c_str()[i])-yoffset) * ymultiplier + dataoffset) << std::endl;
    }

    f << s.str();

    f.close();

    std::cout << "Written " << length << " characters to \"" << filename << "\"" << std::endl;

    return true;
}
