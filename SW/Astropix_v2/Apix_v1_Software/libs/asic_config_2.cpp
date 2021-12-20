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

#include "asic_config_2.h"

ASIC_Config2::ASIC_Config2(std::string devicename)
{
    this->devicename = devicename;
}

std::string ASIC_Config2::GetDeviceName()
{
    return devicename;
}

void ASIC_Config2::SetDeviceName(std::string name)
{
    devicename = name;
}

ASIC_Config2& ASIC_Config2::AddParameter(std::string name, unsigned int bits, int shiftdirection, unsigned int initial, bool optional)
{
    if(bits <1 || bits > 30) {
        throw std::runtime_error("Number of bits must be between 1 and 30");
    }

    //set initial value to maximum, if it is larger than the parameter
    if(initial >= ((unsigned int)1 << bits))
        initial = (1 << bits) - 1;

    indextoname.insert(std::make_pair(parameters.size(), name));
    nametoindex.insert(std::make_pair(name, parameters.size()));

    std::stringstream bitorder("");
    if(optional)
        bitorder << "o";
    switch(shiftdirection)
    {
    case(MSBFirst):
        bitorder << (bits-1);
        for(int i = (int)bits-2; i >= 0; --i)
            bitorder << "," << i;
        break;
    case(LSBFirst):
        bitorder << 0;
        for(int i = 1; i < (int)bits; ++i)
            bitorder << "," << i;
        break;
    default:
        throw std::runtime_error("Shift Direction unknown");
    }

    parameters.push_back(std::make_pair(bitorder.str(), initial));

    return *this;
}

ASIC_Config2& ASIC_Config2::AddParameter(std::string name, std::string bitorder, unsigned int initial, bool optional)
{
    //the parameter is eihter empty or too large for this class:
    if(bitorder.length() < 1 || bitorder.length() > 113) {
        throw std::runtime_error("Bit Order length must be between 1 and 113");
    }

    std::string bitorder_prefix = "";
    if(bitorder.c_str()[0] == 'o')
    {
        bitorder_prefix = "o";
        bitorder = bitorder.substr(1);
    }
    else if(optional)
        bitorder_prefix = "o";

    //check the bitorder string for errors:
    for(unsigned int i = 0; i < bitorder.size(); ++i)
    {
        if(bitorder.c_str()[i] > '9' || (bitorder.c_str()[i] < '0' && bitorder.c_str()[i] != ',' && bitorder.c_str()[i] != '!'))
           throw std::runtime_error("Bit Order string format incorrect");
    }

    //check for completeness:
    int bitvector = 0;
    int bitvalue;
    unsigned int pos = 0;
    unsigned int oldpos = 0;
    bool wasnpos = false;
    while(!wasnpos) //pos != std::string::npos)
    {
        pos = bitorder.find(',', oldpos);
        if(pos == uint(std::string::npos))
        {
            pos = bitorder.length();
            wasnpos = true;
        }
        //check for inverted bit markers
        //   ... at beginning:
        if(bitorder.c_str()[oldpos] == '!')
            ++oldpos;
        //  ... at end:
        if(bitorder.c_str()[pos-1] == '!')
            --pos;
        //  ... in the middle, there should not be a '!', abort on this
        if(pos - oldpos > 1)
        {
            for(unsigned int i = oldpos; i < pos; ++i)
                if(bitorder.c_str()[i] == '!')
                    throw std::runtime_error("Bit Order contains ! in the middle of the string!");
        }
        std::stringstream s("");
        s << bitorder.substr(oldpos, pos - oldpos);
        s >> bitvalue;

        bitvector |= 1 << bitvalue;
        oldpos = pos + 1;
    }

    //check for missing bits:
    for(int i=1; i < 30; ++i)
    {
        if(bitvector < (1 << i) - 1)//this is 0001111111 Ivan
            throw std::runtime_error("Bit vector wrong");
        else if(bitvector == (1 << i) - 1)
            break;
    }

    indextoname.insert(std::make_pair(parameters.size(), name));
    nametoindex.insert(std::make_pair(name, parameters.size()));

    parameters.push_back(std::make_pair(bitorder_prefix + bitorder, initial));

    return *this;
}

void ASIC_Config2::ClearParameters()
{
    parameters.clear();
    nametoindex.clear();
    indextoname.clear();
}

int ASIC_Config2::GetParameterIndexToName(std::string name)
{
    auto index = nametoindex.find(name);

    if(index != nametoindex.end())
        return index->second;
    else
        return -1;
}

std::string ASIC_Config2::GetParameterNameToIndex(unsigned int index)
{
    auto name = indextoname.find(int(index));

    if(name != indextoname.end())
        return name->second;
    else
        return "";
}

int ASIC_Config2::GetParameter(unsigned int index)
{
    if(index >= parameters.size())  //return -1 for invalid indices
        return -1;
    else
        return parameters[index].second;
}

int ASIC_Config2::GetParameter(std::string name)
{
    auto index = nametoindex.find(name);
    //check for validity of the returned index:
    if(index != nametoindex.end())
        return parameters[uint(index->second)].second;
    else    //return -1 for invalid names
        return -1;
}

int ASIC_Config2::GetParameterWidth(unsigned int index)
{
    if(index >= parameters.size())
        return -1;

    //count the commas, as they separate the bits -> #commas+1 = #bits
    int bits = 0;
    // take care of optional parameter marker at the beginning of the string:
    unsigned int pos = 0; //(parameters[index].first.c_str()[0] == 'o')?1:0; //',' does not care about 'o'
    std::stringstream s("");
    while(pos != uint(std::string::npos))
    {
        ++bits;
        pos = parameters[index].first.find(',', pos+1);
    }

    if(parameters[index].first.c_str()[parameters[index].first.length()-1] == ',')
        --bits;

    return bits;
}

int ASIC_Config2::GetParameterWidth(std::string name)
{
    auto index = nametoindex.find(name);
    //check for validity of the returned index:
    if(index == nametoindex.end())
        return -1;

    return GetParameterWidth(uint(index->second));
}

std::string ASIC_Config2::GetParameterBitOrder(unsigned int index)
{
    if(index >= parameters.size())
        return "";
    else
        return parameters[index].first;
}

std::string ASIC_Config2::GetParameterBitOrder(std::string name)
{
    auto index = nametoindex.find(name);
    if(index == nametoindex.end())
        return "";
    else if(parameters[uint(index->second)].first.c_str()[0] == 'o')
        return parameters[uint(index->second)].first.substr(1);
    else
        return parameters[uint(index->second)].first;
}

std::string ASIC_Config2::GetParameterName(unsigned int index)
{
    auto name = indextoname.find(int(index));

    if(name != indextoname.end())
        return name->second;
    else
        return "";
}

bool ASIC_Config2::ParameterIsOptional(unsigned int index)
{
    if(index >= parameters.size())
        return true;
    else
        return (parameters[index].first.c_str()[0] == 'o');
}

bool ASIC_Config2::ParameterIsOptional(std::string name)
{
    auto index = nametoindex.find(name);

    if(index == nametoindex.end())
        return true;
    else
        return (parameters[uint(index->second)].first.c_str()[0] == 'o');
}

bool ASIC_Config2::SetParameterOptionality(unsigned int index, bool optional)
{
    if(index >= parameters.size())
        return false;

    if(optional)
    {
        if(parameters[index].first.c_str()[0] == 'o')
            return true;
        else
        {
            parameters[index].first = std::string("o") + parameters[index].first;
            return true;
        }
    }
    else
    {
        if(parameters[index].first.c_str()[0] != 'o')
            return true;
        else
        {
            parameters[index].first = parameters[index].first.substr(1);
            return true;
        }
    }
}

bool ASIC_Config2::SetParameterOptionality(std::string name, bool optional)
{
    auto index = nametoindex.find(name);

    if(index == nametoindex.end())
        return false;
    else
        return SetParameterOptionality(uint(index->second), optional);
}

bool ASIC_Config2::SetParameter(unsigned int index, unsigned int value)
{
    if(index >= parameters.size())
    {
        std::cout << "Index out of range!" << std::endl;
        return false;
    }


    unsigned int size = 1 << GetParameterWidth(index);

    if(value >= size)
    {
        std::cout << "Value out of range!" << std::endl;
        return false;
    }

    parameters[index].second = int(value);




    return true;
}

bool ASIC_Config2::SetParameter(std::string name, unsigned int value)
{
    auto index = nametoindex.find(name);

    if(index == nametoindex.end())
    {
        std::cout << "Name \"" << name << "\" was not found in " << devicename << "!" << std::endl;
        return false;
    }

    return SetParameter(uint(index->second), value);
}

std::vector<bool> ASIC_Config2::GenerateBitVector(int shiftdirection, bool optout)
{
    //return an empty vector on invalid shiftdirection
    if(shiftdirection < 0 || shiftdirection > 3)
        return std::vector<bool>();

    bool globalinvert = (shiftdirection == GlobalInvertedLSBFirst || shiftdirection == GlobalInvertedMSBFirst);
    if(shiftdirection == GlobalInvertedLSBFirst)
        shiftdirection = MSBFirst;
    else if (shiftdirection == GlobalInvertedMSBFirst)
        shiftdirection = LSBFirst;

    std::vector<bool> bitvector;

    //int index = 0;
    for(auto it = parameters.begin(); it != parameters.end(); ++it)
    {
        if(optout && it->first.c_str()[0] == 'o')
            continue;

        std::string bitorder = it->first;
        if(bitorder.c_str()[0] == 'o')
            bitorder = bitorder.substr(1);

        //shows what was put into the bitstream:
        //std::cout << indextoname.find(index)->second << " " << it->first << std::endl;
        //++index;  //needed for the name output of the contents of the bitstream

        //send LSB first on reversed parameter XOR reversed shift direction:
        if(shiftdirection == MSBFirst)
        {
            unsigned int pos = 0;
            unsigned int oldpos = 0;
            int value = 0;
            while(pos != uint(std::string::npos))
            {
                std::stringstream s("");
                pos = bitorder.find(',', oldpos);
                //check for inversion marker '!':
                //  ... at beginning:
                int invert = 0;
                if(bitorder.c_str()[oldpos] == '!')
                {
                    ++oldpos;
                    invert = 1;
                }
                //  ... at end:
                else if(pos != uint(std::string::npos) && bitorder.c_str()[pos - 1] == '!')
                {
                    --pos;
                    invert = 2;
                }

                if(pos != uint(std::string::npos))
                    s << bitorder.substr(oldpos, pos - oldpos);
                else
                    s << bitorder.substr(oldpos);
                s >> value;
                oldpos = pos + ((invert == 2)?2:1);
                bitvector.push_back(((it->second & (1 << value))?true:false) ^ (invert != 0));//uvan this is important, it picks value-th bit from data staoerd in parameters
            }
        }
        //normally, the MSB is sent first on sending MSBFirst
        else
        {
            unsigned int pos = bitorder.length();
            unsigned int oldpos = uint(std::string::npos);
            int value = 0;
            while(pos != uint(std::string::npos))
            {
                std::stringstream s("");
                pos = bitorder.rfind(',', oldpos);
                int invert = 0;

                if(pos != uint(std::string::npos))
                {
                    //check for inversion marker '!':
                    //  ... at beginning:
                    if(bitorder.c_str()[pos + 1] == '!')
                    {
                        ++pos;
                        invert = 1;
                    }
                    else if(oldpos != uint(std::string::npos) && bitorder.c_str()[oldpos] == '!')
                    {
                        --oldpos;
                        invert = 2;
                    }
                    else if(oldpos == uint(std::string::npos) && bitorder.c_str()[bitorder.length() - 1] == '!')
                    {
                        --oldpos;
                        invert = 3;
                    }
                    s << bitorder.substr(pos + 1, oldpos - pos);
                    s >> value;
                }
                else
                {
                    if(bitorder.c_str()[0] == '!')
                    {
                        pos = 1;
                        invert = 1;
                    }
                    else
                        pos = 0;
                    if(oldpos == uint(std::string::npos) && bitorder.c_str()[bitorder.length() - 1] == '!')
                    {
                        oldpos = bitorder.length() - 1;
                        invert = 2;
                    }
                    else if(oldpos == uint(std::string::npos))
                        oldpos = bitorder.length();
                    s << bitorder.substr(pos, oldpos - pos);
                    s >> value;

                    pos = uint(std::string::npos);
                }

                oldpos = pos - ((invert == 1)?2:1);
                bitvector.push_back(((it->second & (1 << value))?true:false) ^ (invert != 0));
            }
        }
    }

    if (globalinvert)
    {
        std::vector<bool> temp;
        temp.insert(temp.end(), bitvector.rbegin(), bitvector.rend());
        return temp;
    }
    else
        return bitvector;
}

tinyxml2::XMLError ASIC_Config2::LoadFromXMLElement(tinyxml2::XMLElement *parentnode)
{
    if(parentnode == nullptr)
        return tinyxml2::XML_ERROR_EMPTY_DOCUMENT;
    else if(std::string(parentnode->Value()).compare("ShiftRegister") != 0)
        return tinyxml2::XML_ERROR_MISMATCHED_ELEMENT;

    devicename = parentnode->Attribute("name");

    tinyxml2::XMLError error = tinyxml2::XML_NO_ERROR;
    tinyxml2::XMLElement* child = parentnode->FirstChildElement();

    while(child != nullptr)
    {
        if(std::string(child->Value()).compare("Register") == 0)
        {
            int bits = 1;   //only used for MSB/LSBFirst
            std::string writedirection;
            int value;
            if(child->QueryIntAttribute("value", &value) != tinyxml2::XML_NO_ERROR)
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
                            bits = 1;
                    }
                    else if(name.compare("sr.WriteDirection") == 0)
                    {
                        if(std::string(grandchild->GetText()).compare("LSBFirst") == 0)
                            writedirection = "LSBFirst";
                        else if(std::string(grandchild->GetText()).compare("MSBFirst") == 0)
                            writedirection = "MSBFirst";
                        else
                            writedirection = std::string(grandchild->GetText());
                    }
                    //redundant, not needed as value is a pararmeter of `child`, not a child of `child`:
                    else if(name.compare("sr.Value") == 0)
                    {
                        if(grandchild->QueryIntText(&value) != tinyxml2::XML_NO_ERROR)
                            value = 0;
                    }
                }

                if(grandchild != child->LastChildElement())
                    grandchild = grandchild->NextSiblingElement();
                else
                    grandchild = nullptr;
            }

            //std::cout << child->Attribute("name") << "\t\t" << bits << " " << value << std::endl;

            //add the parameter with the MSB/LSB notation:
            if(writedirection.compare("MSBFirst") == 0)
                AddParameter(std::string(child->Attribute("name")), uint(bits), MSBFirst, uint(value));
            else if(writedirection.compare("LSBFirst") == 0)
                AddParameter(std::string(child->Attribute("name")), uint(bits), LSBFirst, uint(value));
            //add the parameter with the bitorder notation:
            else
                AddParameter(std::string(child->Attribute("name")), writedirection, uint(value));
        }


        if(child != parentnode->LastChildElement())
            child = child->NextSiblingElement();
        else
            child = nullptr;
    }

    return error;

}

tinyxml2::XMLError ASIC_Config2::LoadFromXMLFile(std::string filename, bool compressed)
{
    if(compressed)
    {
        std::fstream f;
        f.open(filename, std::ios::in);
        if(!f.is_open())
            return tinyxml2::XML_ERROR_FILE_NOT_FOUND;

        zip_file archive;
        archive.load(filename);

        std::string name = filename.substr(0, filename.rfind('.')) + ".xml";

        archive.extract(name);

        filename = name;
    }

    parameters.clear();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    tinyxml2::XMLNode* config = getNode(&doc, "ShiftRegister");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompressing helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLError ASIC_Config2::SaveToXMLElement(tinyxml2::XMLElement *node, tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* configpar;

    for(unsigned int i = 0; i < parameters.size(); ++i)
    {
        configpar = doc.NewElement("Register");
        configpar->SetAttribute("name", indextoname.find(i)->second.c_str());
        configpar->SetAttribute("value",parameters[i].second);

        tinyxml2::XMLElement* attribute;

        //number of bits:
        int numbits = 0;
        int index = -1;
        std::string bitorder = parameters[i].first;
        while(index < int(bitorder.length()) && index != int(std::string::npos))
        {
            index = bitorder.find(',',index+1);
            if(index != int(bitorder.length())-1 && index != int(std::string::npos))
                ++numbits;
        }
        ++numbits;  //take into account that a list of n elements contains n-1 commas
        //add the resulting number:
        attribute = doc.NewElement("Attribute");
        attribute->SetAttribute("name","sr.size");
        attribute->SetText(numbits);
        configpar->LinkEndChild(attribute);

        //write order:
        attribute = doc.NewElement("Attribute");
        attribute->SetAttribute("name", "sr.WriteDirection");
        attribute->SetText(parameters[i].first.c_str());
        configpar->LinkEndChild(attribute);

        //old format (before switching to Richard compatible format):
        //configpar->SetAttribute("Bits", abs(parameters[i].first));
        //configpar->SetAttribute("WriteDirection", ((parameters[i].first < 0)?"LSBFirst":"MSBFirst"));
        //configpar->SetAttribute("Value", parameters[i].second);

        node->LinkEndChild(configpar);
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError ASIC_Config2::SaveToXMLFile(std::string filename, std::string devicename, bool compressed)
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

    //Add save date to the file:
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    //prepare the configuration node:
    node = doc.NewElement("ShiftRegister");
   if(devicename != "")
        node->SetAttribute("name", devicename.c_str());
   else
       node->SetAttribute("name", this->devicename.c_str());
   std::string datetime = std::ctime(&time);
   if(datetime.c_str()[datetime.length()-1] == '\n' || datetime.c_str()[datetime.length()-1] == '\x0a')
       datetime = datetime.substr(0, datetime.length()-1);
   node->SetAttribute("SaveDate", datetime.c_str());

    //fill the configuration node:
    tinyxml2::XMLError error = SaveToXMLElement(node, doc);
    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    //add the configuration node:
    doc.LinkEndChild(node);

    error = doc.SaveFile(filename.c_str());

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

ASIC_Config2_Manager::ASIC_Config2_Manager(std::string filename)
{
    if(filename != "")
    {
        LoadFile(filename);
        this->filename = filename;
    }
    else
    {
        ClearFile();
        this->filename = "";
    }
}

void ASIC_Config2_Manager::ClearFile()
{
    doc.Clear();

    //include the XML declaration:
    tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
    doc.LinkEndChild(dec);
}

tinyxml2::XMLError ASIC_Config2_Manager::LoadFile(std::string filename, bool compressed)
{
    if(compressed)
    {
        zip_file archive;
        archive.load(filename);
        filename = filename.substr(0, filename.rfind('.')) + ".xml";
        archive.extract(filename);
    }

    tinyxml2::XMLDocument doc2;
    tinyxml2::XMLError error = doc2.LoadFile(filename.c_str());
    if(error == tinyxml2::XML_NO_ERROR)
    {
        this->filename = filename;
        doc.LoadFile(filename.c_str());
    }

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "Error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLError ASIC_Config2_Manager::SaveToFile(std::string filename, bool compressed)
{
    std::string archivename = "";
    if(compressed)
    {
        archivename = filename;
        filename = filename.substr(0, filename.rfind('.')) + ".xml";
    }

    tinyxml2::XMLError error = doc.SaveFile(filename.c_str());
    if(error == tinyxml2::XML_NO_ERROR)
        this->filename = filename;

    if(compressed)
    {
        zip_file archive;
        archive.write(filename, filename);

        archive.save(archivename);

        if(remove(filename.c_str()) != 0)
            std::cout << "error removing compression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLError ASIC_Config2_Manager::AddConfig(ASIC_Config2 &config, std::string devicename)
{
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    tinyxml2::XMLElement* node;

    node = doc.NewElement("ShiftRegister");
    //add the configuration:
    node = doc.NewElement("ShiftRegister");
    node->SetAttribute("name", devicename.c_str());
    std::string datetime = std::ctime(&time);
    if(datetime.c_str()[datetime.length()-1] == '\n' || datetime.c_str()[datetime.length()-1] == '\x0a')
        datetime = datetime.substr(0, datetime.length()-1);
    node->SetAttribute("SaveDate", datetime.c_str());

    tinyxml2::XMLError error = config.SaveToXMLElement(node, doc);
    if(error == tinyxml2::XML_NO_ERROR)
        doc.LinkEndChild(node);

    return error;
}

ASIC_Config2 ASIC_Config2_Manager::GetConfig(std::string devicename)
{
    tinyxml2::XMLElement* node = doc.FirstChildElement();

    while(node != nullptr)
    {
        if(std::string(node->Value()).compare("ShiftRegister") == 0)
        {
            const char* nam = node->Attribute("name");
            if(std::string((nam != 0)?nam:"").compare(devicename) == 0)
            {
                ASIC_Config2 config;
                config.LoadFromXMLElement(node);
                return config;
            }
        }

        if(node != doc.LastChildElement())
            node = node->NextSiblingElement();
        else
            node = nullptr;
    }

    return ASIC_Config2();
}

tinyxml2::XMLElement* ASIC_Config2_Manager::GetConfigNode(std::string devicename)
{
    tinyxml2::XMLElement* node = doc.FirstChildElement();

    while(node != nullptr)
    {
        if(std::string(node->Value()).compare("ShiftRegister") == 0)
        {
            const char* nam = node->Attribute("name");
            if(std::string((nam != 0)?nam:"").compare(devicename) == 0)
            {
                return node;
            }
        }

        if(node != doc.LastChildElement())
            node = node->NextSiblingElement();
        else
            node = nullptr;
    }

    return nullptr;
}

tinyxml2::XMLElement *ASIC_Config2_Manager::GetTreeSourceNode(std::string value, std::string devicename)
{
    if(value != "")
    {
        tinyxml2::XMLElement* node = doc.NewElement(value.c_str());
        node->SetAttribute("name", devicename.c_str());
        return node;
    }
    else
        return nullptr;
}

tinyxml2::XMLDocument *ASIC_Config2_Manager::GetParentDocument()
{
    return &doc;
}

bool ASIC_Config2_Manager::AddConfigTree(tinyxml2::XMLElement *tree)
{
    if(tree == nullptr)
        return false;

    doc.LinkEndChild(tree);
    return true;
}

tinyxml2::XMLElement *ASIC_Config2_Manager::GetConfigTree(std::string value, std::string devicename)
{
    tinyxml2::XMLElement* node = doc.FirstChildElement();

    while(node != nullptr)
    {
        if(std::string(node->Value()).compare(value) == 0)
        {
            const char* nam = node->Attribute("name");
            if(std::string((nam != 0)?nam:"").compare(devicename) == 0)
                return node;
        }

        if(node != doc.LastChildElement())
            node = node->NextSiblingElement();
        else
            node = nullptr;
    }

    return nullptr;
}
