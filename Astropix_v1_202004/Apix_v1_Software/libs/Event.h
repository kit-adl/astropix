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
 * The event class is part of a class structure to              *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The Event class holds a vector of data objects (waveforms,   *
 * histograms and textdata).                                    *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
//#include<iostream>

#include "tinyxml2.h"
#include "tinyxml2_addon.h"

#include "WaveForm.h"
#include "Histogram.h"
#include "textdata.h"

class Event
{
    public:
        Event();
        Event(std::string title);
        Event(std::string title, std::string capturetime);
        Event(tinyxml2::XMLElement* element);
        virtual ~Event();

        /**
         * @brief checks all waveforms and histograms for completeness
         * @details the informationComplete() function of all histograms and waveforms is called
         *             and the result is false if one object returns false
         * @return           - true if all parameters of all included objects are set, false
         *                       otherwise
         */
        bool informationComplete();

        /**
         * @brief returns the description of the event
         * @details 
         * @return           - the descriptive name of the event
         */
        std::string getName();
        /**
         * @brief sets a description of the event
         * @details sets the description of the event. This information is not necessary
         * 
         * @param title      - description to set
         * @return           - true on acceptance, false otherwise
         */
        bool setName(std::string title);
        /**
         * @brief returns the date and time of the capturing of the event
         * @details
         * @return           - date and time of the capturing
         */
        std::string getTimestamp();
        /**
         * @brief set a new value for the capturing timestamp
         * @details
         * 
         * @param datetime   - the new timestamp
         * @return           - true on acceptance, false otherwise
         */
        bool setTimestamp(std::string datetime);
        /**
         * @brief returns the valid flag of the event
         * @details
         * @return            - the valid flag of the event
         */
        bool getValidity();
        /**
         * @brief sets the validity of the event
         * @details
         * 
         * @param validity    - new validity setting
         * @return            - true
         */
        bool setValidity(bool validity);

        /**
         * @brief removes all waveforms and histograms from the event
         * @details 
         */
        void clearEvent();
        /**
         * @brief adds a waveform to the event
         * @details creates an empty waveform and adds it to the event
         * 
         * @param name       - title/description of the waveform (e.g. 'scintillator')
         * @return           - a pointer to the waveform object to insert the data
         */
        WaveForm* addWaveForm(std::string name);
        /**
         * @brief adds a waveform to the event
         * @details adds a copy of an already existing waveform to the event
         * 
         * @param wfm        - the waveform to include in the event
         * @return           - true on success, false otherwise indicating that something is
         *                       wrong with the histogram (no data, incomplete parameters)
         */
        bool addWaveForm(WaveForm* wfm);
        /**
         * @brief adds a histogram to the event
         * @details adds an empty histogram to the event
         * 
         * @param name       - title/description of the new histogram (e.g. 'Ag spectrum')
         * @param horizontal - orientation of the histogram
         * @return           - a pointer to the histogram to insert the data
         */
        Histogram* addHistogram(std::string name, bool horizontal = vertical);
        /**
         * @brief adds a histogram to the event
         * @details adds a copy of an already existing histogram to the event
         * 
         * @param hist       - the histogram to include in the event
         * @return           - true on success, false otherwise indicating that something is
         *                       wrong with the histogram (no data, incomplete parameters)
         */
        bool addHistogram(Histogram* hist);
        /**
         * @brief adds a text data object to the event
         * @details adds an empty text data object to the event
         * 
         * @param name       - title of the text data object
         * @return           - a pointer to the text data object inserted
         */
        textdata* addTextData(std::string name);
        /**
         * @brief adds a text data object to the histogram
         * @details adds an existing text data object as a copy to the event
         * 
         * @param textdat    - the text data object
         * @return           - true on success, false otherwise
         */
        bool addTextData(textdata* textdat);

        /**
         * @brief returns a pointer to a waveform from the event
         * @details returns a pointer to the waveform to the passed index or a null pointer
         *           on an index out of bounds
         * 
         * @param index      - index of the waveform to get the pointer to
         * @return           - pointer to the waveform or a null pointer of invalid index
         */
        WaveForm* getWaveForm(unsigned int index);
        /**
         * @brief finds a waveform by its name
         * @details searches the waveforms in the event for the passed name/title and returns the
         *            first waveform with a matching title
         * 
         * @param name       - name/title of the waveform to find
         * @return           - pointer to the waveform with the passed name or a null pointer if the
         *                       passed name was not found.
         */
        WaveForm* getWaveForm(std::string name);
        /**
         * @brief returns a pointer to a histogram in the event
         * @details returns a pointer to a histogram from the event to the passed index
         * 
         * @param index      - the index of the histogram to return
         * @return           - pointer to the histogram or a null pointer for an invalid index
         */
        Histogram* getHistogram(unsigned int index);
        /**
         * @brief returns a pointer to a histogram from the event
         * @details searches the event's histograms for the first occurence of the passed
         *            name/title and returns the corresponding histogram
         * 
         * @param name       - name/title of the histogram to find
         * @return           - pointer to the histogram or a null pointer when the name was not 
         *                       found
         */
        Histogram* getHistogram(std::string name);
        /**
         * @brief returns a pointer to a text data object from the event
         * @details returns the text data object corresponding to the passed index
         * 
         * @param index      - index of the text data object to return
         * @return           - the text data object or a null pointer on invalid index
         */
        textdata* getTextData(unsigned int index);
        /**
         * @brief returns a pointer to a text data object
         * @details searches the text data objects' names for the passed one and returns the
         *           object at the first occurence of the name
         * 
         * @param name       - name to find
         * @return           - the text data object or a null pointer if the name is not found
         */
        textdata* getTextData(std::string name);

        /**
         * @brief returns the number of waveforms in the event
         * @details
         * @return           - the number of waveforms in the event
         */
        unsigned int getNumWaveForms();
        /**
         * @brief returns the number of histograms in the event
         * @details 
         * @return           - the number of histograms in the event
         */
        unsigned int getNumHistograms();
        /**
         * @brief returns the number of text data objects in the event
         * @details 
         * @return           - the number of text data objects
         */
        unsigned int getNumTextDatas();
        /**
         * @brief returns the sum of waveforms and histograms in the event
         * @details
         * @return           - the numbers of waveforms and histograms combined
         */
        unsigned int getEntries();

        /**
         * @brief get an iterator pointing to the first waveform
         * @details
         * @return           - a waveform iterator at .begin()
         */
        std::vector<WaveForm>::iterator getWaveFormBegin();
        /**
         * @brief get an iterator pointing to the end element of the waveform vector
         * @details
         * @return           - a waveform iterator at .end()
         */
        std::vector<WaveForm>::iterator getWaveFormEnd();
        /**
         * @brief get an iterator pointing to the first histogram
         * @details
         * @return           - a histogram iterator at .begin()
         */
        std::vector<Histogram>::iterator getHistogramBegin();
        /**
         * @brief get an iterator pointing to the end element of the histogram vector
         * @details 
         * @return           - a histogram iterator at .end()
         */
        std::vector<Histogram>::iterator getHistogramEnd();
        /**
         * @brief get an iterator pointing to the first text data object
         * @details
         * @return           - a text data object iterator at .begin()
         */
        std::vector<textdata>::iterator getTextDataBegin();
        /**
         * @brief get an iterator pointing to the end element of the text data object vector
         * @details 
         * @return           - a text data object pointer at .end()
         */
        std::vector<textdata>::iterator getTextDataEnd();


        tinyxml2::XMLElement* getXMLOutput(tinyxml2::XMLDocument* doc);

    protected:

    private:
        std::string name;
        std::string timestamp;
        bool valid;

        std::vector<WaveForm>  waveforms;
        std::vector<Histogram> histograms;
        std::vector<textdata>  datas;
};

#endif // EVENT_H
