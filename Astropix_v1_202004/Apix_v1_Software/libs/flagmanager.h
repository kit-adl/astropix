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
 * This class is based on the implementation of Felix Ehrler.
 */
#ifndef FLAGMANAGER_H
#define FLAGMANAGER_H


/****************************************************************************
 *                                                                          *
 * Flagmanager manages flags for use in FPGA                                *
 *                                                                          *
 * - Names and Values are stored and can be manipulated                     *
 *                                                                          *
 * Author: Felix Ehrler                                                     *
 *                                                                          *
 * Version  1.0 (18.05.2017)                                                *
 *          1.1 (19.08.2019) (Author: Rudolf Schimassek)                    *
 *              added support for flag groups                               *
 *              added XML import/export                                     *
 ****************************************************************************/


#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "libs/tinyxml2_addon.h"
#include "libs/zip_file.h"

class FlagManager
{
public:
    /**
     * @brief FlagManager creates a new flag manager object
     * @param address       - address on the FPGA to write to
     */
    FlagManager(int address = -1);

    /**
     * @brief The entry struct data container for a single "flag" or "flag group"
     */
    struct entry{
        std::string name;
        std::string bitvalues;
        int value;
    };

    /**
     * @brief GetIdentifier provides the identification string for the instance. It is also used
     *          for XML import/export
     * @return              - identification string
     */
    std::string GetIdentifier() const;
    /**
     * @brief SetIdentifier changes the identification string which is also used for XML import
     *          and export
     * @param name          - new identification string for this instance
     */
    void        SetIdentifier(std::string name);

    /**
     * @brief GetFPGAAddress provides the FPGA address for which the configuration is intended to
     *          be sent to
     * @return              - the FPGA address in the firmware
     */
    int  GetFPGAAddress() const;
    /**
     * @brief SetFPGAAddress changes the address to be associated with this instance
     * @param address       - the new address (limited on FPGA to 8 bit)
     */
    void SetFPGAAddress(int address);

    /**
     * @brief GetNumFlags returns the number of flag groups in the object
     * @return              - number of flag groups
     */
    unsigned int GetNumFlags() const;
    /**
     * @brief Clear removes all flag groups from the object
     */
    void Clear();

    /**
     * @brief AddFlag adds a single bit flag at the bit after the largest bit already in use
     * @param name          - identification name for the flag to add
     * @param state         - initial value for the flag
     * @return              - true on successful adding, false otherwise
     */
    bool AddFlag(std::string name, bool state);
    /**
     * @brief AddFlag adds a single bit flag at the desired position
     * @param name          - identification name for the flag to add
     * @param value         - value at which the flag is to be added (1,2,4,8,...)
     *                          other values are also possible, but not recommended
     * @param state         - initial value for the flag
     * @return              - true on successful adding, false otherwise
     */
    bool AddFlag(std::string name, int value, bool state);
    /**
     * @brief AddFlag adds a flag group with a number of bits
     * @param name          - identification name for the flag group to add
     * @param smallestvalue - value of the smallest bit of the group to add
     * @param numbits       - number of bit for this flag group
     * @param value         - initial value for the flag group
     * @param checkposition - validates that the desired bits are not already in use if true,
     *                          skips the check on false (be careful using this!)
     * @return              - true on successful adding, false otherwise
     */
    bool AddFlag(std::string name, int smallestvalue, unsigned int numbits, int value, bool checkposition = true);
    /**
     * @brief AddFlag adds a flag group with specified bit values
     * @param name          - identification name for the flag group to add
     * @param values        - comma separated string containing the values of the bits of the flag
     *                          group, LSB first
     * @param value         - initial value for the flag group
     * @return              - true on successful adding, false otherwise
     */
    bool AddFlag(std::string name, std::string values, int value);

    /**
     * @brief GetFlag provides the value of the flag group
     * @param index         - index of the flag group in the object
     * @return              - value of the flag group as bit value or -1 if the group does not
     *                          exist
     */
    int  GetFlag(unsigned int index) const;
    /**
     * @brief GetFlag provides the value of the flag group from its identification string
     * @param name          - identification string of the flag group of interest
     * @return              - value of the flag group as bit value, or -1 if the group does not
     *                          exist
     */
    int  GetFlag(std::string name) const;

    /**
     * @brief GetFlagWidth provides the number of bits in the flag group
     * @param index         - index of the flag group of interest
     * @return              - the number of bits in the flag group or 0 in case the group does not
     *                          exist
     */
    unsigned int GetFlagWidth(unsigned int index) const;
    /**
     * @brief GetFlagWidth provides the number of bits in the flag group using the identification
     *          string
     * @param name          - identification string of the group of interest
     * @return              - the number of bits in the flag group of interest
     *                          or -1 if it does not exist
     */
    unsigned int GetFlagWidth(std::string name) const;

    /**
     * @brief GetFlagName gets the name for the flag group at the given index
     * @param index         - index of the group of interest
     * @return              - the identification string of the group or an empty string if the
     *                          group does not exist
     */
    std::string GetFlagName(unsigned int index) const;
    /**
     * @brief GetFirstBitPosition provides the output value of the first bit of the flag group
     * @param index         - index of the group of interest
     * @return              - output value of the first bit (LSB) of the group of interest,
     *                          or -1 if the group does not exist
     */
    int         GetFirstBitPosition(unsigned int index) const;
    /**
     * @brief GetFirstBitPosition provides the output value of the first bit of the flag group at
     *          the given identification string
     * @param name          - identification string of the group of interest
     * @return              - output value of the first bit (LSB) of the group of interest,
     *                          or -1 if the group does not exist
     */
    int         GetFirstBitPosition(std::string name) const;
    /**
     * @brief GetBitPositions returns a comma separated list of the bit output values of the
     *          flag group (LSB first)
     * @param index         - index of the flag group of interest
     * @return              - comma separated string with the bit output values or an empty
     *                          string if the group does not exist
     */
    std::string GetBitPositions(unsigned int index) const;
    /**
     * @brief GetBitPositions returns a comma separated list of bit output values for the
     *          flag group of interest (LSB first)
     * @param name          - identification string for the flag group of interest
     * @return              - comma separated string with the bit output values or an empty string
     *                          if the flag group does not exist
     */
    std::string GetBitPositions(std::string name) const;

    /**
     * @brief SetFlag changes the value of the flag group
     * @param index         - index of the flag group to change the value
     * @param value         - new value to use
     * @return              - true on successful change, or false on non-existing groups or
     *                          value out of range
     */
    bool SetFlag(unsigned int index, int value);
    /**
     * @brief SetFlag changes the value of the flag group
     * @param name          - identification string of the flag group of interest
     * @param value         - new value to use
     * @return              - true on successful change, or false on non-existing groups or
     *                          value out of range
     */
    bool SetFlag(std::string name, int value);

    /**
     * @brief GetConfiguration retrieves the resulting value from the settings of all flag groups
     * @return              - the or function of all set flag group bits
     */
    int  GetConfiguration() const;
    /**
     * @brief SetConfiguration directly changes the setting of the flag groups from their output
     *      values
     * @param configuration - the new confiuration to apply
     * @return              - true if all set bits were found in a flag group, false otherwise
     */
    bool SetConfiguration(int configuration);

    /**
     * @brief LoadFromXMLElement extracts the data for this object from an XML subtree
     * @param root          - root node of the subtree to load the data from
     * @return              - XML_NO_ERROR on success or an error code
     */
    tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* rootnode);
    /**
     * @brief LoadFromXMLFile loads the contents of the instance from an XML file
     * @param filename      - file name of the file to load from
     * @param compressed    - if true, interpert the file at `filename` as zip archive and replace
     *                          the extension by "xml" to select the file inside the archive.
     *                          For false, use the passed file as the XML file directly
     * @return              - XML_NO_ERROR on success or an error code otherwise
     */
    tinyxml2::XMLError LoadFromXMLFile(std::string filename, bool compressed = false);

    /**
     * @brief SaveToXMLElement generates an XML tree structure from the contained data for the
     *          passed XML document
     * @param doc           - XML document to host the tree to build
     * @param devicename    - string to override the stored device name / identification string of
     *                          the instance, keep empty to use the internal one
     * @return              - the root node of the XML tree structure or a null pointer on an error
     */
    tinyxml2::XMLElement* SaveToXMLElement(tinyxml2::XMLDocument& doc, std::string identifier = "") const;
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
    tinyxml2::XMLError SavetoXMLFile(std::string filename, std::string identifier = "", bool compressed = false) const;

private:
    std::string identifier;
    int FPGAaddress;
    std::vector<entry> entries;
};

#endif // FLAGMANAGER_H
