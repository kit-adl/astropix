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

#ifndef ASIC_CONFIG2_H
#define ASIC_CONFIG2_H

/****************************************************************************
 *                                                                          *
 * Class to generically generate bit vectors for configurating sensors      *
 *                                                                          *
 * Author: Rudolf Schimassek                                                *
 *                                                                          *
 * Version 2 (02.05.2017)                                                   *
 *              added support for arbitary permutations of the bits         *
 *		   2.1 (08.08.2019)													*
 *				added bit inversion as necessary for ATLASPix3				*
 *         2.2 (09.08.2019)                                                 *
 *              added a manager class and functionality to generate the XML *
 *                  structure without the file to combine several outputs   *
 *                  into one file                                           *
 *         2.3 (12.08.2019)                                                 *
 *              added a device name property for easier storage of several  *
 *                  data structures in one XML file                         *
 *         2.4 (13.08.2019)                                                 *
 *              re-added the globalinverted options                         *
 *          WARNING: the GlobalInverted[M/L]SBFirst names were wrong before *
 *                      GlobalInvertedMSBFirst resulted in bits shifted in  *
 *                      LSBFirst per register. This was corrected in this   *
 *                      version                                             *
 *         2.5 (15.08.2019)                                                 *
 *              added reading from / writing to zip archives                *
 *              replaced QDateTime by chrono/ctime                          *
 ****************************************************************************/

#define ASIC_CONFIG2_MAJORVERSION 2
#define ASIC_CONFIG2_MINORVERSION 4

#include <QObject>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <fstream>

#include <chrono>
#include <ctime>

#include "libs/tinyxml2_addon.h"
#include "libs/zip_file.h"

typedef unsigned int uint;

class ASIC_Config2
{



public:
    ASIC_Config2(std::string devicename = "device");

    // UI ----
    std::vector<QWidget*> widgets;
    //--------


    enum ShiftDirection {MSBFirst = 0, LSBFirst = 1, GlobalInvertedMSBFirst = 2, GlobalInvertedLSBFirst = 3};
        //when loading from a file, any other string than "LSBFirst" will be interpreted as "MSBFirst"

    std::string GetDeviceName();
    void        SetDeviceName(std::string name);

    /**
     * @brief adds a parameter to the configuration at the end
     * @param name              - the name of the parameter for reference/saving to a file
     * @param bits              - number of bits of the parameter/register
     * @param shiftdirection    - whether the MSB or the LSB is to be shifted in first
     * @param initial           - initial value
     * @param optional          - marks the parameter as optional to remove a part of the
     *                              configuration when necessary
     * @return                  - true if the parameter was accepted, false if not
     */
    ASIC_Config2& AddParameter(std::string name, unsigned int bits = 6, int shiftdirection = MSBFirst,
                      unsigned int initial = 0, bool optional = false);
    /**
      * @brief adds a parameter to the configuration at the end
      *
      * @param name     - the name of the parameter (will be saved in the maps)
      * @param bitorder - the comma separated ordering of the bits (limits: {1,...,30} (32bit signed int))
      * @param initial  - the initial value for the parameter
      * @param optional - marks the parameter as optional to remove a part of the configuration
      *                     when necessary
      * @return         - true if the parameter was accepted, false if not
      */
    ASIC_Config2& AddParameter(std::string name, std::string bitorder = "5,4,3,2,1,0",
                      unsigned int initial = 0, bool optional = false);
    /**
     * @brief ClearParameters removes all parameters from the object
     */
    void ClearParameters();

    int GetParameterIndexToName(std::string name);
    std::string GetParameterNameToIndex(unsigned int index);

    /**
     * @brief provides the current value of the parameter
     * @param index     - the index of the parameter to return
     * @return          - the value of the parameter or -1 on an invalid index
     */
    int GetParameter(unsigned int index);
    /**
     * @brief provides the current value of the parameter
     * @param name      - reference name of the parameter
     * @return          - the value of the parameter or -1 on an invalid name
     */
    int GetParameter(std::string name);
    /**
     * @brief returns the number of bits of the parameter
     * @param index     - index of the parameter
     * @return          - the number of bits for this parameter or -1 for an invalid index
     */
    int GetParameterWidth(unsigned int index);
    /**
     * @brief returns the number of bits for the parameter requested
     * @param name      - reference name for the parameter to return
     * @return          - the number of bits for this parameter or -1 on an invalid name
     */
    int GetParameterWidth(std::string name);
    /**
     * @brief reuturns the order of the bits for this parameter
     * @param index     - index of the parameter to return
     * @return          - the comma separated order of the bits or an empty string on an invalid index
     */
    std::string GetParameterBitOrder(unsigned int index);
    /**
     * @brief returns the order of the bits for the parameter requested
     * @param name      - reference name of the index to return
     * @return          - the comma separated order of the bits or an empty string on invalid reference name
     */
    std::string GetParameterBitOrder(std::string name);

    /**
     * @brief returns the name of the parameter at `index`
     * @param index     - index of the parameter of interest
     * @return          - the parameter name or an empty string on invalid index
     */
    std::string GetParameterName(unsigned int index);

    /**
     * @brief ParameterIsOptional queries the state of the parameter for being optional, i.e. if
     *      the parameter can be left out in the bit vector
     * @param index     - index of the parameter of interest
     * @return          - true if the parameter is optional or non-existent, false otherwise
     */
    bool ParameterIsOptional(unsigned int index);
    /**
     * @brief ParameterIsOptional queries if the parameter can be left out in the bit vector
     * @param name      - reference name of the parameter of interest
     * @return          - true if the parameter is optional or non-existent, false otherwise
     */
    bool ParameterIsOptional(std::string name);
    /**
     * @brief SetParameterOptionality changes the parameter between being optional and required
     * @param index     - index of the parameter to change
     * @param optional  - true to make the parameter optional, false to make it required
     * @return          - true if the parameter was found, false otherwise
     */
    bool SetParameterOptionality(unsigned int index, bool optional);
    /**
     * @brief SetParameterOptionality changes the parameter between being optional and required
     * @param name      - reference name of the parameter to change
     * @param optional  - true to make the parameter optional, false to make it required
     * @return          - true if the parameter was found, false if not
     */
    bool SetParameterOptionality(std::string name, bool optional);

    /**
     * @brief changes the value of a parameter
     * @param index     - the index of the parameter to change
     * @param value     - the new value of the parameter
     * @return          - true if the index is valid and the new value is in the possible range, false otherwise
     */
    bool SetParameter(unsigned int index, unsigned int value);
    /**
     * @brief changes the value of a parameter
     * @param name      - the name of the parameter to change
     * @param value     - new value for the parameter
     * @return          - true on successful setting, false otherwise (wrong name, value out of bounds)
     */
    bool SetParameter(std::string name, unsigned int value);

    /**
     * @brief returns the number of entries in the object
     * @return
     */
    unsigned int GetEntries(){return parameters.size();}

    /**
     * @brief generates a bitvector to send to a chip for configuration
     * @param shiftdirection    - for `LSBFirst`, the bit order of every parameter is reversed
     * @param optout            - do not send optional parameters if this parameter is set to true
     * @return                  - the resulting bit vector
     */
    std::vector<bool> GenerateBitVector(int shiftdirection = MSBFirst, bool optout = false);

    /**
     * @brief LoadFromXMLElement decodes the shift register structure from an XMLElement
     *              loaded from an XML file
     * @param parentnode        - parent node of the shift register (value has to be "ShiftRegister")
     * @return                  - a tinyxml2 error code (or tinyxml2::XML_NO_ERROR on success)
     */
    tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* parentnode);
    /**
     * @brief loads the parameter configuration from an XML file
     * @param filename      - filename of the XML file to load
     * @param compressed    - for true, takes the name as an archive name with a contained file
     *                          with the same name apart from the file extension being ".xml"
     *                          This file is then used for loading.
     *                          Filename refers to a normal file for false
     * @return              - a tinyxml2 error code (or tinyxml2::XML_NO_ERROR on success)
     */
    tinyxml2::XMLError LoadFromXMLFile(std::string filename, bool compressed = false);

    /**
     * @brief SaveToXMLElement generates the XML tree structure for the content of the ASIC_Config2
     *          object
     * @param node    - base node to build on, it has to be created in a tinyxml2::XMLDocument
     * @return              - pointer to the filled XML tree representation of this object
     */
    tinyxml2::XMLError SaveToXMLElement(tinyxml2::XMLElement* node, tinyxml2::XMLDocument &doc);
    /**
     * @brief saves the current configuration in an XML file
     * @param filename      - filename to save to
     * @param devicename    - some additional information about what is saved, e.g. "Sample 2"
     * @param compressed    - for true the function takes the filename as name of an archive and
     *                          replaces the file ending by ".xml" for the filename inside the
     *                          archive and saves the data as zip archive.
     *                          Normal file output for false
     * @return              - a tinyxml2 error code (or tinyxml2::XML_NO_ERROR on success)
     */
    tinyxml2::XMLError SaveToXMLFile(std::string filename, std::string devicename = "", bool compressed = false);
private:
protected:
    std::string devicename;
    std::map<int, std::string> indextoname;
    std::map<std::string, int> nametoindex;

    std::vector<std::pair<std::string, int> > parameters;   //first index is the bit order and second is
                                                            // the parameter value itself
                                                            // in the order as it is shifted into the chip
                                                            // '!' indicates the inversion of the bit for sending
};

class ASIC_Config2_Manager
{
public:
    /**
     * @brief ASIC_Config2_Manager
     * @param filename      - filename to load a configuration from, no loading for the empty string
     */
    ASIC_Config2_Manager(std::string filename = "");

    /**
     * @brief ClearFile removes all content from the data structure
     */
    void               ClearFile();
    /**
     * @brief LoadFile fills the data structure with the content from the file specified
     * @param filename      - file name of the file to load the data from
     * @param compressed    - for true, takes the name as an archive name with a contained file
     *                          with the same name apart from the file extension being ".xml"
     *                          This file is then used for loading.
     *                          Filename refers to a normal file for false
     * @return              - an error code if the document contained errors, or XML_NO_ERROR on success
     */
    tinyxml2::XMLError LoadFile(std::string filename, bool compressed = false);
    /**
     * @brief SaveToFile writes the data container content to the file specified on HDD
     * @param filename      - file name to write to
     * @param compressed    - for true the function takes the filename as name of an archive and
     *                          replaces the file ending by ".xml" for the filename inside the
     *                          archive and saves the data as zip archive.
     *                          Normal file output for false
     * @return              - XML_NO_ERROR on success, or an error code otherwise
     */
    tinyxml2::XMLError SaveToFile(std::string filename, bool compressed = false);

    /**
     * @brief AddConfig creates a data tree for the ASIC_Config2 object and adds it to the structure
     * @param config        - configuration object to add to the data structure
     * @param devicename    - device name for identification of the data tree
     * @return              - XML_NO_ERROR on success, or an error code otherwise
     */
    tinyxml2::XMLError AddConfig(ASIC_Config2& config, std::string devicename);
    /**
     * @brief GetConfig fills an ASIC_Config2 object from a data tree with the supplied identifier
     * @param devicename    - identifier of the data tree to use for filling the configuration object
     * @return              - the configuration data container object with the content or an empty
     *                          configuration object on an error (e.g. device name not found)
     */
    ASIC_Config2      GetConfig(std::string devicename);
    tinyxml2::XMLElement* GetConfigNode(std::string devicename);

    /**
     * @brief GetTreeSourceNode provides an XMLElement as root node of the data structure to export
     * @param value         - name of the XMLElement
     * @param devicename    - name of the instance of the data structure for identification
     * @return              - XML_NO_ERROR on success, an error code otherwise
     */
    tinyxml2::XMLElement* GetTreeSourceNode(std::string value = "ShiftRegister", std::string devicename = "mydevice");
    /**
     * @brief GetParentDocument provides a reference to the document for generating new XMLElements
     * @return              - a reference to the XMLDocument
     */
    tinyxml2::XMLDocument* GetParentDocument();
    /**
     * @brief AddConfigTree adds a configuration tree to the data container structure for writing to HDD
     * @param tree          - root node of the tree structure
     * @return              - true on successful adding, false otherwise
     */
    bool               AddConfigTree(tinyxml2::XMLElement* tree);
    /**
     * @brief GetConfigTree provides a pointer to the root node of a data structure for the given
     *          name and identifier
     * @param value         - name of the XML node of interest
     * @param devicename    - identification string for the instance of interest at the parameter `name`
     * @return              - the root node of the tree or a null pointer if the tree was not found
     */
    tinyxml2::XMLElement* GetConfigTree(std::string value, std::string devicename);

private:
    std::string filename;
    tinyxml2::XMLDocument doc;
};

#endif // ASIC_CONFIG2_H
