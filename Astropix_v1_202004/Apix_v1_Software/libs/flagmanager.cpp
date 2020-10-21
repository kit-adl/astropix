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
 * This class is based on the implementation of Felix Ehrler.
 */
#include "flagmanager.h"

FlagManager::FlagManager(int address) : identifier(""), FPGAaddress(address)
{

}

std::string FlagManager::GetIdentifier() const
{
    return identifier;
}

void FlagManager::SetIdentifier(std::string name)
{
    identifier = name;
}

int FlagManager::GetFPGAAddress() const
{
    return FPGAaddress;
}

void FlagManager::SetFPGAAddress(int address)
{
    FPGAaddress = address;
}

unsigned int FlagManager::GetNumFlags() const
{
    return entries.size();
}

void FlagManager::Clear()
{
    entries.clear();
}

bool FlagManager::AddFlag(std::string name, bool state)
{
    if(name == "")
        return false;

    int value = 1;
    //find largest used bit:
    for(auto& it : entries)
    {
        unsigned int oldpos = 0;
        unsigned int pos = oldpos;

        while(pos != static_cast<unsigned int>(std::string::npos))
        {
            pos = it.bitvalues.find(',', oldpos);
            std::stringstream s("");
            s << it.bitvalues.substr(oldpos, pos - oldpos);
            int singlebit;
            s >> singlebit;
            oldpos = pos + 1;

            if(singlebit > value)
                value = singlebit;
        }
    }

    if(entries.size() > 0)
        value = value * 2; //use the next bit

    entry flag;
    flag.name = name;
    flag.value = (state)?1:0;
    std::stringstream s("");
    s << value;
    flag.bitvalues = s.str();

    entries.push_back(flag);

    return true;
}

bool FlagManager::AddFlag(std::string name, int value, bool state)
{
    if(name == "" || value == 0)
        return false;

    entry flag;
    flag.name = name;
    flag.value = state;
    std::stringstream s("");
    s << value;
    flag.bitvalues = s.str();
    bool issinglebit = false;
    for(unsigned int i = 0; i < 8 * sizeof (int); ++i)
    {
        if(value == (1 << i))
        {
            issinglebit = true;
            break;
        }
    }
    if(!issinglebit)
        std::cout << "Warning: value is not a power of 2" << std::endl;

    entries.push_back(flag);

    return true;
}

bool FlagManager::AddFlag(std::string name, int smallestvalue, unsigned int numbits, int initvalue, bool checkposition)
{
    if(name == "" || numbits == 0 || initvalue > (1 << numbits))
        return false;

    int value = 0;
    //find largest used bit:
    if(checkposition)
    {
        for(auto& it : entries)
        {
            unsigned int oldpos = 0;
            unsigned int pos = oldpos;

            while(pos != static_cast<unsigned int>(std::string::npos))
            {
                pos = it.bitvalues.find(',', oldpos);
                std::stringstream s("");
                s << it.bitvalues.substr(oldpos, pos - oldpos);
                int singlebit;
                s >> singlebit;
                oldpos = pos + 1;

                if(singlebit > value)
                    value = singlebit;
            }
        }

        //do not skip the desired bit if it was not used:
        if(value < smallestvalue)
            value = smallestvalue;
        else
            value = value * 2; //use the next bit
    }
    else
        value = 1;

    std::stringstream values("");
    values << value;
    for(unsigned int i = 1; i < numbits; ++i)
        values << "," << (value << i);

    entry flag;
    flag.name = name;
    flag.bitvalues = values.str();
    flag.value = initvalue;

    entries.push_back(flag);

    return true;
}

bool FlagManager::AddFlag(std::string name, std::string values, int value)
{
    if(name == "" || values == "")
        return false;

    entry flag;
    flag.name = name;

    //test passed string for compatibility with comma separated numbers
    for(unsigned int i = 0; i < values.length(); ++i)
    {
        if((values[i] < '0' && values[i] != ',') || values[i] > '9')
            return false;
    }

    //test the contents of the string:
    unsigned int oldpos = 0;
    unsigned int pos = oldpos;
    unsigned int numbits = 0;

    while(pos != static_cast<unsigned int>(std::string::npos))
    {
        pos = values.find(',', oldpos);
        std::stringstream s("");
        s << values.substr(oldpos, pos - oldpos);
        int singlebit;
        s >> singlebit;
        oldpos = pos + 1;

        bool issinglebit = false;
        for(unsigned int i = 0; i < 8 * sizeof (int); ++i)
        {
            if(singlebit == (1 << i))
            {
                issinglebit = true;
                break;
            }
        }
        if(!issinglebit)
            return false;
        ++numbits;
    }

    // value too large to fit into the requested space:
    if(value >= (1 << numbits))
        return false;

    flag.bitvalues = values;
    flag.value = value;

    entries.push_back(flag);

    return true;
}

int FlagManager::GetFlag(unsigned int index) const
{
    if(index >= entries.size())
        return -1;

    return entries[index].value;
}

int FlagManager::GetFlag(std::string name) const
{
    for(auto& it : entries)
    {
        if(it.name.compare(name) == 0)
            return it.value;
    }

    return -1;
}

unsigned int FlagManager::GetFlagWidth(unsigned int index) const
{
    if(index >= entries.size())
        return 0;

    unsigned int pos = 0;
    unsigned int count = 0;

    while(pos != static_cast<unsigned int>(std::string::npos))
    {
        pos = entries[index].bitvalues.find(',', (pos==0)?0:pos+1);
        ++count;
    }

    return count;
}

unsigned int FlagManager::GetFlagWidth(std::string name) const
{
    unsigned int index = 0;
    for(auto& it : entries)
    {
        if(it.name.compare(name) == 0)
            return GetFlagWidth(index);
        ++index;
    }

    return 0;
}

std::string FlagManager::GetFlagName(unsigned int index) const
{
    if(index >= entries.size())
        return "";

    return entries[index].name;
}

int FlagManager::GetFirstBitPosition(unsigned int index) const
{
    if(index >= entries.size())
        return -1;

    std::stringstream s("");
    s << entries[index].bitvalues.substr(0, entries[index].bitvalues.find(','));
    int bit;
    s >> bit;

    return bit;
}

int FlagManager::GetFirstBitPosition(std::string name) const
{
    for(unsigned int i = 0; i < entries.size(); ++i)
        if(entries[i].name.compare(name) == 0)
            return GetFirstBitPosition(i);

    return -1;
}

std::string FlagManager::GetBitPositions(unsigned int index) const
{
    if(index >= entries.size())
        return "";

    return entries[index].bitvalues;
}

std::string FlagManager::GetBitPositions(std::string name) const
{
    for(auto& it : entries)
        if(it.name.compare(name) == 0)
            return it.bitvalues;

    return "";
}

bool FlagManager::SetFlag(unsigned int index, int value)
{
    if(index >= entries.size())
        return false;

    if(value < 0 || value >= (1 << GetFlagWidth(index)))
        return false;

    entries[index].value = value;

    return true;
}

bool FlagManager::SetFlag(std::string name, int value)
{
    unsigned int index = 0;
    for(auto& it : entries)
    {
        if(it.name.compare(name) == 0)
            return SetFlag(index, value);
        ++index;
    }

    return false;
}

int FlagManager::GetConfiguration() const
{
    int result = 0;
    for(auto& it : entries)
    {
        int bitindex = 0;
        unsigned int oldpos = 0;
        unsigned int pos = oldpos;
        while(pos != static_cast<unsigned int>(std::string::npos))
        {
            pos = it.bitvalues.find(',', oldpos);
            std::stringstream s("");
            s << it.bitvalues.substr(oldpos, pos - oldpos);
            oldpos = pos + 1;
            int value;
            s >> value;

            result += ((it.value & (1 << bitindex)) != 0) ? value : 0;
            ++bitindex;
        }
    }

    return result;
}

bool FlagManager::SetConfiguration(int configuration)
{
    for(auto& it : entries)
    {
        unsigned int bitindex = 0;
        unsigned int oldpos = 0;
        unsigned int pos = oldpos;
        while(pos != static_cast<unsigned int>(std::string::npos))
        {
            pos = it.bitvalues.find(',', oldpos);
            std::stringstream s("");
            s << it.bitvalues.substr(oldpos, pos - oldpos);
            oldpos = pos + 1;
            int flag;
            s >> flag;
            if((configuration & flag) != 0) //flag is set in complete configuration
            {
                it.value |= 1 << bitindex; //set the flag
                configuration &= ~flag; //remove the bit from the configuration to mark it as done
            }
            else
                it.value &= ~(1 << bitindex); //clear the flag

            ++bitindex;
        }
    }

    //if non-existent bits are set, return false:
    return configuration == 0;
}

tinyxml2::XMLError FlagManager::LoadFromXMLElement(tinyxml2::XMLElement *rootnode)
{
    if(rootnode == nullptr)
        return tinyxml2::XML_ERROR_EMPTY_DOCUMENT;

    const char* nam = rootnode->Attribute("name");
    identifier = (nam != nullptr)?std::string(nam):"";

    Clear();

    tinyxml2::XMLElement* child = rootnode->FirstChildElement();

    while(child != nullptr)
    {
        if(std::string(child->Value()).compare("Flag") == 0)
        {
            const char* nam = child->GetText();
            std::string bits = "";
            if(nam == nullptr)
                return tinyxml2::XML_ERROR_PARSING_TEXT;
            bits = std::string(nam);

            nam = child->Attribute("name");
            if(nam == nullptr)
                return tinyxml2::XML_ERROR_PARSING_TEXT;
            std::string name = std::string(nam);

            int value = child->IntAttribute("value");

            if(!AddFlag(name, bits, value))
                return tinyxml2::XML_ERROR_PARSING_ELEMENT;
        }

        if(child != rootnode->LastChildElement())
            child = child->NextSiblingElement();
        else
            child = nullptr;
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError FlagManager::LoadFromXMLFile(std::string filename, bool compressed)
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

    tinyxml2::XMLNode* config = getNode(&doc, "FlagManager");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLElement* FlagManager::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string identifier) const
{
    tinyxml2::XMLElement* flagman = doc.NewElement("FlagManager");

    if(identifier == "")
        flagman->SetAttribute("name", this->identifier.c_str());
    else
        flagman->SetAttribute("name", identifier.c_str());

    for(auto& it : entries)
    {
        tinyxml2::XMLElement* flag = doc.NewElement("Flag");
        flag->SetAttribute("name", it.name.c_str());
        flag->SetAttribute("value", it.value);
        flag->SetText(it.bitvalues.c_str());

        flagman->LinkEndChild(flag);
    }

    return flagman;
}

tinyxml2::XMLError FlagManager::SavetoXMLFile(std::string filename, std::string identifier, bool compressed) const
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

    node = SaveToXMLElement(doc, identifier);

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
