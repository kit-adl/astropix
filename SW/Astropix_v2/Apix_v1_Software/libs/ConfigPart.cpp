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

#include "ConfigPart.h"

ConfigPart::ConfigPart(bool intvalue) : name(""), ivalue(0), dvalue(0), intchoice(intvalue)
{

}

ConfigPart::ConfigPart(std::string configname, bool intvalue) : name(configname),
                            ivalue(0), dvalue(0), intchoice(intvalue)
{

}

ConfigPart::ConfigPart(tinyxml2::XMLElement* element)
{
    if(element == nullptr)
	{
		name = "";
		intchoice = true;
		ivalue = 0;
		dvalue = 0;
	}
	else
	{
		name = checkString(element->Value());
		bool val;
		if(element->QueryBoolAttribute("int", &val) == tinyxml2::XML_NO_ATTRIBUTE)
			intchoice = true;
		else
			intchoice = val;

		if(intchoice)
		{
			dvalue = 0;
			ivalue = element->IntAttribute("Value");
		}
		else
		{
			ivalue = 0;
			dvalue = element->DoubleAttribute("Value");
		}
	}
}

ConfigPart::~ConfigPart()
{
	//dtr
}

std::string ConfigPart::getName()
{
	return name;
}

bool ConfigPart::setName(std::string newname)
{
	if(newname != "")
	{
		name = newname;
		return true;
	}
	else
		return false;
}

bool ConfigPart::getValueFormat()
{
	return intchoice;
}

bool ConfigPart::setValueFormat(bool intvalue)
{
	intchoice = intvalue;
	return true;
}

bool ConfigPart::setValue(double value)
{
	intchoice = false;
	dvalue = value;
	return true;
}

bool ConfigPart::setValue(int value)
{
	intchoice = true;
	ivalue = value;
	return true;
}

int ConfigPart::getIntValue()
{
	return ivalue;
}

double ConfigPart::getDoubleValue()
{
	return dvalue;
}

double ConfigPart::getValue()
{
	if(intchoice)
		return double(ivalue);
	else
		return dvalue;
}

tinyxml2::XMLElement* ConfigPart::getXMLOutput(tinyxml2::XMLDocument* doc)
{
	if(name == "")
        return nullptr;

	tinyxml2::XMLElement* node = doc->NewElement(name.c_str());
	if(intchoice)
		node->SetAttribute("Value", ivalue);
	else
		node->SetAttribute("Value", dvalue);

	node->SetAttribute("int", intchoice);

	return node;
}
