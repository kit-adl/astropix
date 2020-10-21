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

#ifndef TDAC_CONFIG_H
#define TDAC_CONFIG_H
/****************************************************************************
 *                                                                          *
 * class for storing TDAC configuration with the option to save and load it *
 * from (un)compressed files.                                               *
 * The pixel matrix has two access options: the geometric position of the   *
 * pixel or a used defined custom address where all pixels in a row share   *
 * the same value.                                                          *
 *                                                                          *
 * Author: Rudolf Schimassek                                                *
 *                                                                          *
 * Version 1.0 (15.08.2019)                                                 *
 *                                                                          *
 ****************************************************************************/


#include <vector>
#include <utility>
#include <string>
#include <stdio.h>
#include <fstream>

#include <chrono>
#include <ctime>

#include "libs/tinyxml2_addon.h"
#include "libs/zip_file.h"

typedef unsigned int uint;

class TDAC_Config
{
public:
    TDAC_Config(int columns = -1, int rows = -1, unsigned int maxvalue = 15);

    /**
     * @brief GetDeviceName provides the identifying string for this instance
     * @return               - string descriptor / identifier for the object instance
     */
    std::string GetDeviceName() const;
    /**
     * @brief SetDeviceName changes the name of the instance
     * @param name           - string to use as new descriptor
     */
    void SetDeviceName(std::string name);

    /**
     * @brief GetNumRows queries the number of rows of the matrix contained
     * @return               - number of rows in the matrix
     */
    unsigned int GetNumRows() const;
    /**
     * @brief SetNumRows changes the number of rows in the matrix
     * @param rows           - new number of rows for the matrix
     * @param updatematrix   - directly rebuild the matrix on true, all contained data is lost
     */
    void SetNumRows(unsigned int rows, bool updatematrix = false);

    /**
     * @brief GetNumColumns queries the number of columns in the matrix image
     * @return               - the number of columns
     */
    unsigned int GetNumColumns() const;
    /**
     * @brief SetNumColumns changes the number of columns in the matrix image
     * @param columns        - new number of columns for the matrix
     * @param updatematrix   - directly rebuild the matrix on true, all contained data is lost
     */
    void SetNumColumns(unsigned int columns, bool updatematrix = false);

    /**
     * @brief GetMaxValue queries the maximum setting of the TDACs
     * @return               - the maximum value as number (inclusive)
     */
    unsigned int GetMaxValue() const;
    /**
     * @brief SetMaxValue changes the maximum possible value for the TDACs. Settings larger
     *          than this value are changed to this number
     * @param maxvalue      - new maximum setting for the TDACs (inclusive)
     */
    void SetMaxValue(unsigned int maxvalue);

    /**
     * @brief UpdateMatrix changes the data container to match the new matrix size
     * @param columns        - new number of columns, if set to a negative number, the internal
     *                          setting is used
     * @param rows           - new number of rows, if set to a negative number, the internal
     *                          setting is used
     */
    void UpdateMatrix(int columns = -1, int rows = -1); //for values <0 use the member variables

    /**
     * @brief GetCustomAddress queries the custom address of the pixel at position (`column`|`row`)
     *          The default value is the same as the geometric address.
     * @param column         - geometric column of the pixel of interest
     * @param row            - geometric row of the pixel of interest
     * @return               - the stored value for the custom address as (`column`|`row`)
     */
    std::pair<int, int> GetCustomAddress(unsigned int column, unsigned int row) const;
    /**
     * @brief SetCustomAddress changes the custom address of the pixel at geometric position
     *          (`column`|`row`). Be aware that the pixels in a row share the storage for
     *          the custom row address so that the last pixel written defines the row index
     *          for all pixels in the row.
     *          The default value of the custom address is the geometric address.
     * @param column         - geometric column of the pixel of interest
     * @param row            - geometric row of the pixel of interest
     * @param coladdress     - custom column index for the pixel (`column`|`row`)
     * @param rowaddress     - custom row index for the pixel (` column`|`row`)
     * @return               - true on successful changing, false on an error (i.e. out of range)
     */
    bool SetCustomAddress(unsigned int column, unsigned int row, int coladdress, int rowaddress);

    /**
     * @brief GetTDACValue queries the current setting for the pixel at geometric position
     *              (`column`|`row`)
     * @param column         - column of the pixel of interest
     * @param row            - row of the pixel of interest
     * @return               - the stored value (>= 0) for the pixel, a negative value on an error
     */
    int GetTDACValue(unsigned int column, unsigned int row) const;
    /**
     * @brief SetTDACValue changes the setting for geometric pixel (`column`|`row`)
     * @param column         - column of the pixel of interest
     * @param row            - row of the pixel of interest
     * @param value          - new setting for this pixel
     * @return               - true on successful change, false on an error
     */
    bool SetTDACValue(unsigned int column, unsigned int row, unsigned int value);

    /**
     * @brief GetTDACAddressValue queries the setting for a pixel using the custom addressing
     * @param coladdress     - column of the custom address
     * @param rowaddress     - row of the custom address
     * @return               - the stored value (>= 0) or a negative value on an error
     */
    int GetTDACAddressValue(int coladdress, int rowaddress);
    /**
     * @brief SetTDACAddressValue changes the setting for one pixel using the custom addressing
     * @param coladdress     - column of the custom address
     * @param rowaddress     - row of the custom address
     * @param value          - new setting for the pixel
     * @return               - true on successful update, false on an error
     */
    bool SetTDACAddressValue(int coladdress, int rowaddress, unsigned int value);

    /**
     * @brief SetAllToValue changes the setting for all pixels to the passed value
     * @param value          - new value for ALL pixels
     * @param ignorelargerthan - do not change values larger than this value (e.g. for disable bit)
     *                              set to negative value to change all values
     * @return               - true on success, false otherwise
     */
    bool SetAllToValue(unsigned int value, int ignorelargerthan = -1);

    /**
     * @brief LoadFromXMLElement extracts the contents for the matrix from an XMLElement
     * @param rootnode       - root node of the tree structure describing the contents
     * @return               - XML_NO_ERROR on success, an error code otherwise
     */
    tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* rootnode);
    /**
     * @brief LoadFromXMLFile loads the matrix from an XML file
     * @param filename       - filename for the file containing the XML representation of the data
     * @return               - XML_NO_ERROR on success, an error code otherwise
     */
    tinyxml2::XMLError LoadFromXMLFile(std::string filename, bool compressed = false);

    /**
     * @brief SaveToXMLElement generates a tree structure for the current configuration on the
     *          passed rootnode
     * @param rootnode       - the element on which the structure is built
     * @param doc            - file reference for creating new XML elements
     * @param devicename     - overwrites the internal device name, keep it empty to use the
     *                          internal value
     * @return               - XML_NO_ERROR on success oe an error code
     */
    tinyxml2::XMLError SaveToXMLElement(tinyxml2::XMLElement* rootnode, tinyxml2::XMLDocument& doc, std::string devicename = "");
    /**
     * @brief SaveToXMLFile saves an XML representation of the data content to a file
     * @param filename       - filename to use for saving
     * @param devicename     - overwrites the internal device name, keep it empty to use the
     *                          internal value
     * @return               - XML_NO_ERROR on success or an error code
     */
    tinyxml2::XMLError SaveToXMLFile(std::string filename, std::string devicename = "", bool compressed = false);

private:
    std::string devicename;

    //outer index is row, inner index is column
    //the first int of the pairs is a field for a custom addressing
    std::vector<std::pair<int, std::vector<std::pair<int, unsigned int> > > > values;
    bool uptodate;

    unsigned int columns;
    unsigned int rows;

    unsigned int maxvalue;
};

#endif // TDAC_CONFIG_H
