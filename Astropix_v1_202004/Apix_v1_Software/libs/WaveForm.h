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
 * The WaveForm class is part of a class structure to           *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The WaveForm class holds the data of one waveform either in  *
 * compressed format as from the oscilloscope or decoded. The   *
 * decoded data can also be exported.                           *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <fstream>

#include "tinyxml2.h"
#include "tinyxml2_addon.h"

#include "Point.h"


class WaveForm
{
    public:
        WaveForm();
        WaveForm(std::string name);
        WaveForm(tinyxml2::XMLElement* element);
        virtual ~WaveForm();

        /**
         * informationComplete() queries whether all additional information is set.
         *      At the moment no plausibility checks are performed. Only values different from
         *      the standard values are accepted.
         *
         * @return          - true if all parameters have been set, false if not
         */
        bool informationComplete();

        /**
         * getName() returns the currently set name for the waveform
         *      Its purpose is to identify the signal (e.g. 'scintillator')
         * 
         * @return          - name of the waveform
         */
        std::string getName(){return name;}
        /**
         * setName() changes the name of the waveform
         *
         * @param val       - new name for the waveform
         */
        void setName(std::string val){name = val;}
        /**
         * getBandwidth() sets a text string as additional information about the measurement
         *      it is intended to be the used bandwidth of the oscilloscope
         *
         * @return          - currently set bandwidth
         */
        std::string getBandwidth(){return bandwidth;}
        /**
         * setBandwidth() changes the value for the bandwidth without checks
         *
         * @param val       - text string representing the new bandwith value
         */
        void setBandwidth(std::string val){bandwidth = val;}
        /**
         * @brief returns the validity flag state
         * @details
         * @return           - the validity flag state
         */
        bool getValidity();
        /**
         * @brief sets the validity flag of the waveform
         * @details
         * 
         * @param validity   - the new validity flag state
         * @return           - true
         */
        bool setValidity(bool validity);

        /**
         * getTimestep() returns the time difference between two neighbouring points
         *
         * @return          - time difference between two points
         */
        double getTimestep(){return timestep;}
        /**
         * setTimestep() sets the time difference between two neighbouring points in the waveform
         *
         * @param val       - new time difference
         * @return          - true if the new value was accepted, false if not
         */
        bool setTimestep(double val);
        /**
         * getScale() returns the voltage difference between the digitisation steps 
         *      of the binary data
         *
         * @return          - voltage difference per digitisation step
         */
        double getScale(){return scale;}
        /**
         * setScale() sets a new value for the digitisation step height
         *
         * @param val       - new digitisation step value
         * @return          - true if the new value has been accepted, false if not
         */
        bool setScale(double val);
        /**
         * getYOffset() returns the offset of the binary data in volts not digitisation steps
         *
         * @return          - offset voltage of the data in the waveform
         */
        double getYOffset(){return yoffset;}
        /**
         * setYOffset() sets a new offset voltage for the binary data
         * 
         * @param val       - new offset voltage
         * @return          - true if the value was accepted, false if not
         */
        bool setYOffset(double val);
        /**
         * @brief getChannelOffset returns the offset of the channel capture in volt
         * @return          - voltage to be added to the decoding result when decompressing waveforms.
         */
        double getChannelOffset(){return channeloffset;}
        /**
         * @brief setChannelOffset sets a new value for the channel offset for waveform decoding
         * @param offset    - the offset voltage in volt to be added to the decoding result
         * @return          - true if the value was accepted, false if not
         */
        bool setChannelOffset(double offset);
        /**
         * getBytesPerPoint() returns the number of bytes in the binary transmission that represent
         *       one data point
         *
         * @return          - number of bytes used per data point
         */
        unsigned short getBytesPerPoint(){return bytesPerPoint;}
        /**
         * setBytesPerPoint() changes the number of bytes used per data point
         *
         * @param val       - new number of bytes to use per data point
         * @return          - true if the new value was accepted, false if not
         */
        bool setBytesPerPoint(unsigned short val);
        /**
         * getBinFormat() returns the encoding of the raw data sent out by the oscilloscope
         *
         * @return          - a text string describing the encoding
         */
        std::string getBinFormat(){return binformat;}
        /**
         * setBinFormat() changes the format of the interpretation of the data from the 
         *      oscilloscope. Currently, only binary data is supported
         *
         * @param val       - new encoding to use for the data
         * @return          - true if the new value was accepted, false if not
         */
        bool setBinFormat(std::string val);


        /**
         * clearBinaryData() removes all binary data from the waveform
         *
         * @return          - true on success, false otherwise
         */
        bool clearBinaryData();
        /**
         * addBinaryData() appends the data provided to the binary data
         *
         * @param data      - data to append to the binary data
         * @param length    - number of bytes in data
         * @return          - length of binary data including the new data
         */
        unsigned int addBinaryData(char* data, unsigned int length);
        /**
         * addBinaryData() appends the data provided to the binary data
         *
         * @param data      - data to append to the binary data
         * @return          - length of binary data including the new data
         */
        unsigned int addBinaryData(std::string data);
        /**
         * getBinaryData() returns parts of the binary data as a vector of chars
         *
         * @param start     - first data point to write out
         * @param ende      - last data point to write out
         * @return          - the part of the binary data between `start` and `ende`
         */
        std::vector<char> getBinaryData(unsigned int start, unsigned int ende);
        /**
         * getBinaryLength() returns the length of the binary data
         *
         * @param points    - if true, the number of data points represented by the data is
         *                      returned. If points=false, the actual length of the data 
         *                      (points*bytesPerPoint) is returned
         * @return          - the number of points (`points`=true), or the number of bytes 
         *                      for `points`=false
         */
        unsigned int getBinaryLength(bool points = true);


        /**
         * decompressData() generates the useable data from the binary data with the parameters
         *      provided
         *
         * @param force     - forces the method to decompress data overwriting the current
         *                      decompressed data in the waveform if set to true
         * @return          - true on success, false otherwise
         */
        bool decompressData(bool force = false);

        /**
         * getLength() returns the length of the decompressed waveform
         * 
         * @return          - length of the waveform im points
         */
        int getLength(){return data.size();}
        /**
         * getPoint() returns one data point from the waveform
         *
         * @param point     - index of the point to return
         * @return          - the data point from the waveform or (0,0) for invalid indices
         */
        Point getPoint(unsigned int point);
        /**
         * getVoltage() returns the voltage to a given time
         *
         * @param time          - the time to get the voltage at
         * @param interpolate   - use the voltage of the last data point before `time` if false
         *                          or interpolate the two surrounding points if true
         * @return              - the voltage at `time` or the next point to it
         */
        double getVoltage(double time, bool interpolate = false);
        /**
         * getDataIteratorBegin() returns an iterator pointing to the first element of the waveform
         *
         * @return           - an iterator pointing to the first element
         */
        std::vector<Point>::iterator getDataIteratorBegin();
        /**
         * getDataIteratorEnd() returns an iterator pointing to the end element of the vector
         *
         * @return           - an iterator pointing to the end element
         */
        std::vector<Point>::iterator getDataIteratorEnd();

        //TODO: functions to insert also decompressed Data and then change the output method to
        //          write out the human readable data points in the xml
        /**
         * @brief adds a data point to the decompressed data
         * @details results in pointwise data output in the file
         * 
         * @param pnt        - the point to add
         * @return           - true on success
         */
        bool addPoint(Point pnt);
        /**
         * @brief adds a data point to the decompressed data
         * @details results in pointwise data output in the file
         * 
         * @param x          - the x value of the point to add
         * @param y          - the y value of the point to add  
         *  
         * @return           - true on success
         */
        bool addPoint(double x, double y);
        /**
         * @brief deletes a point from the decompressed data 
         * @details
         * 
         * @param            - the point to delete from the data vector
         * @return           - a new pointer at the position of the deleted element
         */
        std::vector<Point>::iterator deletePoint(std::vector<Point>::iterator);
        /**
         * @brief removes all decompressed data from the waveform
         * @details also sets the save format to binary again
         */
        void clearData();

        /**
         * @brief generates a tree structure of the waveform
         * @details the waveform data is copied to a tinyxml2 tree structure dependent on the
         *             provided XMLDocument and returned
         * 
         * @param doc        - the XMLDocument to which the tree will belong
         * @return           - a tree structure of the waveform or a null pointer if the waveform was empty
         */
        tinyxml2::XMLElement* getXMLOutput(tinyxml2::XMLDocument* doc);

        /**
         * Print() generates a stringstream to write the human readable data to a file
         *
         * @return           - the content for a file (time and voltage)
         */
        std::string Print();
        /**
         * @brief generate human readable data representation and write it to a file
         * @details
         * 
         * @param filename   - the file to write the data to
         * @return           - true on success, false otherwise
         */
        bool PrintToFile(std::string filename);

        /**    Waveform Manipulation Functions     **/
        void SmoothenWaveForm(unsigned int windowradius);
        double GetSignalHeight(unsigned int numbaselinepoints, bool positivesignal = true);

    protected:

    private:
        std::string name;
        std::string bandwidth;
        bool valid;

        //binary data:
        std::vector<char> bindata;
        std::string binformat;
        unsigned short bytesPerPoint;
        double yoffset;
        double scale;
        double timestep;
        double channeloffset;

        //usable data:
        bool savebinary;
        std::vector<Point> data;
};

#endif // WAVEFORM_H
