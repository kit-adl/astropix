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
 * The SPINexys class is an interface class for sending data    *
 * for SPI configuration to the FPGA keeping track of the FIFO  *
 * sizes of the corresponding modules                           *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (13.09.19)                                      *
 ****************************************************************/

#ifndef SPINEXYS_H
#define SPINEXYS_H

// a class for SPI communication with ATLASPix3

#define _useQT_SPI_

#include <vector>
#include <string>

#ifdef _useQT_SPI_
#include <QProgressBar>
#endif

#include "libs/nexysio.h"
#include "libs/flagmanager.h"
#include "libs/zip_file.h"

typedef void (*WaitFunction)(void);

class SPINexys
{
public:

    ///Flag values for the data lines hidden by the SPI interface
    enum Signals {
        Sin        =      1,
        Ck1        =      2,
        Ck2        =      4,
        Rb         =      8,
        LdDAC      =     16,
        LdConfig   =     32,
        LdVDAC     =     64,
        LdTDAC     =    128,
        LdRow      =    256,
        LdColumn   =    512,
        WriteRAM0  =   1024,
        WriteRAM1  =   2048,
        WriteRAM2  =   4096,
        WriteRAM3  =   8192,
        WriteRAMEn =  16384,
        ResetTS    =  32768,
        Injection  =  65536,
        SPCH       = 131072
    };

    /**
     * @brief SPINexys constructor of the SPI interface class for ATLASPix3
     * @param buffersize    - size of the SPI write buffer on the FPGA for making sure
     *                          that no data is lost
     */
    SPINexys(unsigned int buffersize = 1023);

    /**
     * @brief GetIdentifier provides the instance identification string, also used in
     *      XML import and export
     * @return              - the instance identification string
     */
    std::string GetIdentifier() const;
    /**
     * @brief SetIdentifier changes the instance identification string
     * @param name          - new identification string
     */
    void        SetIdentifier(std::string name);

    /**
     * @brief GetBufferSize provides the buffer size on the FPGA for buffering write commands
     * @return              - the buffer size in bytes
     */
    unsigned int  GetBufferSize();
    /**
     * @brief SetBufferSize changes the buffer size in the software but NOT on the FPGA. Make sure
     *      this value is always smaller or equal to the size in the firmware
     * @param buffersize    - the new buffersize to assume on the FPGA
     */
    void          SetBufferSize(unsigned int buffersize);

    /**
     * @brief GetReadBackEnable provides the setting for data readback in the FPGA module, not the
     *      ASIC. For disabled readback, the SPI state machine skips steps to make the transmission
     *      faster
     * @return              - true for enabled readback
     */
    bool          GetReadBackEnable();
    /**
     * @brief SetReadBackEnable changes the FPGA module readback setting
     * @param nexys         - NexysIO instance with open connection for transmitting the
     *                          configuration
     * @param enable        - new readback enable setting
     * @return              - true on successful update, false otherwise
     */
    bool          SetReadBackEnable(NexysIO *nexys, bool enable);

    /**
     * @brief GetWriteAddress gets the address of the SPI write FIFO on the FPGA
     * @return              - stored address of the SPI write FIFO
     */
    unsigned char GetWriteAddress();
    void          SetWriteAddress(unsigned char address);

    /**
     * @brief GetReadAddress gets the address of the SPI read FIFO on the FPGA
     * @return              - stored address of the SPI read FIFO
     */
    unsigned char GetReadAddress();
    void          SetReadAddress(unsigned char address);

    /**
     * @brief GetClockDivAddress gets the address of the SPI clock divider register
     * @return              - stored address of the clock divider register for SPI
     */
    unsigned char GetClockDivAddress();
    void          SetClockDivAddress(unsigned char address);

    /**
     * @brief GetConfigAddress gets the address of the configuration register for the SPI module
     * @return              - stored address of the SPI configuration register
     */
    unsigned char GetConfigAddress();
    void          SetConfigAddress(unsigned char address);

    /**
     * @brief GetClockDivider retrieves the clock divider value for the SPI module from the 100 MHz
     *      base clock. The SPI module on the ATLASPix3 should not be driven faster
     *      than 10 or 20 MHz
     * @return              - the clock divider from a 100 MHz base clock
     */
    unsigned char GetClockDivider();
    /**
     * @brief SetClockDivider changes the clock divider for the SPI module on the FPGA.
     *      The module should not be driven faster than 10 or 20 MHz
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param clockdiv      - new clock divider from a 100 MHz base clock
     * @return              - true on successful update, false otherwise
     */
    bool          SetClockDivider(NexysIO* nexys, unsigned char clockdiv);

    /**
     * @brief GetSPIEnable gets the enable setting for the FPGA SPI module. The module on the
     *      ATLASPix3 has to be turned on, too.
     * @return              - the enable flag of the SPI module configuration
     */
    bool          GetSPIEnable();
    /**
     * @brief SetSPIEnable changes the enable flag of the SPI module configuration
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param enable        - new enable state for the FPGA module
     * @return              - true on successful update, false otherwise
     */
    bool          SetSPIEnable(NexysIO* nexys, bool enable);

    /**
     * @brief Update sends all configurations to the FPGA. Use in case of e.g. data loading
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @return              - true on successful update, false otherwise
     */
    bool          Update(NexysIO* nexys);

    /**
     * @brief ResetModule sends a reset pulse for the FPGA SPI module
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @return              - true on successful reset pulse sending
     */
    bool          ResetModule(NexysIO* nexys);
    /**
     * @brief ResetWriteFIFO sends a reset pulse for the SPI write FIFO
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @return              - true on successful reset pulse sending
     */
    bool          ResetWriteFIFO(NexysIO* nexys);
    /**
     * @brief ResetReadFIFO sends a reset pulse for the SPI read FIFO
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @return              - true on successful reset pulse sending
     */
    bool          ResetReadFIFO(NexysIO* nexys);

    /**
     * @brief Read reads the specified number of bytes from the SPI read FIFO
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param numchars      - number of characters to read
     * @return              - resulting data string
     */
    std::string   Read(NexysIO* nexys, unsigned int numchars);
    /**
     * @brief Write writes one configuration (24 bit) to the SPI write FIFO making sure the FIFO
     *      has space for the data
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param setting       - the flags as defined in `Signals`
     * @return              - true on successful sending
     */
    bool          Write(NexysIO* nexys, int setting);
    /**
     * @brief Write writes a number of data sets to the SPI write FIFO making sure the data will
     *      not be lost (here, the buffer size is used)
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param data          - the signal configurations to send with flags as in `Signals`
     * @param pb            - pointer to a QProgressBar for displaying progress
     * @return              - true on successful sending, false otherwise
     */
    bool          Write(NexysIO* nexys, std::vector<int> data, void* pb = nullptr, WaitFunction waiter = nullptr);

    /**
     * @brief WriteASIC writes a "bit stream" to the ASIC using the SPI interface and sends a
     *      load signal as specified at the end
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param data          - configuration bits for the chip without load bits
     * @param load          - load flag to use, see `Signals` for options
     * @param pb            - pointer to a QProgressBar for displaying progress
     * @return              - true on successful sending, false otherwise
     */
    bool          WriteASIC(NexysIO* nexys, std::vector<bool> data, int load, void *pb = nullptr, WaitFunction waiter = nullptr);

    //bool         WriteRAM(NexysIO* nexys, ...);

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
    tinyxml2::XMLError SaveToXMLFile(std::string filename, std::string devicename = "", bool compressed = false);
private:
    /**
     * @brief WriteNoNexysCheck writes a setting to the SPI write FIFO without checking the
     *      connection to nexys, the FIFO state is checked
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param setting       - the setting (24 bit) to send to SPI write FIFO
     * @return              - true on success, false otherwise
     */
    bool          WriteNoNexysCheck(NexysIO* nexys, int setting);
    /**
     * @brief WriteNoCheck the data is directly sent to the SPI write FIFO without and check.
     *      Make sure the FIFO has space before using this method
     * @param nexys         - NexysIO instance with open connection for data transmission
     * @param setting       - the setting (24 bit) to send to SPI write FIFO
     * @return              - true on success, false otherwise
     */
    bool          WriteNoCheck(NexysIO* nexys, int setting);

    std::string identifier;

    unsigned int buffersize;

    unsigned char writeaddress;
    unsigned char readaddress;
    unsigned char clockdivaddress;
    unsigned char clockdiv;
    unsigned char configurationaddress;

    FlagManager fm_config;

    FlagManager fm_lines;
};

#endif // SPINEXYS_H
