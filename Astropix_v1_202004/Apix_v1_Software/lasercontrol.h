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
 * GUI implementation for control of the LaserSetup at KIT-ADL  *
 *                                                              *
 * This class implements GUI controls for the laser and the     *
 * stages of the lasersetup using the lasersetup class          *
 * Furthermore, a VISA connection is integrated into the GUI    *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (23.08.19)                                      *
 ****************************************************************/
#ifndef LASERCONTROL_H
#define LASERCONTROL_H

#include <QWidget>
#include <QString>
#include <QThread>
#include <QDir>
#include <QPalette>
#include <QDoubleSpinBox>

#include "lasersetup.h"
#include "libs/zip_file.h"
#include "libs/VISA/visadevice.h"
#include "libs/VISA/keysightosci.h"
#include "libs/VISA/tektronixosci.h"
#include "libs/Measurement.h"
#include "libs/nexysio.h"
#include "fastreadout.h"
#include "libs/timing.h"

namespace Ui {
class LaserControl;
}

class LaserControl : public QWidget
{
    Q_OBJECT

    friend class MainWindow;

    friend double fastrotest(double, double, double, void*);
    friend double testfunclc(double, double, double, void*);
    friend double testfunclc3d(double, double, double, void*);
public:
    explicit LaserControl(QWidget *parent = nullptr);
    ~LaserControl();

    bool VISAWrite(std::string data);

    void logit(std::string logstream);

    NexysIO* GetNexys();
    void SetNexys(NexysIO* newnexys);

    FastReadout* GetFastRO();
    void SetFastRO(FastReadout* newfastro);

    void ResetFastRO();

public slots:
    void ProcessEvents();
    void setWaiter(WaitFunction newwaiter);

private slots:
    void on_B_LS_AutoConnect_clicked();

    void on_B_LS_SearchPorts_clicked();

    void on_B_LS_OpenLaser_clicked();

    void on_B_LS_CloseLaser_clicked();

    void on_B_LS_OpenStages_clicked();

    void on_B_LS_CloseStages_clicked();

    void on_B_LS_LaserOn_clicked();

    void on_SB_LS_Laserfreq_editingFinished();

    void on_SB_LS_Lasertune_editingFinished();

    void on_CB_Laser_TriggerEdge_currentIndexChanged(const QString &arg1);

    void on_CB_Laser_TriggerSource_currentIndexChanged(const QString &arg1);

    void on_SB_Laser_TriggerLevel_editingFinished();

    void on_SB_Laser_xpos_editingFinished();

    void on_SB_Laser_ypos_editingFinished();

    void on_SB_Laser_zpos_editingFinished();

    void on_B_LS_MotorX_clicked();

    void on_B_LS_MotorY_clicked();

    void on_B_LS_MotorZ_clicked();

    void on_B_LS_InitX_clicked();

    void on_B_LS_InitY_clicked();

    void on_B_LS_InitZ_clicked();

    void on_CB_LS_ScanX_currentIndexChanged(int index);

    void on_CB_LS_ScanY_currentIndexChanged(int index);

    void on_CB_LS_ScanZ_currentIndexChanged(int index);

    void on_B_LaserSetup_clicked();

    void on_B_2DScanStart_clicked();

    void on_B_2DScanAbort_clicked();

    void on_B_RangeImportCancel_clicked();

    void on_B_RangeImportOK_clicked();

    void on_B_RangeImport_clicked();

    void on_B_Osci_Scan_clicked();

    void on_B_Osci_Open_clicked();

    void on_B_Osci_Close_clicked();

    void on_B_Osci_Write_clicked();

    void on_B_Osci_Read_clicked();

    void on_B_Osci_GetWfm_clicked();

    void on_B_Close_clicked();

    void on_LE_Osci_Query_returnPressed();

    void on_B_LS_3DScanAbort_clicked();

    void on_B_LS_3D_Scan_clicked();

private:
    void SetSpinBoxMinMax(QDoubleSpinBox* spinstart, QDoubleSpinBox *spinstop, double min, double max);

    Ui::LaserControl *ui;

    WaitFunction waiter;

    LaserSetup setup;
    VISADevice* osci;

    NexysIO*     nexys;
    FastReadout* fastro;

    QPalette palette_button_green;
    QPalette palette_button_gray;
    QPalette palette_button_red;

    std::string archivename; //filename for the laser setup 2D scan
};

#endif // LASERCONTROL_H
