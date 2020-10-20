#include "totcalibration.h"

PointCurve::PointCurve() :  points(std::vector<tuple>()), maximum(0),
            threshold(-1), noise(-1),
            windowstart(-1), windowend(-1), windownoise(-1)
{

}

void PointCurve::SetMaximum(double max)
{
    if(max >= 0)
        maximum = max;
}

double PointCurve::GetMaximum()
{
    return maximum;
}

void PointCurve::AddPoint(double voltage, double hits, double error)
{
    //if(hits > maximum)
    //    maximum = hits;
    points.push_back(tuple(voltage, hits, error));
}

int PointCurve::AddHits(double voltage, double hits)
{
    bool done = false;
    int entries = 0;
    for(auto& it : points)
    {
        if(std::abs(it.x - voltage) < 1e-5)
        {
            done = true;
            it.y += hits;
            entries = int(it.y);
        }
    }
    if(!done)
    {
        AddPoint(voltage, hits);
        entries = hits;
    }

    return entries;
}

void PointCurve::ClearPoints()
{
    maximum = 0;
    threshold = -1;
    noise = -1;

    points.clear();
}

unsigned int PointCurve::GetNumPoints()
{
    return points.size();
}

std::pair<double, double> PointCurve::GetPoint(unsigned int index)
{
    if(index < points.size())
        return std::make_pair(points[index].x, points[index].y);
    else
        return std::make_pair(double(-1e10), double(0));
}

std::pair<double, double> PointCurve::GetNormedPoint(unsigned int index)
{
    if(index < points.size())
    {
        std::pair<double, double> pt;
        pt.first  = points[index].x;
        pt.second = points[index].y/double(maximum);
        return pt;
    }
    else
        return std::make_pair(double(-1e10), double(0));
}

double PointCurve::GetPointError(unsigned int index)
{
    if(index < points.size())
        return points[index].yerr;
    else
        return -1e10;
}

void PointCurve::SetPointError(unsigned int index, double error)
{
    if(index < points.size())
        points[index].yerr = error;
}

double PointCurve::GetPointVoltage(unsigned int index)
{
    if(index < points.size())
        return points[index].x;
    else
        return -1e10;
}

unsigned int PointCurve::GetPointSignals(unsigned int index)
{
    if(index < points.size())
        return points[index].y;
    else
        return 0;
}

int PointCurve::GetSignalsToVolt(double voltage)
{
    for(auto& it : points)
        if(std::abs(it.x - voltage) < 0.001)
            return int(it.y);

    return -1;
}

double PointCurve::GetPointNormedSignals(unsigned int index)
{
    if(index < points.size())
        return points[index].y / double(maximum);
    else
        return 0;
}

double PointCurve::GetIntegral()
{
    double integral = 0;
    for(auto& it : points)
        integral += it.y;

    return integral;
}

double PointCurve::GetThreshold()
{
    return threshold;
}

void PointCurve::SetThreshold(double thr)
{
    threshold = thr;
}

double PointCurve::GetNoise()
{
    return noise;
}

void PointCurve::SetNoise(double noise)
{
    this->noise = noise;
}

double PointCurve::GetWindowStart()
{
    return windowstart;
}

void PointCurve::SetWindowStart(double start)
{
    windowstart = start;
}

double PointCurve::GetWindowEnd()
{
    return windowend;
}

void PointCurve::SetWindowEnd(double end)
{
    windowend = end;
}

double PointCurve::GetWindowNoise()
{
    return windownoise;
}

void PointCurve::SetWindowNoise(double noise)
{
    windownoise = noise;
}

std::map<double, double> PointCurve::GetNormedSortedMap()
{
    std::map<double, double> data;

    for(auto& it : points)
        data.insert(std::make_pair(it.x,it.y/double(maximum)));

    return data;
}

std::string PointCurve::GenerateString(std::string title, bool normed)
{
    std::stringstream s("");

    //header lines:
    s << "# PointCurve: " << title << std::endl;
    if(std::abs(threshold - (-1)) > 1e-5 || std::abs(noise - (-1)) > 1e-5)
        s << "# Threshold: " << threshold << std::endl
          << "# Noise:     " << noise << std::endl;
    else if(std::abs(windowstart - (-1)) > 1e-5 || std::abs(windowend - (-1)) > 1e-5 || std::abs(windownoise - (-1)) > 1e-5)
        s << "# Window Start: " << windowstart << std::endl
          << "# Window End:   " << windowend << std::endl
          << "# Noise:        " << windownoise << std::endl;

    //data lines (including legend):
    if(normed)
    {
        s << "# inj. Voltage (in V); Fraction of Signals detected; Error" << std::endl;
        for(auto& it : points)
            s << it.x << "\t" << it.y/double(maximum) << "\t" << it.yerr / double(maximum) << std::endl;
    }
    else
    {
        s << "# inj. Voltage (in V); Number of Signals detected (of " << maximum << "); Error" << std::endl;
        for(auto& it : points)
            s << it.x << "\t" << it.y << "\t" << it.yerr << std::endl;
    }

    return s.str();
}

double PointCurve::EvalSCurve(double x, double x0, double width)
{
    return (1. + erf((x-x0) / sqrt(2) / width)) * 0.5; // / 2.;
}

double PointCurve::EvalWindow(double x, double x0, double x1, double width)
{
    return (1. + erf((x - x0) / sqrt(2) / width)) * (1. + erf((x1 - x) / sqrt(2) / width)) * 0.25;
}

bool smaller(tuple lhs, tuple rhs)
{
    return lhs.x < rhs.x;
}

void PointCurve::Sort()
{
    std::sort(points.begin(), points.end(), smaller);
}


ToTCalibration::ToTCalibration(const Configuration& config) : interval_start(0), interval_step(1),
    interval_end(255), numsignals(100)
{
    this->config = config;
}

Configuration *ToTCalibration::GetConfig()
{
    return &config;
}

void ToTCalibration::SetConfig(const Configuration &config)
{
    this->config = config;
}

int ToTCalibration::GetInterval_start() const
{
    return interval_start;
}

void ToTCalibration::SetInterval_start(int value)
{
    interval_start = value;
}

int ToTCalibration::GetInterval_step() const
{
    return interval_step;
}

void ToTCalibration::SetInterval_step(int value)
{
    interval_step = value;
}

int ToTCalibration::GetInterval_end() const
{
    return interval_end;
}

void ToTCalibration::SetInterval_end(int value)
{
    interval_end = value;
}

int ToTCalibration::GetNumSignals() const
{
    return numsignals;
}

void ToTCalibration::SetNumSignals(int value)
{
    numsignals = value;
}

int ToTCalibration::GetTSdiv() const
{
    return tsdiv;
}

void ToTCalibration::SetTSdiv(int value)
{
    tsdiv = value;
}

int ToTCalibration::GetTS2div() const
{
    return ts2div;
}

void ToTCalibration::SetTS2div(int value)
{
    ts2div = value;
}

void ToTCalibration::ConfigureInjections(int row, bool flush, int startcol, int endcol)
{
    if(row < 0 || row >= AP3rows)
        return;


    ASIC_Config2* rowconfig = config.GetATLASPixConfig(Configuration::row);
    ASIC_Config2* colconfig = config.GetATLASPixConfig(Configuration::column);

    //injections only to passed row:
    for(int i = 0; i < AP3rows; ++i)
    {
        std::stringstream sr("");
        sr << "eninj_row_" << i;

        rowconfig->SetParameter(sr.str(), (i == row)?1:0);
    }

    //injection to all pixels in the row:
    for(int i = 0; i < AP3columns; ++i)
    {
        std::stringstream sc("");
        sc << "en_inject_col_" << i;

        if(i >= startcol && i <= endcol)
            colconfig->SetParameter(sc.str(), 1);
        else
            colconfig->SetParameter(sc.str(), 0);
    }

    if(flush)
        config.SendUpdate(Configuration::colrow);
}

std::map<Dataset, PointCurve> ToTCalibration::MeasureToT(bool *running, bool updategui,
                                                         Rect activepixels,
                                                         std::string histfilename)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, PointCurve>();

    bool debug = histfilename != "";

    //backup injection settings:
    Injection_Config* injconf_backup = config.GetInjectionConfig();
    if(injconf_backup == nullptr)
    {
        std::cerr << "Error: Injection config empty, aborting measurement." << std::endl;
        return std::map<Dataset,PointCurve>();
    }
    Injection_Config injconf = *injconf_backup; //create a working copy
    config.SetInjectionConfig(&injconf);
    injconf.SetNumPulseSets(static_cast<unsigned int>(numsignals));

    //backup VDAC settings:
    ASIC_Config2* vdac_backup = config.GetATLASPixConfig(Configuration::vdac);
    if(vdac_backup == nullptr)
    {
        std::cerr << "Error: VDAC config empty, aborting measurement." << std::endl;
        return std::map<Dataset, PointCurve>();
    }
    ASIC_Config2 vdac = *vdac_backup; //create a working copy
    config.SetATLASPixConfig(Configuration::vdac, &vdac);

    //ToT-Curve container:
    std::map<Dataset, PointCurve> pointcurves;

    //prepare the injections
    int splitinjections = 1;
    int numsignals_backup = numsignals;
    const int maxnuminjections = 100;
    if(injconf.GetNumPulseSets() > maxnuminjections)
    {
        splitinjections = injconf.GetNumPulseSets() / maxnuminjections + 1;
        injconf.SetNumPulseSets(maxnuminjections);
        //numsignals = maxnuminjections * splitinjections;
    }
    config.StopInjections(false);
    config.ConfigureInjections(true);

    //fitting statistics:
    int goodfits  = 0;
    int totalfits = 0;

    PointCurve dummy;

    //Measurement Loop:
    for(int i = interval_start; i <= interval_end && *running; i += interval_step)
    {
        //set injection voltage:
        vdac.SetParameter("inject", static_cast<unsigned int>(i));
        config.SendUpdate(Configuration::vdac);
        std::cout << "." << std::flush;

        //config.ResetFastReadout(-1, false);
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);
        config.StopFastReadout(false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);
        config.ResetFastReadout(-1, false);
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500); //just as delay between reset and injection start
        config.StartFastReadout(true, false);//Ivan
        //config.GetNexysIO()->Write(0, NexysIO::Ld, false, 500);//Ivan
        Timing::Sleep(100);



                    //just as delay between reset and injection start

        std::vector<Dataset> hitlist;

        //set number of pulses per go to maximum if total number is bigger than
        //  maximum:
        if(splitinjections > 1)
        {
            injconf.SetNumPulseSets(maxnuminjections);
            config.ConfigureInjections(false);
        }
        //else not missing as for total number < maximum, no change in number is necessary

        for(int i = 0; i < splitinjections; ++i)
        {
            //do the remaining injections on last subset:
            if(i + 1 == splitinjections && splitinjections > 1)
            {
                injconf.SetNumPulseSets(uint(numsignals_backup - maxnuminjections * i));

                if(injconf.GetNumPulseSets() == 0)
                    break;

                config.ConfigureInjections(false);
            }

            config.RestartInjections(true);
            Timing::Sleep(10);

            //read the data from the FPGA:
            std::vector<Dataset> hits = config.ReadoutAll(nullptr, false,
                                                          static_cast<unsigned int>(numsignals) * 5,
                                                          ((i < splitinjections - 1)?2:10), 5);
            hitlist.insert(hitlist.end(), hits.begin(), hits.end());
        }

        //delete all hits from outside active area:
        std::vector<Dataset> newlist;
        for(auto& it : hitlist)
        {
            if(it.column >= activepixels.startcol && it.column <= activepixels.endcol
                && it.row >= activepixels.startrow && it.row <= activepixels.endrow)
                newlist.push_back(it);
        }
        //sort the remaining hits into histograms:
        std::map<Dataset, Histogram> newdata = SortInHistogram(newlist);

        std::stringstream svolt("");
        svolt << 1.8*i/255.;
        std::string voltage = svolt.str();
        //fit the histograms and add the resulting parameters (mean/sigma) to the point curves:
        for(auto& it : newdata)
        {
            //add missing signals as length "0":
            if(it.second.getIntegral() < numsignals)
                it.second.Fill(0, numsignals - it.second.getIntegral());

            if(Trimming::FitGaus(it.second, false))
            {
                ++goodfits;

                //Write histogram contents to a dedicated file:
                if(debug)
                {
                    it.second.setName(it.first.ToAddressString()+ " at " + voltage + " V");
                    WriteToFile(histfilename, it.second.Print());
                }

                //add the point to a pointcurve for the respective pixel:
                bool done = false;
                //search for the pointcurve of the pixel:
                for(auto& pcit : pointcurves)
                {
                    if(pcit.first == it.first)
                    {
                        pcit.second.AddPoint(1.8*i/255., it.second.GetMean(), it.second.GetSigma());
                        done = true;
                    }
                }
                //create a new curve if non exists already
                if(!done)
                {
                    PointCurve curve = dummy;
                    curve.AddPoint(1.8*i/255., it.second.GetMean(), it.second.GetSigma());
                    pointcurves.insert(std::make_pair(it.first, curve));
                }
            }
        }
        totalfits += newdata.size();

        config.ProcessEvents();

        dummy.AddPoint(1.8*i/255., 0, 0);
    }
    std::cout << "\n  fitting efficiency: " << goodfits / double(totalfits) << std::endl;

    //restore the back-uped configurations:
    config.SetInjectionConfig(injconf_backup);
    injconf_backup = nullptr;
    config.SetATLASPixConfig(Configuration::vdac, vdac_backup);
    vdac_backup = nullptr;

    numsignals = numsignals_backup;

    return pointcurves;
}

std::map<Dataset, PointCurve> ToTCalibration::MeasureToTArea(bool *running, int pixelsatonce,
                                                             Rect area, std::string filename,
                                                             std::string histfilename)
{
    if(running == nullptr || *running == false)
        return std::map<Dataset, PointCurve>();

    std::map<Dataset, PointCurve> result;

    int lastcol = (area.endcol > AP3columns)?AP3columns - 1:area.endcol;

    for(int row = area.startrow; row <= area.endrow && *running; ++row)
    {
        std::cout << "Starting Row " << row << " ..." << std::endl;

        Rect measurearea = area;
        measurearea.startrow = row;
        measurearea.endrow   = row;
        for(int col = area.startcol; col <= area.endcol && * running; col += pixelsatonce)
        {
            measurearea.startcol = col;
            measurearea.endcol   = col + pixelsatonce - 1;
            if(measurearea.endcol > lastcol)
                measurearea.endcol = lastcol;
            std::cout << "   columns " << measurearea.startcol << " - " << measurearea.endcol
                      << " ..." << std::endl;

            ConfigureInjections(row, true, measurearea.startcol, measurearea.endcol);

            auto partresult = MeasureToT(running, false, measurearea, histfilename);

            result.insert(partresult.begin(), partresult.end());

            //save the data during the measurement:
            if(filename != "")
            {
                for(auto& it : partresult)
                    WriteToFile(filename, it.second.GenerateString(it.first.ToAddressString(),
                                                                   false) + "\n");
            }

            config.ProcessEvents();
        }
    }
    if(*running)
        std::cout << "finished Area " << area.ToString() << std::endl;

    return result;
}

std::map<Dataset, Histogram> ToTCalibration::SortInHistogram(std::vector<Dataset> &data)
{
    if(data.size() == 0)
        return std::map<Dataset, Histogram>();

    std::map<Dataset, Histogram> container;

    for(auto& it : data)
    {
        bool done = false;
        for(auto& hist : container)
        {
            if(hist.first == it)
            {
                done = true;

                hist.second.Fill(it.GetToTValue(tsdiv, ts2div));
            }
        }
        if(!done)
        {
            Histogram hist;
            int div = (tsdiv<ts2div)?tsdiv:ts2div;
            int numbins = 1024;
            if(128 * ts2div < 1024 * tsdiv)
                numbins = 128;
            hist.addBins(numbins);
            hist.setBinWidth(div);
            hist.setFirstBinMean(0.);
            hist.setOrientation(true);

            hist.Fill(it.GetToTValue(tsdiv, ts2div));

            container.insert(std::make_pair(it, hist));
        }
    }

    return container;
}

bool ToTCalibration::WriteToFile(std::string filename, std::string data)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);

    if(!f.is_open())
        return false;

    f << data << std::flush;
    f.close();

    return true;
}

