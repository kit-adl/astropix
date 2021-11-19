#ifndef TOTCALIBRATION_H
#define TOTCALIBRATION_H

#include "trimming.h"
#include "Histogram.h"

struct tuple{
    double x;
    double y;
    double yerr;

    tuple(double x, double y, double err){
        this->x = x;
        this->y = y;
        this->yerr = err;
    }
};

class PointCurve
{
public:
    PointCurve();

    //maximum number of signals (updated when adding points, or set as reference for normalised output)
    void SetMaximum(double max);
    double GetMaximum();

    void AddPoint(double voltage, double hits, double error = 0);
    int AddHits(double voltage, double hits = 1);
    void ClearPoints();
    unsigned int GetNumPoints();
    std::pair<double, double> GetPoint(unsigned int index);
    std::pair<double, double> GetNormedPoint(unsigned int index);
    double GetPointError(unsigned int index);
    void   SetPointError(unsigned int index, double error);

    double GetPointVoltage(unsigned int index);
    unsigned int GetPointSignals(unsigned int index);
    int GetSignalsToVolt(double voltage);
    double GetPointNormedSignals(unsigned int index);

    double GetIntegral();

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
    std::vector<tuple> points;    //inj voltage, number of detected hits
    double maximum;                                    //maximum number of points for one injection height

    double threshold;
    double noise;

    double windowstart;
    double windowend;
    double windownoise;
};

class ToTCalibration
{
public:
    ToTCalibration(const Configuration& config);

    Configuration* GetConfig();
    void SetConfig(const Configuration& config);

    int  GetInterval_start() const;
    void SetInterval_start(int value);

    int  GetInterval_step() const;
    void SetInterval_step(int value);

    int  GetInterval_end() const;
    void SetInterval_end(int value);

    int  GetNumSignals() const;
    void SetNumSignals(int value);

    int  GetTSdiv() const;
    void SetTSdiv(int value);

    int  GetTS2div() const;
    void SetTS2div(int value);

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

    std::map<Dataset, PointCurve> MeasureToT(bool* running, bool updategui, Rect activepixels,
                                             std::string histfilename = "");

    std::map<Dataset, PointCurve> MeasureToTArea(bool* running, int pixelsatonce, Rect area,
                                                 std::string filename = "",
                                                 std::string histfilename = "");

    std::map<Dataset, Histogram> SortInHistogram(std::vector<Dataset>& data);

private:
    bool WriteToFile(std::string filename, std::string data);

    Configuration config;

    int interval_start;
    int interval_step;
    int interval_end;
    int numsignals;

    int tsdiv;
    int ts2div;
};

#endif // TOTCALIBRATION_H
