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
 *
 * Original implementation from Felix Ehrler
 */

#include "vb_config.h"

VB_Config::VB_Config() : devicename("")
{
    VDD = 0;
    Vmax = 0;
}

VB_Config::VB_Config(double VDD, double Vmax) : devicename("")
{
    this->VDD = VDD;
    if (Vmax > VDD)
        Vmax = VDD;
    this->Vmax = Vmax;
}

VB_Config::VB_Config(unsigned int NumVB, double VDD, double Vmax) : devicename("")
{
    this->VDD = VDD;
    if (Vmax > VDD)
        Vmax = VDD;
    this->Vmax = Vmax;
    for (unsigned int i = 0; i < NumVB; ++i)
    {
        for (int j = 0; j <5; ++j)
        {
            std::stringstream s("");
            s << "VB" << i << "V" << j;
            AddDAC(s.str(), 14,2,1,0);
            //std::cout << name << " added!" << std::endl;
        }
    }
}

VB_Config::~VB_Config()
{

}

std::string VB_Config::GetDeviceName()
{
    return devicename;
}

void VB_Config::SetDeviceName(std::string name)
{
    devicename = name;
}

VB_Config& VB_Config::AddDAC(std::string name, int bits, int sparebits, int shiftdirection, double initialVoltage)
{
    indextoname.insert(std::make_pair(values.size(), name));
    nametoindex.insert(std::make_pair(name, values.size()));

    shiftdir.push_back(shiftdirection);
    width.push_back(std::make_pair(bits, sparebits));
    if (initialVoltage < 0 || initialVoltage > Vmax) {
        values.push_back(0);
        throw std::runtime_error( (QString("Vini parameter must be between 0 and ") + Vmax).toStdString());
    } else {
        // Save initial voltage in values list
        values.push_back(initialVoltage);
    }

    return *this;
}

VB_Config& VB_Config::addLoadShiftRegister(unsigned int position) {

    if (position < 1 || position > 8) {
        throw std::runtime_error("Card Position must be 1-8");
    }

    return this->AddDAC(
                (QString().asprintf("Ld_%d",position)).toStdString(),
                -8,
                0,
                1,
                (1<<(position-1)));
}

void VB_Config::ClearDACs()
{
    values.clear();
    nametoindex.clear();
    indextoname.clear();
    shiftdir.clear();
    width.clear();
}

int VB_Config::GetDACIndexToName(std::string name)
{
    auto index = nametoindex.find(name);

    if(index != nametoindex.end())
        return index->second;
    else
        return -1;
}

std::string VB_Config::GetDACNameToIndex(unsigned int index)
{
    auto name = indextoname.find(int(index));

    if(name != indextoname.end())
        return name->second;
    else
        return "";
}

double VB_Config::GetDAC(unsigned int index)
{
    if(index >= values.size())
        return -1;
    else
        return values[index];
}

double VB_Config::GetDAC(std::string name)
{
    auto index = nametoindex.find(name);
    if(index != nametoindex.end())
        return values[static_cast<unsigned int>(index->second)];
    else
        return -1;
}

int VB_Config::GetDACWidth(unsigned int index)
{
    if(index >= width.size())
        return -1;
    else
        return width[index].first;
}

int VB_Config::GetDACWidth(std::string name)
{
    auto index = nametoindex.find(name);
    if(index != nametoindex.end())
        return width[static_cast<unsigned int>(index->second)].first;
    else
        return -1;
}

int VB_Config::GetDACSpare(unsigned int index)
{
    if(index >= width.size())
        return -1;
    else
        return width[index].second;
}

int VB_Config::GetDACSpare(std::string name)
{
    auto index = nametoindex.find(name);
    if(index != nametoindex.end())
        return width[static_cast<unsigned int>(index->second)].second;
    else
        return -1;
}

int VB_Config::GetDACBitOrder(unsigned int index)
{
    if(index >= shiftdir.size())
        return -1;
    else
        return shiftdir[index];
}

int VB_Config::GetDACBitOrder(std::string name)
{
    auto index = nametoindex.find(name);
    if(index == nametoindex.end())
        return -1;
    else
        return shiftdir[static_cast<unsigned int>(index->second)];
}

std::string VB_Config::GetDACName(unsigned int index)
{
    return GetDACNameToIndex(index);
}

bool VB_Config::SetDAC(unsigned int index, double value)
{
    if(index >= values.size())
    {
        std::cout << "Index out of range!" << std::endl;
        return false;
    }

    if (value > Vmax)
    {
        std::cout << "Value exceeds maximum voltage!" << std::endl;
        return false;
    }

    if (value > VDD)
    {
        std::cout << "Value exceeds supply voltage!" << std::endl;
        return false;
    }

    if (value < 0)
    {
        std::cout << "Value can not be smaller than 0!" << std::endl;
        return false;
    }

    unsigned int size = 1 << GetDACWidth(index);

    if(value >= size)
    {
        std::cout << "Value out of range!" << std::endl;
        return false;
    }

     std::cout << "Value changed to: " << value << std::endl;
    values[index] = value;
    return true;
}

bool VB_Config::SetDAC(std::string name, double value)
{
    auto index = nametoindex.find(name);

    if (index == nametoindex.end())
    {
        std::cout << "No DAC named \"" << name << "\" was found!" << std::endl;
        return false;
    }
    return SetDAC(static_cast<unsigned int>(index->second), value);
}

std::vector<bool> VB_Config::GenerateBitVector(int shiftdirection)
{
    //return an empty vector on invalid shiftdirection
    if(shiftdirection != MSBFirst && shiftdirection != LSBFirst)
        return std::vector<bool>();

    std::vector<bool> bitvector;

    std::cout << "Generating vBit Vector for " << this->GetDeviceName() << ", number of widths: " << width.size() << std::endl;
    std::cout << std::flush;

    unsigned int index = values.size();
    for(auto it = values.end(); it != values.begin(); --it)
    {
        index--;

        auto widthSpec = width[index];
        auto dacWidth = widthSpec.first;
        auto dacSparebits = widthSpec.second;

        //std::cout << "Generating value for " << GetDACName(index) << " -W="<<dacWidth <<"/S=" << dacSparebits << std::endl;
       // std::cout << std::flush;

        // Spare bits: add before DAC bits if value is negative
        //-----------------

        if (width[index].second < 0)
            for (int i = 0; i < abs(width[index].second); ++i)
                bitvector.push_back(false);

        //std::cout << *it << std::endl;
        int Steps;
        if(width[index].first > 0)
            Steps = int(floor(((1 << (width[index].first))-1) * values[index]/VDD));
        else
            Steps = int(floor(((1 << (-(width[index].first)))-1) * values[index]));
        //std::cout << Steps << std::endl;
        for (int ibit=0; ibit<std::abs(width[index].first); ibit++)
        {
            if (shiftdir[index] == MSBFirst)
                bitvector.push_back((Steps & (1<<ibit))?true:false);
            else
                bitvector.push_back((Steps & (1<<((std::abs(width[index].first)-1)-ibit)))?true:false);
        }

        // Spare bits: add after DAC bits if value is positive
        //-----------------
        if (width[index].second > 0)
            for (int i = 0; i < abs(width[index].second); ++i)
                bitvector.push_back(false);
    }

    return bitvector;
}

tinyxml2::XMLError VB_Config::LoadFromXMLElement(tinyxml2::XMLElement *root)
{
    if(root->ToElement()->QueryDoubleAttribute("Vdd",&VDD) != tinyxml2::XML_NO_ERROR)
        VDD = 0;
    if(root->ToElement()->QueryDoubleAttribute("Vmax",&Vmax) != tinyxml2::XML_NO_ERROR)
        Vmax = 0;

    ClearDACs();

    devicename = root->Attribute("name");

    tinyxml2::XMLElement* child = root->FirstChildElement();

    while(child != nullptr)
    {
        if(std::string(child->Value()).compare("Register") == 0)
        {
            int bits = 0;
            int spares = 0;
            int writedirection = LSBFirst;

            double value;
            if(child->QueryDoubleAttribute("value", &value) != tinyxml2::XML_NO_ERROR)
                value = 0;

            tinyxml2::XMLElement* grandchild = child->FirstChildElement();
            while(grandchild != nullptr)
            {
                if(std::string(grandchild->Name()).compare("Attribute") == 0)
                {
                    std::string name = std::string(grandchild->Attribute("name"));
                    if(name.compare("sr.size") == 0)
                    {
                        if(grandchild->QueryIntText(&bits) != tinyxml2::XML_NO_ERROR)
                            bits = 0;
                    }
                    else if(name.compare("sr.WriteDirection") == 0)
                    {
                        if(std::string(grandchild->GetText()).compare("LSBFirst") == 0)
                            writedirection = LSBFirst;
                        else if(std::string(grandchild->GetText()).compare("MSBFirst") == 0)
                            writedirection = MSBFirst;
                        else if(grandchild->QueryIntText(&writedirection) != tinyxml2::XML_NO_ERROR)
                            writedirection = LSBFirst;
                    }
                    else if(name.compare("sr.spares") == 0)
                    {
                        if(grandchild->QueryIntText(&spares) != tinyxml2::XML_NO_ERROR)
                            spares = 0;
                    }
                }

                if(grandchild != child->LastChildElement())
                    grandchild = grandchild->NextSiblingElement();
                else
                    grandchild = nullptr;
            }

            if(bits != 0)
                AddDAC(std::string(child->Attribute("name")), bits, spares, writedirection, value);
        }


        if(child != root->LastChildElement())
            child = child->NextSiblingElement();
        else
            child = nullptr;
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError VB_Config::LoadFromXMLFile(std::string filename, bool compressed)
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

    tinyxml2::XMLNode* config = getNode(&doc, "ShiftRegister");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLElement *VB_Config::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string devicename)
{
    tinyxml2::XMLElement* node = doc.NewElement("ShiftRegister");

    //Add save date to the file:
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    //add the configuration:
    if(devicename != "")
        node->SetAttribute("name", devicename.c_str());
    else
        node->SetAttribute("name", this->devicename.c_str());
    std::string datetime = std::ctime(&time);
    if(datetime.c_str()[datetime.length()-1] == '\n' || datetime.c_str()[datetime.length()-1] == '\x0a')
        datetime = datetime.substr(0, datetime.length()-1);
    node->SetAttribute("SaveDate", datetime.c_str());

    node->SetAttribute("Vdd", VDD);
    node->SetAttribute("Vmax", Vmax);

    tinyxml2::XMLElement* configpar;

    for(unsigned int i = 0; i < values.size(); ++i)
    {
        configpar = doc.NewElement("Register");
        configpar->SetAttribute("name", indextoname.find(i)->second.c_str());
        configpar->SetAttribute("value",values[i]);

        tinyxml2::XMLElement* attribute;
        attribute = doc.NewElement("Attribute");
        attribute->SetAttribute("name","sr.size");
        attribute->SetText(width[i].first);
        configpar->LinkEndChild(attribute);

        attribute = doc.NewElement("Attribute");
        attribute->SetAttribute("name","sr.spares");
        attribute->SetText(width[i].second);
        configpar->LinkEndChild(attribute);

        attribute = doc.NewElement("Attribute");
        attribute->SetAttribute("name", "sr.WriteDirection");
        attribute->SetText(shiftdir[i]);
        configpar->LinkEndChild(attribute);

        node->LinkEndChild(configpar);
    }

    return node;
}

tinyxml2::XMLError VB_Config::SaveToXMLFile(std::string filename, std::string devicename, bool compressed)
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


double VB_Config::GetVmax()
{
    return Vmax;
}

bool VB_Config::SetVmax(double value)
{
    if (value > VDD)
    {
        Vmax = VDD;
        return false;
    }
    Vmax = value;
    return true;
}

double VB_Config::GetVDD()
{
    return VDD;
}

void VB_Config::SetVDD(double value)
{
    VDD = value;
}
