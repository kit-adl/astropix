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
 *
 * Original implementation from Felix Ehrler
 */

#ifndef VB_CONFIG_H
#define VB_CONFIG_H

/****************************************************************************
 *                                                                          *
 * Class to control the ADL-VoltageBoards or custom voltage DAC-circuits    *
 *                                                                          *
 * - Simple functions to add one or more VoltageBoards (5 voltages each)    *
 * - Advanced functions to customize everything                             *
 *                                                                          *
 *                                                                          *
 * Author: Felix Ehrler                                                     *
 *                                                                          *
 * Version 1.0 (17.05.2017)                                                 *
 *         2.0 (15.08.2019, Author: Rudolf Schimassek)                      *
 *              added compressed loading / saving                           *
 *              replaced QDateTime with chrono/ctime                        *
 *              added a device name for identification                      *
 ****************************************************************************/

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <fstream>
#include <math.h>

#include <chrono>
#include <ctime>

#include "libs/tinyxml2_addon.h"
#include "libs/zip_file.h"

#include <QString>

class VB_Config
{
public:
    /**
     * @brief VB_Config                 Default Constructor
     */
    VB_Config();

    /**
     * @brief VB_Config                 Constructor for arbitrary DAC-circuits
     * @param VDD                       Supply voltage of the DACs
     * @param Vmax                      Maximum output voltage of the DACs
     */
    VB_Config(double VDD, double Vmax);

    /**
     * @brief VB_Config                 Constructor for VoltageBoards
     * @param NumVB                     Number of VoltageBoards to control. Each VoltageBoard generates 5 voltages. The DACs are named VB1V1 .. VBxVy
     * @param VDD                       Supply voltage of the DACs
     * @param Vmax                      Maximum output voltage of the DACs
     */
    VB_Config(unsigned int NumVB, double VDD, double Vmax);
    virtual ~VB_Config();

    enum ShiftDirection {MSBFirst = 0, LSBFirst = 1};

    std::string GetDeviceName();
    void        SetDeviceName(std::string name);

    /**
     * @brief AddDAC                    Adds a DAC to the system
     * @param name                      name of the added DAC
     * @param bits                      number of bits used in the DAC. Negative value means no rescaling with Vmax/VDD
     * @param sparebits                 number and position of sparebits. negative sparebits are before the used bits.
     * @param shiftdirection            MSB or LSB
     * @param initialVoltage            initial voltage setting
     * @return                          returns reference to this instance to enable config chaining
     * @throws                          std runtime exception if initialVoltage value is not between 0 and Vmax defined in constructor
     */
    VB_Config&  AddDAC(std::string name, int bits, int sparebits, int shiftdirection, double initialVoltage);

    /**
     * Add a DAC for the Load Shift Register on the GECCO  Adapter.
     * DAC is 8 bits with no scaling, provided positino sets a 1 for the bit position
     * representing the Load signal for the function card.
     *
     * For the first function card, Load Ld_C1 , set position to 1
     *
     * @brief addLoadShiftRegister
     * @param position
     * @return
     */
    VB_Config& addLoadShiftRegister(unsigned int position);

    /**
     * @brief ClearDACs deletes all entries in the instance
     */
    void ClearDACs();

    /**
     * @brief GetDACIndexToName         Returns the DAC index for a given DAC name
     * @param name                      name of the requested DAC
     * @return                          returns the index of the requested DAC, or -1 if the DAC has not been found
     */
    int GetDACIndexToName(std::string name);

    /**
     * @brief GetDACNameToIndex         Returns the name of a DAC for a given index
     * @param index                     index if the requested DAC
     * @return                          returns the name of the DAC or an empty string if the DAC could not be found
     */
    std::string GetDACNameToIndex(unsigned int index);

    /**
     * @brief GetDAC                    Returns the DAC value for a given index
     * @param index                     DAC index
     * @return                          returns the DAC value or -1 if DAC could not be found
     */
    double GetDAC(unsigned int index);

    /**
     * @brief GetDAC                    Returns the DAC value for a given DAC name
     * @param name                      DAC name
     * @return                          returns the DAC value or -1 if DAC could not be found
     */
    double GetDAC(std::string name);

    /**
     * @brief GetDACWidth               Returns the number of bits of a certain DAC
     * @param index                     the index of the requested DAC
     * @return                          returns -1 for invalid request, else the number of bits
     */
    int GetDACWidth(unsigned int index);

    /**
     * @brief GetDACWidth               Returns the number of bits of a certain DAC
     * @param name                      name of the requested DAC
     * @return                          returns -1 for invalid request, else the number of bits
     */
    int GetDACWidth(std::string name);

    /**
     * @brief GetDACSpare               Returns the number of spare bits
     * @param index                     index of the requested DAC
     * @return                          returns the number of spare bits of the requested DAC. Signum indicates whether the spare bits are placed before or after the used bits
     */
    int GetDACSpare(unsigned int index);

    /**
     * @brief GetDACSpare               Returns the number of spare bits
     * @param name                      name of the requested DAC
     * @return                          returns the number of spare bits of the requested DAC. Signum indicates whether the spare bits are placed before or after the used bits
     */
    int GetDACSpare(std::string name);

    /**
     * @brief GetDACBitOrder            Returns the bit order of a DAC
     * @param index                     index of the requested DAC
     * @return                          returns -1 if index is out of range, else the bit order
     */
    int GetDACBitOrder(unsigned int index);

    /**
     * @brief GetDACBitOrder            Returns the bit order of a DAC
     * @param name                      name of the requested DAC
     * @return                          returns and empty string if the name has not been found, else the bit order
     */
    int GetDACBitOrder(std::string name);

    /**
     * @brief GetDACName                Returns the name of a DAC
     * @param index                     index of the requested DAC
     * @return                          Returns and empty string if the name has not been found, else the name
     */
    std::string GetDACName(unsigned int index);

    /**
     * @brief SetDAC                    Sets the output voltage of a given DAC
     * @param index                     index of the DAC
     * @param value                     output voltage in V
     * @return                          returns false if the DAC has not been found.
     */
    bool SetDAC(unsigned int index, double value);

    /**
     * @brief SetDAC                    Sets the output voltage of a given DAC
     * @param name                      name of the DAC
     * @param value                     output voltage in V
     * @return                          returns false if the DAC has not been found.
     */
    bool SetDAC(std::string name, double value);

    /**
     * @brief GetEntries                Returns the number of DACs in this Object
     * @return                          number of DACs
     */
    unsigned int GetEntries(){return values.size();}


    /**
     * @brief GenerateBitVector         Generates a configuration bit stream
     * @param shiftdirection            this option can turn around the whole bit stream
     * @return                          returns a vector of bools.
     */
    std::vector<bool> GenerateBitVector(int shiftdirection = MSBFirst);

    /**
     * @brief LoadFromXMLElement extracts the data for this object from an XML subtree
     * @param root          - root node of the subtree to load the data from
     * @return              - XML_NO_ERROR on success or an error code
     */
    virtual tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* root);
    /**
     * @brief LoadFromXMLFile loads the contents of the instance from an XML file
     * @param filename      - file name of the file to load from
     * @param compressed    - if true, interpert the file at `filename` as zip archive and replace
     *                          the extension by "xml" to select the file inside the archive.
     *                          For false, use the passed file as the XML file directly
     * @return              - XML_NO_ERROR on success or an error code otherwise
     */
    virtual tinyxml2::XMLError LoadFromXMLFile(std::string filename, bool compressed = false);

    /**
     * @brief SaveToXMLElement generates an XML tree structure from the contained data for the
     *          passed XML document
     * @param doc           - XML document to host the tree to build
     * @param devicename    - string to override the stored device name / identification string of
     *                          the instance, keep empty to use the internal one
     * @return              - the root node of the XML tree structure or a null pointer on an error
     */
    virtual tinyxml2::XMLElement* SaveToXMLElement(tinyxml2::XMLDocument& doc, std::string devicename = "");
    /**
     * @brief SaveToXMLFile generates a tree structure from the instance and writes it to a file or
     *          archive at the given filename
     * @param filename      - filename of the file or archive to save to
     * @param identifier    - identification string for the instance in the XML structure to override
     *                          the internal one, keep empty to use the internal one
     * @param compressed    - for true, create a zip archive with `filename` as name and replace the
     *                          file extension by "xml" for the file in the archive.
     *                          For false, uses `filename` directly for the XML file
     * @return              - XML_NO_ERROR on success, an error code otherwise
     */
    virtual tinyxml2::XMLError SaveToXMLFile(std::string filename, std::string devicename = "", bool compressed = false);


    ///provides the maximum output voltage for the voltage board
    double  GetVmax();
    bool SetVmax(double value);

    ///provides the supply voltage of the voltage board
    double  GetVDD();
    void    SetVDD(double value);

protected:
    std::string devicename;
    std::map<int, std::string>  indextoname;
    std::map<std::string, int>  nametoindex;
    std::vector<double>         values;
    std::vector<int>            shiftdir;
    std::vector<std::pair<int, int> > width;    //width, spare; negative spare bit means spare bits before used bits, and reverse
    double Vmax;
    double VDD;
};

#endif // VB_CONFIG_H
