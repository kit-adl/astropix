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

#include "tdac_config.h"

TDAC_Config::TDAC_Config(int columns, int rows, unsigned int maxvalue) : devicename(""), uptodate(true)
{
    this->maxvalue = maxvalue;
    if(columns >= 0)
        this->columns  = uint(columns);
    else
        this->columns  = 0;
    if(rows >= 0)
        this->rows     = uint(rows);
    else
        this->rows     = 0;
}

std::string TDAC_Config::GetDeviceName() const
{
    return devicename;
}

void TDAC_Config::SetDeviceName(std::string name)
{
    devicename = name;
}

unsigned int TDAC_Config::GetNumRows() const
{
    return rows;
}

void TDAC_Config::SetNumRows(unsigned int rows, bool updatematrix)
{
    this->rows = rows;
    uptodate = false;

    if(updatematrix)
        UpdateMatrix();
}

unsigned int TDAC_Config::GetNumColumns() const
{
    return columns;
}

void TDAC_Config::SetNumColumns(unsigned int columns, bool updatematrix)
{
    this->columns = columns;
    uptodate = false;

    if(updatematrix)
        UpdateMatrix();
}

unsigned int TDAC_Config::GetMaxValue() const
{
    return maxvalue;
}

void TDAC_Config::SetMaxValue(unsigned int maxvalue)
{
    bool gotsmaller = false;
    if(this->maxvalue > maxvalue)
        gotsmaller = true;

    this->maxvalue = maxvalue;

    //make sure no value is larger than maxvalue:
    if(gotsmaller) //not necessary if range got extended
    {
        for(auto& rowit : values)
        {
            for(auto& colit : rowit.second)
            {
                if(colit.second > maxvalue)
                    colit.second = maxvalue;
            }
        }
    }
}

void TDAC_Config::UpdateMatrix(int columns, int rows)
{
    if(columns < 0)
        columns = int(this->columns);
    else
        this->columns = uint(columns);
    if(rows < 0)
        rows = int(this->rows);
    else
        this->rows = uint(rows);

    values.clear();

    std::vector<std::pair<int, unsigned int> > onerow;

    for(int i = 0; i < columns; ++i)
        onerow.push_back(std::make_pair(i, uint(0)));

    for(int i = 0; i < rows; ++i)
        values.push_back(std::make_pair(i, onerow));

    uptodate = true;
}

std::pair<int, int> TDAC_Config::GetCustomAddress(unsigned int column, unsigned int row) const
{
    if(uptodate)
    {
        if(column >= columns || row >= rows)
            return std::make_pair(-1,-1);
    }
    else
    {
        if(row >= values.size() || (values.size() > 0 && column >= values[1].second.size()))
            return std::make_pair(-1,-1);
    }

    return std::make_pair(values[row].second[column].first, values[row].first);
}

bool TDAC_Config::SetCustomAddress(unsigned int column, unsigned int row, int coladdress, int rowaddress)
{
    if(uptodate)
    {
        if(column >= columns || row >= rows)
            return false;
    }
    else
    {
        if(row >= values.size() || (values.size() > 0 && column >= values[1].second.size()))
            return false;
    }

    values[row].first = rowaddress;
    values[row].second[column].first = coladdress;

    return true;
}

int TDAC_Config::GetTDACValue(unsigned int column, unsigned int row) const
{
    if(uptodate)
    {
        if(column >= columns || row >= rows)
            return -1;
    }
    else
    {
        if(row >= values.size() || (values.size() > 0 && column >= values[1].second.size()))
            return -1;
    }

    return int(((values[row].second)[column]).second);
}

bool TDAC_Config::SetTDACValue(unsigned int column, unsigned int row, unsigned int value)
{
    if(uptodate)
    {
        if(column >= columns || row >= rows)
            return false;
    }
    else
    {
        if(row >= values.size() || (values.size() > 0 && column >= values[1].second.size()))
            return false;
    }

    if(value <= maxvalue)
    {
        values[row].second[column].second = value;
        return true;
    }
    else
        return false;
}

int TDAC_Config::GetTDACAddressValue(int coladdress, int rowaddress)
{
    for(auto& rowit : values)
    {
        if(rowit.first == rowaddress)
        {
            for(auto& colit : rowit.second)
            {
                if(colit.first == coladdress)
                    return int(colit.second);
            }
        }
    }

    return -1;
}

bool TDAC_Config::SetTDACAddressValue(int coladdress, int rowaddress, unsigned int value)
{
    if(value > maxvalue)
        return false;

    for(auto& rowit : values)
    {
        if(rowit.first == rowaddress)
        {
            for(auto& colit : rowit.second)
            {
                if(colit.first == coladdress)
                {
                    colit.second = value;
                    return true;
                }
            }
        }
    }

    return false;

}

bool TDAC_Config::SetAllToValue(unsigned int value, int ignorelargerthan)
{
    if(value > maxvalue)
        return false;
    if(ignorelargerthan < 0)
        ignorelargerthan = int(maxvalue);


    for(auto& rowit : values)
        for(auto& colit : rowit.second)
            if(colit.second <= uint(ignorelargerthan))
                colit.second = value;

    return true;
}

tinyxml2::XMLError TDAC_Config::LoadFromXMLElement(tinyxml2::XMLElement *rootnode)
{
    if(rootnode == nullptr)
        return tinyxml2::XML_ERROR_EMPTY_DOCUMENT;

    values.clear();

    devicename = rootnode->Attribute("name");

    columns  = uint(rootnode->IntAttribute("columns"));
    rows     = uint(rootnode->IntAttribute("rows"));
    maxvalue = uint(rootnode->IntAttribute("maxvalue"));

    tinyxml2::XMLElement* rownode = rootnode->FirstChildElement();

    while(rownode != nullptr)
    {
        if(std::string(rownode->Value()).compare("Row") == 0)
        {
            std::vector<std::pair<int, unsigned int> > row;
            tinyxml2::XMLElement* child = rownode->FirstChildElement();
            while(child != nullptr)
            {
                if(std::string(child->Value()).compare("pix") == 0)
                {
                    int value = child->IntAttribute("val");
                    int id    = child->IntAttribute("addr");
                    row.push_back(std::make_pair(id, value));
                }

                if(child != rownode->LastChildElement())
                    child = child->NextSiblingElement();
                else
                    child = nullptr;
            }
            int id = rownode->IntAttribute("addr");
            values.push_back(std::make_pair(id, row));
        }

        if(rownode != rootnode->LastChildElement())
            rownode = rownode->NextSiblingElement();
        else
            rownode = nullptr;
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError TDAC_Config::LoadFromXMLFile(std::string filename, bool compressed)
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

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());

    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    tinyxml2::XMLNode* node = getNode(&doc, "TDACs");

    if(node != nullptr)
        error = LoadFromXMLElement(node->ToElement());
    else
        error = tinyxml2::XML_ERROR_EMPTY_DOCUMENT;

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "Decompressed file could not be deleted" << std::endl;;
    }


    return error;
}

tinyxml2::XMLError TDAC_Config::SaveToXMLElement(tinyxml2::XMLElement *rootnode, tinyxml2::XMLDocument &doc, std::string devicename)
{
    rootnode->SetValue("TDACs");
    if(devicename != "")
        rootnode->SetAttribute("name", devicename.c_str());
    else
        rootnode->SetAttribute("name", this->devicename.c_str());
    rootnode->SetAttribute("rows", rows);
    rootnode->SetAttribute("columns", columns);
    rootnode->SetAttribute("maxvalue", maxvalue);

    //Add save date to the file:
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string datetime = std::ctime(&time);
    if(datetime.c_str()[datetime.length()-1] == '\n' || datetime.c_str()[datetime.length()-1] == '\x0a')
        datetime = datetime.substr(0, datetime.length()-1);
    rootnode->SetAttribute("SaveDate", datetime.c_str());

    for(auto& rowit : values)
    {
        tinyxml2::XMLElement* row = doc.NewElement("Row");
        row->SetAttribute("addr", rowit.first);

        for(auto& colit : rowit.second)
        {
            tinyxml2::XMLElement* element = doc.NewElement("pix");
            element->SetAttribute("addr", colit.first);
            element->SetAttribute("val", colit.second);

            row->LinkEndChild(element);
        }

        rootnode->LinkEndChild(row);
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError TDAC_Config::SaveToXMLFile(std::string filename, std::string devicename, bool compressed)
{
    std::string archivename = "";
    if(compressed)
    {
        archivename = filename;
        filename = filename.substr(0, filename.rfind('.')) + ".xml";
    }
    tinyxml2::XMLDocument doc;

    tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
    doc.LinkEndChild(dec);

    tinyxml2::XMLElement* node = doc.NewElement("TDACs");

    tinyxml2::XMLError error = SaveToXMLElement(node, doc, devicename);

    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    doc.LinkEndChild(node);

    error = doc.SaveFile(filename.c_str());

    if(compressed)
    {
        zip_file archive;
        archive.write(filename, filename);

        archive.save(archivename);

        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting uncompressed file" << std::endl;
    }

    return error;
}
