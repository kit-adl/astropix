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

#include "lasercontrol.h"
#include "ui_lasercontrol.h"

LaserControl::LaserControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LaserControl), nexys(nullptr), fastro(nullptr)
{
    ui->setupUi(this);

    //rescale the window removing the design space on the right:
    QRect window = this->geometry();
    this->setGeometry(window.left(), window.top()+27, 370, window.height());
    //width = 370;

    ui->GB_MeasurementInfo->setGeometry(42,240,261,231);
    ui->GB_MeasurementInfo->setVisible(false);
    ui->GB_3D_MeasurementInfo->setGeometry(42,220,261,271);
    ui->GB_3D_MeasurementInfo->setVisible(false);
    ui->GB_RangeImport->setGeometry(42,270,261,161);
    ui->GB_RangeImport->setVisible(false);

    //Color palettes for Laser Setup:
    palette_button_gray = palette();
    palette_button_gray.setColor(QPalette::Active, QPalette::Button, QColor(184,184,184));
    //palette_button_gray.setColor(QPalette::Inactive, QPalette::Button, QColor(184,184,184));
    palette_button_green = palette();
    palette_button_green.setColor(QPalette::Active, QPalette::Button, QColor(0,255,0));
    palette_button_green.setColor(QPalette::Inactive, QPalette::Button, QColor(0,255,0));
    palette_button_green.setColor(QPalette::Active, QPalette::Base, QColor(0,255,0));
    palette_button_green.setColor(QPalette::Inactive, QPalette::Base, QColor(0,255,0));
    palette_button_red = palette();
    palette_button_red.setColor(QPalette::Active, QPalette::Button, QColor(255,0,0));
    palette_button_red.setColor(QPalette::Inactive, QPalette::Button, QColor(255,0,0));
    palette_button_red.setColor(QPalette::Active, QPalette::Base, QColor(184,184,184));
    //palette_button_red.setColor(QPalette::Active, QPalette::Base, QColor(184,184,184));

    //Laser Setup Buttons with Color:
    ui->B_LS_MotorX->setFlat(true);
    ui->B_LS_MotorX->setAutoFillBackground(true);
    ui->B_LS_MotorX->setPalette(palette_button_gray);

    ui->B_LS_MotorY->setFlat(true);
    ui->B_LS_MotorY->setAutoFillBackground(true);
    ui->B_LS_MotorY->setPalette(palette_button_gray);

    ui->B_LS_MotorZ->setFlat(true);
    ui->B_LS_MotorZ->setAutoFillBackground(true);
    ui->B_LS_MotorZ->setPalette(palette_button_gray);

    ui->SB_Laser_xpos->setAutoFillBackground(true);
    ui->SB_Laser_ypos->setAutoFillBackground(true);
    ui->SB_Laser_zpos->setAutoFillBackground(true);

    ui->B_LS_InitX->setAutoFillBackground(true);
    ui->B_LS_InitX->setPalette(palette_button_gray);
    ui->B_LS_InitY->setAutoFillBackground(true);
    ui->B_LS_InitY->setPalette(palette_button_gray);
    ui->B_LS_InitZ->setAutoFillBackground(true);
    ui->B_LS_InitZ->setPalette(palette_button_gray);

    //ui->B_LS_LaserOn->setAutoFillBackground(true);
    ui->B_LS_LaserOn->setPalette(palette_button_red);

    ui->SB_LS_Laserfreq->setAutoFillBackground(true);
    ui->SB_LS_Lasertune->setAutoFillBackground(true);

    osci = new VISADevice();
}

LaserControl::~LaserControl()
{
    delete ui;
    nexys = nullptr;
    fastro = nullptr;
}

bool LaserControl::VISAWrite(std::string data)
{
    if(!osci->is_open())
        return false;

    return osci->Write(data) > 0;
}

void LaserControl::logit(std::string logstream)
{
    std::cout << logstream << std::endl;
    QString buffer= QString::fromStdString(logstream);
    ui->Log->append(buffer);
    QTextCursor c =  ui->Log->textCursor();
    c.movePosition(QTextCursor::End);
    ui->Log->setTextCursor(c);
}

NexysIO *LaserControl::GetNexys()
{
    return nexys;
}

void LaserControl::SetNexys(NexysIO *newnexys)
{
    nexys = newnexys;
}

FastReadout *LaserControl::GetFastRO()
{
    return fastro;
}

void LaserControl::SetFastRO(FastReadout *newfastro)
{
    fastro = newfastro;
}

void LaserControl::ResetFastRO()
{
    if(nexys != nullptr && !nexys->is_open())
        return;

    std::vector<byte> reset;

    reset.push_back(byte(fastro->GetConfiguration(true)));
    reset.push_back(byte(fastro->GetConfiguration(false)));

    nexys->Write(FastReadout::FPGAAddress, reset, true, 4);
    nexys->Flush();
}

void LaserControl::ProcessEvents()
{
    QApplication::processEvents();
}

void LaserControl::setWaiter(WaitFunction newwaiter)
{
    this->waiter = newwaiter;
}

void LaserControl::on_B_LS_AutoConnect_clicked()
{
    logit("Searching ports ...");

    if(setup.IdentifyPorts())
    {
        logit("Laser Setup Connected.");
        ui->B_LS_OpenLaser->setEnabled(false);
        ui->B_LS_OpenStages->setEnabled(false);
        ui->B_LS_AutoConnect->setEnabled(false);
        ui->CB_Laser_TriggerEdge->setCurrentIndex(setup.GetTriggerEdge());
        ui->CB_Laser_TriggerSource->setCurrentIndex(setup.GetTriggerSource());
        if(ui->CB_Laser_TriggerSource->currentText().toStdString().compare("ext. Adj.") == 0)
        {
            ui->SB_Laser_TriggerLevel->setEnabled(true);
            ui->SB_Laser_TriggerLevel->setValue(setup.GetTriggerLevel());
        }
        else
            ui->SB_Laser_TriggerLevel->setEnabled(false);
        ui->SB_LS_Lasertune->setValue(setup.GetTuneValue());
        ui->SB_LS_Laserfreq->setValue(setup.GetTriggerFrequency()/1000.);
    }
    else
        logit("Error: Failed to connect to Laser Setup.");
}

void LaserControl::on_B_LS_SearchPorts_clicked()
{
    std::vector<std::string> liste = setup.GetPortList();

    ui->CB_LS_COMPorts->clear();
    for(auto& it : liste)
        ui->CB_LS_COMPorts->addItem(QString(it.c_str()));
}

void LaserControl::on_B_LS_OpenLaser_clicked()
{
    if(ui->CB_LS_COMPorts->currentText() == "")
        return;

    setup.ConfigureLaserPort(ui->CB_LS_COMPorts->currentText().toStdString());
    if(setup.OpenLaserPort())
    {
        logit("Connection to Laser Driver opened.");
        ui->B_LS_OpenLaser->setEnabled(false);
        ui->B_LS_AutoConnect->setEnabled(false);

        ui->CB_Laser_TriggerEdge->setCurrentIndex(setup.GetTriggerEdge());
        ui->CB_Laser_TriggerSource->setCurrentIndex(setup.GetTriggerSource());
        if(ui->CB_Laser_TriggerSource->currentText().toStdString().compare("ext. Adj.") == 0)
        {
            ui->SB_Laser_TriggerLevel->setEnabled(true);
            ui->SB_Laser_TriggerLevel->setValue(setup.GetTriggerLevel());
        }
        ui->SB_LS_Lasertune->setValue(setup.GetTuneValue());
        ui->SB_LS_Laserfreq->setValue(setup.GetTriggerFrequency()/1000.);
    }
    else
        logit("Error opening port to Laser Driver.");
}

void LaserControl::on_B_LS_CloseLaser_clicked()
{
    setup.CloseLaserPort();
    ui->B_LS_OpenLaser->setEnabled(true);
    if(ui->B_LS_OpenStages->isEnabled())
        ui->B_LS_AutoConnect->setEnabled(true);

    ui->SB_Laser_TriggerLevel->setEnabled(false);

    logit("Connection to Laser Driver Closed.");
}

void LaserControl::on_B_LS_OpenStages_clicked()
{
    if(ui->CB_LS_COMPorts->currentText() == "")
        return;

    setup.ConfigureStagePort(ui->CB_LS_COMPorts->currentText().toStdString());
    if(setup.OpenStagePort())
    {
        logit("Connection to Stage Drivers opened.");
        ui->B_LS_OpenStages->setEnabled(false);
        ui->B_LS_AutoConnect->setEnabled(false);
    }
    else
        logit("Error opening port to Stage Drivers.");
}

void LaserControl::on_B_LS_CloseStages_clicked()
{
    setup.CloseStagePort();
    ui->B_LS_OpenStages->setEnabled(true);
    if(ui->B_LS_OpenLaser->isEnabled())
        ui->B_LS_AutoConnect->setEnabled(true);

    logit("Connection to Stage Drivers Closed.");
}

void LaserControl::on_B_LS_LaserOn_clicked()
{
    if(!setup.Laser_is_open())
        return;

    static bool running = false;

    if(ui->B_LS_LaserOn->text().toStdString().compare("Laser On") == 0)
    {
        setup.ActivateLaser(false);
        ui->B_LS_LaserOn->setFlat(false);
        ui->B_LS_LaserOn->setText("Laser Off");
        ui->B_LS_LaserOn->setAutoFillBackground(false);
        ui->B_LS_LaserOn->setPalette(palette_button_gray);
        logit("Laser turned off.");

        running = false;
    }
    else
    {
        setup.ActivateLaser(true);
        if(setup.GetLaserState())
        {
            ui->B_LS_LaserOn->setFlat(true);
            ui->B_LS_LaserOn->setText("Laser On");
            ui->B_LS_LaserOn->setAutoFillBackground(true);
             ui->B_LS_LaserOn->setPalette(palette_button_red);
            logit("Laser activated.");

            running = true;

            //monitor whether the laser gets turned off:
            int counter = 0;
            while(running)
            {
                if(counter == 200)
                {
                    if(!setup.GetLaserState())
                        on_B_LS_LaserOn_clicked();
                    counter = 0;
                }
                else
                    ++counter;
                Timing::Sleep(5);
                QApplication::processEvents();
            }
        }
        else
            logit("Error: Interlock open! Could not turn on Laser!");
    }
}

void LaserControl::on_SB_LS_Laserfreq_editingFinished()
{
    if(!setup.Laser_is_open())
        return;

    static bool running = false;

    setup.SetTuneValue(ui->SB_LS_Lasertune->value());

    if(running)
        return;

    running = true;
    ui->SB_LS_Lasertune->setPalette(palette_button_green);
    ui->SB_LS_Lasertune->update();
    int count = 0;
    while(++count < 0.5/0.005)
    {
        Timing::Sleep(5);
        QApplication::processEvents();
    }

    ui->SB_LS_Lasertune->setPalette(palette_button_gray);
    running = false;
}

void LaserControl::on_CB_Laser_TriggerEdge_currentIndexChanged(const QString &arg1)
{
    if(!setup.Laser_is_open())
        return;

    int newsetting = -1;

    if(arg1.toStdString().compare("Rising Edge") == 0)
        newsetting = LaserSetup::rising;
    else if(arg1.toStdString().compare("Falling Edge") == 0)
        newsetting = LaserSetup::falling;

    if(setup.SetTriggerEdge(static_cast<unsigned int>(newsetting)))
        logit("Laser: Trigger Edge changed");
    else
        logit("Laser Error: failed to set trigger edge");

    return;
}

void LaserControl::on_CB_Laser_TriggerSource_currentIndexChanged(const QString &arg1)
{
    if(!setup.Laser_is_open())
        return;

    int newsetting = -1;

    if(arg1.toStdString().compare("internal") == 0)
    {
        newsetting = LaserSetup::internal;
        ui->SB_Laser_TriggerLevel->setEnabled(false);
    }
    else if(arg1.toStdString().compare("ext. Adj.") == 0)
    {
        newsetting = LaserSetup::extAdj;
        ui->SB_Laser_TriggerLevel->setEnabled(true);
        ui->SB_Laser_TriggerLevel->setValue(setup.GetTriggerLevel());
    }
    else if(arg1.toStdString().compare("ext. TTL") == 0)
    {
        newsetting = LaserSetup::extTTL;
        ui->SB_Laser_TriggerLevel->setEnabled(false);
    }

    if(setup.SetTriggerSource(static_cast<unsigned int>(newsetting)))
        logit("Laser: Trigger Source Set");
    else
        logit("Laser Error: failed to set trigger source");

    return;
}

void LaserControl::on_SB_Laser_TriggerLevel_editingFinished()
{
    if(!setup.Laser_is_open())
        return;

    if(setup.SetTriggerLevel(ui->SB_Laser_TriggerLevel->value()))
        logit("Laser: Trigger Level Changed");
    else
        logit("Laser Error: failed to change Trigger Level");
}

void LaserControl::on_SB_Laser_xpos_editingFinished()
{
    if(!setup.Stages_is_open() || ui->B_LS_InitX->isEnabled()
             || ui->B_LS_MotorX->text().toStdString().compare("Off") == 0)
        return;

    static bool running = false;

    setup.MoveAbsolute(LaserSetup::x, ui->SB_Laser_xpos->value(), 0.1);


    //execute the waiting only once:
    if(running)
        return;

    running = true;

    //show movement with gray background:
    ui->SB_Laser_xpos->setPalette(palette_button_red);
    while(!setup.AxisOnTarget(LaserSetup::x))
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //show finishing moving with green background for 0.5 seconds
    ui->SB_Laser_xpos->setPalette(palette_button_green);
    int count = 0;
    while(++count < 0.5/0.005)
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //go back to default background;
    ui->SB_Laser_xpos->setPalette(palette_button_gray);

    running = false;
}

void LaserControl::on_SB_Laser_ypos_editingFinished()
{
    if(!setup.Stages_is_open() || ui->B_LS_InitY->isEnabled()
            || ui->B_LS_MotorY->text().toStdString().compare("Off") == 0)
        return;

    static bool running = false;

    setup.MoveAbsolute(LaserSetup::y, ui->SB_Laser_ypos->value(), 0.1);


    //execute the waiting for end of moving only once:
    if(running)
        return;

    running = true;

    //show movement with gray background:
    ui->SB_Laser_ypos->setPalette(palette_button_red);
    while(!setup.AxisOnTarget(LaserSetup::y))
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //show finishing moving with green background for 0.5 seconds
    ui->SB_Laser_ypos->setPalette(palette_button_green);
    int count = 0;
    while(++count < 0.5/0.005)
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //go back to default background;
    ui->SB_Laser_ypos->setPalette(palette_button_gray);

    running = false;
}

void LaserControl::on_SB_Laser_zpos_editingFinished()
{
    if(!setup.Stages_is_open() || ui->B_LS_InitZ->isEnabled()
            || ui->B_LS_MotorZ->text().toStdString().compare("Off") == 0)
        return;

    static bool running = false;

    setup.MoveAbsolute(LaserSetup::z, ui->SB_Laser_zpos->value(), 0.1);


    //execute the waiting for end of moving only once:
    if(running)
        return;

    running = true;

    //show movement with gray background:
    ui->SB_Laser_zpos->setPalette(palette_button_red);
    while(!setup.AxisOnTarget(LaserSetup::z))
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //show finishing moving with green background for 0.5 seconds
    ui->SB_Laser_zpos->setPalette(palette_button_green);
    int count = 0;
    while(++count < 0.5/0.005)
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    //go back to default background;
    ui->SB_Laser_zpos->setPalette(palette_button_gray);

    running = false;
}

void LaserControl::on_B_LS_MotorX_clicked()
{
    if(!setup.Stages_is_open())
        return;

    if(ui->B_LS_MotorX->text().toStdString().compare("Off") == 0)
    {
        ui->B_LS_MotorX->setText("On");
        ui->B_LS_MotorX->setPalette(palette_button_red);
        ui->B_LS_InitX->setEnabled(true);
        ui->CB_Stages_X_Ref->setEnabled(true);
        setup.TurnOnMotor(LaserSetup::x);
        logit("Laser Setup: Motor X turned on.");
    }
    else
    {
        ui->B_LS_MotorX->setText("Off");
        ui->B_LS_MotorX->setPalette(palette_button_gray);
        ui->B_LS_InitX->setEnabled(false);
        setup.TurnOffMotor(LaserSetup::x);
        logit("Laser Setup: Motor X turned off.");
    }
}

void LaserControl::on_B_LS_MotorY_clicked()
{
    if(!setup.Stages_is_open())
        return;

    if(ui->B_LS_MotorY->text().toStdString().compare("Off") == 0)
    {
        ui->B_LS_MotorY->setText("On");
        ui->B_LS_MotorY->setPalette(palette_button_red);
        ui->B_LS_InitY->setEnabled(true);
        ui->CB_Stages_Y_Ref->setEnabled(true);
        setup.TurnOnMotor(LaserSetup::y);
        logit("Laser Setup: Motor Y turned on.");
    }
    else
    {
        ui->B_LS_MotorY->setText("Off");
        ui->B_LS_MotorY->setPalette(palette_button_gray);
        ui->B_LS_InitY->setEnabled(false);
        setup.TurnOffMotor(LaserSetup::y);
        logit("Laser Setup: Motor Y turned off.");
    }
}

void LaserControl::on_B_LS_MotorZ_clicked()
{
    if(!setup.Stages_is_open())
        return;

    if(ui->B_LS_MotorZ->text().toStdString().compare("Off") == 0)
    {
        ui->B_LS_MotorZ->setText("On");
        ui->B_LS_MotorZ->setPalette(palette_button_red);
        ui->B_LS_InitZ->setEnabled(true);
        ui->CB_Stages_Z_Ref->setEnabled(true);
        setup.TurnOnMotor(LaserSetup::z);
        logit("Laser Setup: Motor Z turned on.");
    }
    else
    {
        ui->B_LS_MotorZ->setText("Off");
        ui->B_LS_MotorZ->setPalette(palette_button_gray);
        ui->B_LS_InitZ->setEnabled(false);
        setup.TurnOffMotor(LaserSetup::z);
        logit("Laser Setup: Motor Z turned off.");
    }
}

void LaserControl::on_B_LS_InitX_clicked()
{
    static bool running = false;
    if(!setup.Stages_is_open() || running)
        return;

    running = true;

    logit("Initialising X axis");

    ui->B_LS_InitX->setFlat(true);
    ui->B_LS_InitX->update();

    std::string reference = ui->CB_Stages_X_Ref->currentText().toStdString();
    if(reference.find("RefPos") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::x, LaserSetup::refPos);
    else if(reference.find("NegLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::x, LaserSetup::negLimit);
    else if(reference.find("PosLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::x, LaserSetup::posLimit);
    setup.WaitforAxes(LaserSetup::x);

    ui->SB_Laser_xpos->setValue(setup.GetPosition(LaserSetup::x, true));

    ui->SB_Laser_xpos->setMinimum(setup.GetMinPosition(LaserSetup::x,true));
    ui->SB_Laser_xpos->setMaximum(setup.GetMaxPosition(LaserSetup::x,true));

    logit("Initialisation done");
    ui->B_LS_InitX->setEnabled(false);
    ui->B_LS_InitX->setFlat(false);
    ui->CB_Stages_X_Ref->setEnabled(false);

    running = false;
}

void LaserControl::on_B_LS_InitY_clicked()
{
    static bool running = false;
    if(!setup.Stages_is_open() || running)
        return;

    running = true;

    logit("Initialising Y axis");

    ui->B_LS_InitY->setFlat(true);
    ui->B_LS_InitY->update();

    std::string reference = ui->CB_Stages_Y_Ref->currentText().toStdString();
    if(reference.find("RefPos") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::y, LaserSetup::refPos);
    else if(reference.find("NegLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::y, LaserSetup::negLimit);
    else if(reference.find("PosLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::y, LaserSetup::posLimit);
    setup.WaitforAxes(LaserSetup::y);

    ui->SB_Laser_ypos->setValue(setup.GetPosition(LaserSetup::y, true));

    ui->SB_Laser_ypos->setMinimum(setup.GetMinPosition(LaserSetup::y,true));
    ui->SB_Laser_ypos->setMaximum(setup.GetMaxPosition(LaserSetup::y,true));

    logit("Initialisation done");
    ui->B_LS_InitY->setEnabled(false);
    ui->B_LS_InitY->setFlat(false);
    ui->CB_Stages_Y_Ref->setEnabled(false);

    running = false;
}

void LaserControl::on_B_LS_InitZ_clicked()
{
    static bool running = false;
    if(!setup.Stages_is_open() || running)
        return;

    running = true;

    logit("Initialising Z axis");

    ui->B_LS_InitZ->setFlat(true);
    ui->B_LS_InitZ->update();

    std::string reference = ui->CB_Stages_Z_Ref->currentText().toStdString();
    if(reference.find("RefPos") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::z, LaserSetup::refPos);
    else if(reference.find("NegLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::z, LaserSetup::negLimit);
    else if(reference.find("PosLimit") != std::string::npos)
        setup.ReferenceAxis(LaserSetup::z, LaserSetup::posLimit);
    setup.WaitforAxes(LaserSetup::z);

    ui->SB_Laser_zpos->setValue(setup.GetPosition(LaserSetup::z, true));

    ui->SB_Laser_zpos->setMinimum(setup.GetMinPosition(LaserSetup::z,true));
    ui->SB_Laser_zpos->setMaximum(setup.GetMaxPosition(LaserSetup::z,true));

    logit("Initialisation done");
    ui->B_LS_InitZ->setEnabled(false);
    ui->B_LS_InitZ->setFlat(false);
    ui->CB_Stages_Z_Ref->setEnabled(false);

    running = false;
}

void LaserControl::SetSpinBoxMinMax(QDoubleSpinBox* spinstart, QDoubleSpinBox* spinstop, double min, double max)
{
    if(spinstart != nullptr)
    {
        spinstart->setMinimum(min);
        spinstart->setMaximum(max);
        if(spinstart->value() > max)
            spinstart->setValue(max);
        else if(spinstart->value() < min)
            spinstart->setValue(min);
    }
    if(spinstop != nullptr)
    {
        spinstop->setMinimum(min);
        spinstop->setMaximum(max);
        if(spinstop->value() > max)
            spinstop->setValue(max);
        else if(spinstop->value() < min)
            spinstop->setValue(min);
    }
}

void LaserControl::on_CB_LS_ScanX_currentIndexChanged(int index)
{
    int x = index;
    int y = ui->CB_LS_ScanY->currentIndex();
    int z = ui->CB_LS_ScanZ->currentIndex();

    double startx = ui->SB_LS_Scan_StartX->value();
    double stopx  = ui->SB_LS_Scan_StopX->value();
    double stepx  = ui->SB_LS_Scan_StepX->value();

    double starty = ui->SB_LS_Scan_StartY->value();
    double stopy  = ui->SB_LS_Scan_StopY->value();
    double stepy  = ui->SB_LS_Scan_StepY->value();

    SetSpinBoxMinMax(ui->SB_LS_Scan_StartX, ui->SB_LS_Scan_StopX,
                     setup.GetMinPosition(3-x), setup.GetMaxPosition(3-x));

    if(y == x)
    {
        y = 3 - x - z;
        ui->CB_LS_ScanY->setCurrentIndex(y);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartY, ui->SB_LS_Scan_StopY,
                         setup.GetMinPosition(3-y), setup.GetMaxPosition(3-y));

        ui->SB_LS_Scan_StartX->setValue(starty);
        ui->SB_LS_Scan_StopX->setValue(stopy);
        ui->SB_LS_Scan_StepX->setValue(stepy);
        ui->SB_LS_Scan_StartY->setValue(startx);
        ui->SB_LS_Scan_StopY->setValue(stopx);
        ui->SB_LS_Scan_StepY->setValue(stepx);
    }
    if(z == x)
    {
        z = 3 - x - y;
        ui->CB_LS_ScanZ->setCurrentIndex(z);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartZ, nullptr,
                         setup.GetMinPosition(3-z), setup.GetMaxPosition(3-z));
    }
}

void LaserControl::on_CB_LS_ScanY_currentIndexChanged(int index)
{
    int x = ui->CB_LS_ScanX->currentIndex();
    int y = index;
    int z = ui->CB_LS_ScanZ->currentIndex();

    double startx = ui->SB_LS_Scan_StartX->value();
    double stopx  = ui->SB_LS_Scan_StopX->value();
    double stepx  = ui->SB_LS_Scan_StepX->value();

    double starty = ui->SB_LS_Scan_StartY->value();
    double stopy  = ui->SB_LS_Scan_StopY->value();
    double stepy  = ui->SB_LS_Scan_StepY->value();

    SetSpinBoxMinMax(ui->SB_LS_Scan_StartY, ui->SB_LS_Scan_StopY,
                     setup.GetMinPosition(3-y), setup.GetMaxPosition(3-y));

    if(x == y)
    {
        x = 3 - y - z;
        ui->CB_LS_ScanX->setCurrentIndex(x);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartX, ui->SB_LS_Scan_StopX,
                         setup.GetMinPosition(3-x), setup.GetMaxPosition(3-x));

        ui->SB_LS_Scan_StartX->setValue(starty);
        ui->SB_LS_Scan_StopX->setValue(stopy);
        ui->SB_LS_Scan_StepX->setValue(stepy);
        ui->SB_LS_Scan_StartY->setValue(startx);
        ui->SB_LS_Scan_StopY->setValue(stopx);
        ui->SB_LS_Scan_StepY->setValue(stepx);
    }
    if(z == y)
    {
        z = 3 - x - y;
        ui->CB_LS_ScanZ->setCurrentIndex(z);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartZ, nullptr,
                         setup.GetMinPosition(3-z), setup.GetMaxPosition(3-z));
    }
}

void LaserControl::on_CB_LS_ScanZ_currentIndexChanged(int index)
{
    int x = ui->CB_LS_ScanX->currentIndex();
    int y = ui->CB_LS_ScanY->currentIndex();
    int z = index;

    SetSpinBoxMinMax(ui->SB_LS_Scan_StartZ, nullptr,
                     setup.GetMinPosition(3-z), setup.GetMaxPosition(3-z));

    if(x == z)
    {
        x = 3 - y - z;
        ui->CB_LS_ScanX->setCurrentIndex(x);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartX, ui->SB_LS_Scan_StopX,
                         setup.GetMinPosition(3-x), setup.GetMaxPosition(3-x));
    }
    if(y == z)
    {
        y = 3 - x - z;
        ui->CB_LS_ScanY->setCurrentIndex(y);
        SetSpinBoxMinMax(ui->SB_LS_Scan_StartY, ui->SB_LS_Scan_StopY,
                         setup.GetMinPosition(3-y), setup.GetMaxPosition(3-y));
    }
}

double fastrotest(double x, double y, double z, void* mainwindow)
{
    LaserControl* mw = static_cast<LaserControl*>(mainwindow);

    if(mw->nexys == nullptr || !mw->nexys->is_open() || mw->fastro == nullptr)
        return -1;


    const int timeperpoint = 500; //time in ms per measurement point
    const int numbytes = 12240;

    std::string sout = "";
    int soutcounter  = 0;
    int counter      = 0;
    bool decode      = mw->fastro->GetFileReadDecode();
    bool datamux     = mw->fastro->GetDataMuxEnable();

    std::fstream f;
    if(!decode)
        f.open(mw->archivename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    else
        f.open(mw->archivename.c_str(), std::ios::out | std::ios::app);

    if(!f.is_open())
        return -1;

    static std::string filename = mw->archivename;
    if(mw->archivename != filename)
    {
        filename = mw->archivename;
        f << Dataset::GetStringHeader(true) << std::endl;
    }

    f << "# Position: (" << x << "|" << y << "|" << z << ")" << std::endl;

    std::vector<Dataset> data;

    mw->ResetFastRO(); //clear fast readout FIFO

    Timing::TimePoint start = Timing::GetTimeNow();

    while(Timing::TimesToIntervalMS(start, Timing::GetTimeNow()) < timeperpoint)
    {
        std::string answer = mw->nexys->Read(NexysIO::FPGA_READOUT_FIFO, numbytes);

        if(decode)
        {
            answer = FastReadout::RemoveEmptyData(answer);
            data = FastReadout::DecodeMany(datamux, answer);
            counter += data.size();

            for(auto& it : data)
                sout += it.ToString(true) + "\n";
            soutcounter += data.size();
        }
        else
        {
            answer = FastReadout::RemoveEmptyData(answer);
            int entries = answer.length() / 8;
            counter += entries;

            sout += answer;
            soutcounter += entries;
        }

        if(soutcounter >= 250)
        {
            f << sout;
            f.flush();
            sout = "";
            soutcounter = 0;
        }
    }

    f << sout;
    f.flush();
    f.close();

    return counter;
}

double testfunclc(double x, double y, double z, void* mainwindow)
{
    LaserControl* mw = static_cast<LaserControl*>(mainwindow);

    std::stringstream s("");
    s << "Current Position: " << x << " | " << y << " | " << z;
    mw->logit(s.str());

    static double timestep = 0;
    static double yoffset  = 0;
    static double ymult    = 1;
    static double channeloffset = 0;

    static zip_file archive;
    if(archive.get_filename().compare(mw->archivename) != 0)
        archive.reset();
    //const std::string archivename = "Scan.zip";

    if(!mw->osci->is_open())
    {
        //std::vector<std::string> devices = mw->osci->ScanForDevices();
        //if(devices.size() > 0)
            if(mw->osci->open(0))
            {
                std::cout << "Could not open Oscilloscope communication!" << std::endl;
                //return -1;
            }

        mw->osci->SetChannel(1);
        mw->osci->ReadWaveFormSetup(false);

        std::fstream f;
        f.open(mw->archivename.c_str(), std::ios::in | std::ios::binary);
        if(f.is_open())
        {
            f.close();
            archive.load(mw->archivename);
        }
    }


    Measurement measure;

    measure.setName(mw->ui->LE_MeasureTitle->text().toStdString()); //"LaserProfile");
    measure.setMeasurementDevice(mw->ui->LE_MeasureDevice->text().toStdString()); //"904nm Laser");
    measure.setSample(mw->ui->LE_MeasureSample->text().toStdString()); //"LFoundry ChipB");
    measure.setTriggerA(mw->ui->LE_MeasureTrigInfo->text().toStdString()); //"posedge @108mV");


    std::stringstream s2("");
    s2 << "x = " << x << "; y = " << y;
    measure.setSample(s2.str());

    double signalheight = 0;
    const int numsignals = 20;

    for(int i = 0; i < numsignals; ++i)
    {
        std::cout << "i = " << i << std::endl;
        //enable the measurement:
        std::string wfmt = mw->osci->ReadWaveForm(!mw->osci->GetWaveFormSetupStatus(), mw->waiter, 5);
        double timescale = mw->osci->GetTimeScale("");
        std::vector<double> waveform = mw->osci->DecodeWaveFormY(wfmt);

        Event* evt = measure.addEvent("Wfm", "");

        WaveForm* wfm = evt->addWaveForm("AmpOut");
        wfm->setBytesPerPoint(1);
        wfm->setBandwidth("200MHz");
        wfm->setTimestep(timestep);
        wfm->setYOffset(yoffset);
        wfm->setScale(ymult);
        wfm->setChannelOffset(channeloffset);
        if(wfmt.length() > 0) // no timeout occurred
            wfm->addBinaryData(wfmt);
        else
            wfm->addBinaryData("00000000000"); //some constant data

        //std::cout << "wfm complete: " << ((wfm->informationComplete())?"1":"0") << std::endl
        //          << "wfm length:   " << wfm->getBinaryLength() << std::endl;

        double time = 0;
        for(auto& it : waveform)
        {
            wfm->addPoint(time, it);
            time += timescale;
        }
        wfm->SmoothenWaveForm(5);
        signalheight += wfm->GetSignalHeight(1000);
        //std::cout << "accumulated Signal height: " << signalheight << std::endl;
    }

    s.str("");
    s << "x_" << int(x*1000) << "_y_" << int(y*1000) << ".dat";
    std::string data = measure.generateFileStream();
    measure.compressFileStream(archive, s.str(), data);
    archive.save(mw->archivename);

    signalheight /= numsignals;

    //print the result:
    std::stringstream sres("");
    sres << "    -> " << signalheight;
    mw->logit(sres.str());

    //update the progress bar:
    mw->ui->progressBar->setValue(mw->ui->progressBar->value() + 1);

    QApplication::processEvents();

    return signalheight;
}

double testfunclc3d(double x, double y, double z, void* mainwindow)
{
    LaserControl* mw = static_cast<LaserControl*>(mainwindow);

    std::stringstream s("");
    s << "Current Position: " << x << " | " << y;
    mw->logit(s.str());

    static double timestep = 0;
    static double yoffset  = 0;
    static double ymult    = 1;
    static double channeloffset = 0;

    static zip_file archive;
    if(archive.get_filename().compare(mw->archivename) != 0)
        archive.reset();
    //const std::string archivename = "Scan.zip";

    if(!mw->osci->is_open())
    {
        //std::vector<std::string> devices = mw->osci.ScanForDevices();
        //if(devices.size() > 0)
            if(mw->osci->open(0))
            {
                std::cout << "Could not open Oscilloscope communication!" << std::endl;
                //return -1;
            }

            mw->osci->SetChannel(1);
            mw->osci->ReadWaveFormSetup(false);

        std::fstream f;
        f.open(mw->archivename.c_str(), std::ios::in | std::ios::binary);
        if(f.is_open())
        {
            f.close();
            archive.load(mw->archivename);
        }
    }


    Measurement measure;

    measure.setName(mw->ui->LE_MeasureTitle->text().toStdString()); //"LaserProfile");
    measure.setMeasurementDevice(mw->ui->LE_MeasureDevice->text().toStdString()); //"904nm Laser");
    measure.setSample(mw->ui->LE_MeasureSample->text().toStdString()); //"LFoundry ChipB");
    measure.setTriggerA(mw->ui->LE_MeasureTrigInfo->text().toStdString()); //"posedge @108mV");


    std::stringstream s2("");
    s2 << "x = " << x << "; y = " << y;
    measure.setSample(s2.str());

    double signalheight = 0;
    const int numsignals = 20;

    for(int i = 0; i < numsignals; ++i)
    {
        std::cout << "i = " << i << std::endl;
        //enable the measurement:
        std::string wfmt = mw->osci->ReadWaveForm(!mw->osci->GetWaveFormSetupStatus(), mw->waiter, 5);
        double timescale = mw->osci->GetTimeScale("");
        std::vector<double> waveform = mw->osci->DecodeWaveFormY(wfmt);

        Event* evt = measure.addEvent("Wfm", "");

        WaveForm* wfm = evt->addWaveForm("AmpOut");
        wfm->setBytesPerPoint(1);
        wfm->setBandwidth("200MHz");
        wfm->setTimestep(timestep);
        wfm->setYOffset(yoffset);
        wfm->setScale(ymult);
        wfm->setChannelOffset(channeloffset);
        if(wfmt.length() > 0) // no timeout occurred
            wfm->addBinaryData(wfmt);
        else
            wfm->addBinaryData("00000000000"); //some constant data

        //std::cout << "wfm complete: " << ((wfm->informationComplete())?"1":"0") << std::endl
        //          << "wfm length:   " << wfm->getBinaryLength() << std::endl;

        double time = 0;
        for(auto& it : waveform)
        {
            wfm->addPoint(time, it);
            time += timescale;
        }
        wfm->SmoothenWaveForm(5);
        signalheight += wfm->GetSignalHeight(1000);
        //std::cout << "accumulated Signal height: " << signalheight << std::endl;
    }

    s.str("");
    s << "x_" << int(x*1000) << "_y_" << int(y*1000) << "_z_" << int(z*1000) << ".dat";
    std::string data = measure.generateFileStream();
    measure.compressFileStream(archive, s.str(), data);
    archive.save(mw->archivename);

    signalheight /= numsignals;

    //print the result:
    std::stringstream sres("");
    sres << "    -> " << signalheight;
    mw->logit(sres.str());

    //update the progress bar:
    mw->ui->progressBar->setValue(mw->ui->progressBar->value() + 1);

    QApplication::processEvents();

    return signalheight;
}

void LaserControl::on_B_LaserSetup_clicked()
{
    if(!setup.is_open())
    {
        logit("Error: Laser Setup not connected!");
        return;
    }
    if(!setup.GetInitStatus(LaserSetup::all))
    {
        logit("Error: The Stages have not been initialised!");
        return;
    }

    bool oscineeded = !ui->CB_NoOsciNeeded->isChecked();
    bool closeosci = false;
    if(!osci->is_open() && oscineeded)
    {
        osci->open(0);
        if(!osci->is_open())
        {
            logit("Error: No Oscilloscope Connection Open");
            return;
        }
        closeosci = true;
    }

    static bool running = false;
    if(running)
    {
        running = false;
        return;
    }
    running = true;

    //wait for the measurement information to be filled in:
    ui->GB_MeasurementInfo->setGeometry(42,240,261,231);
    ui->GB_MeasurementInfo->setVisible(true);
    while(ui->GB_MeasurementInfo->isVisible())
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    if(!running)
        return;

    ui->B_LaserSetup->setText("Stop Scan");
    ui->B_LS_3D_Scan->setEnabled(false);
    QApplication::processEvents();

    int localx = 3 - ui->CB_LS_ScanX->currentIndex();
    int localy = 3 - ui->CB_LS_ScanY->currentIndex();
    int localz = 3 - ui->CB_LS_ScanZ->currentIndex();


    //setup.InitialiseStages(LaserSetup::refPos);
    //setup.WaitforAxes(LaserSetup::all);

    setup.MoveAbsolute(localz, ui->SB_LS_Scan_StartZ->value());
    setup.WaitforAxes(localz);

    //build a filename for the measurement:
    std::stringstream s("");
    int counter = 1;
    s << "Scan_2D_";
    switch(localx)
    {
    case(LaserSetup::x): s << "X"; break;
    case(LaserSetup::y): s << "Y"; break;
    case(LaserSetup::z): s << "Z"; break;
    }
    switch(localy)
    {
    case(LaserSetup::x): s << "X"; break;
    case(LaserSetup::y): s << "Y"; break;
    case(LaserSetup::z): s << "Z"; break;
    }
    s << "_";
    std::string filename = s.str();
    //prefix finished


    s << counter << ".dat"; // add index and suffix
    //loop until an unused filename is found:
    std::fstream f;
    f.open(s.str().c_str(), std::ios::in);
    while(f.is_open())
    {
        f.close();
        ++counter;
        s.str("");
        s << filename << counter << ".dat";
        f.open(s.str().c_str(), std::ios::in);
    }

    //make the filename an archivename for the measurement function:
    archivename = s.str().substr(0,s.str().length()-4) + ".zip";

    //calculate the number of measurements for the progress bar:
    int measurements = (int((ui->SB_LS_Scan_StopX->value() - ui->SB_LS_Scan_StartX->value()) / ui->SB_LS_Scan_StepX->value()) + 1)
                        * (int((ui->SB_LS_Scan_StopY->value() - ui->SB_LS_Scan_StartY->value()) / ui->SB_LS_Scan_StepY->value()) + 1);
    ui->progressBar->setMaximum(measurements);
    ui->progressBar->setValue(0);

    setup.Scan2D(&running, localx, ui->SB_LS_Scan_StartX->value(), ui->SB_LS_Scan_StopX->value(),
                           localy, ui->SB_LS_Scan_StartY->value(), ui->SB_LS_Scan_StopY->value(),
                           ui->SB_LS_Scan_StepX->value(), ui->SB_LS_Scan_StepY->value(),
                 0, false, ((oscineeded)?testfunclc:fastrotest), static_cast<void*>(this), s.str(), "V");

    if(closeosci)
        osci->close();

    ui->B_LaserSetup->setText("Start 2D Scan");
    ui->B_LS_3D_Scan->setEnabled(true);
    running = false;
}

void LaserControl::on_B_2DScanStart_clicked()
{
    ui->GB_MeasurementInfo->setVisible(false);
}

void LaserControl::on_B_2DScanAbort_clicked()
{
    on_B_LaserSetup_clicked();
    ui->GB_MeasurementInfo->setVisible(false);
}

void LaserControl::on_B_RangeImportCancel_clicked()
{
    //reset the values:
    ui->SB_ScanRange_Xdown->setValue(0.5);
    ui->SB_ScanRange_Xup->setValue(0.5);
    ui->SB_ScanRange_Ydown->setValue(0.5);
    ui->SB_ScanRange_Yup->setValue(0.5);

    ui->GB_RangeImport->setVisible(false);
}

void LaserControl::on_B_RangeImportOK_clicked()
{
    //change the ranges for the 2D Scan accordingly to the current laser position and the ranges provided:
    double startx = 0;
    double stopx  = 0;
    switch(ui->CB_LS_ScanX->currentIndex())
    {
    case(0):
        startx = ui->SB_Laser_xpos->value();
        stopx  = ui->SB_Laser_xpos->value();
        break;
    case(1):
        startx = ui->SB_Laser_ypos->value();
        stopx  = ui->SB_Laser_ypos->value();
        break;
    case(2):
        startx = ui->SB_Laser_zpos->value();
        stopx  = ui->SB_Laser_zpos->value();
        break;
    }

    startx -= ui->SB_ScanRange_Xdown->value();
    if(startx < 0)
        startx = 0;
    ui->SB_LS_Scan_StartX->setValue(startx);
    stopx += ui->SB_ScanRange_Xup->value();
    if(stopx > ui->SB_LS_Scan_StopX->maximum())
        stopx = ui->SB_LS_Scan_StopX->maximum();
    ui->SB_LS_Scan_StopX->setValue(stopx);

    double starty = 0;
    double stopy  = 0;
    switch(ui->CB_LS_ScanY->currentIndex())
    {
    case(0):
        starty = ui->SB_Laser_xpos->value();
        stopy  = ui->SB_Laser_xpos->value();
        break;
    case(1):
        starty = ui->SB_Laser_ypos->value();
        stopy  = ui->SB_Laser_ypos->value();
        break;
    case(2):
        starty = ui->SB_Laser_zpos->value();
        stopy  = ui->SB_Laser_zpos->value();
        break;
    }

    starty -= ui->SB_ScanRange_Ydown->value();
    if(starty < 0)
        starty = 0;
    ui->SB_LS_Scan_StartY->setValue(starty);
    stopy += ui->SB_ScanRange_Yup->value();
    if(stopy > ui->SB_LS_Scan_StopY->maximum())
        stopy = ui->SB_LS_Scan_StopY->maximum();
    ui->SB_LS_Scan_StopY->setValue(stopy);

    switch(ui->CB_LS_ScanZ->currentIndex())
    {
    case(0):
        ui->SB_LS_Scan_StartZ->setValue(ui->SB_Laser_xpos->value());
        break;
    case(1):
        ui->SB_LS_Scan_StartZ->setValue(ui->SB_Laser_ypos->value());
        break;
    case(2):
        ui->SB_LS_Scan_StartZ->setValue(ui->SB_Laser_zpos->value());
        break;
    }



    ui->GB_RangeImport->setVisible(false);
}

void LaserControl::on_B_RangeImport_clicked()
{
    ui->GB_RangeImport->setGeometry(42,270,261,161);
    ui->GB_RangeImport->setVisible(true);
}

void LaserControl::on_B_Osci_Scan_clicked()
{
    std::vector<std::string> devices = osci->ScanForDevices();

    if(osci->GetNumDevicesFound() > 0)
        ui->SB_Osci_Index->setMaximum(osci->GetNumDevicesFound());
    else
        ui->SB_Osci_Index->setMaximum(0);
    ui->SB_Osci_Index->setValue(0);

    logit("Devices found:");
    if(devices.size() > 0)
    {
        for(unsigned int i = 0; i < devices.size(); ++i)
        {
            std::stringstream s("");
            s << "  " << i << ": " << devices[i];
            logit(s.str());
        }
    }
    else
        logit("     none.");
}

void LaserControl::on_B_Osci_Open_clicked()
{
    if(osci == nullptr)
        return;

    int index = ui->SB_Osci_Index->value() - 1;
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
        logit("Opening Keysight Device");
    }
    else if(osci->CheckResourceName(index, "TEKTRONIX"))
    {
        int printlevel = osci->GetPrintLevel();
        delete osci;
        osci = new TektronixOsci(printlevel);
        osci->ScanForDevices();
        logit("Opening Tektronix Device");
    }
    else
        logit("Opening Other Device");

    if(!osci->open(static_cast<unsigned int>(index)))
        logit(std::string("Error opening VISA Device ") + ui->SB_Osci_Index->text().toStdString());
    //else
    //    logit(std::string("Opened VISA Device ") + ui->SB_Osci_Index->text().toStdString());
}

void LaserControl::on_B_Osci_Close_clicked()
{
    osci->close();
    logit("VISA Session Closed.");
}

void LaserControl::on_B_Osci_Write_clicked()
{
    if(!osci->is_open())
        return;

    std::string query = ui->LE_Osci_Query->text().toStdString();

    std::stringstream s("");
    s << "VISA Write: \"" << query << "\" (" << query.length() << " characters)";
    logit(s.str());

    int result = osci->Write(query);

    if(result < 0)
        logit("VISA Write: an error occured");
    else
    {
        std::stringstream s("");
        s << "VISA Write: " << result << " characters written";
        logit(s.str());
    }
}

void LaserControl::on_B_Osci_Read_clicked()
{
    if(!osci->is_open())
        return;

    int chars = ui->SB_Osci_NumChars->value();

    if(chars <= 0)
        return;

    std::string answer = osci->Read(chars);

    std::stringstream s("");
    s << "VISA Read: \"";
    if(answer.c_str()[answer.length()-1] != '\n')
        s << answer << "\"";
    else
        s << answer.substr(0,answer.length()-1) << "\"";
    logit(s.str());
}

void LaserControl::on_B_Osci_GetWfm_clicked()
{
    if(osci == nullptr || !osci->is_open())
        return;

    logit("VISA: Aquiring Wave Form...");

    std::string wfm = osci->ReadWaveForm(!osci->GetWaveFormSetupStatus(), waiter, 5);
    double timescale = osci->GetTimeScale("");
    std::vector<double> waveform = osci->DecodeWaveFormY(wfm);

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

    osci->SaveToFile(("waveform_" + QString::number(index).toStdString() + ".dat"), waveform, timescale, true);

    logit("VISA: WaveForm saved to \"waveform_" + QString::number(index).toStdString()
          + ".dat\"");
}

void LaserControl::on_B_Close_clicked()
{
    this->hide();
}

void LaserControl::on_SB_LS_Lasertune_editingFinished()
{
    if(!setup.Laser_is_open())
        return;

    static bool running = false;

    setup.SetTuneValue(ui->SB_LS_Lasertune->value());

    if(running)
        return;

    running = true;
    ui->SB_LS_Lasertune->setPalette(palette_button_green);
    ui->SB_LS_Lasertune->update();
    int count = 0;
    while(++count < 0.5/0.005)
    {
        Timing::Sleep(5);
        QApplication::processEvents();
    }

    ui->SB_LS_Lasertune->setPalette(palette_button_gray);
    running = false;
}

void LaserControl::on_LE_Osci_Query_returnPressed()
{
    on_B_Osci_Write_clicked();
}

void LaserControl::on_B_LS_3DScanAbort_clicked()
{
    on_B_LS_3D_Scan_clicked();
    ui->GB_3D_MeasurementInfo->setVisible(false);
}

void LaserControl::on_B_LS_3D_Scan_clicked()
{
    if(!setup.is_open())
    {
        logit("Error: Laser Setup not connected!");
        return;
    }
    if(!setup.GetInitStatus(LaserSetup::all))
    {
        logit("Error: The Stages have not been initialised!");
        return;
    }

    bool closeosci = false;
    if(!osci->is_open())
    {
        osci->open(0);
        if(!osci->is_open())
        {
            logit("Error: No Oscilloscope Connection Open");
            return;
        }
        closeosci = true;
    }

    static bool running = false;
    if(running)
    {
        running = false;
        return;
    }
    running = true;

    //wait for the measurement information to be filled in:
    //ui->GB_3D_MeasurementInfo->setGeometry(42,240,261,231);
    ui->SB_LS_Scan_StartZ_2->setMinimum(ui->SB_LS_Scan_StartZ->minimum());
    ui->SB_LS_Scan_StartZ_2->setMaximum(ui->SB_LS_Scan_StartZ->maximum());
    ui->SB_LS_Scan_StartZ_2->setValue(ui->SB_LS_Scan_StartZ->value());
    ui->SB_LS_Scan_StopZ->setValue(ui->SB_LS_Scan_StartZ->value());
    ui->SB_LS_Scan_StopZ->setMinimum(ui->SB_LS_Scan_StartZ->minimum());
    ui->SB_LS_Scan_StopZ->setMaximum(ui->SB_LS_Scan_StartZ->maximum());
    ui->GB_3D_MeasurementInfo->setVisible(true);
    while(ui->GB_3D_MeasurementInfo->isVisible())
    {
        QApplication::processEvents();
        Timing::Sleep(5);
    }

    if(!running)
        return;

    ui->B_LS_3D_Scan->setText("Stop Scan");
    ui->B_LaserSetup->setEnabled(false);
    QApplication::processEvents();

    int localx = 3 - ui->CB_LS_ScanX->currentIndex();
    int localy = 3 - ui->CB_LS_ScanY->currentIndex();
    int localz = 3 - ui->CB_LS_ScanZ->currentIndex();


    //setup.InitialiseStages(LaserSetup::refPos);
    //setup.WaitforAxes(LaserSetup::all);

    setup.MoveAbsolute(localz, ui->SB_LS_Scan_StartZ_2->value(), 0.02);
    setup.WaitforAxes(localz);

    //build a filename for the measurement:
    std::stringstream s("");
    int counter = 1;
    s << "Scan_3D_";
    switch(localx)
    {
    case(LaserSetup::x): s << "X"; break;
    case(LaserSetup::y): s << "Y"; break;
    case(LaserSetup::z): s << "Z"; break;
    }
    switch(localy)
    {
    case(LaserSetup::x): s << "X"; break;
    case(LaserSetup::y): s << "Y"; break;
    case(LaserSetup::z): s << "Z"; break;
    }
    switch(localz)
    {
    case(LaserSetup::x): s << "X"; break;
    case(LaserSetup::y): s << "Y"; break;
    case(LaserSetup::z): s << "Z"; break;
    }
    s << "_";
    std::string filename = s.str();
    //prefix finished


    s << counter << ".dat"; // add index and suffix
    //loop until an unused filename is found:
    std::fstream f;
    f.open(s.str().c_str(), std::ios::in);
    while(f.is_open())
    {
        f.close();
        ++counter;
        s.str("");
        s << filename << counter << ".dat";
        f.open(s.str().c_str(), std::ios::in);
    }

    //make the filename an archivename for the measurement function:
    archivename = s.str().substr(0,s.str().length()-4) + ".zip";

    //calculate the number of measurements for the progress bar:
    int measurements = (int((ui->SB_LS_Scan_StopX->value() - ui->SB_LS_Scan_StartX->value()) / ui->SB_LS_Scan_StepX->value()) + 1)
                        * (int((ui->SB_LS_Scan_StopY->value() - ui->SB_LS_Scan_StartY->value()) / ui->SB_LS_Scan_StepY->value()) + 1)
                        * (int((ui->SB_LS_Scan_StopZ->value() - ui->SB_LS_Scan_StartZ_2->value()) / ui->SB_LS_Scan_StepZ->value()) + 1);
    ui->progressBar->setMaximum(measurements);
    ui->progressBar->setValue(0);

    //TODO: the remains were not updated for 3D scans, also replace testfunclc with a copy writing all 3 dimensions in the file

    double zstart = ui->SB_LS_Scan_StartZ_2->value();
    double zstop  = ui->SB_LS_Scan_StopZ->value();
    double zstep  = ui->SB_LS_Scan_StepZ->value();

    double xstart = ui->SB_LS_Scan_StartX->value();
    double xstop  = ui->SB_LS_Scan_StopX->value();
    double xstep  = ui->SB_LS_Scan_StepX->value();
    double ystart = ui->SB_LS_Scan_StartY->value();
    double ystop  = ui->SB_LS_Scan_StopY->value();
    double ystep  = ui->SB_LS_Scan_StepY->value();

    for(double z = zstart; z <= zstop + 1e-3; z += zstep)
    {
        setup.MoveAbsolute(localz, z, 0.02);
        setup.WaitforAxes(localz);

        setup.Scan2D(&running, localx, xstart, xstop, localy, ystart, ystop, xstep, ystep, 0, false,
                     testfunclc3d, static_cast<void*>(this), s.str(), "V");
    }

    if(closeosci)
        osci->close();

    ui->B_LaserSetup->setText("Start 2D Scan");
    ui->B_LS_3D_Scan->setEnabled(true);
    running = false;
}
