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
 * A class for trimming algorithms for ATLASPix3                *
 * The chip configuration is made via local copies of the GUI   *
 * configuration to be able to reset all settings.              *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (13.09.19)                                      *
 ****************************************************************/

#ifndef TRIMMING_H
#define TRIMMING_H

#include "libs/src/stdafx.h"
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include <cmath>
#include "libs/src/optimization.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <time.h>

#include "../fastreadout.h"
#include "asic_config_2.h"
#include "timing.h"
#include "libs/Histogram.h"

#include "libs/configuration.h"

//define this flag to measure thresholds at TDAC=0 for finding the tuning range and target threshold
//  if it is not defined, use the lower end of the TDAC=7 distribution (µ - 3 sigma) as target:
//#define SCANFULLRANGE

class SCurve
{
public:
    SCurve();

    //maximum number of signals (updated when adding points, or set as reference for normalised output)
    void SetMaximum(int max);
    int GetMaximum();

    void AddPoint(double voltage, int hits);
    int AddHits(double voltage, int hits = 1);
    void ClearPoints();
    unsigned int GetNumPoints();
    std::pair<double, unsigned int> GetPoint(unsigned int index);
    std::pair<double, double> GetNormedPoint(unsigned int index);

    double GetPointVoltage(unsigned int index);
    unsigned int GetPointSignals(unsigned int index);
    int GetSignalsToVolt(double voltage);
    double GetPointNormedSignals(unsigned int index);

    int GetIntegral();

    double GetThreshold();
    void   SetThreshold(double thr);
    double GetNoise();
    void   SetNoise(double noise);

    double GetWindowStart();
    void   SetWindowStart(double start);
    double GetWindowEnd();
    void   SetWindowEnd(double end);
    double GetWindowNoise();
    void   SetWindowNoise(double noise);

    std::map<double, double> GetNormedSortedMap();

    std::string GenerateString(std::string title, bool normed = false);

    static double EvalSCurve(double x, double x0, double width);
    static double EvalWindow(double x, double x0, double x1, double width);

    void Sort();
private:
    std::vector<std::pair<double, unsigned int> > points;    //inj voltage, number of detected hits
    int maximum;                                    //maximum number of points for one injection height

    double threshold;
    double noise;

    double windowstart;
    double windowend;
    double windownoise;
};

struct Rect{
    int startcol;
    int endcol;
    int startrow;
    int endrow;

    std::string ToString(){
        std::stringstream s("");
        s << "(" << startcol << "|" << startrow << ") - (" << endcol << "|" << endrow << ")";
        return s.str();
    }
};

static Rect WholeMatrix = {0, AP3columns-1, 0, AP3rows - 1};


class Trimming
{
public:
    Trimming(const Configuration& config);

    /**
     * @brief GetConfig provides a pointer to the configuration container used by the trimming
     *     object
     * @return               - pointer to the configuration container or a null pointer if it is
     *                           not set
     */
    Configuration*   GetConfig();
    /**
     * @brief SetConfig creates a copy of the configuration object passed, but not of the pointers
     *     contained in it.
     * @param config         - the configuration container to copy for usage in the trimming object
     */
    void            SetConfig(const Configuration& config);

    /**
     * @brief GetIntervalStart retrieves the starting value of the injection for the SCurves
     * @return               - the smallest value to use for the SCurve as TDAC value
     */
    int             GetIntervalStart() const;
    /**
     * @brief SetIntervalStart sets the smallest value to use for injection in SCurves
     * @param value          - the new smallest value to use for the injection VDAC in SCurves
     */
    void            SetIntervalStart(int value);

    /**
     * @brief GetIntervalStep provides the step size between points in the SCurve
     * @return               - step size between points in VDAC tics
     */
    int             GetIntervalStep() const;
    /**
     * @brief SetIntervalStep changes the step size for SCurves during trimming
     * @param value          - the new step size in VDAC tics (0-255 <=> 0-1.8V)
     */
    void            SetIntervalStep(int value);

    /**
     * @brief GetIntervalEnd provides the largest value of inject to use for SCurves in VDAC tics
     * @return               - largest value to use in SCurves in VDAC tics (0-255 <=> 0-1.8V)
     */
    int             GetIntervalEnd() const;
    /**
     * @brief SetIntervalEnd changes the largest inject value to use in SCurves
     * @param value          - the largest inject value for SCurves in VDAC tics (0-255 <=> 0-1.8V)
     */
    void            SetIntervalEnd(int value);

    /**
     * @brief GetNumSignals retrieves the number of signals to use per measurement point in SCurves
     * @return               - number of signals per point in SCurves
     */
    int             GetNumSignals() const;
    /**
     * @brief SetNumSignals changes the number of signals to use to measure one point in SCurves
     * @param value          - the new number of signals to use per point
     */
    void            SetNumSignals(int value);

    int             GetMaxSimultaneousPixels() const;
    void            SetMaxSimultaneousPixels(int value);

    bool            GetTriggerIDUsage() const;
    void            SetTriggerIDUsage(bool usetriggerid);

    int             GetMaxTime() const;
    void            SetMaxTime(int value);

    /**
     * @brief ConfigureInjections configures the injections to be activated for the passed pixel
     *     area. Injection is turned off for all other pixels
     * @param row            - row of the pixels to activate the injection for
     * @param flush          - flush the configuration command if set to true
     * @param startcol       - first column to activate injections for
     * @param endcol         - last column to activate injections for
     */
    void            ConfigureInjections(int row, bool flush = true,
                                        int startcol = 0, int endcol = AP3columns - 1);

    //use only with very small number of pixels at a time:
    /**
     * @brief MeasureFastSCurve makes use of the trigger ID for each signal sending injections for
     *      several injection heights before reading out the data to speed up the measurement.
     *      However, this works only for small numbers of pixels (< 10) for gaining a speedup.
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param numactivepixels
     *                       - number of pixels to measure, used to determine read size and block
     *                          size before reading out data
     * @param sleep          - delay before reading after the last signal in a block
     * @return               - the SCurves for the pixels with signals in a map with the pixel
     *                          information as key
     */
    std::map<Dataset, SCurve> MeasureFastSCurve(bool* running, int numactivepixels = 1, int sleep = 200);
    /**
     * @brief MeasureSCurves measures SCurves for all activated pixels
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param updategui      - update the progressbar set in the configuration object of the
     *                          trimming object instance and write more output to cout
     * @param activepixels   - number of active pixels, used to determine the reading block size
     * @return               - the SCurves of the pixels with signals in a map with pixel
     *                          information as key
     */
    std::map<Dataset, SCurve> MeasureSCurves(bool *running, bool updategui = true,
                                             int activepixels = -1);
    std::map<Dataset, SCurve> MeasureSCurvesDelay(bool *running, bool updategui = true,
                                             int activepixels = -1);//ivan

    /**
     * @brief MeasureAllSCurves measures SCurves for all pixels in the passed area
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param disableothers  - disables all other pixels (via TDAC = 8) during measurement if set
     *                          to true
     * @param updategui      - update the progressbar set in config if set to true and turns on
     *                          some additional output to cout
     * @param startcol       - first column of the area to measure
     * @param endcol         - last column of the area to measure
     * @param startrow       - first row of the area to measure
     * @param endrow         - last row of the area to measure
     * @return               - the SCurves of the pixels with signals in a map with pixel
     *                          information as key
     */
    std::map<Dataset, SCurve> MeasureAllSCurves(bool delayscan, bool* running, bool disableothers = true,
                                                bool updategui = true,
                                                int startcol = 0, int endcol = AP3columns - 1,
                                                int startrow = 0, int endrow = AP3rows - 1);
    /**
     * @brief MeasureSparseSCurves measures a random set of up to a fixed number of rows
     *      as estimate of the result of the full matrix
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param numrows        - maximum number of rows to measure. If it is larger than the number
     *                          of rows in total, a full area scan is performed
     * @param updategui      - update the progressbar set in config if set to true and turns on
     *                          some additional output to cout
     * @param area           - area of the matrix to measure
     * @param randinit       - initialisation value for the random generator. For '0' the system
     *                          time is used. Set this to a fixed value to obtain the same rows
     *                          in different calls
     * @return               - the SCurves for the pixels with signals with pixel information as
     *                          key
     */
    std::map<Dataset, SCurve> MeasureSparseSCurves(bool* running, int numrows, bool updategui = true,
                                                   Rect area = WholeMatrix, int randinit = 0, bool delay =  false);
    /**
     * @brief MeasureSparseSCurves measures a random set of up to a fixed number of rows
     *      as estimate of the result of the full matrix
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param numrows        - maximum number of rows to measure. If it is larger than the number
     *                          of rows in total, a full area scan is performed
     * @param updategui      - update the progressbar set in config if set to true and turns on
     *                          some additional output to cout
     * @param startcol       - first column of the area to measure
     * @param endcol         - last column of the area to measure
     * @param startrow       - first row of the area to measure
     * @param endrow         - last row of the area to measure
     * @param randinit       - initialisation value for the random generator. For '0' the system
     *                          time is used. Set this to a fixed value to obtain the same rows
     *                          in different calls
     * @return               - the SCurves for the pixels with signals with pixel information as
     *                          key
     */
    std::map<Dataset, SCurve> MeasureSparseSCurves(bool* running, int numrows, bool updategui = true,
                                                   int startcol = 0, int endcol = AP3columns - 1,
                                                   int startrow = 0, int endrow = AP3rows - 1, int randinit = 0, bool delay =  false);
    /**
     * @brief MeasureSparseSCurves measures the passed list of rows for the given column interval
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param rows           - the list of the rows to measure. The measurement is performed in the
     *                          order of the list
     * @param updategui      - update the progressbar set in config if set to true and turns on
     *                          some additional output to cout
     * @param startcol       - first column of the area to measure
     * @param endcol         - last column of the area to measure
     * @return               - the SCurves for the pixels with signals with pixel information as
     *                          key
     */
    std::map<Dataset, SCurve> MeasureSparseSCurves(bool* running, std::vector<int> rows, bool updategui = true,
                                                   int startcol = 0, int endcol = AP3columns - 1, bool delay =  false);

    /**
     * @brief RemoveNoise waits on the scale of seconds and increases the threshold for all pixels
     *      that show noise until for a defined time no hits are detected any more
     * @return
     */
    int             RemoveNoise();

    /**
     * @brief FitSingleSCurve fits an error function to an SCurve
     * @param data           - the SCurve to fit the error function to
     * @param print          - activates output about the fit if set to true
     * @param threshold      - initial value for the threshold if the automatic estimate fails
     * @return               - true if the fit converged, false if not
     */
    bool            FitSingleSCurve(SCurve &data, bool print, double threshold = 1.0);
    /**
     * @brief FitSCurves fits all SCurves passed
     * @param scurves        - map containing the SCurves to fit
     * @param print          - activates output about the fits if set to true
     * @param threshold      - initial value for the threshold if the automatic estimate fails
     * @param updategui      - updates the progressbar in config if set to true
     * @return               - the fraction of converged fits
     */
    double          FitSCurves(std::map<Dataset, SCurve> &scurves, bool print,
                               double threshold = 1.0, bool updategui = false);

    /**
     * @brief FitSingleWindow fits a window function (product of two error functions) to the passed
     *      dataset
     * @param data           - the dataset to fit the window function to
     * @param print          - activates output about the fits if set to true
     * @param threshold      - initial value for the window start if the automatic estimate fails
     * @param length         - initial value for the length of the window if the automatic estimate
     *                          fails
     * @return               - true if the fit converged, false if not
     */
    bool            FitSingleWindow(SCurve &data, bool print, double threshold = 1.0,
                                    double length = 1.0);
    /**
     * @brief FitWindows fits window functions to all passed SCurves
     * @param scurves        - map container with the SCurves to fit the window function to
     * @param print          - activates output about the fits if set to true
     * @param threshold      - initial value for the window start if the automatic estimate fails
     * @param length         - initial value for the window length if the automatic estimate fails
     * @param updategui      - updates the progressbar in config if set to true
     * @return
     */
    double          FitWindows(std::map<Dataset, SCurve> &scurves, bool print,
                               double threshold = 1.0, double length = 1.0, bool updategui = false);

    /**
     * @brief FillHistogram uses the threshold value of the SCurves (a fit is not performed!) to
     *      fill a histogram
     * @param SCurves        - the SCurves to use for filling the histogram
     * @param start          - lower edge of the first bin of the histogram
     * @param end            - upper edge of the last bin of the histogram
     * @param binwidth       - width of a single bin in the histogram
     * @return               - the resulting histogram
     */
    Histogram       FillHistogram(std::map<Dataset, SCurve>& SCurves, double start, double end, double binwidth);
    Histogram       FillHistogramWithNoise(std::map<Dataset, SCurve>& SCurves, double start, double end, double binwidth);
    /**
     * @brief FitGaus fits a gaussian to a histogram
     * @param gaussian       - the histogram to fit the gaussian to
     * @param print          - activates output about the fit if set to true
     * @return               - true on convergence of the fit, false otherwise
     */
    static bool     FitGaus(Histogram& gaussian, bool print);

    /**
     * @brief FindMeanThreshold measures a number of rows to estimate the average threshold
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param area           - area to cover
     * @param updategui      - update the progressbar in config if set to true and activates
     *                          some more output to cout
     * @param sparserows     - maximum number of rows to use for the estimate. If it is larger than
     *                          the number of rows in the area, the whole area is measured
     * @param random_init    - initial value for the random generator for selecting the rows to
     *                          measure. Set to '0' to use system time.
     * @return               - the average threshold from all pixels measured
     */
    double          FindMeanThreshold(bool *running, Rect area, bool updategui, int sparserows, int random_init, bool delay = false);
    /**
     * @brief FindVNDACSetting performs a binary scan of VNDAC to find a suitable setting for the trimming
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param area           - area to cover
     * @param updategui      - update the progressbar in config if set to true and activates
     *                          some more output to cout
     * @param sparserows     - maximum number of rows to use for the estimate. If it is larger than
     *                          the number of rows in the area, the whole area is measured
     * @param random_init    - initial value for the random generator for selecting the rows to
     *                          measure. Set to '0' to use system time.
     * @param middlethreshold
     *                       - result of FindMeanThreshold() as reference threshold
     * @param scale          - number of sigmas `middlethreshold` should be below the mean of the
     *                          measured distribution for a VNDAC setting
     * @return               - the resulting VNDAC setting
     */
    int             FindVNDACSetting(bool delay, bool * running, Rect area, bool updategui, int sparserows, int random_init, double middlethreshold, double scale);
    /**
     * @brief TrimRow performs a binary search over the TDACs of one row of pixels to find the best
     *      fitting value for a given goal threshold
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param row            - row to measure
     * @param area           - area to cover, only the column interval is used here
     * @param updategui      - update the progressbar in config if set to true and activates
     *                          some more output to cout
     * @param threshold      - the threshold to get as close as possible to
     */
    void            TrimRow(bool delay, bool* running, int row, Rect area, bool updategui, double threshold);

    /**
     * @brief TrimChip performs trimming of the defined area of the chip starting with
     *      measurement of average threshold, over (optionally) the estimation of VNDAC
     *      to the search for the pixel TDAC values. On successful trimming, the resulting values
     *      are applied to the GUI configuration. On failure or abort, the old values are restored.
     * @param running        - pointer to a boolean. This boolean has to be set to true for the
     *                          method to be executed. Set it to false to abort it.
     * @param skipvndac      - skip VNDAC scan if set to true
     * @param startcol       - first column to trim
     * @param endcol         - last column to trim
     * @param startrow       - first row to trim
     * @param endrow         - last row to trim
     * @param updategui      - update the progressbar in config if set to true and activates
     *                          some more output to cout
     * @return               - true on success, false otherwise
     */
    bool            TrimChip(bool delay, bool* running, double *threshold, bool skipvndac, int startcol = 0, int endcol = AP3columns,
                             int startrow = 0, int endrow = AP3rows, bool updategui = true, void (*showthr)(double, void*) = nullptr);
private:
    Configuration config;

    int interval_start;
    int interval_step;
    int interval_end;
    int numsignals;

    int maxsimultaneouspixels;

    bool usetrigID;
    int  maxtimeperpoint;

    std::vector<std::pair<int, int> > deactivated_pixels;
};

#endif // TRIMMING_H
