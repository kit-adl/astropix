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

#ifndef FASTREADOUT_H
#define FASTREADOUT_H

/************************************************************************
 * FastReadout classes for management of the fast readout and for       *
 * managing the output data from the chip.                              *
 * It consists of a data container (Dataset) and the FastReadout class  *
 * itself which uses FlagManager instances for managing the data and to *
 * provide access to XML loading/saving                                 *
 *                                                                      *
 * Author: Rudolf Schimassek                                            *
 *                                                                      *
 * Version  1.0 (23.08.19)                                              *
 *                                                                      *
 ************************************************************************/


#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <bitset>

#include "libs/flagmanager.h"

typedef unsigned int uint;

/**
 * @brief The Dataset class data container for decoded output data from the chip
 */
class Dataset
{
public:
    Dataset(int column = 0, int row = 0);

    /**
     * @brief operator == tests for equality of the addresses (column and row)
     *          of the two objects
     * @param rhs           - right hand side object
     * @return              - true for address equality, false otherwise
     */
    bool operator==(const Dataset& rhs) const;
    /**
     * @brief operator < compares the columns and then the rows for ordering the objects.
     *          row is only considered if the columns are the same
     * @param rhs           - right hand side object
     * @return              - true if address of right hand side object is larger
     */
    bool operator<(const Dataset& rhs) const;

    int       column;
    int       row;
    int       ts;
    long long timestamp;
    int       tot;
    long long timeoverthreshold;                //initialised to -1 (invalid)
    long long triggerts;
    int       triggerid;
    short     tdac;
    bool      bod_sent;
    short     complete;

    /**
     * @brief GetStringHeader generates a header string for the data contained in the output
     * @param extended      - include time stamp extensions from FPGA to output
     * @return              - a string containing the data headers
     */
    static std::string GetStringHeader(bool extended = false);
    /**
     * @brief ToString generates a string for the data content of the object matching the header
     *          from GetStringHeader()
     * @param extended      - include time stamp extensions from FPGA to output
     * @return              - a string containing the data of the object
     */
    std::string ToString(bool extended = false, int tsdiv = 0, int ts2div = 0) const;

    int  GetToTValue(int tsdiv, int ts2div) const;

    std::string ToAddressString() const;

    /**
     * @brief is_empty gives information about validity of the object
     * @return              - true if the object is invalid. This is derived from the tot member
     *                          being negative
     */
    bool is_empty();

private:
};

/**
 * @brief The FastReadout class class for managing the fast readout of the chip and for decoding
 *          the data from the chip
 */
class FastReadout
{
public:
    ///values for the flags managing the readout module
    enum FastReadoutFlags {enabled   =  1,
                           fifoclear =  2,
                           reset     =  4,
                           trigger   =  8,
                           debug     = 16,
                           datamux   = 32,
                           realign   = 128};

    ///addesses for the FPGA registers controlling parameters of the readout
    static const int FPGAAddress       =  9;
    static const int TrigDelayAddress  = 13;
    static const int TrigLengthAddress = 14;
    static const int TSDiv             = 25;
    static const int TS2Div            = 26;
    static const int TSPhase           = 27;

    /**
     * @brief FastReadout constructor
     * @param config        - starting values for the flags controlling the module
     */
    FastReadout(int config = 0);

    /**
     * @brief GetIdentifier provides the identification name used in XML import/export
     * @return              - identification string for this instance
     */
    std::string GetIdentifier();
    void        SetIdentifier(std::string name);

    /**
     * @brief GetEnabled gets the enable flag for the fast readout module on the FPGA
     * @return              - the current state of the flag
     */
    bool GetEnabled() const;
    /**
     * @brief SetEnabled changes the enable flag of the module
     * @param value         - new setting
     * @return              - complete configuration to send to FPGAAddress with masked reset flags
     */
    int  SetEnabled(bool value);

    /**
     * @brief GetReset provides the reset status for the state machine of the fast readout
     *          module on the FPGA
     * @return              - the reset flag status for the state machine
     */
    bool GetReset() const;
    /**
     * @brief SetReset changes the reset status for the state machine of the FPGA module
     * @param value         - new state for the reset flag
     * @return              - complete configuration to send to FPGAAddress with masked reset flags
     */
    int  SetReset(bool value);
    int  SetRealign(bool value);

    /**
     * @brief GetFifoclear gets the flag state for the FIFO reset for the fast readout module
     * @return              - the status of the FIFO reset flag
     */
    bool GetFifoclear() const;
    /**
     * @brief SetFifoclear changes the FIFO reset flag for the fast readout module
     * @param value         - new setting for the FIFO reset flag
     * @return              - complete configuration to send to FPGAAddress with masked reset flags
     */
    int  SetFifoclear(bool value);

    /**
     * @brief GetTrigger retrieves the state of the trigger flag for the FPGA fast readout module
     *              this is not the trigger for the ASIC, but for accepting the read data and
     *              writing it to the FIFO
     * @return              - state of the firmware trigger of the fast readout module
     */
    bool GetTrigger() const;
    /**
     * @brief SetTrigger changes the state of the fast reaodut firmware trigger for accepting the
     *          read data
     * @param value         - new setting for the firmware trigger flag
     * @return              - complete configuration to send to FPGAAddress with masked reset flags
     */
    int  SetTrigger(bool value);

    /**
     * @brief GetDebugMode provides the setting for the debug mode of the readout
     * @return              - state of the debug mode (true - activated)
     */
    bool GetDebugMode() const;
    /**
     * @brief SetDebugMode changes the debug mode flag for the FPGA module
     * @param value         - new setting for debug mode (true - activated)
     * @return              - complete configuration to send to FPGAAddress with masked reset flags
     */
    int  SetDebugMode(bool value);

    /**
     * @brief GetConfiguration provides the configuration of all flags as present on the register
     *      at FPGAAddress on the FPGA
     * @param withreset     - for true sends the reset signals as set, for false the reset pins
     *                          (fifoclear and reset) are masked
     * @return              - configuration byte for sending to FPGAAddress
     */
    int GetConfiguration(bool withreset = false) const;
    /**
     * @brief SetConfiguration changes the configuration for FPGAAddress as a whole
     * @param config        - the new setting for all flags at FPGAAddress
     * @return              - the resulting configuration with masked reset flags
     */
    int SetConfiguration(int config);

    /**
     * @brief GetTriggerDelay provides the setting for the trigger delay
     * @return              - delay as number of time stamps
     */
    int  GetTriggerDelay() const;
    /**
     * @brief SetTriggerDelay changes the trigger delay
     * @param value         - trigger delay setting in number of time stamps (maximum value 255)
     * @return              - true if the value was accepted, false if not
     */
    bool SetTriggerDelay(int value);

    /**
     * @brief GetTriggerLength gets the length of a trigger signal as to be generated by the fast
     *          readout module
     * @return              - trigger signal length in number of time stamps
     */
    int  GetTriggerLength() const;
    /**
     * @brief SetTriggerLength changes the length of the trigger signal to be generated
     * @param value         - new length of the trigger signal as number of time stamps
     *                          (maximum value 255)
     * @return              - true if the value was accepted, false if not
     */
    bool SetTriggerLength(int value);

    int  GetTSPhase() const;
    bool SetTSPhase(int value);

    /**
     * @brief GetNumDataSets provides the number of datasets to read into the file. For '0'
     *          no limit is applied
     * @return              - the number of data sets to read before ending or 0 for no automated
     *                          abortion
     */
    int  GetNumDataSets();
    /**
     * @brief SetNumDataSets changes the number of datasets to read
     * @param value         - new maximum number (must be smaller than 16,777,216)
     * @return              - true if the value was accepted, false if not
     */
    bool SetNumDataSets(int value);

    /**
     * @brief GetFileReadPrint provides the setting for output during reading data and directly
     *          writing it to a file
     * @return              - true for additional output, false for no output
     */
    bool GetFileReadPrint() const;
    /**
     * @brief SetFileReadPrint changes the setting for output during readout to a file
     * @param value         - new setting for the parameter (true - activated)
     */
    void SetFileReadPrint(bool value);

    /**
     * @brief GetFileReadDecode provides the setting for data decoding during readout to a file
     * @return              - true for decoding, false for no decoding
     */
    bool GetFileReadDecode() const;
    /**
     * @brief SetFileReadDecode changes the setting for data decoding during readout to a file
     * @param value         - new setting for the parameter (true - activated)
     */
    void SetFileReadDecode(bool value);

    /**
     * @brief GetBinaryOutput provides the setting for the output mode of read data
     * @return              - true for bitsets, false for decimal output per byte
     */
    bool GetBinaryOutput() const;
    /**
     * @brief SetBinaryOutput changes the output mode setting for simple read output
     * @param value         - new setting for output mode (true - binary, false - decimal)
     */
    void SetBinaryOutput(bool value);

    /**
     * @brief GetDataMuxEnable provides the data format setting to use on read data
     * @return              - true for output of DataMux, false for old format
     */
    bool GetDataMuxEnable() const;
    /**
     * @brief SetDataMuxEnable changes the interpretation mode for data from fast readout
     * @param value         - new setting for data format (true - DataMux, false - old style)
     */
    void SetDataMuxEnable(bool value);

    /**
     * @brief GetReadoutMode returns the setting of the readout mode (untriggered/triggered)
     * @return              - false for untriggered readout, true for triggered readout
     */
    bool GetReadoutMode();
    /**
     * @brief SetReadoutMode changes the setting for the readout mode in software
     * @param triggered     - true for triggered readout, false for untriggered readout
     */
    void SetReadoutMode(bool triggered);

    bool GetDisableHitWord1() const;
    void SetDisableHitWord1(bool value);
    bool GetDisableHitWord2() const;
    void SetDisableHitWord2(bool value);


    static int DecodeGray(int numbits, int value);

    /**
     * @brief Decode decodes the dataset passed as string containing binary data as read from
     *              the FPGA
     * @param viaDataMux    - if true, data is assumed to be compressed into 64bit.
     *                          for false, the data is assumed to contain markers in first byte,
     *                          data in second byte and additional data in the remaining 6 bytes
     * @param data          - binary data of one dataset as read from the FPGA,
     *                          make sure the data is aligned correctly
     * @return              - a dataset container with the decoded data or an empty container
     *                          on an error
     */
    static Dataset Decode(bool viaDataMux, std::string data);
    /**
     * @brief DecodeMany decodes several datasets at once from a single data string
     * @param viaDataMux    - for true assume the usage of DataMux: the dataset consists of 8 bytes
     *                          for false, the data is spread over several 8 byte entries with the
     *                          identifier in first byte, raw data in second byte and other data in
     *                          the followind 6 bytes
     * @param data          - aligned data to be decoded
     * @return              - a vector of all valid datasets contained in the passed data
     */
    static std::vector<Dataset> DecodeMany(bool viaDataMux, std::string data);

    std::vector<Dataset> DecodeTrigger(std::string data);
    std::vector<Dataset> DecodeManyTrigger(std::string data);

    /**
     * @brief RemoveEmptyData removes all parts containing no valid data from the string returning
     *          only the valid data, but still in binary form
     * @param data          - binary data containing empty data (i.e. 8x '0xff')
     * @return              - the parts of `data` that are not empty datasets
     */
    static std::string RemoveEmptyData(std::string data);

    /**
     * @brief LoadFromXMLElement extracts the data for this object from an XML subtree
     * @param root          - root node of the subtree to load the data from
     * @return              - XML_NO_ERROR on success or an error code
     */
    tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* root);
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
    tinyxml2::XMLElement* SaveToXMLElement(tinyxml2::XMLDocument& doc, std::string devicename = "");
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
    tinyxml2::XMLError SaveToXMLFile(std::string filename, std::string identifier = "", bool compressed = false);

private:
    std::string identifier;
    FlagManager configuration;
    FlagManager nonfpgaconfig;

    bool disablehitword1;
    bool disablehitword2;

    std::vector<Dataset> unfinished_data;
    Dataset trig_datastub;
};

#endif // FASTREADOUT_H
