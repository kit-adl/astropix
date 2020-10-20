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

/********************************************************************
 * Class for managing chip/setup configuration and make the methods *
 * accessible for other parts as for trimming.                      *
 *                                                                  *
 * The class is not suppposed to own any of the pointers stored in  *
 * it. It is an extension reducing the amount of code by improving  *
 * reusability.                                                     *
 *                                                                  *
 * Author: Rudolf Schimassek                                        *
 *                                                                  *
 * Version: 1.0 (13.09.19)                                          *
 ********************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#define _useQT_

#define AP3rows    372
#define AP3columns 132

#include <vector>

#include "libs/nexysio.h"

#include "libs/asic_config_2.h"
#include "libs/vb_config.h"
#include "libs/injection_config.h"
#include "libs/flagmanager.h"
#include "libs/tdac_config.h"
#include "libs/spinexys.h"
#include "libs/cmdconfig.h"

#include "fastreadout.h"

#ifdef _useQT_
#include <QProgressBar>
#include <QTextCursor>
#include <QTextEdit>
#include <QString>
#endif

//#if !defined(VISADEVICE_H) && !defined(CONFIGWAITER)
//#define CONFIGWAITER
typedef void (*WaitFunction)(void);
//#endif

class Configuration
{
public:
    enum configs {dac    =   1,
                  config =   2,
                  vdac   =   4,
                  column =   8,
                  row    =  16,
                  colrow =  24,
                  tdac   =  32,
                  srload =  64};

    enum pinconfig {resets     = 1,
                    modeconfig = 2,
                    modework   = 4};

    enum configways {shiftreg   = 0,
                     noshiftreg = 1,
                     spi        = 2,
                     cmd        = 3};


    Configuration();
    Configuration(const Configuration& ref);
    ~Configuration();
    Configuration operator=(const Configuration& ref);

    /**
     * @brief setWaiter adds a function pointer to a function to call QApplication::processEvents()
     *      of the GUI the configuration object belongs to. It is used to prevent GUI freezing
     *      during execution of long routines
     * @param newwaiter      - function pointer to a function without argument and return value
     */
    void                setWaiter(WaitFunction newwaiter);
    /**
     * @brief ProcessEvents calls the function set using setWaiter() to update the GUI the object
     *      is assiciated with
     */
    void                ProcessEvents();

    /**
     * @brief FindFileName searches an unused filename with increasing index as
     *      `filenameprefix``index``filenamesuffix`
     * @param filenameprefix - prefix of the filename
     * @param filenamesuffix - suffix of the filename
     * @return               - the resulting filename
     */
    static std::string FindFileName(std::string filenameprefix, std::string filenamesuffix);
    static std::string WriteToFile(std::string filename, std::string data);
    /**
     * @brief WriteToFile calls FindFileName() to find a file name and writes the passed data to
     *      this file
     * @param filenameprefix - prefix of the filename
     * @param filenamesuffix - suffix of the filename
     * @param data           - data to write to the file
     * @return               - the name of the file the data was written to
     */
    static std::string WriteToFile(std::string filenameprefix, std::string filenamesuffix, std::string data);

    ///setter and getter for all contained configuration and control objects:

    NexysIO*            GetNexysIO() const;
    void                SetNexysIO(NexysIO* value);

    ASIC_Config2*       GetATLASPixConfig(int flag);
    bool                SetATLASPixConfig(int flag, ASIC_Config2* config);

    TDAC_Config*        GetTDACConfig();
    void                SetTDACConfig(TDAC_Config* config);

    VB_Config*          GetVBConfig(unsigned int index);
    bool                SetVBConfig(unsigned int index, VB_Config* config);
    bool                AddVBConfig( VB_Config *config);

    Injection_Config*   GetInjectionConfig();
    void                SetInjectionConfig(Injection_Config* config);

    FastReadout*        GetFastReadoutConfig();
    void                SetFastReadoutConfig(FastReadout* config);

    FlagManager*        GetFastReadoutClockShiftConfig();
    void                SetFastReadoutClockShiftConfig(FlagManager* config);

    FlagManager*        GetFastReadoutClockSpeedConfig();
    void                SetFastReadoutClockSpeedConfig(FlagManager* config);

    FlagManager*        GetFastReadoutTriggerSettingConfig();
    void                SetFastReadoutTriggerSettingConfig(FlagManager* config);

    FlagManager*        GetPinConfig(int flag);
    bool                SetPinConfig(int flag, FlagManager* config);

    SPINexys*           GetSPIConfig();
    void                SetSPIConfig(SPINexys* config);

    CMDConfig*          GetCMDConfig();
    void                SetCMDConfig(CMDConfig* config);

#ifdef _useQT_
    QProgressBar*       GetProgressBar();
    void                SetProgressBar(QProgressBar* pb);

    QTextEdit*          GetLogWindow();
    void                SetLogWindow(QTextEdit* log);
#endif

    /// control methods for the QProgressBar and the Log window (QTextEdit)
    ///   to prevent accessing nullptrs:
    int                 GetProgressBarValue();
    void                SetProgressBarValue(int value);
    void                SetProgressBarMaximum(int value);
    ///blocking methods to tell methods that they should not change the progress bar:
    bool                GetProgressBarBlocked();
    bool                BlockProgressBar();
    bool                ReleaseProgressBar();
    /**
     * @brief logit adds the passed text to the log window
     * @param text           - the text to add
     * @param header         - a header line that is added after a change of this string compared
     *                          to the last call of the method
     * @param error          - use std::cerr if true, or std::cout if false for the output
     */
    void                logit(std::string text, std::string header = "", bool error = false);

    ///methods for configuring the setup:

    int                 GetConfigMeans();
    bool                SetConfigMeans(int means);
    /// creates a new int to have a separate option compared to the other instances
    /// makes only sense to call on copies (for the "original" object, this method will only
    /// change the configmeans as SetConfigMeans() would do
    bool                SeparateConfigMeans(int newmeans);
    bool                SendUpdateViaSR(int configflags, bool print = false);
    bool                SendUpdateNoSR(int configflags, bool print = false);
    bool                SendUpdateSPI(int configflags, bool print = false);
    bool                SendUpdateCMD(int configflags, bool print = false);
    bool                SendUpdate(int configflags, bool print = false);

    bool                SendCMDSplit(std::vector<byte>& cmd, bool flush = true);
    bool                SendCMDNoSplit(std::vector<byte>& cmd);

    bool                ConfigurePins(bool flush = true);

    bool                ConfigureInjections(bool flush = true);
    bool                StartInjections(bool flush = true);
    bool                RestartInjections(bool flush = true);
    bool                StopInjections(bool flush = true);

    bool                WriteRAMRow(int row, bool flush = true);
    bool                WriteRAMMatrix();

    bool                ConfigureVoltageBoards(bool flush = true);

    bool                ConfigureFastReadout(bool flush = true);
    bool                StartFastReadout(bool flush = true, bool configure = true);
    bool                StopFastReadout(bool flush = true);
    bool                ResetFastReadout(int flags = -1, bool flush = true);

    /// Fast Readout Data reading/processing:

    /// reads all hits from the FPGA or until an abort condition:
    std::vector<Dataset> ReadoutAll(bool* running = nullptr, bool print = false,
                                    unsigned int maxnumhits = 0, int maxemptyreads = 30,
                                    int readsize = 1, int maxtime = 10000);
    /// sort the hits in the vector into a map by the pixel address:
    std::map<Dataset, int> SortHits(const std::vector<Dataset>& data, bool print = false,
                                    bool IDreject = false);
    /// writes the hits into a file:
    bool                WriteHitsToFile(const std::vector<Dataset>& hits, std::string filename);

private:
    int*                configmeans;
    bool                ownconfigmeans;
    NexysIO*            nexys;

    //ATLASPix3 configuration:
    ASIC_Config2*       atlaspix_config;
    ASIC_Config2*       atlaspix_dac;
    ASIC_Config2*       atlaspix_vdac;
    ASIC_Config2*       atlaspix_tdac;
    ASIC_Config2*       atlaspix_row;
    ASIC_Config2*       atlaspix_column;
    ASIC_Config2*       atlaspix_sr_loads;
    TDAC_Config*        tdacs;

    std::vector<VB_Config*>          voltageboards;
    Injection_Config*   injection;

    //Fast Readout:
    FastReadout*        fastro;
    std::string         fastro_lastread;
    FlagManager*        fastro_clockshifts;
    FlagManager*        fastro_clockspeed;
    FlagManager*        fastro_triggersettings;

    FlagManager*        fm_resets;
    FlagManager*        fm_configmode;
    FlagManager*        fm_workmode;

    SPINexys*           spiconfig;

    CMDConfig*          cmdc;

    bool                progressbarblocked;
#ifdef _useQT_
    QProgressBar*       progress;
    QTextEdit*          log;
#endif


    WaitFunction waiter;
};

#endif // CONFIGURATION_H
