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


/********************************************************
 *      Histogram class for counting applications.      *
 *                                                      *
 * Integer bin numbers and bin contents where the       *
 * number of bins is defined in the constructor to      *
 * avoid conversions when changing the binning on a not *
 * empty histogram.                                     *
 *                                                      *
 * Version: 1.0 (April 2017)                            *
 *          1.1 (February 2019)                         *
 *                  changed bin number to a parameter   *
 *                  bins are connected to a value       *
 *                  drawing to arbitrary label added    *
 *                  Gaussian Fit added                  *
 *                                                      *
 * Authors: Rudolf Schimassek                           *
 *          Felix Ehrler                                *
 ********************************************************/

#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <fstream>
#include <string>
#include <vector>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <map>
#include <iomanip>
#include <iostream>

//#define ALGLIB
#ifdef ALGLIB
#include "src/optimization.h"
#endif

class Histogramm
{
public:
    /**
     * @brief Histogramm
     * @param bins: number of bins
     * @param minbin: value of smallest bin
     * @param binsize: size of one bin.
     * @param label: drawing canvas
     */
    Histogramm(int bins = 5, double minbin = 0, double binsize = 1,
               QLabel* label = NULL);
    //clear data vector
    void Clear();
    //Adds a single entry to given bin
    bool Fill(int index);
    //Adds a single entry to given value
    bool Fill(double position);
    //Sets the value of a bin
    void SetBin(int index, int value);
    //Sets the value of a bin with of given value
    void SetBin(double position, int value);
    //returns the entries in a bin
    int GetBin(int index);
    //Returns the number of entries of the bin with most entries
    int GetMax();
    //Returns the bin number of the bin with most entries
    int GetMaxBin();
    //Saves the content of the histogram to given file
    bool save(std::string filename, std::string title);
    //Sets the binsize
    void SetBinsize(double binsize);
    //Returns the binsize
    double GetBinsize();
    //Returns the value of bin 1
    double GetMinBin();
    //Sets the drawing canvas
    void SetLabel(QLabel* label);
    //Returns the total number of entries in the histogram
    int getEntries();
    //Returns the number of bins
    int GetBins();
    //Draws the autoscaled histogram to the given label. drawgaus indicates whether the gaussian fit is to be drawn, too
    void draw(bool drawgaus);

    int GetPrintLevel();
    void SetPrintLevel(int level);
#ifdef ALGLIB
    //Fits a Gaussian to the histogram and stores the fit parameters.
    void FitGaus(double ignorefrac = 0, bool print = 0);
    //Fits a Gaussian to the histogram and stores the fit parameters.
    void Fit2Gaus(double ignorefrac = 0, bool print = 0);
#endif



private:
    QLabel* label;
    std::vector<int> data;
    int bins;
    double binsize;
    double minbin;
    double gaus_sigma = 0;
    double gaus_mu = 0;
    double gaus_yoffset = 0;
    double gaus_amplitude = 0;
    double gaus2_sigma = 0;
    double gaus2_mu = 0;
    double gaus2_amplitude = 0;

    int printlevel;
};

#endif // HISTOGRAMM_H
