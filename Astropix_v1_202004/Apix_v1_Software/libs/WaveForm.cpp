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

#include "WaveForm.h"

WaveForm::WaveForm() : name(""), bandwidth(""), valid(true), binformat("BINARY"), bytesPerPoint(0),
                        yoffset(-1e10), scale(-1e10), timestep(-1e10), channeloffset(0), savebinary(true)
{
    //ctor
}

WaveForm::WaveForm(std::string name) : bandwidth(""), valid(true), binformat("BINARY"), 
                                        bytesPerPoint(0), yoffset(-1e10), scale(-1e10), 
                                        timestep(-1e10), channeloffset(0), savebinary(true)
{
    this->name = name;
}

WaveForm::WaveForm(tinyxml2::XMLElement* element) : binformat("BINARY")
{
    name          = checkString(element->Attribute("Name"));
    bandwidth     = checkString(element->Attribute("Bandwidth"));
    yoffset       = element->DoubleAttribute("YOffset");
    scale         = element->DoubleAttribute("Scale");
    bytesPerPoint = static_cast<unsigned short>(element->IntAttribute("BytesPerPoint"));
    timestep      = element->DoubleAttribute("Timestep");
    channeloffset = element->DoubleAttribute("ChannelOffset");
    bool val;
    if(element->QueryBoolAttribute("valid", &val) != tinyxml2::XML_NO_ATTRIBUTE)
        valid = val;
    else
        valid = true;

    //load binary data from the file
    if(element->QueryBoolAttribute("PointData", &val) == tinyxml2::XML_NO_ATTRIBUTE
        || val == false)
    {
        savebinary = true;
        std::string points = element->GetText();

        char hpt;   //"half point" <=> 4 bits
        char value = 0;
        int length = int(points.length());
        int index = 0;
        while(index < length)
        {
            value <<= 4;
            hpt = points.c_str()[index];
            if(hpt < 58 && hpt > 47)
                value += hpt - 48;   //digits '0' to '9'
            else if(hpt < 71 && hpt > 64)
                value += hpt - 55;   //letters 'A' to 'F'
            else if(hpt < 103 && hpt > 96)
                value += hpt - 87;   //letters 'a' to 'f'
            else
            {
                bindata.clear();    //erase all data to be sure no error is included
                break;  //abort on an error
            }

            if(index % 2 == 1)
            {
                bindata.push_back(value);
                value = 0;
            }

            ++index;
        }

        decompressData();
    }
    else    //load point data from the file
    {
        savebinary = false;

        tinyxml2::XMLElement* point = element->FirstChildElement();
        while(point != nullptr)
        {   
            if(std::string(point->Value()).compare("Point"))
            {
                double xval, yval;
                if(point->QueryDoubleAttribute("x", &xval) != tinyxml2::XML_NO_ATTRIBUTE
                    && point->QueryDoubleAttribute("y", &yval) != tinyxml2::XML_NO_ATTRIBUTE)
                    data.push_back(Point(xval,yval));
            }

            if(point != element->LastChildElement())
                point = point->NextSiblingElement();
            else
                point = nullptr;
        }
        
    }
}

WaveForm::~WaveForm()
{
    //dtor
}

bool WaveForm::informationComplete()
{
    if(savebinary && (name == "" || bandwidth == "" || bytesPerPoint == 0 || std::abs(yoffset - (-1e10)) < 1e-3
        || std::abs(scale - (-1e10))< 1e-3 || std::abs(timestep - (-1e10)) < 1e-3 || bindata.size() == 0))
        return false;
    else if(!savebinary && (name == "" || data.size() == 0))
        return false;
    else
        return true;
}

bool WaveForm::getValidity()
{
    return valid;
}

bool WaveForm::setValidity(bool validity)
{
    valid = validity;
    return true;
}

bool WaveForm::setTimestep(double val)
{
    if(val > 0)
    {
        timestep = val;
        return true;
    }

    return false;
}

bool WaveForm::setScale(double val)
{
    if(val > 0)
    {
        scale = val;
        return true;
    }

    return false;
}

bool WaveForm::setYOffset(double val)
{
    //if(val > 0)
    //{
        yoffset = val;
        return true;
    //}

        //return false;
}

bool WaveForm::setChannelOffset(double offset)
{
    channeloffset = offset;

    return true;
}

bool WaveForm::setBytesPerPoint(unsigned short val)
{
    switch(val)
    {
        case(1):
        case(2):
        case(4):
        case(8):
            bytesPerPoint = val;
            return true;
        default:
            return false;
    }
}

bool WaveForm::setBinFormat(std::string val)
{
    if(val == "BINARY")
    {
        binformat = val;
        return true;
    }

    return false;   //different encodings are not supported at the moment
}

bool WaveForm::clearBinaryData()
{
    bindata.clear();
    return true;
}

unsigned int WaveForm::addBinaryData(char* data, unsigned int length)
{
    for(unsigned int i = 0; i < length; ++i)
        bindata.push_back(data[i]);

    return bindata.size();
}

unsigned int WaveForm::addBinaryData(std::string data)
{
    const int length = int(data.size());
    for(int i = 0; i < length; ++i)
        bindata.push_back(data.c_str()[i]);

    return bindata.size();
}

std::vector<char> WaveForm::getBinaryData(unsigned int start, unsigned int ende)
{
    //if(start < 0)
    //    start = 0;
    /*else*/ if(start >= bindata.size()/bytesPerPoint)
        return std::vector<char>();

    //if(ende < 0)
    //    return std::vector<char>();
    /*else*/ if(ende >= bindata.size()/bytesPerPoint)
        ende = bindata.size() / bytesPerPoint - 1;


    std::vector<char> part;
    part.clear();
    ende = ende*bytesPerPoint+bytesPerPoint-1;
    for(unsigned int i = start * bytesPerPoint; i <= ende; ++i)
        part.push_back(bindata[i]);

    return part;
}

unsigned int WaveForm::getBinaryLength(bool points)
{
    if(points)
        return bindata.size() / bytesPerPoint;
    else
        return bindata.size();
}

bool WaveForm::decompressData(bool force)
{
    //overwriting existing data only on force (for savebinary=true nothing is changed)
    if(!savebinary && !force)
        return false;
    if(!informationComplete())
        return false;

    data.clear();

    int point = 0;
    //double value;

    const unsigned int length = bindata.size();

    for(unsigned int i = static_cast<unsigned int>(0); i < length; ++i)
    {
        if(i % bytesPerPoint == 0 && i != 0)
        {
            data.push_back(Point((i / bytesPerPoint) * timestep,
                                 (point * scale) + yoffset + channeloffset));

            point = 0;
        }
            point = point * 256 + bindata[i];

    }

    data.push_back(Point((length / bytesPerPoint - 1) * timestep, (point * scale) + yoffset + channeloffset));

    //std::cout << "decompressed: " << data.size() << " binary: " << bindata.size() << " / "
    //            << bytesPerPoint << std::endl;

    if(data.size() == bindata.size()/bytesPerPoint)
        return true;
    else
        return false;
}

Point WaveForm::getPoint(unsigned int point)
{
    if(point < data.size())
        return data[point];

    return Point(); //returns a point (0,0)
}

double WaveForm::getVoltage(double time, bool interpolate)
{
    std::vector<Point>::iterator it = data.begin();

    while(it->x < time && it != data.end())
        ++it;

    if(it == data.end())
        return -1e10;

    if(interpolate)
    {
        double x2 = it->x;
        double y2 = it->y;
        --it;
        double x1 = it->x;
        double y1 = it->y;

        return y1 * (time - x1)/(x2 - x1) + y2 * (x2 - time)/(x2 - x1);
    }
    else
    {
        //use the closest point to `time`
        //double dt2 = it->x - time;
        double y = it->y;
        --it;
        double dt = time - it->x;
        if(dt > time - it->x)
            return it->y;
        else
            return y;
    }
}

std::vector<Point>::iterator WaveForm::getDataIteratorBegin()
{
	if(data.size() < bindata.size() / bytesPerPoint)
		decompressData();
		
	return data.begin();
}

std::vector<Point>::iterator WaveForm::getDataIteratorEnd()
{
	if(data.size() < bindata.size() / bytesPerPoint)
		decompressData();
		
	return data.end();
}

bool WaveForm::addPoint(Point pnt)
{
    savebinary = false;
    data.push_back(pnt);
    return true;
}

bool WaveForm::addPoint(double x, double y)
{
    savebinary = false;
    data.push_back(Point(x,y));
    return true;
}

std::vector<Point>::iterator WaveForm::deletePoint(std::vector<Point>::iterator pnt)
{
    savebinary = false;
    return data.erase(pnt);
}

void WaveForm::clearData()
{
    savebinary = true;
    data.clear();
}

tinyxml2::XMLElement* WaveForm::getXMLOutput(tinyxml2::XMLDocument* doc)
{
    //return nothing on incomplete waveform:
    if(!informationComplete() || data.size() == 0)
        return nullptr;

    //create an XML element for the waveform:
    tinyxml2::XMLElement* wfm = doc->NewElement("WaveForm");

    //set the parameters as attributes:
    wfm->SetAttribute("Name",          name.c_str()); //escapeString(name).c_str());
    wfm->SetAttribute("BytesPerPoint", bytesPerPoint);
    wfm->SetAttribute("Timestep",      timestep);
    wfm->SetAttribute("Scale",         scale);
    wfm->SetAttribute("YOffset",       yoffset);
    wfm->SetAttribute("ChannelOffset", channeloffset);
    wfm->SetAttribute("Bandwidth",     bandwidth.c_str()); //escapeString(bandwidth).c_str());
    wfm->SetAttribute("valid",         valid);
    wfm->SetAttribute("PointData",     !savebinary);

    if(savebinary)
    {
        //convert the waveform data to write it to the XML:
        std::stringstream xmldata("");

        for(std::vector<char>::iterator it = bindata.begin(); it != bindata.end(); ++it)
        {
            //one byte of data is encoded as 2 bytes HEX code:
            char hbs = (*it >> 4) & 0b00001111; //higher 4 bits (when the MSB is set, '1's are shifted 
                                                // into the char)
            char lbs = *it & 0b00001111;        //lower 4 bits

            if(hbs < 10)
                xmldata << char(hbs + 48);  //add numbers '0' to '9'
            else
                xmldata << char(hbs + 55);  //add letters 'A' to 'F'

            if(lbs < 10)
                xmldata << char(lbs + 48);  //add numbers '0' to '9'
            else
                xmldata << char(lbs + 55);  //add letters 'A' to 'F'
        }

        wfm->SetText(xmldata.str().c_str());
    }
    else
    {
        for(std::vector<Point>::iterator it = data.begin(); it != data.end(); ++it)
        {
            tinyxml2::XMLElement* pnt = doc->NewElement("Point");
            pnt->SetAttribute("x", it->x);
            pnt->SetAttribute("y", it->y);
            wfm->LinkEndChild(pnt);
        }
    }

    return wfm;
}

std::string WaveForm::Print()
{
    std::stringstream filecontent("");

    if(data.size() == 0)
        return "";

    //include header line:
    filecontent << "# " << name << " (with Bandwidth " << bandwidth << "):" << std::endl;

    //write the data to the stringstream:
    for(std::vector<Point>::iterator it = data.begin(); it != data.end(); ++it)
        filecontent << it->x << "\t" << it->y << std::endl;

    return filecontent.str();
}

bool WaveForm::PrintToFile(std::string filename)
{
    if(data.size() == 0)
        return false;

    std::fstream f;
    //check if the file already exists:
    f.open(filename.c_str(), std::ios::in);
    if(f.is_open())
    {
        f.close();
        return false;
    }
    f.close();
    
    //open file for writing:
    f.open(filename.c_str(), std::ios::out);

    //generate and write the data to the file:
    f << Print(); //.str();

    f.close();

    return true;
}

void WaveForm::SmoothenWaveForm(unsigned int windowradius)
{
    if(data.size() < 2*windowradius+1)
        return;

    std::vector<Point> smoothdata;

    double mean = 0;
    int samplesize = int(windowradius);
    for(unsigned int i = 0; i < windowradius; ++i)
        mean += data[i].y;

    for(unsigned int i = 0; i < data.size(); ++i)
    {
        if(i > windowradius)
        {
            mean -= data[i-windowradius].y;
            --samplesize;
        }
        if(i+windowradius < data.size())
        {
            mean += data[i+windowradius].y;
            ++samplesize;
        }
        smoothdata.push_back(Point(data[i].x,mean/samplesize));
    }

    data.clear();
    data.insert(data.end(), smoothdata.begin(), smoothdata.end());
}

double WaveForm::GetSignalHeight(unsigned int numbaselinepoints, bool positivesignal)
{
    if(data.size() < numbaselinepoints)
        return 0;

    //calculate the baseline:
    double baseline = 0;
    for(unsigned int i = 0; i < numbaselinepoints; ++i)
        baseline += data[i].y;
    baseline /= numbaselinepoints;

    //search for maximum (or minimum) data point:
    double max = baseline;
    for(unsigned int i = numbaselinepoints; i < data.size(); ++i)
    {
        if(positivesignal && data[i].y > max)
            max = data[i].y;
        else if(!positivesignal && data[i].y < max)
            max = data[i].y;
    }

    //calculate difference to baseline:
    if(positivesignal)
        max -= baseline;
    else
        max = baseline - max;

    return max;
}
