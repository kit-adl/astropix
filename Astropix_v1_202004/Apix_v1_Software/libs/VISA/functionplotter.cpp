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

#include "functionplotter.h"

FunctionPlotter::FunctionPlotter(QLabel* label) : keysightosci(true), timescale(1), voltscale(1),
        voltoffset(0)
{
    this->label = label;
}

void FunctionPlotter::SetLabel(QLabel* label)
{
    this->label = label;
}

void FunctionPlotter::SetKeysightOsci(bool keysight)
{
    keysightosci = keysight;
}

double FunctionPlotter::GetTimeScale()
{
    return timescale;
}

void FunctionPlotter::SetTimeScale(double scale)
{
    timescale = scale;
}

double FunctionPlotter::GetVoltScale()
{
    return voltscale;
}

void FunctionPlotter::SetVoltScale(double scale)
{
    voltscale = scale;
}

double FunctionPlotter::GetVoltOffset()
{
    return voltoffset;
}

void FunctionPlotter::SetVoltOffset(double offset)
{
    voltoffset = offset;
}

void FunctionPlotter::ClearData()
{
    data.clear();
    colors.clear();
}

int FunctionPlotter::GetNumPoints(int curve)
{
    if(curve < 0 || curve >= (int)data.size())
        return -1;
    return (int) (data[curve]).size();
}

int FunctionPlotter::GetNumCurves()
{
    return (int)data.size();
}

void FunctionPlotter::AddCurve(QColor col)
{
    data.push_back(std::vector<int>());
    colors.push_back(col);
}

bool FunctionPlotter::AddData(int curve, int point)
{
    if(curve < 0 || curve >= (int)data.size())
        return false;

    data[curve].push_back(point);
    return true;
}

bool FunctionPlotter::AddData(int curve, std::vector<int> points)
{
    if(curve < 0 || curve >= (int)data.size())
        return false;

    data[curve].insert(data[curve].end(), points.begin(), points.end());
    return true;
}

bool FunctionPlotter::AddData(int curve, std::string points)
{
    if(curve < 0 || curve >= (int)data.size())
        return false;

    for(int i = 0; i < (int)points.length(); ++i)
        data[curve].push_back(int((unsigned char)(points.c_str()[i])));
    return true;
}

void FunctionPlotter::Draw()
{
    //The drawing of the histogram is stored on a QPixmap and then transfered to a label
    QPixmap myPix(label->size());
    double width = label->size().width();
    double height = label->size().height();
    QPainter painter(&myPix);

    //Draw a white rectangle as background
    painter.setBrush(Qt::white);
    painter.drawRect(-1,-1,width+1, height+1);

    const int xoffset = 20;
    const int yoffset = 20;
    double heightscale = (height - yoffset) / 256.;
    double widthscale  = 1;
    if(data.size() > 0)
    {
        int i = 0;
        for(i = 0; i < (int)data.size() && data[i].size() == 0; ++i);
        if(i < (int)data.size())
            widthscale = (width - 2 * xoffset) / double(data[i].size());
    }

    //draw the axes: TODO

    painter.setPen(Qt::black);
    // arrow sizes:
    int arrowwidth;
    int arrowheight;
    arrowwidth = width / 100;
    if(arrowwidth > 5)
        arrowwidth = 5;
    else if(arrowwidth < 2)
        arrowwidth = 2;
    arrowheight = height / 20;
    if(arrowheight > 20)
        arrowheight = 20;
    else if(arrowheight < 10)
        arrowheight = 10;

    // y axis:
    arrowwidth = width / 100;
    if(arrowwidth > 5)
        arrowwidth = 5;
    else if(arrowwidth < 2)
        arrowwidth = 2;
    arrowheight = height / 20;
    if(arrowheight > 20)
        arrowheight = 20;
    else if(arrowheight < 10)
        arrowheight = 10;
    painter.drawLine(xoffset - 1, height - yoffset, xoffset - 1, 0);
    painter.drawLine(xoffset - 1, 0, xoffset + arrowwidth - 1, arrowheight);
    painter.drawLine(xoffset, 0, xoffset - arrowwidth, arrowheight);

    //x axis:
    arrowwidth = height / 100;
    if(arrowwidth > 5)
        arrowwidth = 5;
    else if(arrowwidth < 2)
        arrowwidth = 2;
    arrowheight = width / 20;
    if(arrowheight > 20)
        arrowheight = 20;
    else if(arrowheight < 10)
        arrowheight = 10;
    painter.drawLine(xoffset, height - yoffset + 1, width, height - yoffset + 1);
    painter.drawLine(width, height - yoffset + 1, width - arrowheight, height - yoffset - arrowwidth + 1);
    painter.drawLine(width, height - yoffset + 2, width - arrowheight, height - yoffset + arrowwidth + 2);


    for(int col = 0; col < (int)colors.size(); ++col)
    {
        painter.setPen(colors[col]); //Qt::blue);

        if(data[col].size() > 0)
        {
            double lastvalue = 0;

            if(keysightosci)
                lastvalue = (256 - data[col][0]) * heightscale;
            else
            {
                int value = data[col][0];
                if(value >= 128)
                    value -= 256;
                lastvalue = (128 - value) * heightscale;
            }

            for(int i = 1; i < (int)data[col].size(); ++i)
            {
                double value;
                if(keysightosci)
                    value = (256 - data[col][i]) * heightscale;
                else
                {
                    int thisval = data[col][i];
                    if(thisval >= 128)
                        thisval -= 256;
                    value = (128 - thisval) * heightscale;
                }
                painter.drawLine(xoffset + (i-1) * widthscale, lastvalue, xoffset + i * widthscale, value);
                lastvalue = value;
            }
        }
    }


    //transfer Pixmap to label
    label->setPixmap(myPix);
}
