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
 *
 * Initial implementation done by Felix Ehrler
 */

/********************************************************************
 * NexysIO provides efficient communication with the NexysVideo     *
 * FPGA board via the FTDI FT2232h chip.                            *
 * It also includes some functionality to configure connected       *
 * sensors and PCBs.                                                *
 * A large benefit is the complete wrapping of the FTDI driver for  *
 * easy driver replacement.                                         *
 *                                                                  *
 * Author: Felix Ehrler         (initial author)                    *
 *         Rudolf Schimassek    (updates)                           *
 *                                                                  *
 * Version  1.0 (2016) - initial implementation                     *
 *          1.1 (18.04.18) - added proper port closing (now the     *
 *                           port becomes visible to Vivado again   *
 *                           after closing                          *
 *                         - added a read timeout                   *
 *          1.2 (25.05.18) - added fast data acquisition functions  *
 *                           now data read rates up to 4.5 MB/s are *
 *                           possible using a separate thread       *
 ********************************************************************/

#ifndef NEXYSIO_H
#define NEXYSIO_H

#include "ftdi.h"
#include <QBitArray>
#include <thread>
#include <list>

//TODO: remove
#include <fstream>

#include "libs/timing.h"

typedef unsigned char byte;

class NexysIO;

void FastReadWorker(NexysIO* nexys);

class NexysIO
{
    friend void FastReadWorker(NexysIO* nexys);
public:
    NexysIO();
    ~NexysIO();

    const static int FTDIBuffSize = 1024*128;

    enum Header {HRead  = 0x00,
                 HWrite = 0x01};

    enum PatGenAddresses {PGreset     = 2,//0x02,
                          PGsuspend   = 3,//0x03,
                          PGwrite     = 4,//0x04,
                          PGunhandled = 5,//0x05,
                          PGaddress   = 6,//0x06,
                          PGdata      = 7};//0x07};

    enum FPGARegisters {FPGA_ASIC_CONFIG            = 0,
                        FPGA_FAST_READOUT_CONFIG    = 9,
                        FPGA_READOUT_FIFO           = 11,
                        FPGA_VOLTAGEBOARD_CONFIG    = 12
                       };

    enum Value {Ck1      = 0b00000001,
                Ck2      = 0b00000010,
                SinA     = 0b00000100,
                Ld       = 0b00001000,
                Res_n     = 0b00010000,
                SinC     = 0b00100000,
                SinD     = 0b01000000,
                PCB_Ck   = 0b00000001,
                PCB_SIN  = 0b00000010,
                PCB_Ld   = 0b00000100};

    enum behaviour {RstState           =   1,
                    InitState          =   2,   //(while start delay)
                    RstStateOnDone     =   4,   //(otherwise keep last state)
                    InitStateOnDone    =   8,   //(otherwise keep last state)
                    RstStateOnSuspend  =  16,   //(otherwise keep last state)
                    InitStateOnSuspend =  32,   //(otherwise keep last state)
                    NoResetOnPeriod    = 128};  //do not reset timestamp pointer on period reach


    bool initializeFtdi(FTDI*);

    bool is_open();

    /**
     * @brief Check if FPGA is programmed by writing to a register
     * @return          - false if read value does not match write
     */
    bool is_programmed();

    void Close();

    bool AddByte(unsigned char Order);
    bool AddBytes(unsigned char command, unsigned int clockdiv);
    bool AddBytes(std::vector<unsigned char> command, unsigned int clockdiv);

    std::string Read(int count);
    std::string Read(unsigned char address, int count, unsigned int sleeptime = 12);

    /**
     * @brief Write adds the content of values to the write buffer and flushes its content
     * @param address   - address to write the data to
     * @param value     - byte to write
     * @param flush     - flushes the write buffer's contents if true, or not if false
     * @param clockdiv  - reduces the transmission speed sending every data byte `clockdiv` times
     * @return          - false if values is to long for the write buffer
     */
    bool Write(unsigned char address, byte value, bool flush = true, unsigned int clockdiv = 1);

    /**
     * @brief Write adds the content of values to the write buffer and flushes its content
     * @param address   - address to write the data to
     * @param values    - data bytes to write
     * @param flush     - flushes the write buffer's contents if true, or not if false
     * @param clockdiv  - reduces the transmission speed sending every data byte `clockdiv` times
     * @return          - false if values is to long for the write buffer
     */
    bool Write(unsigned char address, std::vector<byte> values, bool flush = true, unsigned int clockdiv = 1);

    bool Write(std::vector<std::pair<byte, byte> > values, bool flush = true, unsigned int clockdiv = 1);

    bool Flush();

    /**
     * @brief WriteASIC writes the data vector to the sensor via the FPGA
     * @param address   - the address in the FPGA to write to for output to the sensor
     * @param values    - the data to send
     * @param Sin       - the bit corresponding to the pin connected to the DataIn of the sensor
     * @param ld        - the bit corresponding to the pin connected to the LdIn of the sensor
     * @param sendload  - determines whether the load signal is transmitted directly with the configuration or not
     * @param clockdiv  - factor for sending each byte multiple times to slow down the data transmission
     * @return          - false if a flush() was necessary, true otherwise
     */
    bool WriteASIC(unsigned char address, std::vector<bool> values,NexysIO::Value  Sin = SinA,
                   NexysIO::Value  ld = Ld, bool sendload = true, const int clockdiv = 8);
    /**
     * @brief ConfigureLoad for configuring a shift register driven by ck1 without shifting the
     *          data in the chip
     * @param address
     * @param values
     * @param sendload
     * @return
     */
    bool ConfigureLoad(unsigned char address, std::vector<bool> values, bool sendload = true);
    /**
     * @brief SendLoad separate command to send a load signal to the chip for fast reconfigurations
     * @return          - true on succesful send false otherwise
     */
    bool SendLoad(unsigned char address, unsigned char Ld, unsigned int clockdiv=6);

    /**
     * Configure Function Cards.
     *
     * Send Provided bits including load bits to the SR, then toggle the Load
     * of the shift register holding the function cards loads.
     *
     * Finishes by resetting the function cards loads to 0
     * @brief WritePCB
     * @param address
     * @param values Bit vector for function card, including bits for the load shift register
     * @param clockdiv
     * @return
     */
    bool WritePCB(unsigned char address, std::vector<bool> values, const unsigned int clockdiv);


    bool AddDelay(unsigned long usec);


    //Pattern generator functions:
    void PatGenReset(bool reset);
    void PatGenSuspend(bool suspend);
    void PatGen(int period = 4, int cycle = 5, int clkdiv = 10, int initdelay = 0, bool oldpatgen = false);
    void RestartPatGen(bool flush = true);

    bool PatGenWrite(unsigned char patgenaddress, unsigned char patgenvalue);

    /**
     * @brief StartFastRead starts another thread just for reading data from the FPGA from a specified address
     * @param address        - the address to read from
     * @param readsize       - as number of blocks of 12240 Bytes, which equals 1530 data sets of 8 Bytes
     * @param readingwait    - delay after sending the read command to the FPGA and before requesting the data
     * @param zerosuppressed - determines whether the read data is directly analysed for invalid data (8 bytes with value 255),
     *                              the zero-suppression reduces the achieved throughput by about 25%
     */
    void StartFastRead(unsigned char address, unsigned int readsize = 19,
                       unsigned int readingwait = 10, bool zerosuppressed = false);
    /**
     * @brief StopFastRead stops the reader thread
     */
    void StopFastRead();
    /**
     * @brief FastRead acquires the specified amount of data from the data buffer
     * @param characters     - the number of characters to retrieve from the data buffer
     * @return               - a string containing up to `characters` characters from the read buffer
     */
    std::string FastRead(unsigned int characters);
    /**
     * @brief FastReadDataAvailable checks whether data is available for readout
     * @return               - true if data is available, false if not
     */
    bool FastReadDataAvailable();
    /**
     * @brief ClearFastReadBuffer removes all data from the read buffer if the worker is not running
     * @return               - true if the data was removed, false otherwise
     */
    bool ClearFastReadBuffer();

private:
    FTDI* ftdi;

    unsigned char FTDIBuff[FTDIBuffSize];
    unsigned int  FTDIBuffPos;


    unsigned char workeraddress;
    unsigned int  workerreadsize;
    unsigned int  workersleep;
    bool workerrunning;
    std::thread* worker;
    std::list<std::string> readbuffer;
    std::string firstelementremains;        //if less data is requested at once than an entry in the queue contains
    bool workerzerosuppressed;

    unsigned char* databuffer;
};

#endif // NEXYSIO_H
