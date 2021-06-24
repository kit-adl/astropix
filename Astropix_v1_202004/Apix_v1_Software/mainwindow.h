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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <iostream>
#include <iomanip>
#include <QMainWindow>
#include <bitset>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

#include <QString>
#include <QThread>
#include <QDir>
#include <QPalette>
#include <QDoubleSpinBox>
//#include <QSpinBox>  //should already be included in QDoubleSpinBox
#include <QSlider>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>

#include "libs/ftdi.h"
#include "libs/nexysio.h"

#include "libs/asic_config_2.h"

#include "libs/vb_config.h"
#include "libs/injection_config.h"
#include "libs/timing.h"
#include "libs/tdac_config.h"
#include "libs/flagmanager.h"
#include "libs/spinexys.h"
//#include "libs/configuration.h"
#include "libs/trimming.h"
#include "libs/totcalibration.h"
#include "libs/cmdconfig.h"
#include "libs/udp_config.h"

#include "libs/Measurement.h"

#include "lasercontrol.h"

#if ASIC_CONFIG2_MAJORVERSION < 2 || ASIC_CONFIG2_MINORVERSION < 4
    #error "not an up-to-date ASIC_Config2 version. must at least be Version 2.4"
#endif

namespace Ui {
class MainWindow;
}

class Trimming;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum ATLASPix3 {columns = 132, rows = 372};

    //> This is a list of ASIC SR configs
    //>  This will generate a tab UI and be updated after each other in Chip
    std::vector<ASIC_Config2*> asicConfigs;

    Injection_Config injection;

    static std::string WriteToFile(std::string filename, std::string data);
    static std::string FindFileName(std::string filenameprefix, std::string filenamesuffix);
    static std::string WriteToFile(std::string filenameprefix, std::string filenamesuffix, std::string data);

    friend void SetTrimThreshold(double thr, void* mw);

    double MeasureDelay();

    // UI Building
    void buildASIConfigUI();

public slots:
    void ProcessEvents();
    void setWaiter(WaitFunction newwaiter);

    bool ChangeConfig(std::string command);
    bool SCurveCommand(std::string command);
    bool TimingCommand(std::string command);

    void UpdateFromGUI();
public slots:
    void logit(std::string logstream, std::string header = "");



    void UpdateFromConfig();
    void UpdateInjectionGUI(bool start);
    void UpdateInjectionBoardFromGUI();

    bool ConfigureInjections(bool flush);
    bool StartInjections(bool flush);
    bool RestartInjections(bool flush);
    bool StopInjections(bool flush);

    std::vector<Dataset> ReadAllHits(bool* running, int maxemptyreads = 30, int readsize = 1);

    void on_SearchDevices_clicked();

    void on_OpenDevice_clicked();

    void on_CloseDevice_clicked();

    void on_Update_clicked();

    void on_B_Config_Load_clicked();

    void on_B_Config_Save_clicked();

    void on_B_WriteVoltageBoards_clicked();

    void on_B_Register_Read_clicked();

    void on_B_Register_Write_clicked();

    //void on_B_StartLaserSetup_clicked();

    void on_B_Injection_StartStop_clicked();

    void on_B_Injection_Restart_clicked();

    void on_B_Config_Injection_clearall_clicked();

    void on_B_Config_Injection_wholerow_clicked();

    void on_B_Config_Injection_wholecol_clicked();

    void SB_Config_Injection_valueChanged(int arg1);

    void on_CB_Config_Injection_clicked(bool checked);

    void on_SB_Config_AmpOut_HB_Col_valueChanged(int arg1);

    void CB_Config_AmpOut_HB_stateChanged(bool checked);

    void on_B_Config_AmpOut_HB_Clear_clicked();

    void on_SB_TDAC_Config_TDAC_valueChanged(int arg1);

    void on_B_TDAC_Config_SetAllToValue_clicked();

    void SB_TDAC_Address_valueChanged(int arg1);

    void on_B_TDAC_Config_WriteRow_clicked();

    void on_B_TDAC_Config_WriteMatrix_clicked();

    void on_B_PinConfig_Update_clicked();

    void on_CB_Configuration_Config_currentIndexChanged(int index);

    void on_SB_SPI_ClockDiv_valueChanged(int arg1);

    void on_CB_SPI_ReadbackEn_stateChanged(int arg1);

    void on_CB_SPI_Enable_stateChanged(int arg1);

    void on_B_SPI_Reset_clicked();

    void on_B_SPI_Reset_WriteFIFO_clicked();

    void on_B_SPI_Reset_ReadFIFO_clicked();


    void on_B_SPI_Read_ReadFIFO_clicked();

    void on_tabWidget_2_currentChanged(int index);

    void SB_Trim_valueChanged(int arg1);

    void on_SB_Trim_NumSignals_valueChanged(int arg1);

    void on_B_SCurve_clicked();

    void on_B_TDAC_Config_SetRowToValue_clicked();

    void on_B_TDAC_Config_SetColToValue_clicked();

    void on_B_AllSCurves_clicked();

    void on_CB_Trig_PosEdge_clicked(bool checked);

    void on_CB_Trig_Source_currentIndexChanged(int index);

    void on_CB_Trig_NoiseReject_currentIndexChanged(int index);

    void on_SB_Trig_PulseDelay_valueChanged(int arg1);

    void on_SB_Trig_PulseLength_valueChanged(int arg1);

    void on_B_Trig_FTDITrigger_clicked();

    void on_B_Trig_Update_clicked();

    void MeasureOsciTiming(std::string archiveprefix, bool *running, bool updategui);

    void MeasureInjectionTiming(std::string archiveprefix, bool* running, bool updategui,
                                int colmin = -1, int colmax = -1, int rowmin = -1, int rowmax = -1);

    void on_B_OsciTiming_Pixel_clicked();

    void on_B_OsciTiming_Matrix_clicked();

    void on_RB_OsciTiming_osci_toggled(bool checked);

    void on_B_Trig_ResetID_clicked();

    void on_B_Trim_Test_clicked();

    void on_B_Thr_Scan_clicked();

    void on_B_Trim_Trim_clicked();

    void on_B_MeasureList_clicked();

    void on_B_MeasureList_Reset_clicked();

    void on_B_DelayMeasure_clicked();

    void on_CB_UDP_Debug_clicked(bool checked);

    void on_CB_UDP_Reset_clicked(bool checked);

    void on_CB_UDP_Reset_Sender_clicked(bool checked);

    void on_CB_UDP_Reset_FIFO_clicked(bool checked);

    void on_SB_UDP_InterpackageDelay_valueChanged(int arg1);

    void on_SB_UDP_Timeout_valueChanged(int arg1);

    void on_LE_UDP_IP_textChanged(const QString &arg1);

    void on_SB_UDP_Port_valueChanged(int arg1);

    void on_LE_UDP_SubnetMask_textChanged(const QString &arg1);

    void on_LE_UDP_MAC_textChanged(const QString &arg1);

    void on_B_UDP_Update_clicked();

    void on_B_Inject_CMD_clicked(bool checked);

    void on_B_CMD_Send_clicked();

    //bool ReadTrigDataSet();

    void on_B_ToTCal_Start_clicked();

    void on_B_CMD_Trig_Send_clicked();

    void on_SB_Trim_MaxSimPixels_valueChanged(int arg1);

    void on_CB_Trim_UseTrigID_clicked(bool checked);

    void on_SB_Trim_MaxTime_valueChanged(int arg1);

    void on_ChipStart_clicked();

    void on_actionLoad_Configuration_triggered();

    void on_LogResetButton_clicked();

private:
    void SetSpinBoxMinMax(QDoubleSpinBox* spinstart, QDoubleSpinBox *spinstop, double min, double max);

    void LoadFileList();
    void SaveFileList();


    Ui::MainWindow *ui;

    WaitFunction waiter;

    LaserControl* lasersetupgui;

    FTDI* ftdi;
    FT_HANDLE* ftdihandle;
    NexysIO* nexys;

    SPINexys spiconfig;
    CMDConfig    cmd;



    ASIC_Config2 atlaspix_config;
    ASIC_Config2 atlaspix_dac;
    ASIC_Config2 atlaspix_vdac;
    ASIC_Config2 atlaspix_tdac;
    ASIC_Config2 atlaspix_row;
    ASIC_Config2 atlaspix_column;
    ASIC_Config2 atlaspix_sr_loads;
    TDAC_Config  tdacs;

    ASIC_Config2_Manager atlaspix_config_manager;


    //VB_Config voltageboards[2];
    std::vector<VB_Config*> voltageboards;

    //objects from GUI building for the Chip Configuration:
    std::vector<QSlider*>  dac_sliders;
    std::vector<QSpinBox*> dac_spinboxes;
    std::vector<QCheckBox*> dac_checkboxes;
    std::vector<QLabel*>   dac_labels;
    std::vector<QWidget*> dac_gbs;

    //objects from GUI building for voltage boards:
    std::vector<QDoubleSpinBox*> vb_spinbox;
    std::vector<QLabel*>         vb_label;

    FlagManager fm_resets;
    FlagManager fm_configmode;
    FlagManager fm_workmode;
    std::vector<QCheckBox*> fm_checkboxes;

    UDP_Config udpro;

    Configuration config;

    Trimming* trim;

    FlagManager trigger_fm;
    bool ReadNoMuxDataSet();
};

#endif // MAINWINDOW_H
