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


#include "tinyxml2_addon.h"

tinyxml2::XMLNode* getNode(tinyxml2::XMLNode* parent, std::string value, tinyxml2::XMLNode* start)
{
    tinyxml2::XMLNode* child;
    if(start != nullptr)
        child = start;
    else
        child = parent->FirstChildElement();

    while(1)
    {
        if(child->Value() == value)
            return child;

        if(child != parent->LastChildElement())
            child = child->NextSiblingElement();
        else
            break;
    }
    return nullptr;
}

//TODO: does not work with tinyxml2 as it does some escaping by itself...
//it can be skipped... (work for nothing, but the check from unescapeStringCheck() is still
//  necessary)
std::string escapeString(std::string text)
{
    std::string convert = text;
    const char escaping[5] = {'&','\"','\'','<','>'};
    const std::string replacement[5] = {"&amp;","&quot;","&apos;","&lt;","&gt;"};
    unsigned int index = 0;

    for(int symbol = 1; symbol < 5; ++symbol)   
            //exclude '&' from the replacement as it is done by tinyxml2
    {
        index = convert.find(escaping[symbol], 0);
        while(index != std::string::npos)
        {
            convert.replace(index,1,replacement[symbol]);
            index = convert.find(escaping[symbol],index+1);
        }
    }

    return convert;
}

std::string unescapeString(std::string escapedtext)
{
    std::string convert = escapedtext;
    const char escaping[5] = {'\"','\'','<','>','&'};
    const std::string replacement[5] = {"&quot;","&apos;","&lt;","&gt;","&amp;"};
    unsigned int index = 0;

    for(int symbol = 0; symbol < 4; ++symbol)   //exclude '&' from the replacement (done by tinyxml2)
    {
        index = convert.find(replacement[symbol], 0);
        while(index != std::string::npos)
        {
            convert.replace(index, (replacement[symbol]).length(), &(escaping[symbol]));
            index = convert.find(replacement[symbol],index+1);
        }
    }

    return convert;    
}

std::string unescapeStringCheck(const char* escapedtext)
{
    if(escapedtext == nullptr)
        return "";

    std::string convert = escapedtext;
    return unescapeString(convert);
}

std::string checkString(const char* text)
{
    if(text == nullptr)
        return "";
    else
        return std::string(text);
}
