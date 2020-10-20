/********************************************************************************
 * C++ Class for usage of the VISA driver for communication with devices as     *
 * oscilloscopes, SMU, function generator,...                                   *
 *                                                                              *
 * Author: Rudolf Schimassek                                                    *
 * Version 1.1 (May 2018)                                                       *
 *                                                                              *
 ********************************************************************************/

#ifndef TEKTRONIXOSCI_H
#define TEKTRONIXOSCI_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>

#include "libs/Measurement.h"

#include "visa.h"

class TektronixOsci
{
public:
    TektronixOsci();

    /**
     * @brief ScanForDevices lists all devices connected to the computer
     * @return               - a vector of strings containing the device responses to "*IDN?"
     */
    std::vector<std::string> ScanForDevices();

    /**
     * @brief GetNumDevicesFound provides the number of connected devices
     * @return               - the number of found devices
     */
    int GetNumDevicesFound();
    bool open(unsigned int index);
    //bool open(char* resourceString);
    bool open(std::string description);
    bool is_open();

    void close();

    /**
     * @brief Read reads the requested number of bytes from the opened device
     * @param characters     - the number of characters to read
     * @return               - the read data or an empty string if nothing to read was available or
     *                              or no device was opened
     */
    std::string Read(int characters);
    /**
     * @brief Write writes the passed string to the opened device
     * @param data           - the command to send to the device. Commands should be terminated
     *                              with a newline character which is added if missing
     * @return               - the number of sent bytes which is 0 if no device is open
     */
    int Write(std::string data);
    /**
     * @brief QueryF queries a floating point value from the opened device
     * @param query          - the command for the query
     * @return               - the resulting value of the query or -1e10 on an error
     */
    double QueryF(std::string query);
    /**
     * @brief QueryI queries an integer value from the opened device
     * @param query          - command for the query
     * @return               - the result of the query or -1047552 (-1023*1024) on an error
     */
    int QueryI(std::string query);

    /**
     * @brief SetUpWaveFormRead sends the commands to configure the readout of waveforms on an oscilloscope
     * @param channel        - the channel name of the desired channel (e.g. "CH1" or "Math1")
     * @param start          - index of the first data point to transmit (starting at 1)
     * @param stop           - index of the last data point to transmit
     * @return               - true on success, false otherwise
     */
    bool SetUpWaveFormRead(std::string channel, int start, int stop);

    /**
     * @brief SetupDataEncoding sets up the data transmission format for waveform transmission on
     *          oscilloscopes
     * @return               - true on success, false otherwise
     */
    bool SetupDataEncoding();
    /**
     * @brief GetTimeStep provides the time difference between two sample points in the
     *              transmitted waveform data
     * @return               - the time in seconds between neighbouring sample points
     *                              or -1 on an error
     */
    double GetTimeStep();
    /**
     * @brief GetYOffset provides the data offset of the encoded data sent by the oscilloscope
     * @return               - the offset of the data or -1e10 on an error
     */
    double GetYOffset();
    /**
     * @brief GetYMultiplier provides the voltage step size of the encoded data sent by the
     *              oscilloscope
     * @return               - the step size in volt or -1e10 on an error
     */
    double GetYMultiplier();
    /**
     * @brief GetChannelOffset provides the channel offset in addition to the display offset
     *             acquired by GetYOffset()
     * @return               - the channel offset in volt
     */
    double GetChannelOffset(std::string channel);

    /**
     * @brief GetWaveForm queries a waveform without configuring the device beforehand. This is
     *              done by the functions SetUpWaveFormRead() and SetupDataEncoding()
     * @return               - the encoded data as received from the oscilloscope
     */
    std::string GetWaveForm();

    /**
     * @brief DecodeAndSaveWaveForm converts the binary data from the oscilloscope to human-
     *              readable format and stores it into a text file
     * @param timestep       - the time step to use for the decoding (GetTimeStep())
     * @param yoffset        - the data offset in y direction (GetYOffset())
     * @param ymultiplier    - the data multiplier (GetYMultiplier())
     * @param data           - the data to decode (GetWaveForm())
     * @param dataoffset     - offset of the decoded data in volt
     * @param filename       - the file name to use for storage on hard disk
     * @return               - true on success, false otherwise
     */
    bool DecodeAndSaveWaveForm(double timestep, double yoffset, double ymultiplier,
                               std::string data, double dataoffset, std::string filename);

    //TODO: add saving with data structure

private:
    ViSession defaultRM;
    ViSession instr;
    ViStatus status;
    char instrResourceString[VI_FIND_BUFLEN];


    //std::vector<std::string> resource;
    std::vector<std::string> resname;
};

#endif // TEKTRONIXOSCI_H
