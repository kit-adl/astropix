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

#include "Measurement.h"

Measurement::Measurement() : name(""), description(""), sample(""), device(""), triggera(""), 
								triggerb("")
{
	events = std::vector<Event>();
}

Measurement::Measurement(std::string title) : name(title), description(""), sample(""), device(""), triggera(""), 
								triggerb("")
{
	events = std::vector<Event>();
}

Measurement::Measurement(Measurement& templ)
{
	name = templ.name;
	description = templ.description;
	sample = templ.sample;
	device = templ.device;
	triggera = templ.triggera;
	triggerb = templ.triggerb;

	events = std::vector<Event>();
}

Measurement::Measurement(tinyxml2::XMLDocument* doc, int measurementnumber)
{
	if(doc == 0)
	{
		name        = "";
		description = "";
		sample      = "";
		device      = "";
		triggera    = "";
		triggerb    = "";
		events      = std::vector<Event>();		
	}
	else
	{
		extractFromXMLTree(doc, measurementnumber);
	}
}

Measurement::~Measurement()
{
	//dtr
}

bool Measurement::informationComplete()
{
	if(name == "" || sample == "" || device == "" || triggera == "")
		return false;
	else
		return true;
}

bool Measurement::setName(std::string newname)
{
	if(newname != "")
	{
		name = newname;
		return true;
	}
	else
		return false;
}

bool Measurement::setDescription(std::string text)
{
	description = text;
	return true;
}

bool Measurement::setSample(std::string text)
{
	if(text != "")
	{
		sample = text;
		return true;
	}
	else
		return false;
}

bool Measurement::setMeasurementDevice(std::string text)
{
	if(text != "")
	{
		device = text;
		return true;
	}
	else
		return false;
}

bool Measurement::setTriggerA(std::string text)
{
	if(text != "")
	{
		triggera = text;
		return true;
	}
	else
		return false;
}

bool Measurement::setTriggerB(std::string text)
{
	if(text != "")
	{
		triggerb = text;
		return true;
	}
	else
		return false;
}

unsigned int Measurement::getNumConfigurations()
{
	return configparameters.size();
}

void Measurement::clearConfigurations()
{
	configparameters.clear();
}

bool Measurement::addConfigurationValue(ConfigPart &part)
{
	configparameters.push_back(part);
	return true;
}

bool Measurement::addConfigurationValue(std::string name, int value)
{
	ConfigPart part(name);
	part.setValue(value);
	configparameters.push_back(part);
	return true;
}

bool Measurement::addConfigurationValue(std::string name, double value)
{
	ConfigPart part(name);
	part.setValue(value);
	configparameters.push_back(part);
	return true;	
}

ConfigPart* Measurement::getConfigurationValue(unsigned int index)
{
	if(index >= configparameters.size())
		return 0;
	else
		return &(configparameters[index]);
}

ConfigPart* Measurement::getConfigurationValue(std::string name)
{
	std::vector<ConfigPart>::iterator it = configparameters.begin();

	while(it != configparameters.end() && it->getName().compare(name) != 0)
		++it;

	if(it == configparameters.end())
		return 0;
	else
		return &(*it);
}

std::vector<ConfigPart>::iterator Measurement::getConfigurationValuesBegin()
{
	return configparameters.begin();
}

std::vector<ConfigPart>::iterator Measurement::getConfigurationValuesEnd()
{
	return configparameters.end();
}

std::vector<ConfigPart>::iterator Measurement::deleteConfigurationValue(
					std::vector<ConfigPart>::iterator it)
{
	return configparameters.erase(it);
}


void Measurement::clearEvents()
{
	events.clear();
}

Event* Measurement::addEvent(std::string name, std::string capturetime)
{
	events.push_back(Event(name,capturetime));

	return &(events.back());
}

bool Measurement::addEvent(Event* evnt)
{
	if(!evnt->informationComplete())
		return false;

	events.push_back(*evnt);

	return true;
}

unsigned int Measurement::getNumEvents()
{
	return events.size();
}

Event* Measurement::getEvent(unsigned int index)
{
	if(index < events.size())
		return &(events[index]);
	else
		return 0;
}

Event* Measurement::getEvent(std::string name, std::vector<Event>::iterator* it)
{
	for((*it)++; (*it) != events.end(); ++(*it))
	{
		if((*it)->getName() == name)
			return &(*(*it));
	}
	return 0;
}

std::vector<Event>::iterator Measurement::getEventBegin()
{
	return events.begin();
}

std::vector<Event>::iterator Measurement::getEventEnd()
{
	return events.end();
}

std::string Measurement::generateFileStream(tinyxml2::XMLDocument* doc, bool deleteDoc)
{
	if(!informationComplete())
        return "";

	//use the exisiting document if one was passed:
	if(doc == 0)
	{
		doc = new tinyxml2::XMLDocument();
		//generate XML declaration and insert it in the document:
		tinyxml2::XMLDeclaration* dec = doc->NewDeclaration("xml version=\"1.0\"");
		doc->LinkEndChild(dec);
	}

	tinyxml2::XMLElement* measure = doc->NewElement("Measurement");

	//insert the additional information for the measurement:
	tinyxml2::XMLElement* parts = 0;

	//Name of the measurement:
	parts = doc->NewElement("Name");
	parts->SetText(name.c_str());
	measure->LinkEndChild(parts);

	//description of the measurement:
	if(description != "")
	{
		parts = doc->NewElement("Description");
		//maybe separate the description into "<p></p>" blocks at "\n" characters
		
		//insert the transformed description into the tree structure:
		parts->SetText(description.c_str());
		measure->LinkEndChild(parts);
	}

	//Sample:
	parts = doc->NewElement("Sample");
	parts->SetText(sample.c_str());
	measure->LinkEndChild(parts);

	//measurement device:
	parts = doc->NewElement("MeasurementDevice");
	parts->SetText(device.c_str());
	measure->LinkEndChild(parts);

	//Trigger:
	tinyxml2::XMLElement* trigger = doc->NewElement("Trigger");
	parts = doc->NewElement("TriggerA");
	parts->SetText(triggera.c_str());
	trigger->LinkEndChild(parts);
	if(triggerb != "")
	{
		parts = doc->NewElement("TriggerB");
		parts->SetText(triggerb.c_str());
		trigger->LinkEndChild(parts);		
	}
	measure->LinkEndChild(trigger);

	//Configuration:
	tinyxml2::XMLElement* config = doc->NewElement("Configuration");
	for(std::vector<ConfigPart>::iterator it = configparameters.begin(); 
					it != configparameters.end(); ++it)
	{
		parts = it->getXMLOutput(doc);
		if(parts != 0)
			config->LinkEndChild(parts);
	}
	if(configparameters.size() > 0)
		measure->LinkEndChild(config);

	//Events:
	unsigned int eventcounter = 1;
	for(std::vector<Event>::iterator it = events.begin(); it != events.end(); ++it)
	{
		parts = it->getXMLOutput(doc);
		if(parts != 0)
		{
			parts->SetAttribute("nr",eventcounter++);
			measure->LinkEndChild(parts);
		}
	}


	doc->LinkEndChild(measure);

	tinyxml2::XMLPrinter printer;
    doc->Print( &printer );
    //std::stringstream output(printer.CStr());

	if(deleteDoc)
		delete doc;
    return std::string(printer.CStr());//output.str();
}

bool Measurement::compressFileStream(zip_file& archive, std::string filename, std::string& filedata)
{
	if(archive.has_file(filename))
		return false;
	/*
	//check, whether the filename provided is already in use:
	std::vector<std::string> filesinarchive = archive.namelist();	//acquire file name list
	//search the list for the desired file name:
	std::vector<std::string>::iterator foundelement = std::find(filesinarchive.begin(), 
																filesinarchive.end(), filename);
	//if the file name is not in use, foundelement will be the end element:
	if(foundelement != filesinarchive.end())
		return false;
	*/
	else
	{
        archive.writestr(filename.c_str(), filedata);	//write the stream in the file
		return true;
	}
}

bool Measurement::saveToFile(std::string filename, bool compressed)
{
	if(compressed)
	{
		zip_file archive;
        //std::stringstream data(generateFileStream());
        std::string data = generateFileStream();

        if(!compressFileStream(archive, filename, data))
			return false;
		archive.save(filename);

		return true;
	}
	else
	{
		std::fstream f;
		//check whether the file already exists:
		f.open(filename.c_str(), std::ios::in);
		if(f.is_open())
		{
			f.close();
			return false;
		}
		f.close();
		//open for writing:
		f.open(filename.c_str(), std::ios::out);
		//generate the output and write it to the file
        f << generateFileStream();
		f.close();
		return true;
	}

}

int Measurement::extractFromXMLTree(tinyxml2::XMLDocument* doc, int measurementnumber)
{
	tinyxml2::XMLNode* mess = doc->FirstChildElement();
	//find the `measurementnumber`^th occurence of a "Measurement" tag:
	for(int i=0;i<measurementnumber;++i)
	{
		mess = getNode(doc,"Measurement", mess);
		if(mess == 0)	//result if not found
			return -3;
	}

	tinyxml2::XMLNode* part;

	//Measurement Name:
	part = getNode(mess, "Name");
	if(part != 0)
		name = checkString(part->ToElement()->GetText());

	//Measurement Description:
	part = getNode(mess, "Description");
	if(part != 0)
		description = checkString(part->ToElement()->GetText());

	//Measurement Sample:
	part = getNode(mess, "Sample");
	if(part != 0)
		sample = checkString(part->ToElement()->GetText());

	//Measurement Device:
	part = getNode(mess, "MeasurementDevice");
	if(part != 0)
		device = checkString(part->ToElement()->GetText());

	//Trigger Settings:
	part = getNode(mess, "Trigger");
	if(part != 0)
	{
		tinyxml2::XMLNode* trig = getNode(part, "TriggerA");
		if(trig != 0)
			triggera = checkString(trig->ToElement()->GetText());

		trig = getNode(part, "TriggerB");
		if(trig != 0)
			triggerb = checkString(trig->ToElement()->GetText());
	}

	//load the configuration:
	part = getNode(mess, "Configuration");
	if(part != 0)
	{
		tinyxml2::XMLElement* configval = part->FirstChildElement();
		while(configval != 0)
		{
			configparameters.push_back(ConfigPart(configval));

			if(configval != part->LastChildElement())
				configval = configval->NextSiblingElement();
			else
				configval = 0;
		}
	}
		
	//load the events:
	part = getNode(mess, "Event");
    while(part != 0)
    {
        events.push_back(Event(part->ToElement()));

        //erase empty elements again:
        if(events.back().getEntries() == 0)
        	events.pop_back();

        if(part != mess->LastChildElement())
        	part = getNode(mess, "Event", part->NextSiblingElement());
        else
        	part = 0;
    }

	return 0;
}

int Measurement::loadFromFile(std::string filename, int measurementnumber)
{
	//load the data to an XML document:
	tinyxml2::XMLDocument doc;
	int error = doc.LoadFile(filename.c_str());
	if(error != 0)
	    return error;

	//load the data to the class structures:
	return extractFromXMLTree(&doc, measurementnumber);
}

int Measurement::loadFromArchive(std::string archivename, std::string filename, 
									int measurementnumber)
{
	zip_file archive;
	archive.load(archivename);	//load the archive
	if(!archive.has_file(filename))	//check whether the file `filename` is included in `archive`
		return -1;

	//inflate the data:
	std::string contents = archive.read(filename);
	//parse the data into an XML document:
	tinyxml2::XMLDocument doc;
	if(!doc.Parse(contents.c_str(),contents.length()))
		return -2;

	//load the data to the class structures:
	return extractFromXMLTree(&doc, measurementnumber);
}
