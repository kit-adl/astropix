#include "udp_config.h"

UDP_Config::UDP_Config() : identifier(""), timeout(0xffffff), timeout_address(-1), port(1234), port_address(-1),
    interpackagedelay(10), delay_address(-1), ip_address(-1), mac_address(-1)
{
    config.SetFPGAAddress(-1);
    config.SetIdentifier("UDP_RO");
    config.AddFlag("reset"            , 1, false);
    config.AddFlag("reset_data_sender", 2, false);
    config.AddFlag("reset_send_fifo"  , 4, false);
    config.AddFlag("debug_mode"       , 8,     0);

    ip[0] = 192;
    ip[1] = 168;
    ip[2] =   1;
    ip[3] = 128;

    subnetmask[0] = 255;
    subnetmask[1] = 255;
    subnetmask[2] = 255;
    subnetmask[3] =   0;

    mac[0] = 2;
    mac[1] = 0;
    mac[2] = 0;
    mac[3] = 0;
    mac[4] = 0;
    mac[5] = 0;
}

void UDP_Config::SetIdentifier(std::string name)
{
    identifier = name;
}

std::string UDP_Config::GetIdentifier() const
{
    return identifier;
}

void UDP_Config::SetConfigAddress(int fpga_address)
{
    config.SetFPGAAddress(fpga_address);
}

int UDP_Config::GetConfigAddress() const
{
    return config.GetFPGAAddress();
}

byte UDP_Config::GetConfigData() const
{
    return byte(config.GetConfiguration());
}

void UDP_Config::SetTimeoutAddress(int fpga_address)
{
    timeout_address = fpga_address;
}

int UDP_Config::GetTimeoutAddress() const
{
    return timeout_address;
}

std::vector<byte> UDP_Config::GetTimeoutData() const
{
    std::vector<byte> data;

    data.push_back(byte(timeout / 256));
    data.push_back(byte(timeout));

    return data;
}

void UDP_Config::SetPortAddress(int fpga_address)
{
    port_address = fpga_address;
}

int UDP_Config::GetPortAddress() const
{
    return port_address;
}

std::vector<byte> UDP_Config::GetPortData() const
{
    std::vector<byte> data;

    data.push_back(byte(port / 256));
    data.push_back(byte(port));

    return data;
}

void UDP_Config::SetInterPackageDelayAddress(int fpga_address)
{
    delay_address = fpga_address;
}

std::vector<byte> UDP_Config::GetInterPackageDelayData() const
{
    std::vector<byte> data;

    data.push_back(byte(interpackagedelay / 65536));
    data.push_back(byte(interpackagedelay / 256));
    data.push_back(byte(interpackagedelay));

    return data;
}

int UDP_Config::GetInterPackageDelayAddress() const
{
    return delay_address;
}

void UDP_Config::SetIPAddress(int fpga_address)
{
    ip_address = fpga_address;
}

std::vector<byte> UDP_Config::GetIPData() const
{
    std::vector<byte> data;

    for(int i = 0; i < 4; ++i)
        data.push_back(byte(ip[i]));

    return data;
}

int UDP_Config::GetIPAddress() const
{
    return ip_address;
}

void UDP_Config::SetSubnetMaskAddress(int fpga_address)
{
    mask_address = fpga_address;
}

std::vector<byte> UDP_Config::GetSubnetMaskData() const
{
    std::vector<byte> data;

    for(int i = 0; i < 4; ++i)
        data.push_back(byte(subnetmask[i]));

    return data;
}

int UDP_Config::GetSubnetMaskAddress() const
{
    return mask_address;
}

void UDP_Config::SetMACAddress(int fpga_address)
{
    mac_address = fpga_address;
}

std::vector<byte> UDP_Config::GetMACData() const
{
    std::vector<byte> data;

    for(int i = 0; i < 6; ++i)
        data.push_back(byte(mac[i]));

    return data;
}

int UDP_Config::GetMACAddress() const
{
    return mac_address;
}

bool UDP_Config::AddressesComplete() const
{
    return (config.GetFPGAAddress() != -1) && timeout_address != -1 && port_address != -1 && delay_address != -1
            && ip_address != -1 && mask_address != -1 && mac_address != -1;
}

bool UDP_Config::GetUDPReset() const
{
    return config.GetFlag("reset") != 0;
}

void UDP_Config::SetUDPReset(bool reset)
{
    config.SetFlag("reset", ((reset)?1:0));
}

bool UDP_Config::GetSenderReset() const
{
    return config.GetFlag("reset_data_sender") != 0;
}

void UDP_Config::SetSenderReset(bool reset)
{
    config.SetFlag("reset_data_sender", ((reset)?1:0));
}

bool UDP_Config::GetFIFOReset() const
{
    return config.GetFlag("reset_send_fifo") != 0;
}

void UDP_Config::SetFIFOReset(bool reset)
{
    config.SetFlag("reset_send_fifo", ((reset)?1:0));
}

bool UDP_Config::GetDebugMode() const
{
    return config.GetFlag("debug_mode") != 0;
}

void UDP_Config::SetDebugMode(bool debug)
{
    config.SetFlag("debug_mode", ((debug)?1:0));
}

int UDP_Config::GetTimeout() const
{
    return timeout;
}

bool UDP_Config::SetTimeout(int timeout)
{
    if(timeout < 0xffffff)
    {
        this->timeout = timeout;
        return true;
    }
    else
        return false;
}

int UDP_Config::GetFPGAPort() const
{
    return port;
}

bool UDP_Config::SetFPGAPort(int port)
{
    if(port < 65536)
    {
        this->port = port;
        return true;
    }
    else
        return false;
}

int UDP_Config::GetInterPackageDelay() const
{
    return interpackagedelay;
}

bool UDP_Config::SetInterPackageDelay(int delay)
{
    if(delay < 65536)
    {
        this->interpackagedelay = delay;
        return true;
    }
    else
        return false;
}

int UDP_Config::GetIPPart(int index) const
{
    if(index < 0 || index > 3)
        return -1;
    else
        return ip[index];
}

std::string UDP_Config::GetIP() const
{
    std::stringstream s("");
    s << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3];
    return s.str();
}

bool UDP_Config::SetIPPart(int index, int value)
{
    if(index < 0 || index > 3 || value < 0 || value > 255)
        return false;
    else
    {
        ip[index] = value;
        return true;
    }
}

bool UDP_Config::SetIP(std::string address)
{
    unsigned int dot[3];
    dot[0] = address.find('.', 0);
    dot[1] = address.find('.', dot[0] + 1);
    dot[2] = address.find('.', dot[1] + 1);

    //not 3 '.' in the string -> no valid IP address:
    if(dot[2] == uint(std::string::npos))
        return false;
    else
    {
        std::stringstream s("");
        s << address.substr(0, dot[0]) << " " << address.substr(dot[0] + 1, dot[1] - dot[0] - 1)
          << " " << address.substr(dot[1] + 1, dot[2] - dot[1] - 1) << " "
          << address.substr(dot[2] + 1);
        int newaddr[4];
        if(s >> newaddr[0] >> newaddr[1] >> newaddr[2] >> newaddr[3])
        {
            for(int i = 0; i < 4; ++i)
                ip[i] = newaddr[i];
            return true;
        }
        else
            return false;
    }
}

int UDP_Config::GetSubnetMaskPart(int index) const
{
    if(index < 0 || index > 3)
        return -1;
    else
        return subnetmask[index];
}

std::string UDP_Config::GetSubnetMask() const
{
    std::stringstream s("");
    s << subnetmask[0] << "." << subnetmask[1] << "." << subnetmask[2] << "." << subnetmask[3];
    return s.str();
}

bool UDP_Config::SetSubnetMaskPart(int index, int value)
{
    if(index < 0 || index > 3 || value < 0 || value > 255)
        return false;
    else
    {
        subnetmask[index] = value;
        return true;
    }
}

bool UDP_Config::SetSubnetMask(std::string mask)
{
    unsigned int dot[3];
    dot[0] = mask.find('.', 0);
    dot[1] = mask.find('.', dot[0] + 1);
    dot[2] = mask.find('.', dot[1] + 1);

    //not 3 '.' in the string -> no valid IP address:
    if(dot[2] == uint(std::string::npos))
        return false;
    else
    {
        std::stringstream s("");
        s << mask.substr(0, dot[0]) << " " << mask.substr(dot[0] + 1, dot[1] - dot[0] - 1)
          << " " << mask.substr(dot[1] + 1, dot[2] - dot[1] - 1) << " "
          << mask.substr(dot[2] + 1);
        int newaddr[4];
        if(s >> newaddr[0] >> newaddr[1] >> newaddr[2] >> newaddr[3])
        {
            for(int i = 0; i < 4; ++i)
                subnetmask[i] = newaddr[i];
            return true;
        }
        else
            return false;
    }
}

int UDP_Config::GetMACPart(int index) const
{
    if(index < 0 || index > 5)
        return -1;
    else
        return mac[index];
}

std::string UDP_Config::GetMAC() const
{
    std::stringstream s("");
    s << DecToHex(mac[0]);
    for(int i = 1; i < 6; ++i)
        s << ":" << DecToHex(mac[i]);

    return s.str();
}

bool UDP_Config::SetMACPart(int index, int value)
{
    if(index < 0 || index > 5 || value < 0 || value > 255)
        return false;
    else
    {
        mac[index] = value;
        return true;
    }
}

bool UDP_Config::SetMAC(std::string address)
{
    unsigned int dot[5];
    dot[0] = address.find(':', 0);
    for(int i = 1; i < 5; ++i)
        dot[i] = address.find(':', dot[i - 1] + 1);

    //not 5 ':' in the string -> no valid IP address:
    if(dot[4] == uint(std::string::npos) || address.length() != 17)
        return false;
    else
    {
        int newaddr[6];
        for(unsigned int i = 0; i < 6; ++i)
        {
            newaddr[i] = HexToDec(address.substr(i*3, 2));
            if(newaddr[i] == -1)
                return false;
        }
        for(unsigned int i = 0; i < 6; ++i)
            mac[i] = newaddr[i];
        return true;
    }
}

tinyxml2::XMLError UDP_Config::LoadFromXMLElement(tinyxml2::XMLElement *rootnode)
{
    if(rootnode == nullptr)
        return tinyxml2::XML_ERROR_EMPTY_DOCUMENT;

    const char* nam = rootnode->Attribute("name");
    identifier = (nam != nullptr)?std::string(nam):"";

    tinyxml2::XMLElement* child = rootnode->FirstChildElement();
    tinyxml2::XMLError error = tinyxml2::XML_NO_ERROR;

    while(child != nullptr)
    {
        std::string name = std::string(child->Value());
        if(name.compare("FlagManager") == 0)
        {
            tinyxml2::XMLError error = config.LoadFromXMLElement(child);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
        }
        else if(name.compare("Timing") == 0)
        {
            error = child->QueryIntAttribute("timeout", &timeout);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
            error = child->QueryIntAttribute("timeout_addr", &timeout_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;

            error = child->QueryIntAttribute("delay", &interpackagedelay);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
            error = child->QueryIntAttribute("delay_addr", &delay_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
        }
        else if(name.compare("Address") == 0)
        {
            error = child->QueryIntAttribute("port", &port);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
            error = child->QueryIntAttribute("port_addr", &port_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;

            nam = child->Attribute("IP");
            if(nam != nullptr)
                SetIP(std::string(nam));
            else
                return tinyxml2::XML_ERROR_PARSING_ATTRIBUTE;
            error = child->QueryIntAttribute("IP_addr", &ip_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;

            nam = child->Attribute("Mask");
            if(nam != nullptr)
                SetSubnetMask(std::string(nam));
            else
                return tinyxml2::XML_ERROR_PARSING_ATTRIBUTE;
            error = child->QueryIntAttribute("Mask_addr", &mask_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;

            nam = child->Attribute("MAC");
            if(nam != nullptr)
                SetMAC(std::string(nam));
            else
                return tinyxml2::XML_ERROR_PARSING_ATTRIBUTE;
            error = child->QueryIntAttribute("MAC_addr", &mac_address);
            if(error != tinyxml2::XML_NO_ERROR)
                return error;
        }

        if(child != rootnode->LastChildElement())
            child = child->NextSiblingElement();
        else
            child = nullptr;
    }

    return tinyxml2::XML_NO_ERROR;
}

tinyxml2::XMLError UDP_Config::LoadFromXMLFile(std::string filename, bool compressed)
{
    if(compressed)
    {
        std::fstream f;
        f.open(filename, std::ios::in);
        if(!f.is_open())
            return tinyxml2::XML_ERROR_FILE_NOT_FOUND;

        zip_file archive;
        archive.load(filename);
        filename = filename.substr(0, filename.rfind('.')) + ".xml";

        archive.extract(filename);
    }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != tinyxml2::XML_NO_ERROR)
        return error;

    tinyxml2::XMLNode* config = getNode(&doc, "UDPConfig");

    error = LoadFromXMLElement(config->ToElement());

    if(compressed)
    {
        if(remove(filename.c_str()) != 0)
            std::cout << "error deleting decompression helper file \"" << filename << "\""
                      << std::endl;
    }

    return error;
}

tinyxml2::XMLElement *UDP_Config::SaveToXMLElement(tinyxml2::XMLDocument &doc, std::string identifier) const
{
    tinyxml2::XMLElement* flagman = doc.NewElement("UDPConfig");

    if(identifier == "")
        flagman->SetAttribute("name", this->identifier.c_str());
    else
        flagman->SetAttribute("name", identifier.c_str());

    tinyxml2::XMLElement* child = config.SaveToXMLElement(doc, "");
    flagman->LinkEndChild(child);

    child = doc.NewElement("Timing");
    child->SetAttribute("timeout", timeout);
    child->SetAttribute("timeout_addr", timeout_address);
    child->SetAttribute("delay", interpackagedelay);
    child->SetAttribute("delay_addr", delay_address);
    flagman->LinkEndChild(child);

    child = doc.NewElement("Address");
    child->SetAttribute("port", port);
    child->SetAttribute("port_addr", port_address);
    child->SetAttribute("IP", GetIP().c_str());
    child->SetAttribute("IP_addr", ip_address);
    child->SetAttribute("Mask", GetSubnetMask().c_str());
    child->SetAttribute("Mask_addr", mask_address);
    child->SetAttribute("MAC", GetMAC().c_str());
    child->SetAttribute("MAC_addr", mac_address);
    flagman->LinkEndChild(child);

    return flagman;
}

tinyxml2::XMLError UDP_Config::SavetoXMLFile(std::string filename, std::string identifier, bool compressed) const
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

int UDP_Config::HexToDec(std::string hexnumber) const
{
    if(hexnumber.substr(0,2).compare("0x") == 0)
        hexnumber = hexnumber.substr(2);

    int number = 0;

    for(unsigned int i = 0; i < hexnumber.length(); ++i)
    {
        switch(hexnumber[i])
        {
        case('a'):
        case('A'):
            number = 16 * number + 10;
            break;
        case('b'):
        case('B'):
            number = 16 * number + 11;
            break;
        case('c'):
        case('C'):
            number = 16 * number + 12;
            break;
        case('d'):
        case('D'):
            number = 16 * number + 13;
            break;
        case('e'):
        case('E'):
            number = 16 * number + 14;
            break;
        case('f'):
        case('F'):
            number = 16 * number + 15;
            break;
        default:
            if(hexnumber[i] >= '0' && hexnumber[i] <= '9')
                number = 16 * number + hexnumber[i] - '0';
            else
                return -1;
            break;
        }
    }

    return number;
}

std::string UDP_Config::DecToHex(int decnumber) const
{
    std::string number = "";
    while((decnumber > 0 || (number == "" && decnumber == 0)) || (number.length() & 1) != 0)
    {
        int val = decnumber % 15;
        if(val <= 9)
            number = char(48 + val) + number;
        else
            number = char('A' - 10 + val) + number;
        decnumber /= 16;
    }

    return number;
}
