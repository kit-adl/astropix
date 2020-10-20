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
 
 /********************************************************************
 * class for plotting data rows (e.g. waveforms) in the Qt GUI      *
 * It is based on Felix Ehrler's version of the Histogram class     *
 *                                                                  *
 * For plotting, a QLabel is used on the GUI which defines the size *
 * of the resulting plot. To plot something, add a curve giving it  *
 * a colour for plotting (e.g. Qt::blue) and the pass the data with *
 * the index to add it to (starting from 0). It the curve index     *
 * does not exist, the function will return false. If the data was  *
 * added successfully, the return value will be true.               *
 * At the end of filling the data structure, call the Draw() method *
 *                                                                  *
 * Version: 1.0 (15.07.19)                                          *
 *                                                                  *
 * Author: Rudolf Schimassek                                        *
 ********************************************************************/

#ifndef FUNCTIONPLOTTER_H
#define FUNCTIONPLOTTER_H

#include <vector>
#include <string>
#include <iostream>

#include <QLabel>
#include <QPainter>
#include <QPixmap>


class FunctionPlotter
{
public:
    FunctionPlotter(QLabel* label = NULL);

    void SetLabel(QLabel* label);

    void SetKeysightOsci(bool keysight);

    double GetTimeScale();
    void   SetTimeScale(double scale);

    double GetVoltScale();
    void   SetVoltScale(double scale);

    double GetVoltOffset();
    void   SetVoltOffset(double offset);

    void ClearData();
    int  GetNumPoints(int curve);
    int  GetNumCurves();
    void AddCurve(QColor col);
    bool AddData(int curve, int point);
    bool AddData(int curve, std::vector<int> points);
    bool AddData(int curve, std::string points);

    void Draw();

private:
    QLabel* label;

    bool keysightosci;
    double timescale;
    double voltscale;
    double voltoffset;
    std::vector<std::vector<int> > data;
    std::vector<QColor> colors;
};

#endif // FUNCTIONPLOTTER_H
