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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <sstream>
#include "math.h"
#include <chrono>
#include <ctime>
#include <thread>

//#include "visa.h"
#include "tektronixosci.h"
#include "keysightosci.h"

#include "wfmpreview.h"
#include "functionplotter.h"

#include <QDateTime>
#include <QLabel>

typedef std::chrono::steady_clock::time_point TimePoint;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    TimePoint GetTimeNow();
    double GetTimeDifference(const TimePoint start, const TimePoint ende);

public slots:
    void ProcessEvents();
    void setWaiter(WaitFunction newwaiter);

private slots:
    void logit(std::string logstream);

    void on_openButton_clicked();

    void on_closeButton_clicked();

    void on_readButton_clicked();

    void on_writeButton_clicked();

    void on_scanButton_clicked();

    void on_getwaveformButton_clicked();

    void on_histogramButton_clicked();

    void on_getmanyButton_clicked();

    void on_query_returnPressed();

    void on_coincidenceButton_clicked();

    void on_B_Keysight_IV_Curve_clicked();

    void on_B_Keysight_IoverT_clicked();

    void on_CB_autorange_source_toggled(bool checked);

    void on_CB_autorange_sense_toggled(bool checked);

    void on_pushButton_clicked();

    void on_CB_IV_currentIndexChanged(int index);

    void SetIVCurve();

    void SetVICurve();

    void on_CB_Chan1_clicked();

    void on_CB_Chan2_clicked();

    void on_CB_Chan3_clicked();

    void on_CB_Chan4_clicked();

    void on_B_StartStop_clicked();

    void on_SB_InitialDelay_valueChanged(double arg1);

    void on_B_WfmDisplay_clicked();

    void on_LE_TimeScale_returnPressed();

    void on_LE_TimeOffset_returnPressed();

    void on_LE_VoltScale_returnPressed();

    void on_LE_VoltOffset_returnPressed();

    void on_SB_Channel_valueChanged(int arg1);

    void on_B_ResetHist_clicked();

    void on_B_Keysight_IwithTrig_clicked();

    void on_B_Keysight_IwithTrig_2_clicked();

    void on_B_TimeToSMU_clicked();

    void on_B_PowerReg_MeasurePnt_clicked();

private:
    Ui::MainWindow *ui;

    WfmPreview *wfmdisplay;

    WaitFunction waiter;

    VISADevice* osci;
};

#endif // MAINWINDOW_H
