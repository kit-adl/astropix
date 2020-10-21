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

/****************************************************************
 * The textdata class is part of a class structure to           *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The textdata class holds arbitrary data as text for sources  *
 * other than waveforms or histograms.                          *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <string>
#include <sstream>
#include <fstream>

#include "tinyxml2.h"
#include "tinyxml2_addon.h"

class textdata
{
	public:
		textdata();
		textdata(std::string title);
		textdata(std::string title, std::string content);
		textdata(tinyxml2::XMLElement* element);
		~textdata();

		bool informationComplete();

		std::string getName();
		bool setName(std::string title);

		bool getValidity();
		bool setValidity(bool newvalidity);

		std::string getData();
		bool setData(std::string content);
		void clearData();
		bool addData(std::string addcontent);

		tinyxml2::XMLElement* getXMLOutput(tinyxml2::XMLDocument* doc);

        std::string Print();
		bool PrintToFile(std::string filename);

	protected:

	private:
		std::string name;
		std::string data;
		bool valid;
};

#endif //TEXTDATA_H
