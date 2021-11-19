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

#include "Event.h"

Event::Event() : name(""), timestamp(""), valid(true)
{
	//ctr
	waveforms  = std::vector<WaveForm>();
	histograms = std::vector<Histogram>();
	//datas      = std::vector<textdata>(0);
}

Event::Event(std::string title) : name(title), timestamp(""), valid(true)
{
	//ctr
	waveforms  = std::vector<WaveForm>();
	histograms = std::vector<Histogram>();
	//datas      = std::vector<textdata>(0);
}

Event::Event(std::string title, std::string capturetime) : name(title), timestamp(capturetime), 
																valid(true)
{
	//ctr
	waveforms  = std::vector<WaveForm>();
	histograms = std::vector<Histogram>();
	//datas      = std::vector<textdata>(0);
}

Event::Event(tinyxml2::XMLElement* element)
{
	if(element == 0)
		return;

	name      = checkString(element->Attribute("Name"));
	timestamp = checkString(element->Attribute("CaptureTime"));
	bool val;
	if(element->QueryBoolAttribute("valid", &val) != tinyxml2::XML_NO_ATTRIBUTE)
		valid = val;
	else
		valid = true;


	tinyxml2::XMLElement* part = element->FirstChildElement();

	while(part != 0)
	{
		if(std::string(part->Value()).compare("WaveForm") == 0)
			waveforms.push_back(WaveForm(part));
		else if(std::string(part->Value()).compare("Histogram") == 0)
			histograms.push_back(Histogram(part));
		else if(std::string(part->Value()).compare("Textdata") == 0)
		{
			if(checkString(part->Attribute("Name")) != "")
			{
				textdata text;
				text.setName(checkString(part->Attribute("Name")));
				text.setData(checkString(part->GetText()));
				datas.push_back(text);
			}
		}
		//else
		//	std::cout << "Error Value: " << part->Value() << std::endl;

		if(part != element->LastChildElement())
			part = part->NextSiblingElement();
		else
			part = 0;
	}
}

Event::~Event()
{
	//dtr
}

bool Event::informationComplete()
{
	for(std::vector<WaveForm>::iterator it = waveforms.begin(); it != waveforms.end(); ++it)
	{
		if(!it->informationComplete())
			return false;
	}

	for(std::vector<Histogram>::iterator it = histograms.begin(); it != histograms.end(); ++it)
	{
		if(!it->informationComplete())
			return false;
	}

	for(std::vector<textdata>::iterator it = datas.begin(); it != datas.end(); ++it)
	{
		if(!it->informationComplete())
			return false;
	}

	return true;
}

std::string Event::getName()
{
	return name;
}

bool Event::setName(std::string title)
{
	name = title;
	return true;
}

std::string Event::getTimestamp()
{
	return timestamp;
}

bool Event::setTimestamp(std::string datetime)
{
	timestamp = datetime;
	return true;
}

bool Event::getValidity()
{
	return valid;
}

bool Event::setValidity(bool validity)
{
	valid = validity;
	return true;
}

void Event::clearEvent()
{
	waveforms.clear();
	histograms.clear();
}

WaveForm* Event::addWaveForm(std::string name)
{
	waveforms.push_back(WaveForm(name));

	return &(waveforms.back());
}

bool Event::addWaveForm(WaveForm* wfm)
{
	if(!wfm->informationComplete())
		return false;
	else
	{
		waveforms.push_back(*wfm);
		return true;
	}
}

Histogram* Event::addHistogram(std::string name, bool horizontal)
{
	histograms.push_back(Histogram(name, horizontal));

	return &(histograms.back());
}

bool Event::addHistogram(Histogram* hist)
{
	if(!hist->informationComplete() || hist->getNumBins() == 0)
		return false;
	else
	{
		histograms.push_back(*hist);
		return true;
	}
}

textdata* Event::addTextData(std::string name)
{
	datas.push_back(textdata(name));

	return &(datas.back());
}

bool Event::addTextData(textdata* textdat)
{
	if(!textdat->informationComplete())
		return false;
	else
	{
		datas.push_back(*textdat);
		return true;
	}
}


WaveForm* Event::getWaveForm(unsigned int index)
{
	if(index < waveforms.size())
		return &(waveforms[index]);
	else
		return 0;
}

WaveForm* Event::getWaveForm(std::string name)
{
	std::vector<WaveForm>::iterator it = waveforms.begin();

	while(it != waveforms.end())
	{
		if(it->getName().compare(name) == 0)
			return &(*it);
	}
	return 0;
}

Histogram* Event::getHistogram(unsigned int index)
{
	if(index < histograms.size())
		return &(histograms[index]);
	else
		return 0;
}

Histogram* Event::getHistogram(std::string name)
{
	std::vector<Histogram>::iterator it = histograms.begin();

	while(it != histograms.end())
	{
		if(it->getName().compare(name) == 0)
			return &(*it);
	}
	return 0;
}

textdata* Event::getTextData(unsigned int index)
{
	if(index < datas.size())
		return &(datas[index]);
	else
		return 0;
}

textdata* Event::getTextData(std::string name)
{
	std::vector<textdata>::iterator it = datas.begin();

	while(it != datas.end())
	{
		if(it->getName().compare(name) == 0)
			return &(*it);
	}
	return 0;	
}


unsigned int Event::getNumWaveForms()
{
	return waveforms.size();
}

unsigned int Event::getNumHistograms()
{
	return histograms.size();
}

unsigned int Event::getNumTextDatas()
{
	return datas.size();
}

unsigned int Event::getEntries()
{
	return waveforms.size() + histograms.size() + datas.size();
}

std::vector<WaveForm>::iterator Event::getWaveFormBegin()
{
	return waveforms.begin();
}

std::vector<WaveForm>::iterator Event::getWaveFormEnd()
{
	return waveforms.end();
}

std::vector<Histogram>::iterator Event::getHistogramBegin()
{
	return histograms.begin();
}

std::vector<Histogram>::iterator Event::getHistogramEnd()
{
	return histograms.end();
}

std::vector<textdata>::iterator Event::getTextDataBegin()
{
	return datas.begin();
}

std::vector<textdata>::iterator Event::getTextDataEnd()
{
	return datas.end();
}


tinyxml2::XMLElement* Event::getXMLOutput(tinyxml2::XMLDocument* doc)
{
	if(waveforms.size() == 0 && histograms.size() == 0 && datas.size() == 0)
		return 0;

	//Create a new Event-Node:
	tinyxml2::XMLElement* event = doc->NewElement("Event");

	if(name != "")
		event->SetAttribute("Name", name.c_str());
	if(timestamp != "")
		event->SetAttribute("CaptureTime", timestamp.c_str());
	event->SetAttribute("valid",valid);

	//add the waveforms:
	for(std::vector<WaveForm>::iterator it = waveforms.begin(); it != waveforms.end(); ++it)
	{
		tinyxml2::XMLElement* wfm = it->getXMLOutput(doc);
		if(wfm != 0)
			event->LinkEndChild(wfm);
	}

	//add the histograms:
	for(std::vector<Histogram>::iterator it = histograms.begin(); it != histograms.end(); ++it)
	{
		tinyxml2::XMLElement* hist = it->getXMLOutput(doc);
		if(hist != 0)
			event->LinkEndChild(hist);
	}

	//add the text data
	for(std::vector<textdata>::iterator it = datas.begin(); it != datas.end(); ++it)
	{
		tinyxml2::XMLElement* textdat = it->getXMLOutput(doc);
		if(textdat != 0)
			event->LinkEndChild(textdat);
	}


	return event;
}
