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

#include "histogramm.h"

Histogramm::Histogramm(int bins, double minbin, double binsize, QLabel* label) : printlevel(0)
{
    this->bins = bins;
    for(int i=0;i<bins;i++)
        data.push_back(0);
    this->label = label;
    this->minbin = minbin;
    this->binsize = binsize;
}

void Histogramm::Clear()
{
    for(int i=0;i<bins;i++)
        data[i]=0;
}

bool Histogramm::Fill(int index)
{
    if(0<=index&&index<bins)
    {
        ++data[index];
        return true;
    }
    return false;
}

bool Histogramm::Fill(double position)
{
    return Fill((int)((position-minbin)/binsize));
}

void Histogramm::SetBin(int index, int value)
{
    if(0<=index&&index<bins)
        data[index]=value;
}

void Histogramm::SetBin(double position, int value)
{
    SetBin((int)((position-minbin)/binsize), value);
}

int Histogramm::GetBin(int index)
{
    if(0<=index&&index<bins)
        return data[index];
    else
        return 0;
}

int Histogramm::GetMax()
{
    int max=0;
    for(int i=0;i<bins;i++)
        max=(data[i]>max)?data[i]:max;

    if(max == 0)
        max = 1;
    return max;
}

int Histogramm::GetMaxBin()
{
    int max=0;
    int maxbin;
    for(int i=0;i<bins;i++)
    {
        if (data[i] > max)
        {
            max = data[i];
            maxbin = i;
        }
    }
    if(max == 0)
        maxbin = 1;
    return maxbin;
}

bool Histogramm::save(std::string filename, std::string title)
{
    std::fstream Output(filename.c_str(),std::ios::out|std::ios::app);

    if (!Output.is_open())
        return false;

    if(title != "")
        Output << "# " << title << std::endl;

    Output.precision(6);
    Output.setf(std::ios::fixed, std::ios::floatfield);

    for(int i = 0; i<bins; i++)
    {
        Output << (int)i << "\t"<< (int)data[i] << std::endl;
    }
    Output << std::endl;

    Output.close();

    return true;
}

void Histogramm::SetBinsize(double binsize)
{
    this->binsize = binsize;
}

double Histogramm::GetBinsize()
{
    return binsize;
}

double Histogramm::GetMinBin()
{
    return minbin;
}

void Histogramm::SetLabel(QLabel *label)
{
    this->label = label;
}

int Histogramm::getEntries()
{
    int retval = 0;
    for (auto it: data)
        retval += it;
    return retval;
}

int Histogramm::GetBins()
{
    return bins;
}

#ifdef ALGLIB

double pi = 3.14159265;

double EvalGaus(double x, double sigma, double mu, double amp, double yoffset)
{
    return yoffset + amp*(exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi));
}

void gaus_fvec(const alglib::real_1d_array &var, alglib::real_1d_array &fi, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[i] = EvalGaus(it->first, var[0], var[1], var[2], var[3]) - it->second;
        ++it;
    }
}
void gaus_jac(const alglib::real_1d_array &var, alglib::real_1d_array &fi, alglib::real_2d_array &jac, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        double x = it->first;
        double sigma = var[0];
        double mu = var[1];
        double amp = var[2];
        double yoffset = var[3];
        fi[i] = EvalGaus(x, sigma, mu, amp, yoffset) - it->second;

        jac[i][0] = amp*((exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi)))*
                (pow(mu,2)-pow(sigma,2)+pow(x,2)-2*mu*x)/
                (pow(sigma,4)*sqrt(2.0*pi));
        jac[i][1] = amp*(x-mu)*
                ((exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi)))/
                (pow(sigma,3)*sqrt(2.0*pi));
        jac[i][2] = (exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi));
        jac[i][3] = 1;
        ++it;
    }
}

void Histogramm::FitGaus(double ignorefrac, bool print)
{
    std::map<double, double>* curve = new std::map<double, double>();
    double i = 0;
    int maxpos = 0;
    double max = 0;
    for (auto it: data)
    {
        if (i< ignorefrac*bins)
            curve->insert(std::make_pair(i, 0));
        else
            curve->insert(std::make_pair(i, it));

        if (max<it)
        {
            max = it;
            maxpos = i;
        }

        if (print)
            std::cout << i << "\t" << it << std::endl;
        i++;
    }


    //set starting values:
    alglib::real_1d_array var;
    var.setlength(4);
    var[0] = 0.05*bins;   //sigma
    var[1] = maxpos; //mu
    var[2] = max; // amp
    var[3] = 0; //yoffset

    alglib::ae_int_t maxits = 1000;

    //containers for state and result:
    alglib::minlmstate  state;
    alglib::minlmreport report;

    //fitting:
    alglib::minlmcreatev(curve->size(), var, 1.e-4, state);
    alglib::minlmsetcond(state, 1e-10, maxits);
    //alglib::minlmsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlmoptimize(state, gaus_fvec, gaus_jac, NULL, static_cast<void*>(curve));
    alglib::minlmresults(state, var, report);

    //write out information about the fit:
    if(1)
    {
        std::cout << "Fit Results:\n"
                  << "    Iterations:   " << report.iterationscount << std::endl
                  << "    Termination:  " << int(report.terminationtype) << std::endl
                  << "    Parameters:   sigma = " << var[0] << "; mu = " << var[1] << "; amp = " << var[2] << "; yoffset = " << var[3] << std::endl;
    }
    this->gaus_sigma = var[0];
    this->gaus_mu = var[1];
    this->gaus_amplitude = var[2];
    this->gaus_yoffset = var[3];

    delete curve;
}

double EvalGaus2(double x, double sigma, double mu, double amp, double yoffset, double sigma2, double mu2, double amp2)
{
    return yoffset + amp*(exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi)) + amp2*(exp(-0.5*pow((x-mu2)/sigma2,2.0)))/(sigma2*sqrt(2.0*pi));
}

void gaus2_fvec(const alglib::real_1d_array &var, alglib::real_1d_array &fi, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        fi[i] = EvalGaus2(it->first, var[0], var[1], var[2], var[3], var[4], var[5], var[6]) - it->second;
        ++it;
    }
}
void gaus2_jac(const alglib::real_1d_array &var, alglib::real_1d_array &fi, alglib::real_2d_array &jac, void *ptr)
{
    std::map<double, double>* fitcurve = static_cast<std::map<double, double>* >(ptr);

    auto it = fitcurve->begin();
    for(unsigned int i = 0; i < fitcurve->size(); ++i)
    {
        double x = it->first;
        double sigma = var[0];
        double mu = var[1];
        double amp = var[2];
        double yoffset = var[3];
        double sigma2 = var[4];
        double mu2 = var[5];
        double amp2 = var[6];
        fi[i] = EvalGaus2(x, sigma, mu, amp, yoffset, sigma2, mu2, amp2) - it->second;

        jac[i][0] = amp*((exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi)))*
                (pow(mu,2)-pow(sigma,2)+pow(x,2)-2*mu*x)/
                (pow(sigma,4)*sqrt(2.0*pi));
        jac[i][1] = amp*(x-mu)*
                ((exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi)))/
                (pow(sigma,3)*sqrt(2.0*pi));
        jac[i][2] = (exp(-0.5*pow((x-mu)/sigma,2.0)))/(sigma*sqrt(2.0*pi));
        jac[i][3] = 1;
        jac[i][4] = amp2*((exp(-0.5*pow((x-mu2)/sigma2,2.0)))/(sigma2*sqrt(2.0*pi)))*
                (pow(mu2,2)-pow(sigma2,2)+pow(x,2)-2*mu2*x)/
                (pow(sigma2,4)*sqrt(2.0*pi));
        jac[i][5] = amp2*(x-mu2)*
                ((exp(-0.5*pow((x-mu2)/sigma2,2.0)))/(sigma2*sqrt(2.0*pi)))/
                (pow(sigma2,3)*sqrt(2.0*pi));
        jac[i][6] = (exp(-0.5*pow((x-mu2)/sigma2,2.0)))/(sigma2*sqrt(2.0*pi));
        ++it;
    }
}

void Histogramm::Fit2Gaus(double ignorefrac, bool print)
{
    std::map<double, double>* curve = new std::map<double, double>();
    double i = 0;
    for (auto it: data)
    {
        if (i< ignorefrac*bins)
            curve->insert(std::make_pair(i, 0));
        else
            curve->insert(std::make_pair(i, it));
        if (print)
            std::cout << i << "\t" << it << std::endl;
        i++;
    }
    FitGaus(ignorefrac,0);


    //set starting values:
    alglib::real_1d_array var;
    var.setlength(7);
    var[0] = this->gaus_sigma;   //sigma
    var[1] = this->gaus_mu*0.95; //mu
    var[2] = this->gaus_amplitude; // amp
    var[3] = this->gaus_yoffset; //yoffset
    var[4] = this->gaus_sigma; //sigma2
    var[5] = this->gaus_mu*1.05; // mu2
    var[6] = this->gaus_amplitude/3; //amp2

    alglib::ae_int_t maxits = 1000;

    //containers for state and result:
    alglib::minlmstate  state;
    alglib::minlmreport report;

    //fitting:
    alglib::minlmcreatev(curve->size(), var, 1.e-4, state);
    alglib::minlmsetcond(state, 1e-10, maxits);
    //alglib::minlmsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlmoptimize(state, gaus2_fvec, gaus2_jac, NULL, static_cast<void*>(curve));
    alglib::minlmresults(state, var, report);

    //write out information about the fit:
    if(1)
    {
        std::cout << "Fit Results:\n"
                  << "    Iterations:   " << report.iterationscount << std::endl
                  << "    Termination:  " << int(report.terminationtype) << std::endl
                  << "    Parameters:   sigma = " << var[0]*binsize << "; mu = " << var[1]*binsize << "; amp = " << var[2] << "; yoffset = " << var[3] << std::endl
                  << "    Parameters:   sigma2 = " << var[4]*binsize << "; mu2 = " << var[5]*binsize << "; amp2 = " << var[6] << std::endl;
    }
    this->gaus_sigma = var[0];
    this->gaus_mu = var[1];
    this->gaus_amplitude = var[2];
    this->gaus_yoffset = var[3];
    this->gaus2_sigma = var[4];
    this->gaus2_mu = var[5];
    this->gaus2_amplitude = var[6];

    delete curve;
}

#endif

void Histogramm::draw(bool drawgaus)
{
    //The drawing of the histogram is stored on a QPixmap and then transfered to a label
    QPixmap myPix(label->size());
    double width = label->size().width();
    double height = label->size().height();
    QPainter painter(&myPix);

    //Draw a white rectangle as background
    painter.setBrush( Qt::white );
    painter.drawRect(-1,-1,width+1, height+1);

    //find the bin with max value for normation
    double norm=0;
    for (auto it : data)
    {
        if (it > norm)
            norm = it;
    }

    //Draw histogram
    width -=10;
    height-=10;
    int value, lastvalue = height+5;

    for (int i = 0; i < bins; i++)
    {
        value = (height+5)-((height)/norm*data[i]);
        painter.drawLine(5+((width)/bins)*i, lastvalue, 5+((width)/bins)*i, value);
        painter.drawLine(5+((width)/bins)*i, value, 5+((width)/bins)*(i+1),value);
        lastvalue = value;
    }

    if(printlevel > 0)
    {
        std::cout << "MinValue          = " << minbin               << std::endl
                  << "MaxValue          = " << minbin+binsize*bins  << std::endl
                  << "Number of bins    = " << bins                 << std::endl
                  << "Number of entries = " << getEntries()         << std::endl;
    }

#ifdef ALGLIB
    //draw gaussian if requested and available
    if (gaus_amplitude != 0 && drawgaus)
    {
        double last_y, y, last_x, x;
        painter.setPen(QPen(Qt::red,2));
        for (int i = 0; i< label->size().width()-9;i++)
        {
            x = i;
            y = EvalGaus(x*(1.0*(bins-1))/(label->size().width()-10),gaus_sigma,gaus_mu,gaus_amplitude, gaus_yoffset);
            //std::cout << x << " " << y << std::endl;
            if (i==0)
            {
                last_y = y;
                last_x = x;

            }
            painter.drawLine(last_x+5 , height+5-(last_y/norm*height), x+5, height+5-(y/norm*height));
            last_y = y;
            last_x = x;
        }
    }
    if (gaus2_amplitude != 0 && drawgaus)
    {
        double last_y, y, last_x, x;
        painter.setPen(QPen(Qt::red,2));
        for (int i = 0; i< label->size().width()-9;i++)
        {
            x = i;
            y = EvalGaus(x*(1.0*(bins-1))/(label->size().width()-10),gaus2_sigma,gaus2_mu,gaus2_amplitude, gaus_yoffset);
            //std::cout << x << " " << y << std::endl;
            if (i==0)
            {
                last_y = y;
                last_x = x;

            }
            painter.drawLine(last_x+5 , height+5-(last_y/norm*height), x+5, height+5-(y/norm*height));
            last_y = y;
            last_x = x;
        }
    }
    if (gaus_amplitude != 0 && gaus2_amplitude != 0 && drawgaus)
    {
        double last_y, y, last_x, x;
        painter.setPen(QPen(Qt::blue,2));
        for (int i = 0; i< label->size().width()-9;i++)
        {
            x = i;
            y = EvalGaus2(x*(1.0*(bins-1))/(label->size().width()-10),gaus_sigma,gaus_mu,gaus_amplitude, gaus_yoffset,gaus2_sigma,gaus2_mu,gaus2_amplitude);
            //std::cout << x << " " << y << std::endl;
            if (i==0)
            {
                last_y = y;
                last_x = x;

            }
            painter.drawLine(last_x+5 , height+5-(last_y/norm*height), x+5, height+5-(y/norm*height));
            last_y = y;
            last_x = x;
        }
    }
#else
    (void) drawgaus;
#endif

    //transfer Pixmap to label
    label->setPixmap(myPix);
}

int Histogramm::GetPrintLevel()
{
    return printlevel;
}

void Histogramm::SetPrintLevel(int level)
{
    printlevel = level;
}

