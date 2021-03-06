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

#include "fastreadout.h"

FastReadout::FastReadout(int config)
{
    configuration.SetIdentifier("chip");
    configuration.AddFlag("enabled", config & enabled);
    configuration.AddFlag("fifoclear", config & fifoclear);
    configuration.AddFlag("reset", config & reset);
    configuration.AddFlag("trigger", config & trigger);
    configuration.AddFlag("debug", config & debug);
    configuration.AddFlag("datamux", config & datamux);
    configuration.AddFlag("realign", config & realign, true);

    nonfpgaconfig.SetIdentifier("fpga");
    nonfpgaconfig.AddFlag("trigdelay", 1, 8, 0);
    nonfpgaconfig.AddFlag("triglength", 1, 8, 0, false);
    nonfpgaconfig.AddFlag("tsphase", 1, 8, 0, false);
    nonfpgaconfig.AddFlag("filereadprint", false);
    nonfpgaconfig.AddFlag("filereaddecode", false);
    nonfpgaconfig.AddFlag("binaryoutput", false);
    nonfpgaconfig.AddFlag("viadatamux", false);
    nonfpgaconfig.AddFlag("numsignals", 1, 24, 0, false);
    nonfpgaconfig.AddFlag("triggeredro", false);
}

std::string FastReadout::GetIdentifier()
{
    return identifier;
}

void FastReadout::SetIdentifier(std::string name)
{
    identifier = name;
}

bool FastReadout::GetEnabled() const
{
    return configuration.GetFlag("enabled");
}

int FastReadout::SetEnabled(bool value)
{
    configuration.SetFlag("enabled", value);

    return GetConfiguration();
}

bool FastReadout::GetReset() const
{
    return configuration.GetFlag("reset");
}

int FastReadout::SetReset(bool value)
{
    configuration.SetFlag("reset", value);

    return GetConfiguration();
}

bool FastReadout::GetFifoclear() const
{
    return configuration.GetFlag("fifoclear");
}

int FastReadout::SetFifoclear(bool value)
{
    configuration.SetFlag("fifoclear", value);

    return GetConfiguration();
}

bool FastReadout::GetTrigger() const
{
    return configuration.GetFlag("trigger");
}

int FastReadout::SetTrigger(bool value)
{
    configuration.SetFlag("trigger", value);

    return GetConfiguration();
}

bool FastReadout::GetDebugMode() const
{
    return configuration.GetFlag("debug");
}

int FastReadout::SetDebugMode(bool value)
{
    configuration.SetFlag("debug", value);

    return GetConfiguration();
}

int FastReadout::GetConfiguration(bool withreset) const
{
    if(withreset)
        return configuration.GetConfiguration();
    else
        return configuration.GetConfiguration() & ~(fifoclear + reset);
}

int FastReadout::SetConfiguration(int config)
{
    configuration.SetConfiguration(config);

    return configuration.GetConfiguration();
}

int FastReadout::GetTriggerDelay() const
{
    return nonfpgaconfig.GetFlag("trigdelay");
}

bool FastReadout::SetTriggerDelay(int value)
{
    return nonfpgaconfig.SetFlag("trigdelay", value);
}

int FastReadout::GetTriggerLength() const
{
    return nonfpgaconfig.GetFlag("triglength");
}

bool FastReadout::SetTriggerLength(int value)
{
    return nonfpgaconfig.SetFlag("triglength", value);
}

int FastReadout::GetTSPhase() const
{
    return nonfpgaconfig.GetFlag("tsphase");
}

bool FastReadout::SetTSPhase(int value)
{
    return nonfpgaconfig.SetFlag("tsphase", value);
}

int FastReadout::GetNumDataSets()
{
    return nonfpgaconfig.GetFlag("numsignals");
}

bool FastReadout::SetNumDataSets(int value)
{
    return nonfpgaconfig.SetFlag("numsignals", value);
}

bool FastReadout::GetFileReadPrint() const
{
    return nonfpgaconfig.GetFlag("filereadprint");
}

void FastReadout::SetFileReadPrint(bool value)
{
    nonfpgaconfig.SetFlag("filereadprint", value);
}

bool FastReadout::GetFileReadDecode() const
{
    return nonfpgaconfig.GetFlag("filereaddecode");
}

void FastReadout::SetFileReadDecode(bool value)
{
    nonfpgaconfig.SetFlag("filereaddecode", value);
}

bool FastReadout::GetBinaryOutput() const
{
    return nonfpgaconfig.GetFlag("binaryoutput");
}

void FastReadout::SetBinaryOutput(bool value)
{
    nonfpgaconfig.SetFlag("binaryoutput", value);
}

bool FastReadout::GetDataMuxEnable() const
{
    return nonfpgaconfig.GetFlag("viadatamux");
}

void FastReadout::SetDataMuxEnable(bool value)
{
    nonfpgaconfig.SetFlag("viadatamux", value);
    configuration.SetFlag("datamux", value);
}

bool FastReadout::GetReadoutMode()
{
    return nonfpgaconfig.GetFlag("triggeredro");
}

void FastReadout::SetReadoutMode(bool triggered)
{
    nonfpgaconfig.SetFlag("triggeredro", triggered);
}

int FastReadout::DecodeGray(int numbits, int value)
{
    int normal = value & (1 << (numbits-1));
    for(int i = numbits - 2; i >= 0; --i)
        normal |= (value ^ (normal >> 1)) & (1 << i);
    return normal;
}

Dataset FastReadout::Decode(bool viaDataMux, std::string data)
{
    if(viaDataMux)
    {
        if(data == "")
            return Dataset();

        Dataset ds = Dataset();
        int casecounter[3] = {0};
        while(data != "")
        {
            switch(data.c_str()[0])
            {
            case(1):
                ++casecounter[0];
                ds.triggerts = 0;
                for(int i = 1; i < 6; ++i)
                    ds.triggerts = ds.triggerts * 256 + (int(data.c_str()[i]) & 255);
                ds.triggerid = 256 * (int(data.c_str()[6]) & 255) + (int(data.c_str()[7]) & 255);
                break;
            case(2):
                ++casecounter[1];
                ds.timeoverthreshold = 0;
                for(int i = 1; i < 6; ++i)
                    ds.timeoverthreshold = ds.timeoverthreshold * 256 + (int(data.c_str()[i]) & 255);
                ds.tot = ds.timeoverthreshold % 128;
                ds.column = 131 - (uint(data.c_str()[6]) & 255);
                ds.triggerid = ds.triggerid + 256 * 256 * (int(data.c_str()[7]) & 255);
                break;
            case(3):
                ++casecounter[2];
                ds.timestamp = 0;
                for(int i = 3; i < 8; ++i)
                    ds.timestamp = 256 * ds.timestamp + (int(data.c_str()[i]) & 255);
                ds.ts = ds.timestamp % 1024;
                ds.row = (int(data.c_str()[1]) & 255) * 256 + (int(data.c_str()[2]) & 255);
                if(ds.row < 186)
                    ds.row = 185 - ds.row;
                break;
            }

            if(data.length() > 8)
                data = data.substr(8);
            else
                data = "";
        }

        if(casecounter[0] == 1 && casecounter[1] == 1 && casecounter[2] == 1)
            return ds;
        else
            return Dataset();
    }
    else
    {
        if(data == "")
            return Dataset();

        Dataset ds = Dataset();
        int firsttime[13] = {0};
        while(data != "")
        {
            ++firsttime[static_cast<unsigned int>(data.c_str()[0])];
            switch(data.c_str()[0])
            {
            //decoded TS2:
            //case(1):
            //case(5):
            //    break;
            case(6):  ds.column = int(data.c_str()[1]) & 255; break;
            case(7):  ds.row    = (int(data.c_str()[1]) & 1) * 256; break;
            case(8):
            {
                ds.row   |= int(data.c_str()[1]) & 255;
                int id = 0;
                for(int i = 5; i < 8; ++i)
                    id = id * 256 + (int(data.c_str()[i]) & 255);
                ds.triggerid = id;
            }
                break;
            case(9):
            {
                ds.ts     = (int(data.c_str()[1]) & 3) * 256;
                int trigts = 0;
                for(int i = 5; i < 8; ++i)
                    trigts = trigts * 256 + (int(data.c_str()[i]) & 255);
                ds.triggerts = trigts;
            }
                break;
            case(10):
            {
                ds.ts    |= int(data.c_str()[1]) & 255;
                long long ext_ts2 = 0;
                for(int i = 3; i < 8; ++i)
                    ext_ts2 = ext_ts2 * 256 + (int(data.c_str()[i]) & 255);
                ds.timeoverthreshold = ext_ts2;
            }
                break;
            case(11):
            {
                long long ext_ts = 0;
                for(int i = 3; i < 8; ++i)
                    ext_ts = ext_ts * 256 + (int(data.c_str()[i]) & 255);
                ds.timestamp = ext_ts;
            }
                break;
            case(12):
                ds.tot = (int(data.c_str()[1]) & 127);
                break;
            }

            if(data.length() > 8)
                data = data.substr(8);
            else
                data = "";
        }

        //check if more then one dataset is contained in the data:
        for(int i = 0; i < 13; ++i)
            if(firsttime[i] > 1 && (i != 1 && i != 5))
                return Dataset();

        if(ds.tot < 0)
            return Dataset();
        else
        {
            ds.column = 131 - ds.column;
            //fix mixed up bits in row and ToT:
            int row = ((ds.row & 2)?1:0) + ((ds.row & 1)?2:0);
            for(int i = 0; i < 7; ++i)
                row += (ds.tot & (64 >> i))?(4 << i):0;
            int tot = 0;
            for(int i = 0; i < 6; ++i)
                tot += (ds.row & (256 >> i))?(1 << i):0;
            if(row < 186)
                ds.row = 185 - row;
            else
                ds.row = row;
            ds.tot = tot;

            //Gray-Decode the timestamps:
            ds.tot = DecodeGray(7, int(ds.tot));
            ds.ts  = DecodeGray(10, ds.ts);

            return ds;
        }
    }
}

std::vector<Dataset> FastReadout::DecodeMany(bool viaDataMux, std::string data)
{
    if(data == "")
        return std::vector<Dataset>();
    std::string onedataset = "";
    std::vector<Dataset> decodeddata;
    for(unsigned int i = 0; i < data.length(); i += 8)
    {
        if(viaDataMux)
        {
            if(data.length() >= i + 24 && data.c_str()[i] == 1 && data.c_str()[i+8] == 2 && data.c_str()[i+16] == 3)
            {
                Dataset dat = Decode(true, data.substr(i, 24));
                if(!dat.is_empty())
                    decodeddata.push_back(dat);
            }
        }
        else
        {
            onedataset = onedataset + data.substr(i,8);
            if(data.c_str()[i] == 12)
            {
                Dataset dat = Decode(false, onedataset);
                if(!dat.is_empty())
                    decodeddata.push_back(dat);
                onedataset = "";
            }
        }
    }

    return decodeddata;
}

std::vector<Dataset> FastReadout::DecodeTrigger(std::string data)
{
    std::vector<Dataset> result;
    data = RemoveEmptyData(data);
    if(data.length() < 8)
        return result;

    switch((data[0] >> 4) & 15)
    {
        case(1):
            trig_datastub.triggerts = int(data[0]) & 3;
            for(int i = 1; i < 5; ++i)
                trig_datastub.triggerts = trig_datastub.triggerts * 256 + (int(data[i]) & 255);
            trig_datastub.triggerts = trig_datastub.triggerts * 64 + (int(data[5] >> 2) & 63);
            trig_datastub.ts = (int(data[6]) & 3) * 256 + (int(data[7]) & 255); //still Gray encoded
            trig_datastub.ts = DecodeGray(10, trig_datastub.ts);
            //datasets[index].m_trigtag = (int(package[5]) & 3) * 32 + (int(package[6] >> 3) & 31);
            //datasets[index].m_fifofull = ((package[6] & 4)?true:false);
            trig_datastub.complete |= 1;
        break;
        case(2):
            trig_datastub.timestamp = (int(data[3]) & 255);
            for(int i = 4; i < 8; ++i)
                trig_datastub.timestamp = trig_datastub.timestamp * 256 + (int(data[i]) & 255);
            trig_datastub.triggerid = (int(data[0]) & 15);
            for(int i = 1; i < 3; ++i)
                trig_datastub.triggerid = trig_datastub.triggerid * 256 + (int(data[i]) & 255);
            trig_datastub.complete |= 2;
        break;
        case(3):
            trig_datastub.bod_sent = (data[0] & 8)?true:false;
            trig_datastub.timeoverthreshold = (int(data[1]) & 255);
            for(int i = 2; i < 5; ++i)
                trig_datastub.timeoverthreshold = trig_datastub.timeoverthreshold * 256 + (int(data[i]) & 255);
            trig_datastub.column = 131 - (int(data[7]) & 255);
            trig_datastub.row = (int(data[5]) & 1) * 256 + (int(data[6]) & 255);
            trig_datastub.row = (~trig_datastub.row) & 511;
            if(trig_datastub.row < 186)
                trig_datastub.row = 185 - trig_datastub.row;
            trig_datastub.tot     = ((int(data[5]) / 2) & 127);
            trig_datastub.tot     = (~trig_datastub.tot) & 127; //TS2 bits are inverted
            trig_datastub.tot     = DecodeGray(7, trig_datastub.tot);
            trig_datastub.complete |= 4;
        break;
        case(4):
            trig_datastub.bod_sent = (data[0] & 8)?true:false;
            trig_datastub.column = 131 - (int(data[7]) & 255);
            trig_datastub.row = (int(data[5]) & 1) * 256 + (int(data[6]) & 255);
            trig_datastub.row = (~trig_datastub.row) & 511;
            if(trig_datastub.row < 186)
                trig_datastub.row = 185 - trig_datastub.row;
            trig_datastub.ts = (int(data[4]) & 7) * 128 + (int(data[5] >> 1) & 127);
            trig_datastub.ts = DecodeGray(10, trig_datastub.ts);
            std::cerr << "TS decoding is not tested for TS format 2" << std::endl;
            trig_datastub.timeoverthreshold = (int(data[0]) & 7);
            for(int i = 1; i < 4; ++i)
                trig_datastub.timeoverthreshold = trig_datastub.timeoverthreshold * 256 + (int(data[i]) & 255);
            trig_datastub.timeoverthreshold = trig_datastub.timeoverthreshold * 32 + (int(data[4] >> 3) & 32);
            trig_datastub.complete |= 4; //case 3 and 4 are used either one or the other, so it is ok
                                             //  to share the complete flag
        break;
        case(8): //debug output
        break;
        default:
            std::cerr << "This should not happen. Is this triggered readout?" << std::endl;
                      //<< "   package (ID: " << CharToHex(packageid/256) << " "
                      //<< CharToHex(packageid % 256) << "): ";
            break;
    }

    if(trig_datastub.complete & 4)
    {
        //directly reject hits with invalid address:
        if(trig_datastub.row >= 0 && trig_datastub.row < 372
                && trig_datastub.column >= 0 && trig_datastub.column < 132)
            unfinished_data.push_back(trig_datastub);
        //clear the spot:
        trig_datastub.complete &= ~4;
    }

    if(trig_datastub.complete & 1) //End-of-Event word was sent
    {
        //complete all datasets in the queue and finish them
        for(auto& it : unfinished_data)
        {
            it.triggerts = trig_datastub.triggerts;
            it.ts        = trig_datastub.ts;
            //trig_tag?
            //fifo_full?
            it.complete |= 1;

            //accept only events with beginning of data word
            if(it.complete == 7)
                result.push_back(it);
        }

        trig_datastub = Dataset();

        //result.insert(result.end(), unfinished_data.begin(), unfinished_data.end());
        unfinished_data.clear();
    }

    return result;
}

std::vector<Dataset> FastReadout::DecodeManyTrigger(std::string data)
{
    if(data == "")
        return std::vector<Dataset>();

    data = RemoveEmptyData(data);
    std::vector<Dataset> decodeddata;
    for(unsigned int i = 0; i < data.length(); i += 8)
    {
        std::vector<Dataset> newhits = DecodeTrigger(data.substr(i,8));
        if(newhits.size() > 0)
            decodeddata.insert(decodeddata.end(), newhits.begin(), newhits.end());

    }

    return decodeddata;
}

std::string FastReadout::RemoveEmptyData(std::string data)
{
    std::string fulldata = "";

    for(unsigned int i = 0; i < data.length(); i += 8)
    {
        std::string set = data.substr(i, 8);
        if(set.compare("\xff\xff\xff\xff\xff\xff\xff\xff") != 0)
            fulldata += set;
    }

    return fulldata;
}

tinyxml2::XMLError FastReadout::LoadFromXMLElement(tinyxml2::XMLElement *root)
{
    tinyxml2::XMLError error = tinyxml2::XML_NO_ERROR;

    if(root == nullptr)
        return tinyxml2::XML_ERROR_FILE_NOT_FOUND;

    tinyxml2::XMLElement* node = root->FirstChildElement();
    while(node != nullptr)
    {
        const char* nam = node->Attribute("name");
        std::string name = (nam != nullptr)?std::string(nam):"";
        if(name.compare(configuration.GetIdentifier()) == 0)
            error = configuration.LoadFromXMLElement(node);
        else if(name.compare(nonfpgaconfig.GetIdentifier()) == 0)
            error = nonfpgaconfig.LoadFromXMLElement(node);

        if(error != tinyxml2::XML_NO_ERROR)
            return error;

        if(node != root->LastChildElement())
            node = node->NextSiblingElement();
        else
            node = nullptr;
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError FastReadout::LoadFromXMLFile(std::string filename, bool compressed)
{
    if(compressed)
    {
        zip_file archive;
        archive.load(filename);
        filename = filename.substr(0, filename.rfind('.')) + ".xml";

        archive.extract(filename);
    }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    tinyxml2::XMLNode* config = getNode(&doc, "FastReadout");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLElement *FastReadout::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string devicename)
{
    tinyxml2::XMLElement* node = doc.NewElement("FastReadout");
    if(devicename != "")
        node->SetAttribute("name", devicename.c_str());
    else
        node->SetAttribute("name", identifier.c_str());

    tinyxml2::XMLElement* child = configuration.SaveToXMLElement(doc);
    node->LinkEndChild(child);

    child = nonfpgaconfig.SaveToXMLElement(doc);
    node->LinkEndChild(child);

    child = nullptr;

    return node;
}

tinyxml2::XMLError FastReadout::SaveToXMLFile(std::string filename, std::string identifier, bool compressed)
{
    std::string archivename = "";
    if(compressed)
    {
        archivename = filename;
        filename = filename.substr(0, filename.rfind('.')) + ".xml";
    }

    //create a new XML Document:
    tinyxml2::XMLDocument doc;

    //include the XML declaration:
    tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
    doc.LinkEndChild(dec);

    tinyxml2::XMLElement* node;

    node = SaveToXMLElement(doc, identifier);

    if(node != nullptr)
        doc.LinkEndChild(node);

    tinyxml2::XMLError error = doc.SaveFile(filename.c_str());

    if(compressed)
    {
        zip_file archive;
        archive.write(filename, filename);

        archive.save(archivename);

        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting compression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

bool FastReadout::GetDisableHitWord1() const
{
    return disablehitword1;
}

void FastReadout::SetDisableHitWord1(bool value)
{
    disablehitword1 = value;
}

bool FastReadout::GetDisableHitWord2() const
{
    return disablehitword2;
}

void FastReadout::SetDisableHitWord2(bool value)
{
    disablehitword2 = value;
}

Dataset::Dataset(int column, int row) : ts(0), timestamp(0), tot(-1), timeoverthreshold(0),
    triggerts(0), triggerid(0), tdac(-1), bod_sent(false), complete(0)
{
    this->column = column;
    this->row    = row;
}

bool Dataset::operator==(const Dataset &rhs) const
{
    return (column == rhs.column && row == rhs.row);
}

bool Dataset::operator<(const Dataset &rhs) const
{
    return (column < rhs.column || (column == rhs.column && row < rhs.row));
}

std::string Dataset::GetStringHeader(bool extended)
{
    if(!extended)
        return "# Column;\tRow;\tTimestamp;\tToT;\tTriggerTS;\tTriggerID";
    else
        return "# Column;\tRow;\tTimestamp;\tToT;\tTriggerTS;\tTriggerID;\text. TS;\text. ToT";
}

std::string Dataset::ToString(bool extended, int tsdiv, int ts2div) const
{
    std::stringstream s("");
    s << column << "\t" << row << "\t" << ts << "\t" << tot << "\t" << triggerts << "\t" << triggerid;
    if(extended)
        s << "\t" << timestamp << "\t" << timeoverthreshold;

    if(tsdiv != 0 && ts2div != 0)
    {
        int range = 128 * ts2div;
        if(range > 1024 * tsdiv)
            range = 1024 * tsdiv;
        int ts1 = (ts % 1024) * tsdiv;
        int ts2 = (tot % 128) * ts2div;
        ts1 = ts1 % (128 * ts2div);
        ts2 = ts2 % (1024 * tsdiv);
        int calctot = ts2 - ts1;
        if(calctot < 0)
            calctot += range;

        s << "\t-> ToT: " << calctot;
    }

    return s.str();
}

int Dataset::GetToTValue(int tsdiv, int ts2div) const
{
    if(tsdiv > 0 && ts2div > 0)
    {
        int range = 128 * ts2div;
        if(range > 1024 * tsdiv)
            range = 1024 * tsdiv;
        int ts1 = (ts % 1024) * tsdiv;
        int ts2 = (tot % 128) * ts2div;
        ts1 = ts1 % (128 * ts2div);
        ts2 = ts2 % (1024 * tsdiv);
        int calctot = ts2 - ts1;
        if(calctot < 0)
            calctot += range;

        return calctot;
    }
    else
        return -1;
}

std::string Dataset::ToAddressString() const
{
    std::stringstream s("");
    s << "(" << column << "|" << row << ")";
    return s.str();
}

bool Dataset::is_empty()
{
    return (tot < 0);
}
