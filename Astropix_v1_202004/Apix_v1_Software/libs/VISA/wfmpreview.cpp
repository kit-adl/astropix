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

#include "wfmpreview.h"
#include "ui_wfmpreview.h"

WfmPreview::WfmPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WfmPreview)
{
    ui->setupUi(this);

    plotter = FunctionPlotter(ui->L_Plot);
    hist    = Histogramm(252, 0, 1, ui->L_Plot);
}

WfmPreview::~WfmPreview()
{
    delete ui;
}

QLabel *WfmPreview::GetPlotLabel()
{
    return ui->L_Plot;
}

void WfmPreview::ActivatePlot(bool waveform)
{
    if(waveform)
    {
        ui->RB_hist->setChecked(false);
        ui->RB_wfm->setChecked(true);
    }
    else
    {
        ui->RB_hist->setChecked(true);
        ui->RB_wfm->setChecked(false);
    }
}

void WfmPreview::on_B_Close_clicked()
{
    this->hide();
}

void WfmPreview::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    ui->L_Plot->setGeometry(5,5,this->width()-10, this->height()-5-40);
    ui->B_Close->setGeometry(this->width() - ui->B_Close->width() - 5, this->height() - ui->B_Close->height() - 5, 91, 31);

    ui->RB_wfm->setGeometry(20, this->height() - ui->RB_wfm->height() - 10, 82, 17);
    ui->RB_hist->setGeometry(20 + 10 + ui->RB_wfm->width(), this->height() - ui->RB_wfm->height() - 10, 82, 17);

    if(this->isVisible())
    {
        if(ui->RB_wfm->isChecked())
            plotter.Draw();
        if(ui->RB_hist->isChecked())
            hist.draw(false);
    }
}

void WfmPreview::on_RB_wfm_toggled(bool checked)
{
    if(checked) // && plotter.GetNumCurves() > 0)
        plotter.Draw();
    else //if(hist.getEntries() > 0)
        hist.draw(false);
}
