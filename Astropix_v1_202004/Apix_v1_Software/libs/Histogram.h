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
 * The Histogram class is part of a class structure to          *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The Histogram class holds the data of one histogram and the  *
 * parameters for a Gaussian fit to it.                         *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>


#include "tinyxml2.h"
#include "tinyxml2_addon.h"

enum orientations : bool {vertical = false, horizontal = true};

class Histogram
{
    public:
        Histogram();
        Histogram(std::string title, bool orientation = vertical);
        Histogram(tinyxml2::XMLElement* element);
        virtual ~Histogram();

        /**
         * @brief checks the parameters of the histogram
         * @details checks the bin parameters and the name for changes from the standard values
         * @return          - true if all parameters have been changed, or false otherwise
         */
        bool informationComplete();

        /**
         * @brief get the name/title of the histogram
         * @return          - the name of the histogram as a string
         */
        std::string getName(){return name;}
        /**
         * @brief change the name of the histogram
         * 
         * @param val       - new name for the histogram
         * @return          - true on success, false otherwise
         */
        bool setName(std::string val);
        /**
         * @brief return the orientation of the histogram
         * @details for a horizontal histogram with time as base for binning this function returns
         *              true, for a histogram binning a voltage it is false
         * @return          - true for a horizontal histogram, false for a vertical one
         */ 
        bool getOrientation(){return horizontal;}
        /**
         * @brief set the orientation of the histogram
         * @details set the orientation to true for a horizontal histogram and to false for a 
         *              vertical one
         * 
         * @param orientation   - use the enum orientations to set the new orientation
         */
        void setOrientation(bool orientation){horizontal = orientation;}
        /**
         * @brief returns the validity flag state of the histogram
         * @details
         * @return            - the validity flag state
         */
        bool getValidity();
        /**
         * @brief sets the validity flag of the histogram
         * @details
         * 
         * @param validity    - new validity flag state
         * @return            - true
         */
        bool setValidity(bool validity);

        /**
         * @brief returns the mean of the first bin of the histogram
         * @details this function returns the mean value of the first bin of the histogram
         * @return              - mean of the first bin
         */
        double getFirstBinMean(){return firstbin;}
        /**
         * @brief changes the mean of the first bin
         * @details sets a new value for the first bin's mean
         * 
         * @param mean          - new mean
         * @return              - true if the new value was accepted, false otherwise
         */
        bool setFirstBinMean(double mean);
        /**
         * @brief returns the width of a bin
         * @details the distance between the mean values of two neighbouring bins
         * @return width of a single bin
         */
        double getBinWidth(){return stepwidth;}
        /**
         * @brief changes the width of the bins
         * @details the width of all bins is changed shifting the bins' means
         * 
         * @param width         - new width of a bin
         * @return              - true if the new width was accepted, false if not
         */
        bool setBinWidth(double width);
        /**
         * @brief returns the integral over all bins
         * @details this function returns either the sum of entries or the integral over all bins
         *            of the histogram
         * 
         * @param entries       - if true, the number of entries is returned, for false, the 
         *                          entries are multiplied with the bin width
         * @return              - integral or number of entries
         */
        double getIntegral(bool entries = true);

        ///Methods to set / get parameters of a fitted gaussian distribution:
        double GetMean();
        void   SetMean(double value);
        double GetSigma();
        void   SetSigma(double value);
        double GetAmplitude();
        void   SetAmplitude(double value);
        double FindMinimum();

        /**
         * @brief deletes all bins from the histogram
         * @details 
         */
        void clearHistogram();
        /**
         * @brief returns the number of bins in the histogram
         * @details 
         * @return              - number of bins
         */
        unsigned int getNumBins(){return data.size();}
        /**
         * @brief adds a number of bins to the histogram
         * @details adding a number of bins to the histogram, the histogram will cover a larger
         *              interval
         * 
         * @param numBins       - the number of bins to add
         */
        void addBins(unsigned int numBins);
        /**
         * @brief returns the contents of a bin
         * @details returns the number of entries in the bin with the passed index
         * 
         * @param bin           - index of the bin to return
         * @return              - contents of the chosen bin
         */
        int getBinContent(unsigned int bin);
        /**
         * @brief sets the contents of a bin to a specific value
         * @details replaces the entries of the bin with the passed value
         * 
         * @param bin           - index of the bin to exchange the contents
         * @param value         - new contents of the bin
         * 
         * @return              - true on success, false otherwise
         */
        bool setBinContent(unsigned int bin, int value);
        /**
         * @brief adds a number of entries to a bin
         * @details increases the number of entries in a bin by the passed value
         * 
         * @param bin           - index of the bin to change the contents
         * @param value         - value to add to the bins contents
         * 
         * @return              - true on success, false otherwise
         */
        bool addBinContent(unsigned int bin, int value);
        /**
         * @brief add entries to the bin for a continuous position
         * @details the entries are added to the bin which covers `position`
         * 
         * @param position      - value of the entries to add
         * @param value         - number of entries to add
         * 
         * @return              - true on success, false otherwise
         */
        bool Fill(double position, int value = 1);

        /**
         * @brief fill the histogram from comma separated data
         * @details the comma separated valued are set as number of entries in consecutive bins
         *              starting from a passed bin
         * 
         * @param firstBin      - bin index to add the first number to
         * @param values        - string containing the contents of the bins (comma separated,
         *                          integer and >= 0)
         * 
         * @return              - the total number of entries including the ones from this call
         */
        int Fill(unsigned int firstBin, std::string values);
        /**
         * @brief returns the bin index for position
         * @details the index of the bin covering `position` is calculated and returned
         * 
         * @param position      - position of interest
         * @return              - index of the corresponding bin
         */
        unsigned int getIndexToPosition(double position);
        /**
         * @brief returns the mean of the bin
         * @details the mean of the bin with index `bin` is returned
         * 
         * @param bin           - index of the bin of interest
         * @return              - mean of the bin
         */
        double getPositionToIndex(unsigned int bin);

        /**
         * @brief returns an iterator pointing to the first element
         * 
         * @return              - an iterator pointing to the first bin
         */
        std::vector<int>::iterator getDataIteratorBegin();
        /**
         * @brief returns an iterator pointing to the end element of the bin vector
         * 
         * @return              - an iterator pointing to the last element
         */
        std::vector<int>::iterator getDataIteratorEnd();

        /**
         * @brief generates a tree structure for the histogram
         * @details the histogram data and parameters are copied to a tinyxml tree structure
         * 
         * @param doc           - the XMLDocument to which the tree will belong
         * @return              - the resulting tree root node or a null pointer when information
         *                             is missing
         */
        tinyxml2::XMLElement* getXMLOutput(tinyxml2::XMLDocument* doc);

        /**
         * @brief generates saveable output from the histogram data
         * @details
         * @return              - a stringstream for writing it to a file
         */
        std::string Print();
        /**
         * @brief generates a human readable form of the data and saves it to a file
         * @details
         * 
         * @param filename   - filename to save the data to
         * @return           - true on success, false otherwise
         */
        bool PrintToFile(std::string filename);

    protected:

    private:
        void calculateIntegral();

        std::string name;
        bool horizontal;        //orientation of the histogram
        bool valid;             //flag to mark bad objects

        double firstbin;        //mean of the first bin
        double stepwidth;       //distance to the next mean
        unsigned int integral;  //sum over all bins

        double mean;
        double sigma;
        double amplitude;
        double minimum;

        std::vector<int> data;
};

#endif // HISTOGRAM_H
