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
 * The ConfigPart class is part of a class structure to         *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The ConfigPart class holds data describing measurement       *
 * conditions giving the context of a measurement.              *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef CONFIGPART_H
#define CONFIGPART_H

#include <string>

#include "tinyxml2.h"
#include "tinyxml2_addon.h"

class ConfigPart
{
	public:
		/**
		 * @brief constructor for a configuration value
		 * @details
		 * 
		 * @param intvalue   - true for an integer attribute, false for a double attribute
		 */
		ConfigPart(bool intvalue = true);
		/**
		 * @brief constructor for a configuration value with a name
		 * @details 
		 * 
		 * @param name       - name of the configuration value
		 * @param intvalue   - true for an integer attribute, false for a double attribute
		 */
		ConfigPart(std::string name, bool intvalue = true);
		/**
		 * @brief creates a new ConfigPart object from an XML element
		 * @details
		 * 
		 * @param element    - the element to load from
		 */
		ConfigPart(tinyxml2::XMLElement* element);
		~ConfigPart();

		/**
		 * @brief returne the name of the configuration parameter
		 * @details
		 * @return           - the name of the parameter
		 */
		std::string getName();
		/**
		 * @brief changes the name of the configuration parameter
		 * @details
		 * 
		 * @param newname    - the new name of the parameter
		 * @return           - true on success, false otherwise
		 */
		bool setName(std::string newname);

		/**
		 * @brief returns the format of the parameter
		 * @details 
		 * @return           - true for an integer parameter, false for a double parameter
		 */
		bool getValueFormat();
		/**
		 * @brief changes the value format of the parameter
		 * @details
		 * 
		 * @param intvalue   - true for an integer parameter, false for a double parameter
		 * @return           - true on success
		 */
		bool setValueFormat(bool intvalue);

		/**
		 * @brief sets the parameter to a double value
		 * @details changes the parameter type if necessary and stores the new value
		 * 
		 * @param value      - the new double value of the parameter
		 * @return           - true on success
		 */
		bool setValue(double value);
		/**
		 * @brief sets the parameter to an integer value
		 * @details changes the parameter type if necessary and stores the new value
		 * 	
		 * @param value      - the new integer value of the parameter
		 * @return           - true on success
		 */
		bool setValue(int value);

		/**
		 * @brief returns the integer value of the parameter
		 * @details it does not matter what the type of the parameter is, the last integer value
		 *            set is returned
		 * @return           - the last integer value set for this element
		 */
		int getIntValue();
		/**
		 * @brief returns the double value of the parameter
		 * @details it does not matter what the type of the parameter is, the last double value
		 *           set is returned
		 * @return           - the last double value set for this element
		 */
		double getDoubleValue();
		/**
		 * @brief returns the currently set parameter value
		 * @details the integer or double value currently set is returned as a double
		 * @return           - the parameter value (int is casted to double)
		 */
		double getValue();

		/**
		 * @brief creates an XML element for this configuration value
		 * @details an XML element is created and returned for this configuration part
		 * 
		 * @param doc        - the XML document to which the XML element to create will belong
		 * @return           - the created XML element as a pointer
		 */
		tinyxml2::XMLElement* getXMLOutput(tinyxml2::XMLDocument* doc);
	protected:

	private:
		std::string name;
		int         ivalue;
		double      dvalue;
		bool        intchoice;	//determines whether the parameter is meant to be int (true)
								// or double (false)
};

#endif //CONFIGPART_H
