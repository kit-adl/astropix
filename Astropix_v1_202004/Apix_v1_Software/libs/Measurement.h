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
 * The measurement class is part of a class structure to        *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The Measurement class holds a vector of event objects.       *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/
#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>

//XML Library:
#include "tinyxml2.h"
#include "tinyxml2_addon.h"

//Compression Library:
#include "zip_file.h"

#include "Event.h"
#include "ConfigPart.h"

class Measurement
{
	public:
		Measurement();
		Measurement(std::string name);
		Measurement(Measurement& templ);	//to copy everything except the events
		Measurement(tinyxml2::XMLDocument* doc, int measurementnumber = 1);
		~Measurement();

		/**
		 * @brief checks whether all information is provided or not
		 * @details
		 * @return            - true on complete information set, false otherwise
		 */
		bool informationComplete();

		/**
		 * @brief returns the name of the measurement
		 * @details
		 * @return            - name of the measurement
		 */
		std::string getName(){return name;}
		/**
		 * @brief changes the name of the measurement
		 * @details
		 * 
		 * @param newname     - the text to change the name to
		 * @return            - true if the new name was accepted, false if not
		 */
		bool setName(std::string newname);
		/**
		 * @brief returns the description of the measurement
		 * @details
		 * @return            - the description text of the measurement
		 */
		std::string getDescription(){return description;}
		/**
		 * @brief changes the description text of the measurement
		 * @details 
		 * 
		 * @param text        - the new description text
		 * @return            - true if the description text was accepted, false if not
		 */
		bool setDescription(std::string text);
		/**
		 * @brief returns the sensor sample measured
		 * @details returns the text describing which sensor was used in this measurement
		 * @return            - the sensor sample description
		 */
		std::string getSample(){return sample;}
		/**
		 * @brief changes the sensor sample description
		 * @details
		 * 
		 * @param text        - the new sample description
		 * @return            - true if the new text was accepted, false if not
		 */
		bool setSample(std::string text);

		/**
		 * @brief returns the text describing the measurement devices (e.g. Tektronix MDO3024,...)
		 * @details
		 * @return            - the string describing the measurement devices
		 */
		std::string getMeasurementDevice(){return device;}
		/**
		 * @brief changes the string describing the measurement devices
		 * @details
		 * 
		 * @param text        - the new device description
		 * @return            - true if the new text was accepted, false if not
		 */
		bool setMeasurementDevice(std::string text);
		/**
		 * @brief returns the settings and configurations of the first trigger
		 * @details
		 * @return            - the string describing the settings and configurations of the first 
		 *  				  	  trigger
		 */	
		std::string getTriggerA(){return triggera;}
		/**
		 * @brief changes the trigger configuration description
		 * @details
		 * 
		 * @param text        - string describing the new trigger configuration
		 * @return            - true on acceptance, false if not
		 */
		bool setTriggerA(std::string text);
		/**
		 * @brief returns the settings and configurations of the second trigger
		 * @details 
		 * @return            - string describing the settings of the second trigger
		 */
		std::string getTriggerB(){return triggerb;}
		/**
		 * @brief changes the configuration description string for the second trigger
		 * @details
		 * 
		 * @param text        - the new configuraion description of the second trigger
		 * @return            - true on acceptance, false otherwise
		 */
		bool setTriggerB(std::string text);

		/**
		 * @brief returns the number of configuration parameters for this measurement
		 * @details
		 * @return            - the number of parameters
		 */
		unsigned int getNumConfigurations();
		/**
		 * @brief deletes all configuration parameters from the measurement
		 * @details
		 */
		void clearConfigurations();
		/**
		 * @brief adds a configuration value to the measurement
		 * @details
		 * 
		 * @param part        - the configuration parameter object to add
		 * @return            - true on success, false otherwise
		 */
		bool addConfigurationValue(ConfigPart &part);
		/**
		 * @brief adds an integer configuration parameter to the measurement
		 * @details
		 * 
		 * @param name        - descriptor of the parameter (e.g. 'Vias-DAC')
		 * @param value       - integer value of the parameter
		 * 
		 * @return            - true on successful adding to the measurement
		 */
		bool addConfigurationValue(std::string name, int value);
		/**
		 * @brief adds a double configuration parameter to the measurement
		 * @details
		 * 
		 * @param name        - descriptor of the parameter (e.g. 'Threshold Voltage')
		 * @param value       - double value of the parameter
		 * 
		 * @return            - true on successful adding to the measurement
		 */
		bool addConfigurationValue(std::string name, double value);

		/**
		 * @brief returns a configuration parameter
		 * @details gets a ConfigPart object of the measurement or a null pointer
		 * 
		 * @param index       - index of the parameter
		 * @return            - a ConfigPart object pointer or a null pointer on incalid index
		 */
		ConfigPart* getConfigurationValue(unsigned int index);
		/**
		 * @brief returns a configuration parameter
		 * @details returns a ConfigPart object pointer to a name
		 * 
		 * @param name        - the parameter name to search for
		 * @return            - a ConfigPart object pointer or a null pointer if the given name
		 *                         was not found
		 */
		ConfigPart* getConfigurationValue(std::string name);
		/**
		 * @brief returns an iterator pointing to the first configuration parameter
		 * @details
		 * @return            - an iterator pointing to the first element in the 
		 * 							configuration vector
		 */
		std::vector<ConfigPart>::iterator getConfigurationValuesBegin();
		/**
		 * @brief returns an iterator pointing to the end element of the configuration vector
		 * @details
		 * @return            - an iterator at the end element of the configuration vector
		 */
		std::vector<ConfigPart>::iterator getConfigurationValuesEnd();
		/**
		 * @brief removes an element from the configuration vector
		 * @details
		 * 
		 * @param it          - iterator pointing to the ConfigPart object to remove from 
		 *                        the vector
		 * @return            - an iterator pointing to the new element at the position of the
		 *                        deleted element
		 */
		std::vector<ConfigPart>::iterator deleteConfigurationValue(
								std::vector<ConfigPart>::iterator it);

		/**
		 * @brief removes all events from the measurement
		 * @details all events in this measurement are deleted, the other settings remain unchanged
		 */
		void clearEvents();
		/**
		 * @brief creates a new event in this measurement and returns a pointer to the event to
		 *             fill it with data
		 * @details creates an empty event and adds it to the measurement. A pointer to the event
		 *             is returned to fill it with data
		 * 
		 * @param name        - name of the event to create
		 * @param capturetime - point in time of the measurement of the event
		 * @return            - a pointer to the event created
		 */
		Event* addEvent(std::string name = "", std::string capturetime = "");
		/**
		 * @brief adds an event to the measurement
		 * @details the event provided is copied to the measurement
		 * 
		 * @param evnt        - the event to add to the measurement
		 * @return            - true if the event was accepted and added, false if not
		 */
		bool addEvent(Event* evnt);
		/**
		 * @brief returns the number of events in the measurement
		 * @details
		 * @return            - the number of events in the measurement
		 */
		unsigned int getNumEvents();

		/**
		 * @brief returns a pointer to an event
		 * @details returns a pointer to an event in the measurement based on the index
		 * 
		 * @param index       - the index of the event in the measurement
		 * @return            - a pointer to the event or a null pointer on invalid index
		 */
		Event* getEvent(unsigned int index);
		/**
		 * @brief returns an event pointer for a specific event name
		 * @details starting from `it` or the first event if not provided, the events are searched
		 *            for an event with the given name
		 * 
		 * @param name        - the name of the event to find
		 * @param it          - iterator pointing to the first event to consider in the search,
		 *                        passing a null pointer, the search starts at the first event in
		 *                        the measurement
		 * 
		 * @return            - a pointer to the event found or a null pointer if no event with
		 *                         name `name` was found
		 */
		Event* getEvent(std::string name, std::vector<Event>::iterator* it = 0);

		/**
		 * @brief returns an iterator pointing to the first event in the measurement
		 * @details
		 * @return            - iterator pointing to the first event
		 */
		std::vector<Event>::iterator getEventBegin();
		/**
		 * @brief returns an iterator pointing to the end position in the event vector if the
		 *           measurement
		 * @details
		 * @return            - iterator pointing to the end element of the vector
		 */
		std::vector<Event>::iterator getEventEnd();

		/**
		 * @brief generates XML output for the measurement to save it to an archive or a file
		 * @details the measurement structure with all events is converted to an XML tree structure
		 *            and written to a stringstream for displaying, saveing and compressing
		 * 
		 * @param doc         - XMLDocument to add the measurement data to. If a null pointer is
		 *                        passed, an XMLDocument is generated internally. This is to write
		 *                        several measurements to one file, not deleting the XMLDocument
		 *                        and passing it to this method of several measurement objects.
		 * @param deleteDoc   - deletes the XMLDocument after the output was generated if true
		 * 
		 * @return            - stringstream with the textoutput in XML format
		 */
        std::string generateFileStream(tinyxml2::XMLDocument* doc = 0, bool deleteDoc = true);
		/**
		 * @brief compresses the filestream and saves it in an archive
		 * @details the provided filestream is compressed and saved in an archive using the
		 *             passed filename
		 * 
		 * @param archive     - archive object to save the data to
		 * @param filename    - filename to use for the data in the archive
         * @param filedata    - data to compress into the archive
		 * @return            - true on successful writing, false otherwise
		 */
        bool compressFileStream(zip_file& archive, std::string filename, std::string &filedata);
		/**
		 * @brief saves the data of the measurement to a file
		 * @details the data of the measurement is saved to a file (uncompressed) or an archive
		 *            (compressed) where the filename and the archive name are the same
		 * 
		 * @param filename    - name of the file and the archive (for compressed saving)
		 * @param compressed  - save as an archive if true, or as an XML file if false
		 * 
		 * @return            - true on successful writing, false otherwise
		 */
		bool saveToFile(std::string filename, bool compressed = true);

		/**
		 * @brief loads the data for a measurement from the XMLDocument
		 * @details copies the data from the `measurementnumber`th measurement structure to this
		 *            Measurement object
		 * 
		 * @param doc         - the loaded tinyxml2::XMLDocument containing the data to copy
		 * @param measurementnumber
		 * 					  - the number of the measurement structure to load
		 * 
		 * @return 			  - 0 on success, an error code otherwise
		 */
		int extractFromXMLTree(tinyxml2::XMLDocument* doc, int measurementnumber = 1);
		/**
		 * @brief loads measurement data from an XML file
		 * @details copies the n-th measurement structure from the file to the object structure
		 * 
		 * @param filename    - the file to load from
		 * @param measurementnumber
		 * 					  - the number of the measurement structure to load
		 * 
		 * @return            - 0 on success, an error code otherwise (>0 XML loading, <0 structure
		 *                        analysis error)
		 */
		int loadFromFile(std::string filename, int measurementnumber = 1);
		/**
		 * @brief loads a measurement structure from an archive
		 * @details
		 * 
		 * @param archivename - file name of the archive
		 * @param filename    - file name inside the archive
		 * @param measurementnumber
		 *                    - number of the measurement structure to load
		 * @return            - 0 on success, an error code otherwise
		 */
		int loadFromArchive(std::string archivename, std::string filename, 
								int measurementnumber = 1);

	protected:

	private:
		std::string name;			//measurement (e.g. 'Injection Circuit Calibration')
		std::string description;	//description (setup details, ...)

		std::string sample;			//measured device (e.g. 'H35Demo 80Ω #1')

		std::string device;			//measurement device (e.g. 'Tektronix MDO 3024')
		std::string triggera;
		std::string triggerb;

		std::vector<Event> events;

		//Measurement configuration:
		std::vector<ConfigPart> configparameters;
};

#endif //MEASUREMENT_H
