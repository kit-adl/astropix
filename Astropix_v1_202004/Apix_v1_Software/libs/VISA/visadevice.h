/*
 * Osci_SMU Readout Project
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu) and
 *					   Felix Ehrler (felix.ehrler@kit.edu)
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


/************************************************************
 * Class (Structure) for communication with VISA devices    *
 *                                                          *
 * Author: Rudolf Schimassek                                *
 * Version: 1.0 (18.12.18)                                  *
 *                                                          *
 * This is a wrapper class for the NI-VISA driver to get    *
 * rid of the c-string/ViString interface and use std       *
 * strings. Also, the search and selection of devices is    *
 * simplified.                                              *
 * For use of oscilloscopes, predefined methods are         *
 * prepared and implemented in the TektronixOsci and        *
 * KeysightOsci classes. These can take a function pointer  *
 * to process events from the gui while waiting for device  *
 * responses. To use them, check the device identifier for  *
 * the respective names and replace the VISADevice object   *
 * with the opened device.                                  *
 *                                                          *
 *                                                          *
 * Opening procedure (for opening device at index `index`): *
 *                                                          *
 * VISADevice* osci = new VISADevice();                     *
 *                                                          *
 * osci->ScanForDevices();                                  *
 * if(osci->CheckResourceName(index, "TEKTRONIX"))          *
 * {                                                        *
 *     int printlevel = osci->GetPrintLevel();              *
 *     delete osci;                                         *
 *     osci = new TektronixOsci(printlevel);                *
 *     osci->ScanForDevices();                              *
 * }                                                        *
 * else if(osci->CheckResourceName(index, "KEYSIGHT"))      *
 *     int printlevel = osci->GetPrintLevel();              *
 *     delete osci;                                         *
 *     osci = new KeysightOsci(printlevel);                 *
 *     osci->ScanForDevices();                              *
 * }                                                        *
 * if(!osci->open(index))                                   *
 *     std::cerr << "Could not open Device " << index       *
 *               << std::endl;                              *
 ************************************************************/

#ifndef VISADEVICE_H
#define VISADEVICE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>

#include "visa.h"

//operating system "independent" sleep method:
void sleep(int ms);

/**
 * To use this class, it is advised to add two methods to the mainwindow class where this class is used:
 *    1. a public slot calling QApplication::processEvents()
 *              void MainWindow::ProcessEvents()
 *              {
 *                  QApplication::processEvents();
 *              }
 *    2. a method to set a function pointer:
 *              [in mainwindow.h add a private member:]
 *              class MainWindow ....
 *                  ...
 *              private:
 *                  WaitFunction waiter;
 *              };
 *              [another public slot to set this function pointer:]
 *              void MainWindow::setWaiter(WaitFunction newwaiter)
 *              {
 *                  this->waiter = newwaiter;
 *              }
 * Then, in main.cpp, a function to set MainWindow::waiter to MainWindow::ProcessEvents. Change main.cpp to this:
 *
 *      #include "mainwindow.h"
 *      #include <QApplication>
 *
 *      MainWindow* myw = nullptr;   //makes the mainwindow object available for myevents()
 *
 *      void myevents()
 *      {
 *          myw->ProcessEvents();
 *      }
 *
 *      int main(int argc, char *argv[])
 *      {
 *          QApplication a(argc, argv);
 *          MainWindow w;
 *          myw = &w;
 *
 *          w.setWaiter(myevents);   //sets myevents as waiting function
 *
 *          //from here, normal contents:
 *          w.show();
 *
 *          return a.exec();
 *      }
 *
 * The reason for this is to avoid circular dependencies between the VISADevice class and MainWindow.
 * Also, if no waiting inside class methods is required, the changes above are not necessary.
 */
typedef void (*WaitFunction)(void);

class VISADevice
{
public:
    enum levels {NONE      = 0,
                 ERRORS    = 1,
                 INFO      = 2,
                 READWRITE = 3,
                 DEBUG     = 4,
                 ALL       = 4};

    VISADevice(int printlevel = ERRORS);
    virtual ~VISADevice();

    /**
     * @brief ScanForDevices searches for USB connected VISA devices and returns a list of identifiers
     * @return               - vector of identifier strings for the devices found. The order is the same
     *                          as for the device index (see open(int index)).
     */
    std::vector<std::string> ScanForDevices();

    /**
     * @brief GetNumDevicesFound provides the number of devices found during last search
     * @return               - the number of USB devices found
     */
    int GetNumDevicesFound();
    bool is_open();
    bool open(unsigned int index);
    bool open(std::string description);
    /**
     * @brief CheckResourceName tests if the given string is contained in the resource identifier.
     *              this can be used to test for the producer of a device as "TEKTRONIX" or "KEYSIGHT"
     * @param index          - index of the device to test
     * @param findstring     - the string to search for
     * @return               - true if `findstring` was found in the resource identifier of the device
     *                          at `index`, false if not
     */
    bool CheckResourceName(int index, std::string findstring);

    void close();

    /**
     * @brief GetPrintLevel returns the setting for which command line output should be generated
     *          0 - no output at all
     *          1 - only error messages
     *          2 - also information messages
     *          3 - also reading and writing information
     *          4 - also debugging information
     *      (see enum levels)
     * @return          - the currently selected information level
     */
    int GetPrintLevel();
    bool SetPrintLevel(int level);

    /**
     * @brief Read reads up to `characters` characters from the opened device. If less characters than
     *          requested are read, only those are returned as a string.
     *          On an error on no opened connection, an empty string is returned.
     * @param characters     - the maximum number of characters to read
     * @return               - read data converted to an std::string with unchanged data
     */
    std::string Read(int characters);
    /**
     * @brief Write writes the passed string to the device. A newline character is added at the end
     *          if it is not present already.
     * @param data           - the string to write to the device
     * @return               - the number of characters written to the device or 0 on an error
     */
    int Write(std::string data);
    /**
     * @brief QueryF queries a floating point value from the device using the provided query string.
     *          a newline character is added at the end if it is not already present before sending.
     * @param query          - the query to receive the value including questionmark if necessary
     * @return               - the returned value assuming a text representation as answer. On an
     *                          error, -1e10 is returned
     */
    double QueryF(std::string query);
    /**
     * @brief QueryI behaves the same as QueryF but an integer value is parsed and returned. Querying
     *          a floating point value with this method can lead to wrong results.
     * @param query          - the query to receive the integer value
     * @return               - the returned value from the device converted to integer. On an error,
     *                          -1023*1024 = -1047552 is returned
     */
    int QueryI(std::string query);

    // === Oscilloscope Readout Methods ===

    // (not implemented in VISADevice, refer to TektronixOsci and KeysightOsci for implementations)
    /**
     * @brief SetAcquisitionState changes the acquisition state of the oscilloscope as pressing the
     *              Start/Stop button
     * @param run            - the new run state (true - running, false - stopped)
     * @return               - true if the command was sent successfully, false otherwise
     */
    virtual bool SetAcquisitionState(bool run);
    /**
     * @brief SetChannel sends a command to the oscilloscope to select a channel for the readout
     * @param index          - index of the channel to select
     * @return               - true on successful sending, false if index out of bounds or failed
     *                            sending
     */
    virtual bool SetChannel(int index);
    /**
     * @brief GetWaveFormSetupStatus checks whether the parameters for waveform decoding have been
     *          set
     * @return               - true if the parameters were set since the last device opening, false
     *                          otherwise
     */
    virtual bool GetWaveFormSetupStatus();
    /**
     * @brief ReadWaveFormSetup queries all required values to decode a waveform
     * @param restart        - in case of Keysight oscilloscope restarts the acquisition if true,
     *                            currently no effect for Tektronix devices
     * @return               - 0 on success, negative values indicate the cause of the failure
     */
    virtual int ReadWaveFormSetup(bool restart = true);
    /**
     * @brief ReadWaveForm reads a waveform from the device and returns the data as a string
     * @param setup          - call ReadWaveFormSetup() if true, if false the function is only called
     *                          if GetWaveFormSetupStatus() returns false
     * @param waitmethod     - optional function pointer (void (*waitmethod)(void)) to call while the
     *                          method is waiting for the response of the device, e.g. for calling
     *                          QApplication::processEvents() using the implementation described above
     *                          If no function is passed, nothing will be executed during waiting
     * @param sleeptime      - time in milliseconds to sleep between testing for the response of the
     *                          device and executing waitmethod
     * @return               - the time step of the data terminated by a newline and then the binary
     *                          data of the waveform or an empty string on an error
     */
    virtual std::string ReadWaveForm(bool setup = false, WaitFunction waitmethod = 0, int sleeptime = 5);
    /**
     * @brief GetTimeScale provides the time step between neighbouring points of the waveform
     * @param data           - if waveform data from ReadWaveForm() is provided, it is extracted from
     *                          this parameter. If it is left empty, the value from ReadWaveFormSetup()
     *                          is used
     * @return               - the time step between neighbouring points in seconds or -1 on an error
     */
    virtual double GetTimeScale(std::string data);
    virtual double GetDispTimeScale();
    /**
     * @brief SetDispTimeScale changes the time scale on the oscilloscope
     * @param value          - Time per division in seconds
     * @return               - true if the value was sent successfully, false otherwise
     */
    virtual bool SetDispTimeScale(double value);
    virtual double GetDispTimeOffset();
    /**
     * @brief SetDispTimeOffset changes the position of the trigger time in the displayed waveform
     * @param offset         - offset as time in seconds
     * @return               - true if the value was sent successfully, false otherwise
     */
    virtual bool SetDispTimeOffset(double offset);
    virtual double GetVoltageScale(int channel);
    /**
     * @brief SetVoltageScale changes the voltage scale for one channel
     * @param channel        - channel index running from 1 to 4
     * @param scale          - scale as voltage per division
     * @return               - true if the value was sent successfully, false otherwise
     */
    virtual bool SetVoltageScale(int channel, double scale);
    virtual double GetVoltageOffset(int channel);
    /**
     * @brief SetVoltageOffset changes the vertical offset of one channel
     * @param channel        - channel index running from 1 to 4
     * @param offset         - vertical offset in divisions
     * @return               - true if the value was sent successfully, false otherwise
     */
    virtual bool SetVoltageOffset(int channel, double offset);
    /**
     * @brief DecodeWaveFormY uses the parameters from ReadWaveFormSetup() to decode the binary data
     *          from ReadWaveForm()
     * @param data           - binary waveform data from ReadWaveFrom()
     * @return               - a vector with the y values from the waveform, retrieve the time scale
     *                          from GetTimeScale(). Or an empty vector on an error
     */
    virtual std::vector<double> DecodeWaveFormY(std::string data);
    /**
     * @brief DecodeWaveFormXY decodes the binary waveform data from ReadWaveForm() into X/Y pairs
     *          using the decoding parameters from the last ReadWaveFromSetup() call
     * @param data           - the binary waveform data from ReadWaveForm() to decode
     * @return               - a vector of x/y value pairs or an empty vector on an error
     */
    virtual std::vector<std::pair<double, double> > DecodeWaveFormXY(std::string data);

    /**
     * @brief SetupSingleTrigger puts the oscilloscope in single event mode to wait for a new
     *          waveform to be captured. Call this before calling WaitForSingleTrigger()
     * @return               - true if the setup was successful, false otherwise
     */
    virtual bool SetupSingleTrigger();
    /**
     * @brief WaitForSingleTrigger waits until a new trigger occured for the setup set by
     *          SetupSingleTrigger().
     * @param waitmethod     - function pointer to call during waiting for the next event
     *                          this may be e.g. the ProcessEvents() method as described
     *                          above. If no function is passed, nothing will be executed
     * @param sleeptime      - time in milliseconds between testing for a new signal and
     *                          executing waitmethod
     * @param timeout        - timeout in seconds after which the method returns
     * @return               - true if a new trigger was found, false if it was aborted due to
     *                          an error or timeout
     */
    virtual bool WaitForSingleTrigger(WaitFunction waitmethod, int sleeptime = 5, double timeout = 5);

    /**
     * @brief SaveToFile writes a decoded waveform to a file
     * @param filename       - filename to use
     * @param data           - decoded waveform data (y values) to write
     * @param timescale      - timescale for the data
     * @param compact        - if true, only the y values are written and the timescale is added at
     *                          the beginning of the file as a comment. If false, x and y values are
     *                          written out separated by a TAB character.
     * @return               - true on successful write, false otherwise
     */
    bool SaveToFile(std::string filename, std::vector<double> data, double timescale, bool compact = true);
    /**
     * @brief SaveToFile writes a decoded wavefrom to a file. The same as above, but with xy data
     * @param filename       - filename to use
     * @param data           - decoded waveform data (x and y values) to write
     * @param compact        - if true, only the y values are written and the timescale is added at
     *                          the beginning of the file as a comment. If false, x and y values are
     *                          written out separated by a TAB character.
     * @return               - true on successful write, false otherwise
     */
    bool SaveToFile(std::string filename, std::vector<std::pair<double, double> > data, bool compact = false);
    /**
     * @brief SaveCompressed writes the binary data including the necessary parameters and formula into a file
     * @param filename       - filename to use
     * @param data           - binary waveform data
     * @return               - true on successful write, false otherwise
     */
    virtual bool SaveCompressed(std::string filename, std::string data, bool extendedparameters = true);

protected:
    ViSession defaultRM;
    ViSession instr;
    ViStatus status;
    char instrResourceString[VI_FIND_BUFLEN];

    std::vector<std::string> resname;

    int printlevel;
    bool waveformsetupdone;
};

#endif // VISADEVICE_H
