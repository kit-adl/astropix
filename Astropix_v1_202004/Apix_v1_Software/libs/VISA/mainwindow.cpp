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


#include "mainwindow.h"
#include "ui_mainwindow.h"

//#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
//#define _CRT_SECURE_NO_DEPRECATE
//#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

#include <QString>

//static ViSession defaultRM = VI_NULL;
//static ViSession instr = VI_NULL;
static ViStatus status;
//static char instrResourceString[VI_FIND_BUFLEN];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_CB_IV_currentIndexChanged(ui->CB_IV->currentIndex());

    osci = new VISADevice();

    wfmdisplay = new WfmPreview();
}

MainWindow::~MainWindow()
{
    delete wfmdisplay;

    delete ui;
    if(osci != nullptr)
    {
        osci->close();
        delete osci;
        osci = nullptr;
    }
}

TimePoint MainWindow::GetTimeNow()
{
    return std::chrono::steady_clock::now();
}

double MainWindow::GetTimeDifference(const TimePoint start, const TimePoint ende)
{
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(ende - start).count();
    return duration / 1000000.;
}

void MainWindow::ProcessEvents()
{
    QApplication::processEvents();
}

void MainWindow::setWaiter(WaitFunction newwaiter)
{
    this->waiter = newwaiter;
}

void MainWindow::logit(std::string logstream)
{
    std::cout << logstream << std::endl;
    QString Logbuffer= QString::fromStdString(logstream);
    ui->Log->append(Logbuffer);
}

void MainWindow::on_scanButton_clicked()
{
    if(osci == nullptr)
        osci = new VISADevice();

    std::vector<std::string> devices = osci->ScanForDevices();

    int index = 0;
    for(auto& it : devices)
    {
        std::stringstream s("");
        s << "Device " << ++index << ": " << it;
        logit(s.str());
    }

    if(devices.size() == 0)
    {
        logit("No Devices found!");
    }

    //set maximum index for the selection spin box:
    ui->IndexBox->setMaximum(int(devices.size()));
    if(ui->IndexBox->value() > int(devices.size()))
        ui->IndexBox->setValue(int(devices.size()));
    if(devices.size() > 0)
    {
        ui->IndexBox->setMinimum(1);
        if(ui->IndexBox->value() == 0)
            ui->IndexBox->setValue(1);
    }
    else
        ui->IndexBox->setMinimum(0);
}


void MainWindow::on_openButton_clicked()
{
    if(osci == nullptr)
        return;

    int index = ui->IndexBox->value() - 1;
    if(index < 0 || index > int(osci->GetNumDevicesFound()))
    {
        std::cerr << "Device index out of range" << std::endl;
        return;
    }

    if(osci->CheckResourceName(index, "KEYSIGHT"))
    {
        int printlevel = osci->GetPrintLevel();
        delete osci;
        osci = new KeysightOsci(printlevel);
        osci->ScanForDevices();
        wfmdisplay->plotter.SetKeysightOsci(true);

        ui->B_ResetHist->setEnabled(false);

        std::cout << "Opening Keysight Device" << std::endl;
    }
    else if(osci->CheckResourceName(index, "TEKTRONIX"))
    {
        int printlevel = osci->GetPrintLevel();
        delete osci;
        osci = new TektronixOsci(printlevel);
        osci->ScanForDevices();

        wfmdisplay->plotter.SetKeysightOsci(false);

        ui->B_ResetHist->setEnabled(true);

        std::cout << "Opening Tektronix Device" << std::endl;
    }
    else
    {
        ui->B_Keysight_IV_Curve->setEnabled(true);
        ui->B_Keysight_IoverT->setEnabled(true);
        ui->B_Keysight_IwithTrig->setEnabled(true);
        ui->B_Keysight_IwithTrig_2->setEnabled(true);
        ui->B_PowerReg_MeasurePnt->setEnabled(true);
        std::cout << "Opening Other Device" << std::endl;
    }

    if(!osci->open(static_cast<unsigned int>(index)))
    {
        std::cout << "Could not open Device " << index << std::endl;
        return;
    }

    //update the display scales for oscilloscopes:
    //  (will return invalid numbers for failure, e.g. in non-Keysight or non-Tektronix devices)
    ui->LE_TimeScale->setText(QString::number(osci->GetDispTimeScale()));
    ui->LE_TimeOffset->setText(QString::number(osci->GetDispTimeOffset()));
    ui->LE_VoltScale->setText(QString::number(osci->GetVoltageScale(ui->SB_Channel->value())));
    ui->LE_VoltOffset->setText(QString::number(osci->GetVoltageOffset(ui->SB_Channel->value())));

    logit("Session opened.");

}

void MainWindow::on_closeButton_clicked()
{
    if(osci != nullptr)
        osci->close();

    ui->B_Keysight_IoverT->setEnabled(false);
    ui->B_Keysight_IV_Curve->setEnabled(false);
    ui->B_Keysight_IwithTrig->setEnabled(false);
    ui->B_Keysight_IwithTrig_2->setEnabled(false);
    ui->B_PowerReg_MeasurePnt->setEnabled(false);

    logit("Session closed.");
}

void MainWindow::on_readButton_clicked()
{
    //only do something if a session is open
    if(osci == nullptr || !osci->is_open())
        return;

    int characters = ui->readZeichenBox->value();
    logit("read " + QString::number(ui->readZeichenBox->value()).toStdString()
          + " characters...");

    std::string answer = osci->Read(characters);

    if(answer.length() > 0)
        logit(QString::number(answer.length()).toStdString() + " characters read: \n"
              + answer);
    else
        logit("0 characters read");

    return;
}

void MainWindow::on_writeButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    logit("write " + QString::number(ui->query->text().length()).toStdString()
          + " characters: \"" + ui->query->text().toStdString() + "\"");

    int written = osci->Write(ui->query->text().toStdString());

    logit(QString::number(written).toStdString() + " characters written");

    return;
}

void MainWindow::on_getwaveformButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    logit("Aquiring Wave Form...");

    QCheckBox* cbchannel[4] = {ui->CB_Chan1, ui->CB_Chan2, ui->CB_Chan3, ui->CB_Chan4};
    int numchannels = 0;
    for(int i = 0; i < 4; ++i)
        if(cbchannel[i]->isChecked())
            ++numchannels;

    if(numchannels == 0)
        return;
    else if(numchannels > 1)
    {
        std::vector<double> waveform[4];

        if(wfmdisplay->isVisible())
        {
            wfmdisplay->plotter.ClearData();
            wfmdisplay->plotter.AddCurve(Qt::blue);
            wfmdisplay->plotter.AddCurve(Qt::green);
            wfmdisplay->plotter.AddCurve(Qt::red);
            wfmdisplay->plotter.AddCurve(Qt::gray);
        }

        for(int i = 0; i < 4; ++i)
        {
            if(!cbchannel[i]->isChecked())
                continue;

            if(!osci->SetChannel(i+1))
                break;
            if(osci->ReadWaveFormSetup(false) < 0)
                break;
            std::string wfm = osci->ReadWaveForm(false, waiter, 5);
            if(wfmdisplay->isVisible())
            {
                unsigned int startpos = wfm.find('\n'); //do not use the timing information as data points
                wfmdisplay->plotter.AddData(i, wfm.substr(startpos + 1));
            }
            waveform[i] = osci->DecodeWaveFormY(wfm);
        }
        double timescale = osci->GetTimeScale("");

        if(wfmdisplay->isVisible())
        {
            wfmdisplay->ActivatePlot(true);
            wfmdisplay->plotter.Draw();
        }

        //search for unused filename "waveformset_%i.dat":
        std::fstream f;
        int index = 0;
        do
        {
            f.close();
            ++index;
            f.open(("waveformset_" + QString::number(index).toStdString()
                    + ".dat").c_str(), std::ios::in);
        }while(f.is_open());
        f.close();

        std::stringstream s("");
        s << "waveformset_" << index << ".dat";
        f.open(s.str().c_str(), std::ios::out | std::ios::app);

        f << "# Timescale: " << timescale << std::endl;

        f << "# Channels: ";
        for(int i = 0; i < 4; ++i)
            if(cbchannel[i]->isChecked())
                f << " Ch" << (i+1);
        f << std::endl;

        int length = 0;
        for(int i = 0; i < 4; ++i)
            if(int(waveform[i].size()) > length)
                length = int(waveform[i].size());

        std::stringstream s2("");
        for(int i = 0; i < length; ++i)
        {
            for(int j = 0; j < 4; ++j)
                if(waveform[j].size() > 0)
                    s2 << waveform[j][static_cast<unsigned int>(i)] << " ";
            s2 << std::endl;
        }

        f << s2.str();
        f.flush();
        f.close();

        logit("WaveForm saved to \"waveformset_" + QString::number(index).toStdString()
              + ".dat\"");
    }
    else
    {
        std::string wfm = osci->ReadWaveForm(!osci->GetWaveFormSetupStatus(), waiter, 5);
        double timescale = osci->GetTimeScale("");
        std::vector<double> waveform = osci->DecodeWaveFormY(wfm);

        if(wfmdisplay->isVisible())
        {
            wfmdisplay->plotter.SetTimeScale(osci->GetTimeScale(""));
            wfmdisplay->plotter.ClearData();
            wfmdisplay->plotter.AddCurve(Qt::blue);
            unsigned int startpos = wfm.find('\n'); //do not use the timing information as data points
            wfmdisplay->plotter.AddData(0, wfm.substr(startpos + 1));
            wfmdisplay->ActivatePlot(true);
            wfmdisplay->plotter.Draw();
        }

        //search for unused filename "waveform_%i.dat":
        std::fstream f;
        int index = 0;
        do
        {
            f.close();
            ++index;
            f.open(("waveform_" + QString::number(index).toStdString()
                    + ".dat").c_str(), std::ios::in);
        }while(f.is_open());
        f.close();

        if(!ui->CB_Compressed->isChecked())
            osci->SaveToFile(("waveform_" + QString::number(index).toStdString() + ".dat"), waveform, timescale, true);
        //else
        //    osci->SaveCompressed(("waveform_" + QString::number(index).toStdString() + ".dat"), wfm, true);

        logit("WaveForm saved to \"waveform_" + QString::number(index).toStdString()
              + ".dat\"");
    }
}

void MainWindow::on_histogramButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    logit("getHistogram");

    //--- get the parameters to transform to voltage ---
    osci->Write("Histogram:Box?");
    sleep(50);
    std::string box = osci->Read(60);

    unsigned int starts[4] = {0, 0, 0, 0};
    for(unsigned int i = 1; i < 4; ++i)
        starts[i] = box.find(',',starts[i-1]+1);

    double top, bottom;

    {
        std::stringstream s("");
        s << box.substr(starts[1]+1, starts[2]-starts[1]);
        s >> top;
    }
    {
        std::stringstream s("");
        s << box.substr(starts[3] + 1);
        s >> bottom;
    }

    double step = (top - bottom) / 250;
    double firstmiddle = top + step / 2;

    if(osci->Write("Histogram:Data?") == 0)
    {
        logit("Error requesting histogram data");
        return;
    }

    //search unused filename:
    std::fstream f;
    int index = 0;
    do
    {
        f.close();
        ++index;
        f.open(("histogram_" + QString::number(index).toStdString()
                + ".dat").c_str(), std::ios::in);
    }while(f.is_open());
    f.close();

    //open the usused file:
    f.open(("histogram_" + QString::number(index).toStdString()
            + ".dat").c_str(), std::ios::out);

    logit("write Histogram data to \"histogram_" + QString::number(index).toStdString()
          + ".dat\"");

    std::stringstream sdata("");

    index = 0;          //now the variable will be used as index for the bin,
                        // not the file index
    long entries = 0;
    std::string answer = "";
    int numberlength = 0;
    do
    {
        answer = osci->Read(100);
        unsigned int i=0;
        while(i < answer.length())
        {
            //building of the number of entries for a bin
            while(i < answer.length() && answer.c_str()[i] != ',' && answer.c_str()[i] != 10) //do not use ',' and newline (10)
            {
                entries = entries * 10 + static_cast<long>(answer.c_str()[i] - '0');
                ++i;
                ++numberlength;
            }

            //at the end of the buffer, it is not clear whether the number was
            //  complete or not, so it will be continued with the next buffer
            if(i != answer.length())
            {
                //add the line to the file
                sdata << firstmiddle - (step * index) << "\t" << entries << std::endl;
                wfmdisplay->hist.SetBin(index, entries);
                //prepare for the next bin:
                entries = 0;
                numberlength = 0;
                ++i;    //buffer[i] = ',' now
                ++index;
            }
        }
    }while(answer.length() == 100);

    if(numberlength > 0) //entries > 0)
    {
        sdata << firstmiddle - step * index << "\t" << entries << std::endl;
        wfmdisplay->hist.SetBin(index, entries);
        ++index;
    }

    f << sdata.rdbuf();
    f.flush();
    f.close();

    if(wfmdisplay->isVisible())
    {
        wfmdisplay->ActivatePlot(false);
        wfmdisplay->hist.draw(false);
    }

    logit("wrote histogram with " + QString::number(index).toStdString()
          + " entries");
}

void MainWindow::on_getmanyButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    //snippet to abort the measurement:
    static bool running = false;
    if(running)
    {
        running = false;
        return;
    }

    //Rate Timeout variables (ring storage):
    int timeout = ui->SB_Timeout->value();
    const int ringsize = ui->SB_RateAbort_RingSize->value();
    TimePoint* ring = nullptr;
    int ringindex = 0;
    double maxrate = ui->SB_RateAbort_Rate->value();

    //only apply if activated:
    if(ringsize > 0)
    {
        ring = new TimePoint[static_cast<unsigned int>(ringsize)];

        //fill the ring:
        ring[0] = GetTimeNow();
        for(int i = 1; i < ringsize; ++i)
            ring[i] = ring[0];
    }

    //save the channel configuration:
    bool recordchannel[4];
    recordchannel[0] = ui->CB_Chan1->isChecked();
    recordchannel[1] = ui->CB_Chan2->isChecked();
    recordchannel[2] = ui->CB_Chan3->isChecked();
    recordchannel[3] = ui->CB_Chan4->isChecked();
    //count the number of channels per event:
    int numchannels = 0;
    for(int i = 0; i < 4; ++i)
        if(recordchannel[i])
            ++numchannels;

    //change gui to "Stop Measurement":
    running = true;
    std::string caption = ui->getmanyButton->text().toStdString();
    ui->getmanyButton->setText("Stop Measurement");
    QApplication::processEvents();

    //prepare the first measurement:
    if(!osci->SetupSingleTrigger())
    {
        std::cerr << "Error setting up single trigger mode. Aborting." << std::endl;
        running = false;
    }
    if(!osci->WaitForSingleTrigger(waiter, 10, timeout))
    {
        std::cerr << "Error querying trigger state. aborting" << std::endl;
        running = false;
    }
    if(ringsize > 0)
        ring[ringindex++] = GetTimeNow();

    int count = ui->WFNrBox->value();
    int fileindex = 0;

    for(int i = 0; i < count && running; ++i)
    {
        if(numchannels > 1)
        {
            std::string wfm[4];
            std::vector<double> waveform[4];

            //get the waveforms for the selected channels:
            for(int chan = 0; chan < 4; ++chan)
            {
                if(!recordchannel[chan])
                    continue;

                if(!osci->SetChannel(chan+1))
                    break;
                if(osci->ReadWaveFormSetup(false) < 0)
                    break;

                wfm[chan] = osci->ReadWaveForm(false, waiter, 5);

                //process the read waveform:
                waveform[chan] = osci->DecodeWaveFormY(wfm[chan]);
            }

            double timescale = osci->GetTimeScale("");

            //prepare the oscilloscope for the next signal:
            if(!osci->SetupSingleTrigger())
            {
                std::cerr << "Error setting up single trigger mode. Aborting." << std::endl;
                break;
            }

            //  search unused filename:
            std::fstream f;
            do
            {
                f.close();
                std::stringstream s("");
                s << "waveformset_" << ++fileindex << ".dat";
                f.open(s.str().c_str(), std::ios::in);
            }while(f.is_open());
            f.close();

            //  save to found filename:
            std::stringstream s("");
            s << "waveformset_" << fileindex << ".dat";
            f.open(s.str().c_str(), std::ios::out | std::ios::app);
            if(!f.is_open())
                break;

            //header:
            f << "# Timescale: " << timescale << std::endl << "# Channels: ";
            for(int chan = 0; chan < 4; ++chan)
                if(recordchannel[chan])
                    f << " Ch" << chan+1;
            f << std::endl;

            //find the data length:
            unsigned int length = 0;
            for(int chan = 0; chan < 4; ++chan)
                if(length < waveform[chan].size())
                    length = waveform[chan].size();

            std::stringstream sdata("");

            for(unsigned int point = 0; point < length; ++point)
            {
                if(recordchannel[0])
                    sdata << waveform[0][point] << "\t";
                if(recordchannel[1])
                    sdata << waveform[1][point] << "\t";
                if(recordchannel[2])
                    sdata << waveform[2][point] << "\t";
                if(recordchannel[3])
                    sdata << waveform[3][point] << "\t";
                sdata << std::endl;
            }

            f << sdata.str();
            f.flush();
            f.close();

            logit(std::string("Saved event to \"") + s.str() + "\"");
        }
        else
        {
            std::string wfm = osci->ReadWaveForm(false, waiter, 5);

            //prepare the oscilloscope for the next signal:
            if(!osci->SetupSingleTrigger())
            {
                std::cerr << "Error setting up single trigger mode. Aborting." << std::endl;
                break;
            }

            //process the read waveform:
            //  decoding
            double timescale = osci->GetTimeScale("");
            std::vector<double> waveform = osci->DecodeWaveFormY(wfm);

            //  search unused filename:
            std::fstream f;
            do
            {
                f.close();
                std::stringstream s("");
                s << "waveform_" << ++fileindex << ".dat";
                f.open(s.str().c_str(), std::ios::in);
            }while(f.is_open());
            f.close();

            //  save to found filename:
            std::stringstream s("");
            s << "waveform_" << fileindex << ".dat";
            osci->SaveToFile(s.str(), waveform, timescale, true);

            logit(std::string("Saved event to \"") + s.str() + "\"");
        }


        //std::stringstream s("");
        //s << "Recorded " << (i+1) << " events";
        //logit(s.str());

        //wait for the next trigger:
        if(!osci->WaitForSingleTrigger(waiter, 10, timeout))
        {
            std::cerr << "Error querying trigger state. aborting" << std::endl;
            break;
        }
        if(ringsize > 0)
        {
            ring[ringindex++] = GetTimeNow();
            if(ringindex == ringsize)
                ringindex = 0;

            if(ring[ringsize-1] != ring[ringsize-2]) //the ring has completely been filled
            {
                if(ringindex == 0)
                {
                    if(GetTimeDifference(ring[0], ring[ringsize-1]) < (ringsize - 1) / maxrate)
                    {
                        running = false;
                        std::cerr << "Error: too large event rate" << std::endl;
                    }
                }
                if(GetTimeDifference(ring[ringindex], ring[ringindex-1]) < (ringsize - 1) / maxrate)
                {
                    running = false;
                    std::cerr << "Error: too large event rate" << std::endl;
                }
            }
        }

    }

    //restore gui after measurement finished or abort:
    ui->getmanyButton->setText(caption.c_str());
    running = false;
}

void MainWindow::on_query_returnPressed()
{
    on_writeButton_clicked();
}

void MainWindow::on_coincidenceButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    logit("Aquiring Coincidence Wave Forms ...");

    if(osci->Write("DATA:ENCDG RIBINARY") == 0
        || osci->Write("WFMOutpre:BYT_Nr 1") == 0)
    {
        std::cerr << "Error: Could not send encoding command" << std::endl;
        return;
    }

    int fileindex = 0;

    //--- set transmission format --
    ui->runningcheckBox->setChecked(true);

    while(ui->runningcheckBox->checkState())
    {
        //wait for an event:
        int acqs = -1;
        int startacqs = -1;
        while(acqs == startacqs)
        {
            acqs = osci->QueryI("ACQ:NUMACq?");
            if(acqs == -1023*1024)
            {
                logit("Error getting acquisitions");
                return;
            }
            if(startacqs == -1)
                startacqs = acqs;
        }

        //set input source: Ch1 = Scintillator
        if(osci->Write("Data:Source Ch1") == 0)
        {
            logit("Error getting \"Scintillator\" WaveForm");
            return;
        }

        //get the scintillator waveform (Channel 1):
        std::string wfm = osci->ReadWaveForm(false, waiter, 5);
        double timescale = osci->GetTimeScale("");
        std::vector<double> waveform = osci->DecodeWaveFormY(wfm);

        //if there was a signal:
        if(waveform.front() - waveform.back() > 0.1)
        {
            //find unused filename:
            std::fstream f;
            do{
                f.close();
                std::stringstream s("");
                s << "waveform_" << ++fileindex << "_scintillator.dat";
                f.open(s.str().c_str(), std::ios::in);
            }while(f.is_open());
            f.close();

            //save scintillator signal:
            std::stringstream s("");
            s << "waveform_" << fileindex << "_scintillator.dat";
            osci->SaveToFile(s.str(), waveform, timescale, true);

            //set input source: Ch2 = Signal
            if(osci->Write("Data:Source Ch2") == 0)
            {
                logit("Error getting \"Signal\" WaveForm");
                return;
            }

            //get signal waveform:
            wfm = osci->ReadWaveForm(false, waiter, 5);
            waveform.clear();
            waveform = osci->DecodeWaveFormY(wfm);
            //save signal waveform:
            s.str("");
            s << "waveform_" << fileindex << "_signal.dat";
            osci->SaveToFile(s.str(), waveform, timescale, true);
        }

        QApplication::processEvents();
    }
}

void MainWindow::on_B_Keysight_IV_Curve_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    static bool running = false;
    bool IoverV = ui->CB_IV->currentIndex() == 0;


    if(running)
    {
        ui->B_Keysight_IV_Curve->setText("Measure IV Curve");
        running = false;
        return;
    }

    logit("Start Measuring IV Curve ...");

    running = true;
    ui->B_Keysight_IV_Curve->setText("Stop IV Measurement");

    //retrieve scan parameters from GUI:
    double start   = ui->SB_SweepStart->value();
    for (int i = 0; i < ui->CB_SweepStart->currentIndex(); i++)
        start/=1000;

    double end     = ui->SB_SweepStop->value();
    for (int i = 0; i < ui->CB_SweepStop->currentIndex(); i++)
        end/=1000;

    double step    = ui->SB_SweepStep->value();
    for (int i = 0; i < ui->CB_SweepStep->currentIndex(); i++)
        step/=1000;

    double compli = ui->SB_Compliance->value();
    for (int i = 0; i < ui->CB_Compliance_unit->currentIndex(); i++)
        compli/=1000;

    int    samples = ui->SB_Samples->value();

    osci->Write("*RST");
    if (IoverV)
    {
        osci->Write(":sour:func:mode volt");
        osci->Write(std::string(":sour:volt:range:auto ") + ((ui->CB_autorange_source->isChecked())?"on":"off"));
    }
    else
    {
        osci->Write(":sour:func:mode curr");
        osci->Write(std::string(":sour:curr:range:auto ") + ((ui->CB_autorange_source->isChecked())?"on":"off"));
    }

    if(!ui->CB_autorange_source->isChecked())
    {
        std::string numbertext = ui->CB_SourceRange->currentText().toStdString();
        numbertext = numbertext.substr(0, numbertext.find(" "));
        if (IoverV)
            osci->Write(":sour:volt:range " + numbertext);
        else
            osci->Write(":sour:curr:range " + numbertext);
    }

    if (IoverV)
    {
        osci->Write(":sens:func curr");
        osci->Write(std::string(":sens:curr:range:auto ") + ((ui->CB_autorange_sense->isChecked())?"on":"off"));
    }
    else
    {
        osci->Write(":sens:func volt");
        osci->Write(std::string(":sens:volt:range:auto ") + ((ui->CB_autorange_sense->isChecked())?"on":"off"));
    }
    if(!ui->CB_autorange_sense->isChecked())
    {
        std::string numbertext = ui->CB_SenseRange->currentText().toStdString();
        numbertext = numbertext.substr(0, numbertext.find(" "));
        if (IoverV)
            osci->Write(std::string(":sens:curr:rang ") + numbertext);
        else
            osci->Write(std::string(":sens:volt:rang ") + numbertext);
    }

    if (IoverV)
    {
        osci->Write(":sens:curr:prot " + QString::number(ui->SB_Compliance->value()).toStdString()
                                + "e-" + QString::number(3*ui->CB_Compliance_unit->currentIndex()).toStdString()); //current compliance
        osci->Write(":sens:curr:aper " + QString::number(ui->SB_measuretime->value()).toStdString());  //set measurement duration
    }
    else
    {
        osci->Write(":sens:volt:prot " + QString::number(ui->SB_Compliance->value()).toStdString()
                                + "e-" + QString::number(3*ui->CB_Compliance_unit->currentIndex()).toStdString()); //current compliance
        osci->Write(":sens:volt:aper " + QString::number(ui->SB_measuretime->value()).toStdString());  //set measurement duration
    }

    if(status != VI_SUCCESS)
        return;


    if(std::abs(step) < 1e-4)
        return;

    //adapt step value to scan direction:
    if(start < end)
    {
        if(step < 0)
            step = -step;
    }
    else if(start > end)
    {
        if(step > 0)
            step = -step;
    }

    std::stringstream s("");
    if (IoverV)
        s << ":sour:volt " << start;
    else
        s << ":sour:curr " << start;
    osci->Write(s.str());
    osci->Write(":outp on");

    if(status != VI_SUCCESS)
    {
        running = false;
        ui->B_Keysight_IV_Curve->setText("Measure IV Curve");
        return;
    }

    std::fstream f;
    std::stringstream sf("");
    static int findex = 1;
    do{
        f.close();
        sf.str("");
        if (IoverV)
            sf << "IVCurve_" << findex++ << ".dat";
        else
            sf << "VICurve_" << findex++ << ".dat";
        f.open(sf.str().c_str(), std::ios::in);
    }while(f.is_open());

    f.open(sf.str().c_str(), std::ios::out);
    if (IoverV)
        f << "# Voltage (in V); Current (in A); RMS of Current (in A);" << std::endl;
    else
        f << "# Current (in A); Voltage (in V); RMS of Voltage (in V);" << std::endl;

    for(double voltage = start; ((voltage <= end) ^ (step < 0)) && running; voltage += step)
    {
        //wait for the SMU to reach the desired voltage:
        Sleep(500);

        std::string answer;
        //extract the data from the answer string:
        double average = 0;
        double stabw   = 0;
        for(int i = 0; i < samples; ++i)
        {
            if (IoverV)
                osci->Write(":meas:curr? (@1)");
            else
                osci->Write(":meas:volt? (@1)");
            answer = osci->Read(14);
            std::stringstream si(answer);
            double vald;
            si >> vald;
            average += vald;
            stabw   += vald * vald;
        }
        average /= samples;
        stabw   /= samples;
        stabw   -= average * average;
        stabw    = sqrt(stabw);
        //std::cout << "Answer for Read at " << voltage << "V: " << average << " +/- " << stabw << std::endl;
        std::stringstream sinfo("");
        if(IoverV)
            sinfo << "   ... " << voltage << "V done";
        else
            sinfo << "   ... " << voltage << "A done";
        logit(sinfo.str());

        f << voltage << "\t" << average << "\t" << stabw << std::endl;


        //set the voltage for the next measurement:
        s.str("");
        if (IoverV)
            s << ":sour:volt " << voltage;
        else
            s << ":sour:curr " << voltage;
        osci->Write(s.str());

        if ((average*average >= compli*compli * 0.999* 0.999) && (ui->CB_StopOnCompliance->isChecked()))
        {
            std::cout << "Compliance reached." << std::endl;
            break;
        }

        QApplication::processEvents();
    }

    f.close();

    std::stringstream logstream("");
    logstream << "Wrote measurement results to \"" << sf.str() << "\"";
    logit(logstream.str());

    //ramp down the voltage and turn off:
    if (IoverV)
        osci->Write(":sour:volt 0");
    else
        osci->Write(":sour:curr 0");
    Sleep(3000);
    osci->Write(":outp off");

    running = false;
    ui->B_Keysight_IV_Curve->setText("Measure IV Curve");

    logit("Done measureing IV Curve");


    return;
}

void MainWindow::on_B_Keysight_IoverT_clicked()
{
    static bool running = false;

    if(running)
    {
        ui->B_Keysight_IV_Curve->setText("Measure IV Curve");
        running = false;
        return;
    }

    logit("Start Measuring Current over Time ...");

    running = true;
    ui->B_Keysight_IoverT->setText("Stop I over t Measurement");

    if(osci == nullptr || !osci->is_open())
        return;

    //retrieve scan parameters from GUI:
    int    samples = ui->SB_Samples->value();

    osci->Write("*RST");
    osci->Write(":sour:func:mode volt");
    osci->Write(std::string(":sour:volt:range:auto ") + ((ui->CB_autorange_source->isChecked())?"on":"off"));
    if(!ui->CB_autorange_source->isChecked())
    {
        std::string numbertext = ui->CB_SourceRange->currentText().toStdString();
        numbertext = numbertext.substr(0,numbertext.find(" "));
        osci->Write(":sour:volt:range " + numbertext);
    }

    osci->Write(":sens:func curr");
    osci->Write(std::string(":sens:curr:range:auto ") + ((ui->CB_autorange_sense->isChecked())?"on":"off"));
    if(!ui->CB_autorange_sense->isChecked())
    {
        std::string numbertext = ui->CB_SenseRange->currentText().toStdString();
        numbertext = numbertext.substr(0,numbertext.find(" "));
        osci->Write(std::string(":sens:curr:rang ") + numbertext);
    }

    osci->Write(":sens:curr:prot " + QString::number(ui->SB_Compliance->value()).toStdString()
                            + "e-" + QString::number(3*ui->CB_Compliance_unit->currentIndex()).toStdString()); //current compliance
    osci->Write(":sens:curr:aper " + QString::number(ui->SB_measuretime->value()).toStdString());  //set measurement duration

    if(status != VI_SUCCESS)
        return;


    //measure at 5V in reverse:
    osci->Write(":sour:volt -5");
    osci->Write(":outp on");

    if(status != VI_SUCCESS)
    {
        running = false;
        ui->B_Keysight_IoverT->setText("Measure Current");
        return;
    }

    std::fstream f;
    std::stringstream sf("");
    static int findex = 1;
    do{
        f.close();
        sf.str("");
        sf << "IoverT_" << findex++ << ".dat";
        f.open(sf.str().c_str(), std::ios::in);
    }while(f.is_open());

    f.open(sf.str().c_str(), std::ios::out);
    f << "# DateTime; Current (in A); RMS of Current (in A);" << std::endl;

    int counter = 0;    //for reducing output to log

    while(running)
    {
        //wait for the SMU to reach the desired voltage:
        Sleep(1000);

        std::string answer;
        //extract the data from the answer string:
        double average = 0;
        double stabw   = 0;
        for(int i = 0; i < samples; ++i)
        {
            osci->Write(":meas:curr? (@1)");
            answer = osci->Read(14);
            std::stringstream si(answer);
            double vald;
            si >> vald;
            average += vald;
            stabw   += vald * vald;
        }
        average /= samples;
        stabw   /= samples;
        stabw   -= average * average;
        stabw    = sqrt(stabw);
        //std::cout << "Answer for Read at " << voltage << "V: " << average << " +/- " << stabw << std::endl;

        if(counter++ == 0)
        {
            std::stringstream sinfo("");
            sinfo << " current: " << average << "A done";
            logit(sinfo.str());
        }
        else if(counter == 15)
            counter = 0;

        QDateTime datetime = QDateTime::currentDateTime();
        f << datetime.toString("dd.MM.yy hh:mm:ss").toStdString() << "\t" << average << "\t" << stabw << std::endl;



        QApplication::processEvents();
    }

    f.close();

    std::stringstream logstream("");
    logstream << "Wrote measurement results to \"" << sf.str() << "\"";
    logit(logstream.str());

    //ramp down the voltage and turn off:
    osci->Write(":sour:volt 0");
    Sleep(3000);
    osci->Write(":outp off");

    running = false;
    ui->B_Keysight_IoverT->setText("Measure Current");

    logit("Done measureing Current over Time");


    return;
}

void MainWindow::on_CB_autorange_source_toggled(bool checked)
{
    ui->CB_SourceRange->setEnabled(!checked);
}

void MainWindow::on_CB_autorange_sense_toggled(bool checked)
{
    ui->CB_SenseRange->setEnabled(!checked);
}

/**
 * @brief MainWindow::on_pushButton_clicked method for generating hits with different delays,
 *              the readout is done separately and not in this method
 */
void MainWindow::on_pushButton_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    double time = ui->SB_InitialDelay->value()*1e-6;
    double delaystep = ui->SB_DelayStep->value()*1e-9;

    //set initial delay:
    std::stringstream s("");
    s << "trig2:delay " << time;

    std::string text = s.str();

    logit("write " + QString::number(text.length()).toStdString()
          + " characters: \"" + text + "\"");

    //turn on output:
    //osci->Write("output2 on");

    for(int i = 0; i < 260; ++i)
    {
        time += delaystep; //10e-9;

        std::stringstream s("");
        s << "trig2:delay " << time;

        std::string text = s.str();

        logit("write " + QString::number(text.length()).toStdString()
              + " characters: \"" + text + "\"");


        int writtenchars = osci->Write(text);

        logit(QString::number(writtenchars).toStdString() + " characters written");

        Sleep(30);
    }
    //osci->Write("output2 off");

}

void MainWindow::on_CB_IV_currentIndexChanged(int index)
{
    (void) index;
    if (ui->CB_IV->currentIndex() == 0)
        SetIVCurve();
    else
        SetVICurve();
}

void MainWindow::SetIVCurve()
{
    ui->CB_SweepStart->clear();
    ui->CB_SweepStart->addItem("V");
    ui->CB_SweepStart->addItem("mV");
    ui->CB_SweepStart->addItem("µV");
    ui->CB_SweepStart->addItem("nV");

    ui->CB_SweepStop->clear();
    ui->CB_SweepStop->addItem("V");
    ui->CB_SweepStop->addItem("mV");
    ui->CB_SweepStop->addItem("µV");
    ui->CB_SweepStop->addItem("nV");

    ui->CB_SweepStep->clear();
    ui->CB_SweepStep->addItem("V");
    ui->CB_SweepStep->addItem("mV");
    ui->CB_SweepStep->addItem("µV");
    ui->CB_SweepStep->addItem("nV");

    ui->CB_SourceRange->clear();
    ui->CB_SourceRange->addItem("200 V");
    ui->CB_SourceRange->addItem("20 V");
    ui->CB_SourceRange->addItem("2 V");
    ui->CB_SourceRange->addItem("200 mV");

    ui->CB_SenseRange->clear();
    ui->CB_SenseRange->addItem("10 A");
    ui->CB_SenseRange->addItem("3 A");
    ui->CB_SenseRange->addItem("1.5 A");
    ui->CB_SenseRange->addItem("1 A");
    ui->CB_SenseRange->addItem("100 mA");
    ui->CB_SenseRange->addItem("10 mA");
    ui->CB_SenseRange->addItem("1 mA");
    ui->CB_SenseRange->addItem("100 µA");
    ui->CB_SenseRange->addItem("10 µA");
    ui->CB_SenseRange->addItem("1 µA");
    ui->CB_SenseRange->addItem("100 nA");

    ui->CB_Compliance_unit->clear();
    ui->CB_Compliance_unit->addItem("A");
    ui->CB_Compliance_unit->addItem("mA");
    ui->CB_Compliance_unit->addItem("µA");
    ui->CB_Compliance_unit->addItem("nA");

}

void MainWindow::SetVICurve()
{
    ui->CB_SweepStart->clear();
    ui->CB_SweepStart->addItem("A");
    ui->CB_SweepStart->addItem("mA");
    ui->CB_SweepStart->addItem("µA");
    ui->CB_SweepStart->addItem("nA");

    ui->CB_SweepStop->clear();
    ui->CB_SweepStop->addItem("A");
    ui->CB_SweepStop->addItem("mA");
    ui->CB_SweepStop->addItem("µA");
    ui->CB_SweepStop->addItem("nA");

    ui->CB_SweepStep->clear();
    ui->CB_SweepStep->addItem("A");
    ui->CB_SweepStep->addItem("mA");
    ui->CB_SweepStep->addItem("µA");
    ui->CB_SweepStep->addItem("nA");

    ui->CB_SourceRange->clear();
    ui->CB_SourceRange->addItem("10 A");
    ui->CB_SourceRange->addItem("3 A");
    ui->CB_SourceRange->addItem("1.5 A");
    ui->CB_SourceRange->addItem("1 A");
    ui->CB_SourceRange->addItem("100 mA");
    ui->CB_SourceRange->addItem("10 mA");
    ui->CB_SourceRange->addItem("1 mA");
    ui->CB_SourceRange->addItem("100 µA");
    ui->CB_SourceRange->addItem("10 µA");
    ui->CB_SourceRange->addItem("1 µA");
    ui->CB_SourceRange->addItem("100 nA");

    ui->CB_SenseRange->clear();
    ui->CB_SenseRange->addItem("200 V");
    ui->CB_SenseRange->addItem("20 V");
    ui->CB_SenseRange->addItem("2 V");
    ui->CB_SenseRange->addItem("200 mV");

    ui->CB_Compliance_unit->clear();
    ui->CB_Compliance_unit->addItem("V");
    ui->CB_Compliance_unit->addItem("mV");
    ui->CB_Compliance_unit->addItem("µV");
    ui->CB_Compliance_unit->addItem("nV");
}


void MainWindow::on_CB_Chan1_clicked()
{
    if(!osci->is_open())
        return;

    if(ui->CB_Chan1->isChecked() && !ui->CB_Chan2->isChecked() && !ui->CB_Chan3->isChecked() && !ui->CB_Chan4->isChecked())
        osci->SetChannel(1);
}

void MainWindow::on_CB_Chan2_clicked()
{
    if(!osci->is_open())
        return;

    if(!ui->CB_Chan1->isChecked() && ui->CB_Chan2->isChecked() && !ui->CB_Chan3->isChecked() && !ui->CB_Chan4->isChecked())
        osci->SetChannel(2);
}

void MainWindow::on_CB_Chan3_clicked()
{
    if(!osci->is_open())
        return;

    if(!ui->CB_Chan1->isChecked() && !ui->CB_Chan2->isChecked() && ui->CB_Chan3->isChecked() && !ui->CB_Chan4->isChecked())
        osci->SetChannel(3);
}

void MainWindow::on_CB_Chan4_clicked()
{
    if(!osci->is_open())
        return;

    if(!ui->CB_Chan1->isChecked() && !ui->CB_Chan2->isChecked() && !ui->CB_Chan3->isChecked() && ui->CB_Chan4->isChecked())
        osci->SetChannel(4);
}

void MainWindow::on_B_StartStop_clicked()
{
    if(!osci->is_open())
        return;

    static bool running = false;

    if(running)
    {
        running = false;
        ui->B_StartStop->setText("Start");
        osci->SetAcquisitionState(false);
    }
    else
    {
        running = true;
        ui->B_StartStop->setText("Stop");
        osci->SetAcquisitionState(true);
    }
}

void MainWindow::on_SB_InitialDelay_valueChanged(double arg1)
{
    if(!osci->is_open() || !ui->CB_AutoUpdate->isChecked())
        return;

    std::stringstream s("");
    s << "trig2:delay " << arg1/1.e6;

    osci->Write(s.str());
}

void MainWindow::on_B_WfmDisplay_clicked()
{
    wfmdisplay->show();

    wfmdisplay->plotter.Draw();
}


void MainWindow::on_LE_TimeScale_returnPressed()
{
    if(!osci->is_open())
        return;

    std::stringstream s("");
    s << ui->LE_TimeScale->text().toStdString();
    double timescale;
    if(s >> timescale)
    {
        if(osci->SetDispTimeScale(timescale))
            logit(std::string("Set time scale to " + ui->LE_TimeScale->text().toStdString() + " s"));
    }
    else
        logit("Time format not recognised!");
}

void MainWindow::on_LE_TimeOffset_returnPressed()
{
    if(!osci->is_open())
        return;

    std::stringstream s("");
    s << ui->LE_TimeOffset->text().toStdString();
    double offset;
    if(s >> offset)
    {
        if(osci->SetDispTimeOffset(offset))
            logit("Set time offset to " + ui->LE_TimeOffset->text().toStdString() + " s");
    }
    else
        logit("Time Format not recognised!");
}

void MainWindow::on_LE_VoltScale_returnPressed()
{
    if(!osci->is_open())
        return;

    std::stringstream s("");
    s << ui->LE_VoltScale->text().toStdString();
    double vscale;
    if(s >> vscale)
    {
        if(osci->SetVoltageScale(ui->SB_Channel->value(), vscale))
        {
            std::stringstream s("");
            s << "Set voltage scale for channel " << ui->SB_Channel->value()
              << " to " << ui->LE_VoltScale->text().toStdString() << " V/div";
            logit(s.str());
        }
    }
    else
        logit("Number not recognised!");
}

void MainWindow::on_LE_VoltOffset_returnPressed()
{
    if(!osci->is_open())
        return;

    std::stringstream s("");
    s << ui->LE_VoltOffset->text().toStdString();
    double voffset;
    if(s >> voffset)
    {
        if(osci->SetVoltageOffset(ui->SB_Channel->value(), voffset))
        {
            std::stringstream s("");
            s << "Set voltage offset for channel " << ui->SB_Channel->value()
              << " to " << ui->LE_VoltOffset->text().toStdString() << " div";
            logit(s.str());
        }
    }
    else
        logit("Number not recognised!");
}

void MainWindow::on_SB_Channel_valueChanged(int arg1)
{
    if(!osci->is_open())
        return;

    double scale  = osci->GetVoltageScale(arg1);
    double offset = osci->GetVoltageOffset(arg1);

    ui->LE_VoltScale->setText(QString::number(scale));
    ui->LE_VoltOffset->setText(QString::number(offset));
}

void MainWindow::on_B_ResetHist_clicked()
{
    if(!osci->is_open())
        return;

    osci->Write("HIStogram:COUNt RESET");

    logit("Histogram Reset");
}

void monitorworker(VISADevice* osci, bool* running, int oversampling)
{
    if(running == nullptr)
        return;

    std::fstream f;

    std::string filename = "";
    if(*running)
    {
        int index = 0;
        do
        {
            f.close();
            std::stringstream s("");
            s << "Current_Trigger_" << ++index << ".dat";
            f.open(s.str().c_str(), std::ios::in);
            filename = s.str();
        }while(f.is_open());

        f.close();
        f.open(filename.c_str(), std::ios::out | std::ios::app);

        f << "# hours, minutes, seconds; DigIO; current (in A)" << std::endl;
    }

    std::stringstream sout("");

    int counter = 0;
    int measurecounter = 0;
    while(*running)
    {
        if(measurecounter >= oversampling)
        {
            osci->Write(":system:time?;:source:dig:data?;:fetch:array:curr?");
            osci->Write(":init.acq");  //":init");  //source voltage is not used and hence not measured
            measurecounter = 0;
        }
        else if(measurecounter == 0)
            osci->Write(":system:time?;:source:dig:data?;:fetch:array:curr?");
        else
            osci->Write(":system:time?;:source:dig:data?");

        ++measurecounter;

        std::string answer = osci->Read(47);
        sout << answer;
        ++counter;
        if(counter >= 100)
        {
            f << sout.str();
            sout.str("");
            counter = 0;
        }
    }

    f << sout.str();
    f.flush();
    f.close();
}

void MainWindow::on_B_Keysight_IwithTrig_clicked()
{
    if(!osci->is_open())
        return;

    static bool running = false;
    static std::thread* worker = nullptr;

    if(running)
    {
        running = false;
        std::cout << "stop monitor" << std::endl;


        if(worker != nullptr)
        {
            if(worker->joinable())
                worker->join();

            delete worker;
            worker = nullptr;
        }

        ui->B_Keysight_IwithTrig->setText("Monitor Current/Trigger");
        return;
    }

    running = true;
    ui->B_Keysight_IwithTrig->setText("Stop Measurement");
    std::cout << "start monitor" << std::endl;

    worker = new std::thread(monitorworker, osci, &running, ui->SB_TriggerOversampling->value());

}

void MainWindow::on_B_Keysight_IwithTrig_2_clicked()
{
    logit("not implemented");
    //the idea is to monitor the digital input of the SMU for the trigger,
    //  then wait for the falling edge of it with high precision (just read out the digital inputs)
    //  and only then start measuring the current all the time with the digital inputs at the same
    //  low frequency. In case the trigger goes high again, go back to waiting for the falling edge
    //  of it:
    //
    //   1. wait for trigger rising edge
    //   2. wait for trigger falling edge
    //   3. measure current and trigger with a frequency limited by the current measurement
    //      -> in case of trigger == 1, go back to 2
}

void MainWindow::on_B_TimeToSMU_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    QDateTime time = QDateTime::currentDateTime();
    std::stringstream s("");
    s << ":system:date " << time.date().year() << "," << time.date().month() << ","
                         << time.date().day();

    osci->Write(s.str());
    s.str("");
    s << ":system:time " << time.time().hour() << "," << time.time().minute() << ","
      << time.time().second();
    osci->Write(s.str());

    logit("Updated time on SMU at " + time.toString().toStdString());
}

void MainWindow::on_B_PowerReg_MeasurePnt_clicked()
{
    static std::string filename = "";

    std::fstream f;

    if(filename.compare(ui->LE_PowerReg_Filename->text().toStdString()) != 0)
    {
        filename = ui->LE_PowerReg_Filename->text().toStdString();
        f.open(filename.c_str(), std::ios::out | std::ios::app);
        f << "# VDAC Setting (8bit); Output Voltage (V)" << std::endl;
    }
    else
        f.open(filename.c_str(), std::ios::out | std::ios::app);

    double value = 0;
    for(int i = 0; i < 10; ++i)
        value += osci->QueryF(ui->LE_PowerReg_Query->text().toStdString());

    value = value / 10.;

    f << ui->SB_PowerRegVDAC->value() << "\t" << value << std::endl;

    ui->SB_PowerRegVDAC->setValue(ui->SB_PowerRegVDAC->value() + 1);

    f.close();

    std::cout << "measured " << value << " V" << std::endl;
}
