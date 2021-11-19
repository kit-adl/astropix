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

#include "Histogram.h"

Histogram::Histogram() : name(""), 	horizontal(orientations::vertical), valid(true), 
                            firstbin(-1e10), stepwidth(-1e10), integral(0), mean(-1), sigma(-1),
                            amplitude(-1)
{
    //ctor
}

Histogram::Histogram(std::string title, bool orientation) : valid(true), firstbin(-1e10), 
																stepwidth(-1e10), integral(0)
{
	name = title;
	horizontal = orientation;
}

Histogram::Histogram(tinyxml2::XMLElement* element)
{
	name = checkString(element->Attribute("Name"));
    horizontal = (std::string(element->Attribute("Typ")).compare("horizontal") == 0)?orientations::horizontal
													:orientations::vertical;
	firstbin = element->DoubleAttribute("FirstBinMean");
	stepwidth = element->DoubleAttribute("BinWidth");
	bool val;
	if(element->QueryBoolAttribute("valid", &val) != tinyxml2::XML_NO_ATTRIBUTE)
		valid = val;
	else
		valid = true;


	//load the bin data to the object:
	std::string bindata = checkString(element->GetText());	//the csv data
	int value = 0;
	int index = 0;
	int length = bindata.length();
	bool minus = false;
	char character;


	while(index < length)
	{
		character = bindata.c_str()[index];
		if(character == '-')
			minus = true;
		else if(character < 58 && character > 47)
			value = value * 10 + character - 48;
		else if(character == ',')
		{
			if(minus)
				value = -value;

			data.push_back(value);
			value = 0;
			minus = false;
		}
		else	//abort on wrong character
		{
			data.clear();
			return;
		}

		++index;
	}

	if(character != ',')
	{
		if(minus)
			value = -value;

		data.push_back(value);
	}
    minimum = 0;
}

Histogram::~Histogram()
{
    //dtor
}

bool Histogram::informationComplete()
{
	if(firstbin == -1e10 || stepwidth == -1e10 || name == "")
		return false;
	else 
		return true;
}

bool Histogram::setName(std::string val)
{
	name = val;
	return true;
}

bool Histogram::getValidity()
{
	return valid;
}

bool Histogram::setValidity(bool validity)
{
	valid = validity;
	return true;
}

bool Histogram::setFirstBinMean(double mean)
{
	firstbin = mean;
	return true;
}

bool Histogram::setBinWidth(double width)
{
	if(width == 0)
		return false;

	stepwidth = width;
	return true;
}

double Histogram::getIntegral(bool entries)
{
	if(entries)
		return integral;
	else
        return integral * fabs(stepwidth);
}

double Histogram::GetMean()
{
    return mean;
}

void Histogram::SetMean(double value)
{
    mean = value;
}

double Histogram::GetSigma()
{
    return sigma;
}

void Histogram::SetSigma(double value)
{
    sigma = value;
}

double Histogram::GetAmplitude()
{
    return amplitude;
}

void Histogram::SetAmplitude(double value)
{
    amplitude = value;
}

void Histogram::calculateIntegral()
{
	unsigned int sum = 0;
	for(std::vector<int>::iterator it = data.begin(); it != data.end(); ++it)
		sum += *it;

	integral = sum;
}



double Histogram::FindMinimum()
{

    calculateIntegral();
    double sum = 0;
    unsigned int binno = 0;
    minimum = 0;
    for(std::vector<int>::iterator it = data.begin(); it != data.end(); ++it){
        sum += *it;
        if(sum < 0.01*integral){
            minimum = firstbin + binno*stepwidth;
            binno++;
        }
    }
    return minimum;
}


void Histogram::clearHistogram()
{
	data.clear();
	integral = 0;
}

void Histogram::addBins(unsigned int numBins)
{
    for(unsigned int i = 0; i < numBins; ++i)
		data.push_back(0);
}

int Histogram::getBinContent(unsigned int bin)
{
	if(bin < data.size())
		return data[bin];
	else
		return -1;
}

bool Histogram::setBinContent(unsigned int bin, int value)
{
	if(bin < data.size())
	{
		integral -= data[bin] - value;
		data[bin] = value;
		return true;
	}
	else
		return false;
}

bool Histogram::addBinContent(unsigned int bin, int value)
{
	if(bin < data.size())
	{
		data[bin] += value;
		integral += value;
		return true;
	}
	else
		return false;
}

bool Histogram::Fill(double position, int value)
{
	return addBinContent(getIndexToPosition(position), value);
}

int Histogram::Fill(unsigned int firstBin, std::string values)
{
    unsigned int bin = firstBin; //firstbin;
	int value = 0;

	int length = values.length();
	for(int i = 0; i < length; ++i)
	{
		if(values.c_str()[i] != ',')
		{
			value = value*10 + values.c_str()[i] - 48;
		}
		else
		{
			if(bin >= data.size())
				data.push_back(value);
			else
				data[bin] += value;

			integral += value;

			value = 0;
			++bin;
		}
	}
	//also add the last bin content
	if(bin >= data.size())
		data.push_back(value);
	else
		data[bin] += value;

	integral += value;

	return integral;

}

unsigned int Histogram::getIndexToPosition(double position)
{
	position -= firstbin - stepwidth/2;	//shift the lower end of the first bin to 0

	return position / stepwidth;
}

double Histogram::getPositionToIndex(unsigned int bin)
{
	if(bin < data.size())
		return firstbin + bin*stepwidth;
	else
		return -1e10;
}

std::vector<int>::iterator Histogram::getDataIteratorBegin()
{
	return data.begin();
}

std::vector<int>::iterator Histogram::getDataIteratorEnd()
{
	return data.end();
}

tinyxml2::XMLElement* Histogram::getXMLOutput(tinyxml2::XMLDocument* doc)
{
	if(!informationComplete() || data.size() == 0)
        return nullptr;

	tinyxml2::XMLElement* hist = doc->NewElement("Histogram");

	//add the parameters as attributes:
	hist->SetAttribute("Name",         escapeString(name).c_str());
	hist->SetAttribute("Typ",          ((horizontal)?"horizontal":"vertical"));
	hist->SetAttribute("FirstBinMean", firstbin);
	hist->SetAttribute("BinWidth",     stepwidth);
	hist->SetAttribute("valid",        valid);

    if(amplitude > 0)
    {
        hist->SetAttribute("gaussianamplitude", amplitude);
        hist->SetAttribute("gaussianmean", mean);
        hist->SetAttribute("gaussiansigma", sigma);
    }

	//add the data:
	std::stringstream xmldata("");

	std::vector<int>::iterator it = data.begin();
	xmldata << *it;	//no check as data.size()==0 is checked at the beginning
	for(++it; it != data.end(); ++it)	//add the rest of the data comma separated
	{
		xmldata << "," << *it;
	}

	hist->SetText(xmldata.str().c_str());

	return hist;
}

std::string Histogram::Print()  //To generate a human readable format for saving to a file
{
	std::stringstream filecontent("");
	//do nothing on empty histogram (no bins):
	if(data.size() == 0)
        return "";

	filecontent << "# " << name << ((horizontal)?" (horizontal)":" (vertical)") << std::endl
				<< "# Integral: " << integral << std::endl;

    if(amplitude > 0)
        filecontent << "# Gaussian parameters: amplitude = " << amplitude << " ; mean = " << mean
                    << " ; sigma = " << sigma << " ; minumum = " << minimum <<  std::endl;

	double mean = firstbin;
	for(std::vector<int>::iterator it = data.begin(); it != data.end(); ++it)
	{
		filecontent << mean << "\t" << *it << std::endl;
		mean += stepwidth;
	}
	
    return filecontent.str();
}

bool Histogram::PrintToFile(std::string filename)
{
	if(data.size() == 0)	//do not do anything with no bins
		return false;

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
