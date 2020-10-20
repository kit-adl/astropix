#ifndef UDP_CONFIG_H
#define UDP_CONFIG_H

#include <string>
#include <vector>

#include "flagmanager.h"
#include "tinyxml2.h"
#include "tinyxml2_addon.h"


typedef unsigned char byte;
typedef unsigned int  uint;

class UDP_Config
{
public:
    UDP_Config();

    void SetIdentifier(std::string name);
    std::string GetIdentifier() const;

    int  GetConfigAddress() const;
    void SetConfigAddress(int fpga_address);
    byte GetConfigData() const;

    int  GetTimeoutAddress() const;
    void SetTimeoutAddress(int fpga_address);
    std::vector<byte> GetTimeoutData() const;

    int  GetPortAddress() const;
    void SetPortAddress(int fpga_address);
    std::vector<byte> GetPortData() const;

    int  GetInterPackageDelayAddress() const;
    void SetInterPackageDelayAddress(int fpga_address);
    std::vector<byte> GetInterPackageDelayData() const;

    int  GetIPAddress() const;
    void SetIPAddress(int fpga_address);
    std::vector<byte> GetIPData() const;

    int  GetSubnetMaskAddress() const;
    void SetSubnetMaskAddress(int fpga_address);
    std::vector<byte> GetSubnetMaskData() const;

    int  GetMACAddress() const;
    void SetMACAddress(int fpga_address);
    std::vector<byte> GetMACData() const;

    bool AddressesComplete() const;

    bool GetUDPReset() const;
    void SetUDPReset(bool reset);

    bool GetSenderReset() const;
    void SetSenderReset(bool reset);

    bool GetFIFOReset() const;
    void SetFIFOReset(bool reset);

    bool GetDebugMode() const;
    void SetDebugMode(bool debug);

    int  GetTimeout() const;
    bool SetTimeout(int timeout);

    int  GetFPGAPort() const;
    bool SetFPGAPort(int port);

    int  GetInterPackageDelay() const;
    bool SetInterPackageDelay(int delay);

    int  GetIPPart(int index) const;
    std::string GetIP() const;
    bool SetIPPart(int index, int value);
    bool SetIP(std::string address);

    int  GetSubnetMaskPart(int index) const;
    std::string GetSubnetMask() const;
    bool SetSubnetMaskPart(int index, int value);
    bool SetSubnetMask(std::string mask);

    int  GetMACPart(int index) const;
    std::string GetMAC() const;
    bool SetMACPart(int index, int value);
    bool SetMAC(std::string address);

    /**
     * @brief LoadFromXMLElement extracts the data for this object from an XML subtree
     * @param root          - root node of the subtree to load the data from
     * @return              - XML_NO_ERROR on success or an error code
     */
    tinyxml2::XMLError LoadFromXMLElement(tinyxml2::XMLElement* rootnode);
    /**
     * @brief LoadFromXMLFile loads the contents of the instance from an XML file
     * @param filename      - file name of the file to load from
     * @param compressed    - if true, interpert the file at `filename` as zip archive and replace
     *                          the extension by "xml" to select the file inside the archive.
     *                          For false, use the passed file as the XML file directly
     * @return              - XML_NO_ERROR on success or an error code otherwise
     */
    tinyxml2::XMLError LoadFromXMLFile(std::string filename, bool compressed = false);

    /**
     * @brief SaveToXMLElement generates an XML tree structure from the contained data for the
     *          passed XML document
     * @param doc           - XML document to host the tree to build
     * @param devicename    - string to override the stored device name / identification string of
     *                          the instance, keep empty to use the internal one
     * @return              - the root node of the XML tree structure or a null pointer on an error
     */
    tinyxml2::XMLElement* SaveToXMLElement(tinyxml2::XMLDocument& doc, std::string identifier = "") const;
    /**
     * @brief SaveToXMLFile generates a tree structure from the instance and writes it to a file or
     *          archive at the given filename
     * @param filename      - filename of the file or archive to save to
     * @param identifier    - identification string for the instance in the XML structure to override
     *                          the internal one, keep empty to use the internal one
     * @param compressed    - for true, create a zip archive with `filename` as name and replace the
     *                          file extension by "xml" for the file in the archive.
     *                          For false, uses `filename` directly for the XML file
     * @return              - XML_NO_ERROR on success, an error code otherwise
     */
    tinyxml2::XMLError SavetoXMLFile(std::string filename, std::string identifier = "", bool compressed = false) const;
private:
    int  HexToDec(std::string hexnumber) const;
    std::string DecToHex(int decnumber) const;

    std::string identifier;

    FlagManager config;
    int timeout; //24bit used
    int timeout_address;
    int port;    //16bit used
    int port_address;
    int interpackagedelay; //16bit used
    int delay_address;

    int ip[4];
    int ip_address;
    int subnetmask[4];
    int mask_address;
    int mac[6];
    int mac_address;
};

#endif // UDP_CONFIG_H
