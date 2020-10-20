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

#include "textdata.h"

textdata::textdata() : name(""), data(""), valid(true)
{
	
}

textdata::textdata(std::string title) : name(title), data(""), valid(true)
{
	
}

textdata::textdata(std::string title, std::string content) : name(title), data(content), 
						valid(true)
{
	
}

textdata::textdata(tinyxml2::XMLElement* element)
{
    if(element == nullptr)
	{
		name  = "";
		data  = "";
		valid = false;
	}

	name = checkString(element->Attribute("Name"));
	data = checkString(element->GetText());

	bool val;
	if(element->QueryBoolAttribute("valid", &val) != tinyxml2::XML_NO_ATTRIBUTE)
		valid = val;
	else
		valid = true;
}

/*textdata::textdata(textdata &textdat)
{
	name = textdat.getName();
	data.str(textdat.getData());
	data.setValidity(textdat.getValidity());
}*/

textdata::~textdata()
{
	//dtr
}

bool textdata::informationComplete()
{
	if(name == "" || data.compare("") == 0)
		return false;
	else
		return true;
}

std::string textdata::getName()
{
	return name;
}
		
bool textdata::setName(std::string title)
{
	if(title != "")
	{
		name = title;
		return true;
	}
	else
		return false;
}

bool textdata::getValidity()
{
	return valid;
}

bool textdata::setValidity(bool newvalidity)
{
	valid = newvalidity;
	return true;
}

std::string textdata::getData()
{
	return data;
}

bool textdata::setData(std::string content)
{
	data = content;
	return true;
}

void textdata::clearData()
{
	data = "";
}

bool textdata::addData(std::string addcontent)
{
	data = data + addcontent;
	return true;
}

tinyxml2::XMLElement* textdata::getXMLOutput(tinyxml2::XMLDocument* doc)
{
	if(name == "" || data == "")
        return nullptr;

	tinyxml2::XMLElement* node = doc->NewElement("Textdata");

	node->SetAttribute("Name",  name.c_str());
	node->SetAttribute("valid", valid);

	node->SetText(data.c_str());

	return node;
}

std::string textdata::Print()
{
	std::stringstream printed(""), hline("");

	//make a line of '#' for a border of the title:
    int length = int(name.length()) + 4;
	for(int i = 0; i < length; ++i)
		hline << "#";

	//header:
	printed << hline.str() << std::endl
			<< "# " << name << " #" << std::endl 
			<< hline.str() 
			<< std::endl << std::endl;

	//content:
	printed << data << std::endl;

    return printed.str();
}

bool textdata::PrintToFile(std::string filename)
{
	std::fstream f;
	f.open(filename.c_str(), std::ios::in);
	if(f.is_open())
	{
		f.close();
		return false;
	}
	f.close();
	f.open(filename.c_str(), std::ios::out);

    f << Print();

	f.close();

	return true;
}
