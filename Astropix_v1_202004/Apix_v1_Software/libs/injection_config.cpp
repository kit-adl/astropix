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

#include "injection_config.h"

Injection_Config::Injection_Config() : VB_Config(3.3, 1.8), numpulsesets(0), numpulsesinaset(0), initdelay(0),
            period(4), clockdiv(2), synced(false), tsoverflowsync(false), outputchannel(0)
{
    devicename = "InjectionBoard";
}

unsigned int Injection_Config::GetNumPulseSets() const
{
    return numpulsesets;
}

bool Injection_Config::SetNumPulseSets(unsigned int value)
{
    if(value <= 65535)
    {
        numpulsesets = value;
        return true;
    }
    else
        return false;
}

unsigned int Injection_Config::GetNumPulsesInaSet() const
{
    return numpulsesinaset;
}

bool Injection_Config::SetNumPulsesInaSet(unsigned int value)
{
    if(value <= 255)
    {
        numpulsesinaset = value;
        return true;
    }
    else
        return false;
}

unsigned int Injection_Config::GetInitDelay() const
{
    return initdelay;
}

bool Injection_Config::SetInitDelay(unsigned int value)
{
    if(value <= 65535)
    {
        initdelay = value;
        return true;
    }
    else
        return false;
}

unsigned int Injection_Config::GetPeriod() const
{
    return period;
}

bool Injection_Config::SetPeriod(unsigned int value)
{
    if(value <= 255)
    {
        period = value;
        return true;
    }
    else
        return false;
}

unsigned int Injection_Config::GetClockDiv() const
{
    return clockdiv;
}

bool Injection_Config::SetClockDiv(unsigned int value)
{
    if(value <= 65535)
    {
        clockdiv = value;
        return true;
    }
    else
        return false;
}

bool Injection_Config::GetSynced() const
{
    return synced;
}

bool Injection_Config::SetSynced(bool value)
{
    synced = value;
    return true;
}

bool Injection_Config::GetTSOverflowSync() const
{
    return tsoverflowsync;
}

bool Injection_Config::SetTSOverflowSync(bool sync)
{
    tsoverflowsync = sync;
    return true;
}

int Injection_Config::GetOutputChannel()
{
    return outputchannel;
}

bool Injection_Config::SetOutputChannel(int flags)
{
    outputchannel = flags;
    return true;
}

tinyxml2::XMLError Injection_Config::LoadFromXMLElement(tinyxml2::XMLElement *node)
{
    tinyxml2::XMLError error = VB_Config::LoadFromXMLElement(node);

    tinyxml2::XMLElement* child = node->FirstChildElement();

    while(child != nullptr)
    {
        std::string val = std::string(child->Value());
        if(val.compare("NumPulseSets") == 0)
            SetNumPulseSets(static_cast<unsigned int>(child->IntAttribute("value")));
        else if(val.compare("NumPulsesInaSet") == 0)
            SetNumPulsesInaSet(static_cast<unsigned int>(child->IntAttribute("value")));
        else if(val.compare("InitDelay") == 0)
            SetInitDelay(static_cast<unsigned int>(child->IntAttribute("value")));
        else if(val.compare("Period") == 0)
            SetPeriod(static_cast<unsigned int>(child->IntAttribute("value")));
        else if(val.compare("ClockDiv") == 0)
            SetClockDiv(static_cast<unsigned int>(child->IntAttribute("value")));
        else if(val.compare("Synced") == 0)
            SetSynced(child->BoolAttribute("value"));
        else if(val.compare("TSOverflowSync") == 0)
            SetTSOverflowSync(child->BoolAttribute("value"));
        else if(val.compare("OutputChannel") == 0)
            SetOutputChannel(child->IntAttribute("value"));


        if(child != node->LastChildElement())
            child = child->NextSiblingElement();
        else
            child = nullptr;
    }

    return error;
}

tinyxml2::XMLElement *Injection_Config::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string devicename)
{
    tinyxml2::XMLElement* node = VB_Config::SaveToXMLElement(doc, devicename);

    tinyxml2::XMLElement* child = doc.NewElement("NumPulseSets");
    child->SetAttribute("value", GetNumPulseSets());
    node->LinkEndChild(child);
    child = doc.NewElement("NumPulsesInaSet");
    child->SetAttribute("value", GetNumPulsesInaSet());
    node->LinkEndChild(child);
    child = doc.NewElement("InitDelay");
    child->SetAttribute("value", GetInitDelay());
    node->LinkEndChild(child);
    child = doc.NewElement("Period");
    child->SetAttribute("value", GetPeriod());
    node->LinkEndChild(child);
    child = doc.NewElement("ClockDiv");
    child->SetAttribute("value", GetClockDiv());
    node->LinkEndChild(child);
    child = doc.NewElement("Synced");
    child->SetAttribute("value", GetSynced());
    node->LinkEndChild(child);
    child = doc.NewElement("TSOverflowSync");
    child->SetAttribute("value", GetTSOverflowSync());
    node->LinkEndChild(child);
    child = doc.NewElement("OutputChannel");
    child->SetAttribute("value", GetOutputChannel());
    node->LinkEndChild(child);

    child = nullptr;

    return node;
}
