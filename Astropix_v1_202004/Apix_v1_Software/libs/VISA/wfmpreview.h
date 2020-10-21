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

#ifndef WFMPREVIEW_H
#define WFMPREVIEW_H

#include <QWidget>
#include <QLabel>

#include "functionplotter.h"
#include "histogramm.h"

namespace Ui {
class WfmPreview;
}

class WfmPreview : public QWidget
{
    Q_OBJECT

    friend class MainWindow;

public:
    explicit WfmPreview(QWidget *parent = 0);
    ~WfmPreview();

    QLabel* GetPlotLabel();

    void ActivatePlot(bool waveform);

private slots:
    void on_B_Close_clicked();
    void on_RB_wfm_toggled(bool checked);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::WfmPreview *ui;

    FunctionPlotter plotter;

    Histogramm hist;
};

#endif // WFMPREVIEW_H
