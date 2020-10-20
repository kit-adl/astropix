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

/********************************************************
 * Implementation of the Oscilloscope readout commands 	*
 * for Keysight oscilloscopes over the VISA interface.	*
 *														*
 * Author: Rudolf Schimassek							*
 * Version 1.0 (18.12.18)								*
 ********************************************************/

#ifndef KEYSIGHTOSCI_H
#define KEYSIGHTOSCI_H

#include "math.h"
#include "visadevice.h"

class KeysightOsci : public VISADevice
{
public:
    KeysightOsci(int printlevel = ERRORS);

    int StrToInt(std::string number);
    double StrToDouble(std::string number);

    //commented out methods are implemented in the VISADevice class and listed for completeness

    //std::vector<std::string> ScanForDevices();

    //int GetNumDevicesFound();
    //bool open(unsigned int index);
    //bool open(std::string description);

    //void close();

    //std::string Read(int characters, bool print = false);
    //int Write(std::string data, bool print = false);
    //double QueryF(std::string query);
    //int QueryI(std::string query);

    //Oscilloscope Methods:
    bool SetAcquisitionState(bool run);
    bool SetChannel(int index);
    //bool GetWaveFormSetupStatus();
    int ReadWaveFormSetup(bool restart = true);
    std::string ReadWaveForm(bool setup = false, WaitFunction waitmethod = 0, int sleeptime = 5);
    double GetTimeScale(std::string data);
    double GetDispTimeScale();
    bool SetDispTimeScale(double value);
    double GetDispTimeOffset();
    /**
     * @brief SetDispTimeOffset
     * @param offset   in seconds from trigger to reference point (left, right or center of screen)
     * @return
     */
    bool SetDispTimeOffset(double offset);
    double GetVoltageScale(int channel);
    bool SetVoltageScale(int channel, double scale);
    double GetVoltageOffset(int channel);
    bool SetVoltageOffset(int channel, double offset);
    std::vector<double> DecodeWaveFormY(std::string data);
    std::vector<std::pair<double, double> > DecodeWaveFormXY(std::string data);

    bool SetupSingleTrigger();
    bool WaitForSingleTrigger(WaitFunction waitmethod, int sleeptime = 5, double timeout = 5);

    //bool SaveToFile(std::string filename, std::vector<double> data, double timescale, bool compact = true);
    //bool SaveToFile(std::string filename, std::vector<std::pair<double, double> > data, bool compact = false);
    bool SaveCompressed(std::string filename, std::string data, bool extendedparameters = true);

private:
    int pntfrmt       = 0;
    int pnttype       = 0;
    int numpnts       = 0;
    int avgbase       = 0;
    double xincr      = 0;
    double xorigin    = 0;
    double xreference = 0;
    double yincr      = 0;
    double yorigin    = 0;
    double yreference = 0;

};

#endif // KEYSIGHTOSCI_H
