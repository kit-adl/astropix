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

#include "configuration.h"

Configuration::Configuration() : nexys(nullptr), atlaspix_config(nullptr),
    atlaspix_dac(nullptr), atlaspix_vdac(nullptr), atlaspix_tdac(nullptr), atlaspix_row(nullptr),
    atlaspix_column(nullptr), atlaspix_sr_loads(nullptr), tdacs(nullptr), injection(nullptr),
    fastro(nullptr), fastro_lastread(""), fm_resets(nullptr), fm_configmode(nullptr),
    fm_workmode(nullptr), spiconfig(nullptr), cmdc(nullptr), progressbarblocked(false),
    waiter(nullptr)
{
    configmeans = new int(1);
    ownconfigmeans = true;
   //voltageboards[0] = nullptr;
    //voltageboards[1] = nullptr;
#ifdef _useQT_
    progress = nullptr;
    log      = nullptr;
#endif
}

Configuration::Configuration(const Configuration &ref)
{
    configmeans            = ref.configmeans;
    ownconfigmeans         = false;
    nexys                  = ref.nexys;
    atlaspix_config        = ref.atlaspix_config;
    atlaspix_dac           = ref.atlaspix_dac;
    atlaspix_vdac          = ref.atlaspix_vdac;
    atlaspix_tdac          = ref.atlaspix_tdac;
    atlaspix_column        = ref.atlaspix_column;
    atlaspix_row           = ref.atlaspix_row;

    atlaspix_sr_loads      = ref.atlaspix_sr_loads;
    tdacs                  = ref.tdacs;

    injection              = ref.injection;
    fastro                 = ref.fastro;
    fastro_lastread        = ref.fastro_lastread;
    fastro_clockshifts     = ref.fastro_clockshifts;
    fastro_clockspeed      = ref.fastro_clockspeed;
    fastro_triggersettings = ref.fastro_triggersettings;

    fm_resets              = ref.fm_resets;
    fm_configmode          = ref.fm_configmode;
    fm_workmode            = ref.fm_workmode;

    spiconfig              = ref.spiconfig;
    cmdc                   = ref.cmdc;

    voltageboards = ref.voltageboards;
    //voltageboards[0]       = ref.voltageboards[0];
    //voltageboards[1]       = ref.voltageboards[1];

    progressbarblocked     = ref.progressbarblocked;
#ifdef _useQT_
    progress               = ref.progress;
    log                    = ref.log;
#endif

    waiter                 = ref.waiter;
}

Configuration::~Configuration()
{
    if(ownconfigmeans)
        delete configmeans;
    configmeans            = nullptr;
    nexys                  = nullptr;
    atlaspix_config        = nullptr;
    atlaspix_dac           = nullptr;
    atlaspix_vdac          = nullptr;
    atlaspix_tdac          = nullptr;
    atlaspix_row           = nullptr;
    atlaspix_column        = nullptr;

    atlaspix_sr_loads      = nullptr;
    tdacs                  = nullptr;

    voltageboards.empty();
   // voltageboards[0]       = nullptr;
   // voltageboards[1]       = nullptr;
    injection              = nullptr;

    fastro                 = nullptr;
    //fastro_lastread        = "";
    fastro_clockshifts     = nullptr;
    fastro_clockspeed      = nullptr;
    fastro_triggersettings = nullptr;

    fm_resets              = nullptr;
    fm_configmode          = nullptr;
    fm_workmode            = nullptr;

    spiconfig              = nullptr;
    cmdc                   = nullptr;

    waiter                 = nullptr;


#ifdef _useQT_
    progress               = nullptr;
    log                    = nullptr;
#endif
}

Configuration Configuration::operator=(const Configuration &ref)
{
    if(ownconfigmeans)
        delete configmeans;
    configmeans            = ref.configmeans;
    ownconfigmeans         = false;
    nexys                  = ref.nexys;
    atlaspix_config        = ref.atlaspix_config;
    atlaspix_dac           = ref.atlaspix_dac;
    atlaspix_vdac          = ref.atlaspix_vdac;
    atlaspix_tdac          = ref.atlaspix_tdac;
    atlaspix_column        = ref.atlaspix_column;
    atlaspix_row           = ref.atlaspix_row;

    atlaspix_sr_loads      = ref.atlaspix_sr_loads;
    tdacs                  = ref.tdacs;

    injection              = ref.injection;
    fastro                 = ref.fastro;
    fastro_lastread        = ref.fastro_lastread;
    fastro_clockshifts     = ref.fastro_clockshifts;
    fastro_clockspeed      = ref.fastro_clockspeed;
    fastro_triggersettings = ref.fastro_triggersettings;

    fm_resets              = ref.fm_resets;
    fm_configmode          = ref.fm_configmode;
    fm_workmode            = ref.fm_workmode;

    spiconfig              = ref.spiconfig;
    cmdc                   = ref.cmdc;

     voltageboards = ref.voltageboards;
   // voltageboards[0]       = ref.voltageboards[0];
    //voltageboards[1]       = ref.voltageboards[1];

    progressbarblocked     = ref.progressbarblocked;
#ifdef _useQT_
    progress               = ref.progress;
    log                    = ref.log;
#endif

    waiter                 = ref.waiter;

    return ref;
}

void Configuration::setWaiter(WaitFunction newwaiter)
{
    this->waiter = newwaiter;
}

void Configuration::ProcessEvents()
{
    if(waiter != nullptr)
        waiter();
}

std::string Configuration::FindFileName(std::string filenameprefix, std::string filenamesuffix)
{
    std::fstream f;
    int fileindex = 0;
    do{
        f.close();
        std::stringstream s("");
        s << filenameprefix << ++fileindex << filenamesuffix;
        f.open(s.str().c_str(), std::ios::in);
    }while(f.is_open());

    std::stringstream s("");
    s << filenameprefix << fileindex << filenamesuffix;

    return s.str();
}

std::string Configuration::WriteToFile(std::string filename, std::string data)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    if(!f.is_open())
        return "";

    f << data;
    f.flush();
    f.close();

    return filename;
}


std::string Configuration::WriteToFile(std::string filenameprefix, std::string filenamesuffix, std::string data)
{
    std::fstream f;
    std::string filename = FindFileName(filenameprefix, filenamesuffix);
    f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);

    f << data;
    f.flush();
    f.close();

    return filename;
}

NexysIO* Configuration::GetNexysIO() const
{
    return nexys;
}

void Configuration::SetNexysIO(NexysIO* value)
{
    nexys = value;
}

ASIC_Config2* Configuration::GetATLASPixConfig(int flag)
{
    switch (flag)
    {
    case(dac):
        return atlaspix_dac;
    case(config):
        return atlaspix_config;
    case(vdac):
        return atlaspix_vdac;
    case(column):
        return atlaspix_column;
    case(row):
        return atlaspix_row;
    case(tdac):
        return atlaspix_tdac;
    case(srload):
        return atlaspix_sr_loads;
    default:
        return nullptr;
    }
}

bool Configuration::SetATLASPixConfig(int flag, ASIC_Config2 *config)
{
    switch (flag)
    {
    case(dac):
        atlaspix_dac = config;
        return true;
    case(configs::config):
        atlaspix_config = config;
        return true;
    case(vdac):
        atlaspix_vdac = config;
        return true;
    case(column):
        atlaspix_column = config;
        return true;
    case(row):
        atlaspix_row = config;
        return true;
    case(tdac):
        atlaspix_tdac = config;
        return true;
    case(srload):
        atlaspix_sr_loads = config;
        return true;
    default:
        return false;
    }
}

TDAC_Config *Configuration::GetTDACConfig()
{
    return tdacs;
}

void Configuration::SetTDACConfig(TDAC_Config *config)
{
    tdacs = config;
}

VB_Config *Configuration::GetVBConfig(unsigned int index)
{
    if (index<0 || index >= this->voltageboards.size()) {
        throw std::runtime_error("Index of VB Config out of range ");
    }

    return voltageboards[index];

}

bool Configuration::AddVBConfig( VB_Config *config) {
    this->voltageboards.push_back(config);
    return true;
}

bool Configuration::SetVBConfig(unsigned int index, VB_Config *config)
{

    if (index<0 || index >= this->voltageboards.size()) {
        //throw std::runtime_error("Index of VB Config out of range ");
        return false;
    }
    voltageboards[index] = config;
    return true;
    /*switch(index)
    {
    case(0):
        voltageboards[0] = config;
        return true;
    case(1):
        voltageboards[1] = config;
        return true;
    default:
        return false;
    }*/
}

Injection_Config *Configuration::GetInjectionConfig()
{
    return injection;
}

void Configuration::SetInjectionConfig(Injection_Config *config)
{
    injection = config;
}

FastReadout *Configuration::GetFastReadoutConfig()
{
    return fastro;
}

void Configuration::SetFastReadoutConfig(FastReadout *config)
{
    fastro = config;
}

FlagManager *Configuration::GetFastReadoutClockShiftConfig()
{
    return fastro_clockshifts;
}

void Configuration::SetFastReadoutClockShiftConfig(FlagManager *config)
{
    fastro_clockshifts = config;
}

FlagManager *Configuration::GetFastReadoutClockSpeedConfig()
{
    return fastro_clockspeed;
}

void Configuration::SetFastReadoutClockSpeedConfig(FlagManager *config)
{
    fastro_clockspeed = config;
}

FlagManager *Configuration::GetFastReadoutTriggerSettingConfig()
{
    return fastro_triggersettings;
}

void Configuration::SetFastReadoutTriggerSettingConfig(FlagManager *config)
{
    fastro_triggersettings = config;
}

FlagManager *Configuration::GetPinConfig(int flag)
{
    switch(flag)
    {
    case(resets):
        return fm_resets;
    case(modeconfig):
        return fm_configmode;
    case(modework):
        return fm_workmode;
    default:
        return nullptr;
    }
}

bool Configuration::SetPinConfig(int flag, FlagManager *config)
{
    switch(flag)
    {
    case(resets):
        fm_resets = config;
        return true;
    case(modeconfig):
        fm_configmode = config;
        return true;
    case(modework):
        fm_workmode = config;
        return true;
    default:
        return false;
    }
}

SPINexys *Configuration::GetSPIConfig()
{
    return spiconfig;
}

void Configuration::SetSPIConfig(SPINexys *config)
{
    spiconfig = config;
}

CMDConfig *Configuration::GetCMDConfig()
{
    return cmdc;
}

void Configuration::SetCMDConfig(CMDConfig *config)
{
    cmdc = config;
}

#ifdef _useQT_
QProgressBar *Configuration::GetProgressBar()
{
    return progress;
}

void Configuration::SetProgressBar(QProgressBar *pb)
{
    progress = pb;
}

QTextEdit *Configuration::GetLogWindow()
{
    return log;
}

void Configuration::SetLogWindow(QTextEdit *log)
{
    this->log = log;
}
#endif

int Configuration::GetProgressBarValue()
{
#ifdef _useQT_
    if(progress != nullptr)
        return progress->value();
    else
        return 0;
#else
    return 0;
#endif
}

void Configuration::SetProgressBarValue(int value)
{
#ifdef _useQT_
    if(progress != nullptr)
        progress->setValue(value);
#else
    (void) value;
#endif
}

void Configuration::SetProgressBarMaximum(int value)
{
#ifdef _useQT_
    if(progress != nullptr)
        progress->setMaximum(value);
#else
    (void) value;
#endif
}

bool Configuration::GetProgressBarBlocked()
{
    return progressbarblocked;
}

bool Configuration::BlockProgressBar()
{
    if(progressbarblocked)
        return false;
    else
    {
        progressbarblocked = true;
        return true;
    }
}

bool Configuration::ReleaseProgressBar()
{
    if(!progressbarblocked)
        return false;
    else
    {
        progressbarblocked = false;
        return true;
    }
}

void Configuration::logit(std::string text, std::string header, bool error)
{
    if(error)
        std::cerr << text << std::endl;
    else
        std::cout << text << std::endl;

#ifdef _useQT_
    if(log != nullptr)
    {
        static std::string localheader = "";

        QString buffer= QString::fromStdString(text);
        if(header.compare(localheader) != 0)
        {
            localheader = header;
            if(header != "")
                log->append(QString::fromStdString(header));
        }

        log->append(buffer);
        QTextCursor c =  log->textCursor();
        c.movePosition(QTextCursor::End);
        log->setTextCursor(c);
    }
#else
    (void) header;
#endif
}

int Configuration::GetConfigMeans()
{
    return *configmeans;
}

bool Configuration::SetConfigMeans(int means)
{
    if(means < shiftreg || means > cmd)
        return false;

    *configmeans = means;
    return true;
}

bool Configuration::SeparateConfigMeans(int newmeans)
{
    if(newmeans < shiftreg || newmeans > cmd)
        return false;

    if(ownconfigmeans)
        *configmeans = newmeans;
    else
    {
        ownconfigmeans = true;
        configmeans = new int(newmeans);
    }

    return true;
}

bool Configuration::SendASICConfigsViaSR(std::vector<ASIC_Config2*> configs, bool print) {

    if(!nexys->is_open()) {
        logit("Nexys is not opened");
         return false;
    }

    // Reverse the list of Configs since they must be written in reversed order in the ASIC
    //----------------
    std::reverse(configs.begin(),configs.end());

    // Setup
    //-------------

    //-- Length of config for progress
    size_t bitLength = std::accumulate(
                configs.begin(),
                configs.end(),
                0,
                [](const size_t previous, ASIC_Config2 * config) {
       return previous + config->GenerateBitVector().size();
    });

     logit(QString().asprintf("Number of bits in config: %llu",bitLength).toStdString());

    // Update Progress Bar
    //-----------
    if(!progressbarblocked) {
        SetProgressBarMaximum(int(bitLength));
        SetProgressBarValue(0);
        ProcessEvents();
    }

    // Loop over Configs
    // Send using neys interface
    //------------
    for (ASIC_Config2 * config : configs) {

        // Send
        //----------
        std::vector<bool> bits = config->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst);

        logit(QString().asprintf("- Writing %llu bits for %s",bits.size(),config->GetDeviceName().c_str()).toStdString());

        bool result = nexys->WriteASIC(
                    0x00,
                    bits,
                    false);

        if (!result) {

            logit(
               QString().asprintf("Error writing SR Config: %s",config->GetDeviceName().c_str())
                        .toStdString()
            );

            return false;
        }

        // Update Progress
        //------------
        if(!progressbarblocked)  {
            SetProgressBarValue(GetProgressBarValue() + bits.size());
            ProcessEvents();
        }

    }


    return true;
}

bool Configuration::SendUpdateViaSR(int configflags, bool print)
{
    if(!nexys->is_open() || configflags == 0)
        return false;

    unsigned int daclength = atlaspix_dac->GenerateBitVector().size();
    unsigned int configlength = atlaspix_config->GenerateBitVector().size();
    unsigned int vdaclength = atlaspix_vdac->GenerateBitVector().size();
    //unsigned int tdaclength = atlaspix_tdac->GenerateBitVector().size();
    unsigned int rowlength  = atlaspix_row->GenerateBitVector().size();
    unsigned int columnlength = atlaspix_column->GenerateBitVector().size();
    unsigned int total = 0;
    if(configflags & dac)
        total += daclength;
    if(configflags & config)
        total += configlength;
    if(configflags & vdac)
        total += vdaclength;
    if(configflags & row)
        total += rowlength;
    if(configflags & column)
        total += columnlength;
    //total += tdaclength;

    if(!progressbarblocked)
    {
        SetProgressBarMaximum(int(total));
        SetProgressBarValue(0);
        ProcessEvents();
    }

    bool result = true;

    if(configflags & dac)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_dac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(daclength));
            ProcessEvents();
        }
    }
    if(configflags & config)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_config->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(configlength));
            ProcessEvents();
        }
    }
    if(configflags & vdac)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_vdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(vdaclength));
            ProcessEvents();
        }
    }
    //result &= nexys->WriteASIC(0x00, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
    //if(!progressbarblocked)
    //{
    //    SetProgressBarValue(GetProgressBarValue() + int(tdaclength));
    //    ProcessEvents();
    //}
    if(configflags & row)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(rowlength));
            ProcessEvents();
        }
    }
    if(configflags & column)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_column->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(columnlength));
            ProcessEvents();
        }
    }

    if(print)
    {
        if(result && nexys->Flush())
            logit("Configurating chip successful.");
        else
            logit("Configuration of the chip failed...");
    }

    return result;
}

bool Configuration::SendUpdateNoSR(int configflags, bool print)
{
    if(!nexys->is_open() || configflags == 0)
        return false;

    unsigned int daclength = atlaspix_dac->GenerateBitVector().size();
    unsigned int configlength = atlaspix_config->GenerateBitVector().size();
    unsigned int vdaclength = atlaspix_vdac->GenerateBitVector().size();
    //unsigned int tdaclength = atlaspix_tdac->GenerateBitVector().size();
    unsigned int rowlength  = atlaspix_row->GenerateBitVector().size();
    unsigned int columnlength = atlaspix_column->GenerateBitVector().size();
    unsigned int total = 0;
    if(configflags & dac)
        total += daclength;
    if(configflags & config)
        total += configlength;
    if(configflags & vdac)
        total += vdaclength;
    if(configflags & column)
        total += columnlength;
    if(configflags & row)
        total += rowlength;
    //total += tdaclength;

    if(!progressbarblocked)
    {
        SetProgressBarMaximum(int(total));
        SetProgressBarValue(0);
        ProcessEvents();
    }
    bool result = true;

    if(configflags & dac)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_dac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                   NexysIO::SinA, NexysIO::Ld, false);
        result &= nexys->Write(16, 1 + 128, false, 20 * 8); //LdDAC & output enable
        result &= nexys->Write(16, 128, false, 8);          //LdDAC off & output enable
        result &= nexys->Write(16, 0, false, 8);            //disable output
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(daclength));
            ProcessEvents();
        }
    }
    if(configflags & config)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_config->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                   NexysIO::SinA, NexysIO::Ld, false);
        result &= nexys->Write(16, 2 + 128, false, 20 * 8);
        result &= nexys->Write(16, 128, false, 8);
        result &= nexys->Write(16, 0, false, 8);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(configlength));
            ProcessEvents();
        }
    }
    if(configflags & vdac)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_vdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                   NexysIO::SinA, NexysIO::Ld, false);
        result &= nexys->Write(16, 4 + 128, false, 20 * 8);
        result &= nexys->Write(16, 128, false, 8);
        result &= nexys->Write(16, 0, false, 8);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(vdaclength));
            ProcessEvents();
        }
    }
    //result &= nexys->WriteASIC(0x00, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
    //                            NexysIO::SinA, NexysIO::Ld, false);
    //result &= nexys->Write(16, 8 + 128, false, 20 * 8);
    //result &= nexys->Write(16, 128, false, 8);
    //result &= nexys->Write(16, 0, false, 8);
    //if(!progressbarblocked)
    //{
    //    SetProgressBarValue(GetProgressBarValue() + int(tdaclength));
    //    ProcessEvents();
    //}
    if(configflags & row)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                   NexysIO::SinA, NexysIO::Ld, false);
        result &= nexys->Write(16, 16 + 128, false, 20 * 8);
        result &= nexys->Write(16, 128, false, 8);
        result &= nexys->Write(16, 0, false, 8);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(rowlength));
            ProcessEvents();
        }
    }
    if(configflags & column)
    {
        result &= nexys->WriteASIC(0x00, atlaspix_column->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                   NexysIO::SinA, NexysIO::Ld, false);
        result &= nexys->Write(16, 32 + 128, false, 20 * 8);
        result &= nexys->Write(16, 128, false, 8);
        result &= nexys->Write(16, 0, false, 8);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + int(columnlength));
            ProcessEvents();
        }
    }

    if(print)
    {
        if(result && nexys->Flush())
            logit("Configurating chip successful.");
        else
            logit("Configuration of the chip failed...");
    }

    return result;
}

bool Configuration::SendUpdateSPI(int configflags, bool print)
{
    if(!nexys->is_open() || configflags == 0)
        return false;

    unsigned int daclength = atlaspix_dac->GenerateBitVector().size() - 3 * 16;
    unsigned int configlength = atlaspix_config->GenerateBitVector().size() - 3 * 16;
    unsigned int vdaclength = atlaspix_vdac->GenerateBitVector().size() - 3 * 16;
    //unsigned int tdaclength = atlaspix_tdac->GenerateBitVector().size() - 3 * 16;
    unsigned int rowlength  = atlaspix_row->GenerateBitVector().size() - 3 * 16;
    unsigned int columnlength = atlaspix_column->GenerateBitVector().size() - 3 * 16;
    unsigned int total = 0;
    if(configflags & dac)
        total += daclength;
    if(configflags & config)
        total += configlength;
    if(configflags & vdac)
        total += vdaclength;
    if(configflags & row)
        total += rowlength;
    if(configflags & column)
        total += columnlength;
    //total += tdaclength;

    if(!progressbarblocked)
    {
        SetProgressBarMaximum(int(total));
        SetProgressBarValue(0);
        ProcessEvents();
    }
    bool result = true;

#ifndef _useQT_
    void* progress = nullptr;
#endif

    void* disableprogress = nullptr;
    if(!progressbarblocked)
        disableprogress = progress;

    if(configflags & dac)
        result &= spiconfig->WriteASIC(nexys, atlaspix_dac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                      SPINexys::LdDAC, disableprogress, waiter);
    if(configflags & config)
        result &= spiconfig->WriteASIC(nexys, atlaspix_config->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                      SPINexys::LdConfig, disableprogress, waiter);
    if(configflags & vdac)
        result &= spiconfig->WriteASIC(nexys, atlaspix_vdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                      SPINexys::LdVDAC, disableprogress, waiter);
    //result &= spiconfig->WriteASIC(nexys, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
    //                                SPINexys::LdTDAC, disableprogress, waiter);
    if(configflags & row)
        result &= spiconfig->WriteASIC(nexys, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                        SPINexys::LdRow, disableprogress, waiter);
    if(configflags & column)
        result &= spiconfig->WriteASIC(nexys, atlaspix_column->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                        SPINexys::LdColumn, disableprogress, waiter);

    if(print)
    {
        if(result && nexys->Flush())
            logit("Configurating chip successful.");
        else
            logit("Configuration of the chip failed...");
    }

    return result;

}

bool Configuration::SendUpdateCMD(int configflags, bool print)
{
    if(!nexys->is_open() || configflags == 0)
        return false;

    //const int syncwordsperblock = 60;   //it is crucial to have this value > 50 or the
                                        //  written data in the chip will be corrupted!!!
    const int syncwordsperblock = 1;//ivan cmd



    bool result = true;
    std::vector<byte> cmd;

    int total = 0;
    if(configflags & config)
        ++total;
    if(configflags & dac)
        ++total;
    if(configflags & vdac)
        ++total;
    if(configflags & row)
        ++total;
    if(configflags & column)
        ++total;

    if(!progressbarblocked)
    {
        SetProgressBarMaximum(int(total));
        SetProgressBarValue(0);
        ProcessEvents();
    }

    if(configflags & config)
    {
        cmd = cmdc->GenerateRegWriteByteCode(16,
                    atlaspix_config->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                    CMDConfig::ldconfig, syncwordsperblock);
        result &= SendCMDSplit(cmd);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + 1);
            ProcessEvents();
        }
    }
    if(configflags & dac)
    {
        cmd = cmdc->GenerateRegWriteByteCode(16,
                    atlaspix_dac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                    CMDConfig::lddac, syncwordsperblock);
        result &= SendCMDSplit(cmd);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + 1);
            ProcessEvents();
        }
    }
    if(configflags & vdac)
    {
        cmd = cmdc->GenerateRegWriteByteCode(16,
                    atlaspix_vdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                    CMDConfig::ldvdac, syncwordsperblock);
        result &= SendCMDSplit(cmd);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + 1);
            ProcessEvents();
        }
    }
    if(configflags & row)
    {
        cmd = cmdc->GenerateRegWriteByteCode(16,
                    atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                    CMDConfig::ldrow, syncwordsperblock);
        result &= SendCMDSplit(cmd);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + 1);
            ProcessEvents();
        }
    }
    if(configflags & column)
    {
        cmd = cmdc->GenerateRegWriteByteCode(16,
                    atlaspix_column->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                    CMDConfig::ldcolumn, syncwordsperblock);
        result &= SendCMDSplit(cmd);
        if(!progressbarblocked)
        {
            SetProgressBarValue(GetProgressBarValue() + 1);
            ProcessEvents();
        }
    }

    if(print)
    {
        //nexys->Flush(); //the flush is already done by the Write() method
        if(result)
            logit("Configurating chip successful.");
        else
            logit("Configuration of the chip failed...");
    }

    return result;
}

bool Configuration::SendUpdate(int configflags, bool print)
{
    switch(*configmeans)
    {
    case(shiftreg):
        return SendUpdateViaSR(configflags, print);
    case(noshiftreg):
        return SendUpdateNoSR(configflags, print);
    case(spi):
        return SendUpdateSPI(configflags, print);
    case(cmd):
        return SendUpdateCMD(configflags, print);
    default:
        logit("called with unknown configuration method", "", true);
        return false;
    }
}

bool Configuration::SendCMDSplit(std::vector<byte> &cmd, bool flush)
{

/*
    if(((nexys->Read(50,1))[0] & 1) == 0)
    {
        logit("CMD Decoder Module not enabled. Aborting");
        return false;
    }
*/

    //send short commands at once (i.e. longer than the FIFO):
    //std::cout << "command size is " << cmd.size() << std::endl;

    if(cmd.size() < 8192){ //ivan cmd was 1024 8192
        if(flush) return nexys->Write(51, cmd, flush, 1);
        else{
            nexys->Write(51, cmd, flush, 1);
            return true;
        }
    }


    //split long commands
    else
    {
        bool result = true;
        std::vector<byte> cmd_part;
        //process 167 commands at once (1002 / 6 = 167):
        for(unsigned int loops = 0; loops < cmd.size(); loops += 1002)
        {
            //wait for complete processing of one part before sending the next:
            while(((nexys->Read(50, 1))[0] & 4) == 0)
                Timing::Sleep(20);

            std::cout << "preparing sending of part " << (loops/1002 + 1) << std::endl;

            cmd_part.clear();
            for(unsigned int i = 0; i < 1002 && loops + i < cmd.size(); ++i)
                cmd_part.push_back(cmd[i + loops]);

            result &= nexys->Write(51, cmd_part, true, 1);
        }

        return result;
    }
}


bool Configuration::SendCMDNoSplit(std::vector<byte> &cmd)
{


    //send short commands at once (i.e. longer than the FIFO):
    std::cout << "command size is " << cmd.size() << std::endl;
    nexys->Write(51, cmd, false, 1);

    return true;

}



bool Configuration::ConfigurePins(bool flush)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool status = true;
    status &= nexys->Write(static_cast<unsigned char>(fm_resets->GetFPGAAddress()),
                           byte(fm_resets->GetConfiguration()), false, 1);
    status &= nexys->Write(static_cast<unsigned char>(fm_configmode->GetFPGAAddress()),
                           byte(fm_configmode->GetConfiguration()), false, 1);
    status &= nexys->Write(static_cast<unsigned char>(fm_workmode->GetFPGAAddress()),
                           byte(fm_workmode->GetConfiguration()), flush, 1);

    return status;
}

bool Configuration::ConfigureInjections(bool flush)
{
    if(!nexys->is_open())
        return false;

    bool status = true;

    //signal strength:
    status &= nexys->WritePCB(NexysIO::FPGA_VOLTAGEBOARD_CONFIG, injection->GenerateBitVector(), 8);

    //sync and output settings:
    status &= nexys->Write(Injection_Config::OutputSyncAddress,
                           byte(injection->GetSynced() * Injection_Config::SyncFlag
                           + (injection->GetOutputChannel() & ~Injection_Config::SyncFlag)
                                + injection->GetTSOverflowSync() * 8));

    //clock divider:
    int clockdiv = int(injection->GetClockDiv());
    if(clockdiv > 0)
        --clockdiv;

    //timing, number of pulse sets:
    nexys->PatGen(int(injection->GetPeriod()), int(injection->GetNumPulseSets()),
                                clockdiv, int(injection->GetInitDelay()));

    //pulses in a set:
    status &= nexys->PatGenWrite(7, static_cast<unsigned char>(injection->GetNumPulsesInaSet()));

    if(flush)
        nexys->Flush();

    return status;
}

bool Configuration::StartInjections(bool flush)
{
    nexys->PatGenSuspend(1);
    nexys->PatGenReset(1);
    nexys->PatGenReset(0);
    nexys->PatGenSuspend(0);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

bool Configuration::RestartInjections(bool flush)
{
    nexys->PatGenReset(1);
    nexys->PatGenReset(0);
    nexys->PatGenSuspend(0);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

bool Configuration::StopInjections(bool flush)
{
    nexys->PatGenSuspend(true);
    nexys->PatGenReset(true);
    if(flush)
        return nexys->Flush();
    else
        return true;
}

bool Configuration::WriteRAMRow(int row, bool flush)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    //const int syncwordsperblock = 60;
    const int syncwordsperblock = 1;//cmd ivan

    //avoid writing RAM at low thresholds:
    int thr = atlaspix_vdac->GetParameter("Th");
    int bl  = atlaspix_vdac->GetParameter("BL");


    if(thr - bl < 25)
    {
        if(bl + 35 < 255)
            atlaspix_vdac->SetParameter("Th", uint(bl) + 35);
        else
            atlaspix_vdac->SetParameter("BL", uint(thr) - 35);

        SendUpdate(Configuration::vdac, false);
        if(*configmeans != cmd) nexys->Flush();
        Timing::Sleep(250);
    }


    //select the row of interest (and turn off all others):
    for(int i = 0; i < AP3rows; ++i)
    {
        std::stringstream s("");
        s << "enWrRAM_row_" << i;
        atlaspix_row->SetParameter(s.str(), (i == row)?1:0);
    }

    bool result = true;
#ifndef _useQT_
    void* progress = nullptr;
#endif
    void* disableprogress = nullptr;
    if(!progressbarblocked)
        disableprogress = progress;

    switch(*configmeans)
    {
        case(shiftreg):
            //logit("WrTDAC using SR");
            result &= nexys->WriteASIC(0, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
            result &= nexys->Flush();
        break;
        case(noshiftreg):
            //logit("WrTDAC NOT using SR");
            result &= nexys->WriteASIC(0, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                       NexysIO::SinA, NexysIO::Ld, false);
            result &= nexys->Write(16, 16 + 128, false, 8);
            result &= nexys->Write(16, 0, true, 8);
        break;
        case(spi):
            //logit("WrTDAC using SPI");
            result &= spiconfig->WriteASIC(nexys, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst,true),
                                           SPINexys::LdRow, nullptr, waiter); //disableprogress, waiter);
            result &= nexys->Flush();
        break;
        case(cmd):
        {
            std::vector<byte> data = cmdc->GenerateRegWriteByteCode(16, atlaspix_row->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                                                          CMDConfig::ldrow, syncwordsperblock);
            result &= SendCMDSplit(data, true);//can be false
        }
        break;
        default:
            logit("TDAC writing not implemented for this configuration version");
            return false;
    }

    if(!result)
    {
        logit("Writing RAM not successful");
        return false;
    }

    bool cmd_activatedramen = false;
    if(*configmeans == cmd && cmdc->GetConfigBit("wrramen") == 0)
    {
        cmd_activatedramen = true;
        cmdc->SetConfigBit("wrramen", false);
        std::vector<byte> data = cmdc->GenerateSetBitByteCode(16, 1, 1);
        result &= SendCMDSplit(data, true);//becuase of symetry
    }

    for(int i = 1; i < 16; i <<= 1)
    {
        for(unsigned int col = 0; col < AP3columns; ++col)
        {
            std::stringstream s("");
            s << "TDAC_in_col_" << col;
            atlaspix_tdac->SetParameter(s.str(), (((tdacs->GetTDACValue(col,static_cast<unsigned int>(row)) & i) != 0)?1:0));
        }

        switch(*configmeans)
        {
            case(shiftreg):
                result &= nexys->WriteASIC(0, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, false));
                result &= nexys->Write(0, NexysIO::Ld, false, 100);
                //send WrRAM signal:
                atlaspix_sr_loads->SetParameter("WrRam", static_cast<unsigned int>(i));
                result &= nexys->ConfigureLoad(0, atlaspix_sr_loads->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst), true);
                //result &= nexys->Flush();
            break;
            case(noshiftreg):
                result &= nexys->WriteASIC(0, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                           NexysIO::SinA, NexysIO::Ld, false, 4);
                //send load for TDAC shift register:
                result &= nexys->Write(16, 8 + 128, false, 8);
                result &= nexys->Write(16, 128, false, 4);  //keep the outputs enabled to enable the WrRAM signal to be sent
                result &= nexys->Write(0, 0, false, 100); //pause to give the system setup time
                //send WrRAM signal:
                result &= nexys->Write(17, byte(i), false, 8);
                result &= nexys->Write(17, 0, false, 4);
            break;
            case(spi):
                result &= spiconfig->WriteASIC(nexys,atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                               SPINexys::LdTDAC, nullptr, waiter); //disableprogress, waiter);
                //send load pulse:
                result &= spiconfig->Write(nexys, SPINexys::WriteRAM0 * i);
                result &= spiconfig->Write(nexys, 0);
            break;
            case(cmd):
            {
                std::vector<byte> data = cmdc->GenerateRegWriteByteCode(16, atlaspix_tdac->GenerateBitVector(ASIC_Config2::GlobalInvertedLSBFirst, true),
                                                                        CMDConfig::ldtdac, syncwordsperblock);
                result &= SendCMDSplit(data, true);//last this was important

                std::string fieldname = "wrram";
                switch(i)
                {
                    case(1): fieldname += "0"; break;
                    case(2): fieldname += "1"; break;
                    case(4): fieldname += "2"; break;
                    case(8): fieldname += "3"; break;
                }
                if(!cmdc->SetConfigBit(fieldname, true))
                    std::cerr << "CMD RAM Write: denominator wrong" << std::endl;
                data = cmdc->GenerateSetBitByteCode(16, 1, 1);
                result &= SendCMDSplit(data, true);//this was also important for last columns
                cmdc->SetConfigBit(fieldname, false);
                data = cmdc->GenerateSetBitByteCode(16, 1, 1);
                result &= SendCMDSplit(data, true);




                //AddSyncWord(vec, syncspacing);




            }
            break;
            default:
                logit("TDAC writing not implemented for this configuration version");
                return false;
        }

    }
    if(*configmeans == noshiftreg)
        result &= nexys->Write(16, 0 , false, 8); //turn off outputs (to high impedance state)
    else if(cmd_activatedramen)
    {
        cmdc->SetConfigBit("wrramen", false);
        std::vector<byte> data = cmdc->GenerateSetBitByteCode(16, 1, 1);
        result &= SendCMDSplit(data, true);
    }

    //nexys->Flush();

    if(flush)//ivan cmd
        if(*configmeans != cmd) result &= nexys->Flush();


    //prevent accidental RAM writing by turning off again the wrRAM flags:
    for(int i = 0; i < AP3rows; ++i)
    {
        std::stringstream s("");
        s << "enWrRAM_row_" << i;
        atlaspix_row->SetParameter(s.str(), 0);
    }

    //recover the old threshold settings after writing RAM:


    if(atlaspix_vdac->GetParameter("Th") != thr || atlaspix_vdac->GetParameter("BL") != bl)
    {
        atlaspix_vdac->SetParameter("Th", uint(thr));
        atlaspix_vdac->SetParameter("BL", uint(bl));

        SendUpdate(Configuration::vdac);
        if(*configmeans != cmd) nexys->Flush();
        Timing::Sleep(250);
    }


    if(!result)
        logit("Writing RAM not successful");
    //else
    //    logit("RAM written for row " + QString::number(row).toStdString());

    return result;
}

bool Configuration::WriteRAMMatrix()
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    //avoid writing RAM at low thresholds:
    int thr = atlaspix_vdac->GetParameter("Th");
    int bl  = atlaspix_vdac->GetParameter("BL");
    if(thr - bl < 25)
    {
        if(bl + 35 < 255)
            atlaspix_vdac->SetParameter("Th", uint(bl) + 35);
        else
            atlaspix_vdac->SetParameter("BL", uint(thr) - 35);

        SendUpdate(Configuration::vdac, false);
        if(*configmeans != cmd) nexys->Flush();
        Timing::Sleep(250);
    }

    bool result = true;
    if(!progressbarblocked)
    {
        SetProgressBarMaximum(AP3rows);
        SetProgressBarValue(0);
    }
    //enable RAM writing for CMD decoder (if not already on):
    bool cmd_ramen_activated = false;
    if(*configmeans == cmd && cmdc->GetConfigBit("wrramen") == 0)
    {
        cmd_ramen_activated = true;
        cmdc->SetConfigBit("wrramen", true);
        std::vector<byte> data = cmdc->GenerateSetBitByteCode(16, 1, 1);
        result &= SendCMDSplit(data);
    }

    for(int i = 0; i < AP3rows && result; ++i)
    {
        result &= WriteRAMRow(i, false);
        if(!progressbarblocked)
        {
            SetProgressBarValue(i);
            ProcessEvents();
        }

        if(*configmeans == Configuration::spi)
            std::cout << "\rwriting row " << i;
    }

    if(*configmeans == Configuration::spi)
        std::cout << std::endl;
    //deactivate CMD RAM writing if it was turned on for this method:
    else if(cmd_ramen_activated)
    {
        cmdc->SetConfigBit("wrramen", false);
        std::vector<byte> data = cmdc->GenerateSetBitByteCode(16, 1, 1);
        result &= SendCMDSplit(data);
    }

    if(*configmeans != cmd) result &= nexys->Flush();

    //recover the old settings after writing RAM:
    if(atlaspix_vdac->GetParameter("Th") != thr || atlaspix_vdac->GetParameter("BL") != bl)
    {
        atlaspix_vdac->SetParameter("Th", uint(thr));
        atlaspix_vdac->SetParameter("BL", uint(bl));

        SendUpdate(Configuration::vdac);
        if(*configmeans != cmd) nexys->Flush();
        Timing::Sleep(250);
    }

    if(!progressbarblocked)
        SetProgressBarValue(AP3rows);

    return result;
}

bool Configuration::ConfigureVoltageBoards(bool flush)
{
    if(nexys == nullptr || !nexys->is_open()) {
        logit("Configuration Nexys is not set or not opened");
        return false;
    }

    // Clock Div is a slowing parameter to define the length of the clock pulses for Shif Registers
    const int clockdiv = 8;

    // Old code wih fixed 2 voltage boards
    //---------
    bool status = true;

    //status &= nexys->WritePCB(NexysIO::FPGA_VOLTAGEBOARD_CONFIG, voltageboards[0]->GenerateBitVector(), clockdiv);
   // status &= nexys->WritePCB(NexysIO::FPGA_VOLTAGEBOARD_CONFIG, voltageboards[1]->GenerateBitVector(), clockdiv);
    for (VB_Config * vb : this->voltageboards) {

       // logit((QString().asprintf("Writing Voltage board: %s",vb->GetDeviceName().c_str())).toStdString());
        std::vector<bool> bits = vb->GenerateBitVector();
        std::cout << "Done Generating Bit Vector ( "<< bits.size() <<")" << std::endl;
       // vb->GetEntries()

        // Write PCB generates the SR Clocking of the provided bits
        status &= this->nexys->WritePCB(NexysIO::FPGA_VOLTAGEBOARD_CONFIG, bits, clockdiv);
    }


    // Flus all bytes to FPGA now?
    //---------------
    if(flush)
        status &= nexys->Flush();

    return status;
}

bool Configuration::ConfigureFastReadout(bool flush)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool status = true;
    //also send the other parameters to make sure they are as set in the GUI:
    status &= nexys->Write(FastReadout::TrigDelayAddress, byte(fastro->GetTriggerDelay()),false);
    status &= nexys->Write(FastReadout::TrigLengthAddress, byte(fastro->GetTriggerLength()), false);

    //clock divider from chip configuration:
    status &= nexys->Write(FastReadout::TSDiv, byte(atlaspix_config->GetParameter("ckdivend")), false);
    status &= nexys->Write(FastReadout::TS2Div, byte(atlaspix_config->GetParameter("ckdivend2")), false);
    status &= nexys->Write(FastReadout::TSPhase, byte(fastro->GetTSPhase()), true);
    if(flush)
        status &= nexys->Flush();

    return status;
}

bool Configuration::StartFastReadout(bool flush, bool configure)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;

    bool status = true;
    if(configure)
        status &= ConfigureFastReadout(false);
    status &= nexys->Write(FastReadout::FPGAAddress, byte(fastro->SetEnabled(true)));
    if(flush)
        status &= nexys->Flush();

    return status;
}

bool Configuration::StopFastReadout(bool flush)
{
    if(nexys == nullptr || !nexys->is_open())
        return false;
    bool status = true;
    status &= nexys->Write(FastReadout::FPGAAddress, byte(fastro->SetEnabled(false)));;
    if(flush)
        status &= nexys->Flush();

    return status;
}

bool Configuration::ResetFastReadout(int flags, bool flush)
{
    fastro_lastread = "";

    if((fastro->GetConfiguration(true) & (FastReadout::reset + FastReadout::fifoclear)) == 0)
    {
        logit("Nothing to do here. Quitting...");
        return true;
    }

    std::vector<byte> reset;

    if(flags == -1)
        reset.push_back(byte(fastro->GetConfiguration(true)));
    else
        reset.push_back(byte(fastro->GetConfiguration(false) | flags));
    reset.push_back(byte(fastro->GetConfiguration(false)));

    bool status = true;
    status &= nexys->Write(FastReadout::FPGAAddress, reset, flush, 4);

    return status;
}

std::vector<Dataset> Configuration::ReadoutAll(bool *running, bool print, unsigned int maxnumhits,
                                               int maxemptyreads, int readsize, int maxtime)
{
    bool localrunner = true;
    if(running == nullptr)
        running = &localrunner;

    if(nexys == nullptr || !nexys->is_open() || !(*running) || readsize <= 0)
        return std::vector<Dataset>();

    if(print)
    {
        logit("Reading Data ...");
        ProcessEvents();
    }

    std::vector<Dataset> hits;
    std::map<Dataset, unsigned int> hitcnter;
    int counter = 0;

    bool datamux   = fastro->GetDataMuxEnable();
    bool triggered = fastro->GetReadoutMode();
    std::string answer = "";

    int endcondition = (datamux)?3:12;

    Timing::TimePoint start = Timing::GetTimeNow();

    while(*running && counter < maxemptyreads)
    {
        //answer += nexys->Read(NexysIO::FPGA_READOUT_FIFO, 256 * readsize);//ivan 128 * readsize 12240

        //ivan

        std::string line = nexys->Read(NexysIO::FPGA_READOUT_FIFO, 512);//was 8
        int abortcounter = 0;

        //
        while((line.c_str()[0] & 0xff) != 0xff && abortcounter < 1024)//stop at ff Ivan 2048
        {
            answer = answer + line;
            line = nexys->Read(NexysIO::FPGA_READOUT_FIFO, 512);//was 8
            abortcounter ++;

        }


        //ivan



        answer = FastReadout::RemoveEmptyData(answer);
        std::vector<Dataset> newhits;
        if(answer != "")
        {
            if(triggered)
            {
                newhits = fastro->DecodeManyTrigger(answer);
                answer = "";
            }
            else
            {
                answer = FastReadout::RemoveEmptyData(answer);
                newhits = FastReadout::DecodeMany(datamux, answer);
                unsigned int linestart = (answer.length() / 8 - 1) * 8; //start of the last line in the data
                //last dataset is complete:
                if(answer.c_str()[linestart] == endcondition)
                    answer = "";
                //remove all but the last (incomplete) dataset:
                else
                {
                    for(unsigned int i = 1; i < 10; ++i)
                    {
                        if(int(linestart) - int(i * 8) >= 0 && answer.c_str()[linestart - i * 8] == endcondition)
                        {
                            answer = answer.substr(linestart - (i-1) * 8);
                            break;
                        }
                    }
                    if(answer.length() == linestart + 7)
                        answer = "";
                }
            }

            //add the newly read hits to the list of read hits:
            hits.insert(hits.end(), newhits.begin(), newhits.end());
            //insert them into the hit counter:
            for(auto& it : newhits)
            {
                auto elem = hitcnter.find(it);
                if(elem != hitcnter.end())
                    ++(elem->second);
                else
                    hitcnter.insert(std::make_pair(it, 1));
            }
            //abort on too many hits:
            for(auto& it : hitcnter)
            {
                if(it.second > maxnumhits)
                    break;
            }
            //if(hits.size() > maxnumhits && maxnumhits > 0)
            //    break;
            //abort on reading longer than 10s:
            if(Timing::TimesToIntervalMS(start, Timing::GetTimeNow()) > maxtime)
                break;
        }

        //no data counter:
        if(newhits.size() == 0)
            ++counter;
        else
        {
            counter = 0;
            if(print)
                ProcessEvents();
        }
    }

    if(print)
    {
        std::stringstream s("");
        s << "  -> in total: " << hits.size() << " hits" << std::endl;
        logit(s.str());
    }

    return hits;
}

std::map<Dataset, int> Configuration::SortHits(const std::vector<Dataset> &data, bool print, bool IDreject)
{
    std::map<Dataset, int> sortedhits;
    std::map<Dataset, int> lastids;     //for storing the last IDs for the pixel to reject double hits
    for(const auto& it : data)
    {
        auto entry = sortedhits.find(it);
        auto identry = lastids.find(it);

        if(entry != sortedhits.end())
        {
            if(!IDreject || it.triggerid != identry->second)
                ++(entry->second);
            identry->second = it.triggerid;
        }
        else
        {
            sortedhits.insert(std::make_pair(it, 1));
            lastids.insert(std::make_pair(it, it.triggerid));
        }
    }

    if(print)
    {
        std::stringstream s("");
        s << Dataset::GetStringHeader(true) << ";\t-> Count" << std::endl;
        int integral = 0;
        for(const auto& it : sortedhits)
        {
            s << it.first.ToString(true) << "\t-> " << it.second << std::endl;
            integral += it.second;
        }
        s << "  -> in total: " << integral << " hits" << std::endl;

        logit(s.str());
    }

    return sortedhits;
}

bool Configuration::WriteHitsToFile(const std::vector<Dataset> &hits, std::string filename)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::app);
    if(!f.is_open())
        return false;

    std::stringstream s("");
    for(const auto& it : hits)
        s << it.ToString(true) << std::endl;

    f << s.str();
    f.flush();

    f.close();
    return true;
}
