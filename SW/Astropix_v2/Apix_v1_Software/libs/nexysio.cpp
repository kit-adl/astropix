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
 * Initial author of this class is Felix Ehrler
 */

#include "nexysio.h"
#include <stdio.h>
#include <iostream>
#include <QBitArray>
#include <sstream>

//#################################################################
//### This Class provides functions for efficient communication ###
//### with the NexysVideo FPGA board via FTDI. It also includes ###
//### functions for configuration of connected Sensors and PCB. ###
//### by Felix Ehrler                                           ###
//###                                                           ###
//### for versioning, see header file                           ###
//#################################################################

NexysIO::NexysIO() : workeraddress(11), workerreadsize(232560), workersleep(10),
    workerrunning(false), worker(nullptr), workerzerosuppressed(false)
{
    this->ftdi = nullptr;
    FTDIBuffPos = 0;

    //readbuffer = new std::list<std::string>();

    databuffer = new unsigned char[64001]();
}

NexysIO::~NexysIO()
{
    if(worker != nullptr)
    {
        workerrunning = false;
        worker->join();
        delete worker;
        worker = nullptr;
    }

    //delete readbuffer;
    //readbuffer = 0;
    delete[] databuffer;
}

//Inizialize FTDI communication (Pointer to FTDI used in main program)
bool NexysIO::initializeFtdi(FTDI* extftdi)
{
  this->ftdi = extftdi;
  if(this->ftdi == nullptr)
    return false;
  else
      return true;
}

bool NexysIO::is_open()
{
    return (ftdi != nullptr);
}

void NexysIO::Close()
{
    if(ftdi != nullptr)
    {
        ftdi->Close();
        ftdi = nullptr;
    }
}

//Add a single byte to the buffer
bool NexysIO::AddByte(unsigned char Order)
{
    if (FTDIBuffPos > FTDIBuffSize-1)
        Flush();

    FTDIBuff[FTDIBuffPos++] = Order;
    return true;
}

bool NexysIO::AddBytes(unsigned char command, unsigned int clockdiv)
{
    bool noneedtosend = true;

     //printf("---> Adding command: %x\n",command);

    if (FTDIBuffPos + clockdiv > FTDIBuffSize-1) {
         Flush();

    }


    for(unsigned int i = 0; i < clockdiv; ++i)
        FTDIBuff[FTDIBuffPos++] = command;

    return noneedtosend;
}

bool NexysIO::AddBytes(std::vector<unsigned char> command, unsigned int clockdiv)
{
    bool noneedtosend = true;

    if (FTDIBuffPos + clockdiv*command.size() > FTDIBuffSize-1)
        Flush();

    for(auto it = command.begin(); it != command.end(); ++it)
    {
        for(unsigned int i = 0; i < clockdiv; ++i)
            FTDIBuff[FTDIBuffPos++] = *it;
    }

    return noneedtosend;
}

//Send the buffer to the FPGA
bool NexysIO::Flush()
{
    if(ftdi == nullptr)
        return false;

    //std::cout << "Flushing" << std::endl;

    //std::cout << FTDIBuff[i]<< std::endl;
    bool ftStatus = false;
    unsigned long int BytesWritten = 0;
    //std::cout << "Bytes written to FTDI: " << BytesWritten << std::endl;
    std::cout << "Number of Bytes in Buffer: " << FTDIBuffPos << std::endl;
    if (FTDIBuffPos > 0 )
      {
        ftStatus = ftdi->Write(static_cast<unsigned char*>(FTDIBuff), FTDIBuffPos, &BytesWritten );
        //std::cout << "Bytes written to FTDI: " << BytesWritten << std::endl;
      }
    //std::cout << "Bytes written to FTDI: " << BytesWritten << std::endl;
    FTDIBuffPos = 0;
    return ftStatus;
}

std::string NexysIO::Read(int count)
{
    if(ftdi == nullptr)
        return "";

    std::string data="";
    //unsigned char* text = new unsigned char[64001]; //64k is the maximum frame size of USB
    unsigned long readbytes = 0;

    while(count > 61200) //63448)
    {
        //ftdi->Read(text, 63448, &readbytes);
        ftdi->Read(databuffer, 61200, &readbytes); //63448, &readbytes);
        for (unsigned int i = 0; i < readbytes; ++i)
            data += char(databuffer[i]);
            //data += char(text[i]);

        if(readbytes != 61200) //63448)
        {
            std::cout << "Error: Full Read too short (" << readbytes << "/61200)." << std::endl;
            Timing::Sleep(10);
            //delete[] text;
            //return data;
            count -= readbytes;
        }
        else
        {
            count -= 61200; //63448;
            Timing::Sleep(3);
        }
    }

    //ftdi->Read(text, static_cast<unsigned int>(count), &readbytes);
    ftdi->Read(databuffer, static_cast<unsigned int>(count), &readbytes);
    for (unsigned int i = 0; i < readbytes; i++)
        data += char(databuffer[i]);
        //data += char(text[i]);

    int abortloop = 5;
    while(readbytes != static_cast<unsigned int>(count) && count > 0 && abortloop-- >= 0)
    {
        std::cout << "Error: Rest Read too short (" << readbytes << "/" << count << ")" << std::endl;
        ftdi->ReOpen();

        Timing::Sleep(100);
        count -= readbytes;
        ftdi->Read(databuffer, static_cast<unsigned int>(count), &readbytes);
        for(unsigned int i = 0; i < readbytes; ++i)
            data += char(databuffer[i]);
        std::cout << "   reread received " << readbytes << " bytes." << std::endl;
    }

    //delete[] text;

    return data;
}

std::string NexysIO::Read(unsigned char address, int count, unsigned int sleeptime)
{
    if(ftdi == nullptr || count <= 0)
        return "";

    //Send a read query to the nexys board
    int localcount = count;
#if defined(__linux__)
    int numcmds = 0;
#endif
    while(localcount > 61200) //63448)
    {
        AddByte(NexysIO::HRead);
        AddByte(address);
        AddByte(239); //247);
        AddByte( 16); //208); //216);
        localcount -= 61200; //63448;
#if defined(__linux__)
        ++numcmds;
#endif
    }

    AddByte(NexysIO::HRead);
    AddByte(address);                                       //address to read from
    AddByte(static_cast<unsigned char>(localcount / 256));  //read length (high byte)
    AddByte(static_cast<unsigned char>(localcount % 256));  //read length (low byte)
    Flush();                                                //send the query
#if defined(__linux__)
    Timing::Sleep(sleeptime); // + 10 + 3 * numcmds);                 //give the nexys board time to respond
    //one full read of 63440 bytes should take the FPGA about 0.63ms to fill the FIFO, so add
    // this time (with safety factor) to the read delay
#else
    Timing::Sleep(sleeptime);                               //give the nexys board time to respond
#endif

    return Read(count);                                     //read the data from the nexys board
}

//########################
//### Sensor Functions ###
//########################


bool NexysIO::Write(unsigned char address, byte value, bool flush, unsigned int clockdiv)
{
    if(ftdi == nullptr)
        return false;

    //command header:
    AddByte(NexysIO::HWrite);                               //Write Header
    AddByte(address);                                       //Address (Fast Readout Control)
    AddByte(static_cast<unsigned char>(clockdiv >> 8));     //Length High Byte
    AddByte(static_cast<unsigned char>(clockdiv % 256));    //Length Low Byte

    AddBytes(value, clockdiv);

    if(flush)
        Flush();

    return true;
}


bool NexysIO::Write(unsigned char address, std::vector<byte> values, bool flush, unsigned int clockdiv)
{
    if(ftdi == nullptr)
        return false;

    //get the length of the command to send
    int size = int(values.size()) * int(clockdiv);
    //abort on too long commands:
    //if(size >= 1 << 16)
    //    return false;

    //command header:
    AddByte(NexysIO::HWrite);                       //Write Header
    AddByte(address);                               //Address (Fast Readout Control)
    AddByte(static_cast<unsigned char>(size >> 8)); //Length High Byte
    AddByte(static_cast<unsigned char>(size));      //Length Low Byte


    for(auto it = values.begin(); it != values.end(); ++it)
        AddBytes(*it, clockdiv);

    if(flush)
        Flush();

    return true;
}

bool NexysIO::Write(std::vector<std::pair<byte, byte> > values, bool flush, unsigned int clockdiv)
{
    bool returnval = true;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        byte temp1 = it->first;
        byte temp2 = it->second;
        returnval &= Write(temp1, temp2, false, clockdiv);
        //std::cout << (int)temp1 << "," << (int)temp2 << std::endl;
    }
    if (flush)
        Flush();
    return returnval;
}


bool NexysIO::WriteASIC(unsigned char address, std::vector<bool> values, NexysIO::Value Sin, NexysIO::Value ld, bool sendload, const int clockdiv)
{
    if(ftdi == nullptr)
        return false;

    std::cout << "Selected Sin and Load!   : " << Sin << " -> " << ld << std::endl;

    //unsigned int initialPosition = FTDIBuffPos;

    int length = int(values.size()) * 5 * clockdiv;
    if(sendload)
        length += 140 * clockdiv;  //(1 + 10 + 95 + 16*2 + 2) * clockdiv;
    /*if (length > 64000)
    {
        unsigned int index = 0;
        unsigned int bitspercommand = static_cast<unsigned int>(63800/ (5 * clockdiv));
        while (index < values.size() - bitspercommand)
        {
            std::vector<bool> valuepart;
            valuepart.reserve(static_cast<unsigned int>(bitspercommand));
            for (unsigned int i = 0; i<bitspercommand; i++)
            {
                valuepart.push_back(values[i+index]);
            }
            std::cout << std::endl;
            WriteASIC(address, valuepart, Sin, ld, false);
            index+=bitspercommand;
            std::cout << "Long Command (>64000): Part of command sent. (total: " << length << ")" << std::endl;
        }
        std::vector<bool> temp;
        temp.reserve(values.size()-index);
        for (unsigned int i = 0; i<values.size()-index; i++)
        {
            temp.push_back(values[i+index]);
        }
        values.clear();
        values.insert(values.end(), temp.begin(), temp.end());
        length = int(values.size()) * 5 * clockdiv;
        if(sendload)
            length += 115 * clockdiv;
    }*/
    unsigned char pattern = 0;

    AddByte(NexysIO::HWrite);           //Send Header
    AddByte(address);                   //Send Address
    AddByte(static_cast<unsigned char>(length / 256));              //Send Length
    AddByte(static_cast<unsigned char>(length % 256));

    unsigned int ckdiv = static_cast<unsigned int>(clockdiv);

   // for (auto it = values.begin(); it != values.end(); ++it)      //Send Values
   for (bool it : values)
    {


        pattern = ((it )?Sin:0);  //ld is used as !OE for shift registers (ICs) -> needs to be 1 during Ck1 pulses
      //  printf("Pattern for %d: %x\n",it,pattern);
       // std::cout << "Pattern for : " << it << "->" << std::atoi(pattern) << " -> " << Sin << std::endl;

        AddBytes(pattern,       ckdiv);
        AddBytes(pattern | Ck1, ckdiv);
        AddBytes(pattern,       ckdiv);
        AddBytes(pattern | Ck2, ckdiv);
        AddBytes(pattern,       ckdiv);
    }


    if(sendload)
    {
        //additional Ck1 pulse for updating the shift registers:
        //AddBytes(ld | Ck1, ckdiv);
        AddBytes(ld,       10 * ckdiv);
        //inverted Ld pulse:
        AddBytes(0,  20 * ckdiv);
       // AddBytes(ld, 75 * ckdiv); //give time for the pull-down resistors to discharge the load lines

        //fill the shift registers with 0, and keep outputs off (-> ld on)
       /* for(int i = 0; i < 16; ++i)
        {
            AddBytes(ld | Ck2, ckdiv);
            AddBytes(ld,       ckdiv);
        }
        AddBytes(ld | Ck1, ckdiv);
        AddBytes(ld,       ckdiv);*/
    }

    return true; //(initialPosition < FTDIBuffPos);
}

bool NexysIO::ConfigureLoad(unsigned char address, std::vector<bool> values, bool sendload)
{
    if(ftdi == nullptr)
        return false;

    const int clockdiv = 8;
    //unsigned int initialPosition = FTDIBuffPos;

    unsigned char pattern = Ld;
    int length = clockdiv * (3 * int(values.size()) + ((sendload)?140:0));

    AddByte(NexysIO::HWrite);           //Send Header
    AddByte(address);                   //Send Address
    AddByte(static_cast<unsigned char>(length / 256));              //Send Length
    AddByte(static_cast<unsigned char>(length % 256));

    for (auto it : values)      //Send Values
    {
        pattern = Ld + ((it)?SinA:0);

        AddBytes(pattern,       clockdiv);
        AddBytes(pattern | Ck2, clockdiv);  //used as shift clock for shift registers (ICs)
        AddBytes(pattern,       clockdiv);
    }

    if(sendload)
    {
        //additional Ck1 pulse for updating the shift registers:
        AddBytes(Ld | Ck1, clockdiv);
        AddBytes(Ld,       10 * clockdiv);
        //inverted Ld pulse:
        AddBytes(0,  20 * clockdiv);
        AddBytes(Ld, 75 * clockdiv); //give time for the pull-down resistors to discharge the load lines

        //fill the shift registers with 0, and keep outputs off (-> ld on)
        for(int i = 0; i < 16; ++i)
        {
            AddBytes(Ld | Ck2, clockdiv);
            AddBytes(Ld,       clockdiv);
        }
        AddBytes(Ld | Ck1, clockdiv);
        AddBytes(Ld,       clockdiv);
    }

    return true; // (initialPosition < FTDIBuffPos);
}

bool NexysIO::SendLoad(unsigned char address, unsigned char Ld, unsigned int clockdiv)
{
    if(ftdi == nullptr) // || FTDIBuffPos != 0)
        return false;

    //const int clockdiv = 6;
    int length = 8 * clockdiv;

    AddByte(NexysIO::HWrite);           //Send Header
    AddByte(address);                   //Send Address
    AddByte(static_cast<unsigned char>(length / 256));              //Send Length
    AddByte(static_cast<unsigned char>(length % 256));


    AddBytes(0,      clockdiv);
    AddBytes(Ld, 4 * clockdiv);
    AddBytes(0,  3 * clockdiv);

    return true;
    //return Flush();
}

//#####################
//### PCB Functions ###
//#####################

bool NexysIO::WritePCB(unsigned char address, std::vector<bool> values, const unsigned int clockdiv)
{
    if(ftdi == nullptr)
        return false;

    unsigned int initialPosition = FTDIBuffPos;

    unsigned int length = (values.size() * 3 + 4 + 8 * 2) * clockdiv;
    if (length > 65535)
        return false;

    unsigned char pattern = 0x00;

    // Write Protocol Header
    // - Write byte
    // - Address
    // - Number of bytes written in series to the register
    AddByte(NexysIO::HWrite);                     //Send Header
    AddByte(address);                   //Send Address
    AddByte(static_cast<unsigned char>(length / 256));                   //Send Length
    AddByte(static_cast<unsigned char>(length % 256));

    // Add Values for whole Shift Register Chain
    //-----------
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        pattern = (*it)?PCB_SIN:0;

        AddBytes(pattern         , clockdiv);
        AddBytes(pattern | PCB_Ck, clockdiv);
        AddBytes(pattern         , clockdiv);
    }

    // Load Signal:
    // The laod signals for the function cards are connected
    // to an 8 bit SR first in the chain
    //  - Toggle Load of the Loads SR to activate the appropriate Card load
    //  - Send 8 bits 0 to the SR chain to reset the load signal
    //  - Load again to set all loads back to 0
    //------------------

    //activate Load signal (as output of Shift Register):
    AddBytes(PCB_Ld, clockdiv); // 1 to PCB_Ld bit
    AddBytes(0     , clockdiv); // 0 to the whole register


    // Just clock 8 times
    //fill shift register with '0':
    //  (clock for the 14bit DACs is halted internally during Load, and others don't care)
    //  (the shift register on the RegisterCard in edge sensitive on load signal input, so also ok)
    for(int i = 0; i < 8; ++i)
    {
        AddBytes(PCB_Ck, clockdiv); // Ck -> 1 , others to 0
        AddBytes(0     , clockdiv); // Ck -> 0
    }

    //second load pulse to deactivate the load signals for the function cards:
    AddBytes(PCB_Ld, clockdiv);
    AddBytes(0     , clockdiv);

    // Finished

    return (initialPosition < FTDIBuffPos);
}

bool NexysIO::AddDelay(unsigned long usec)
{
    if(ftdi == nullptr)
        return false;

    int length = usec * 1000 / 17;

    AddByte(NexysIO::HWrite);                //Send Header
    AddByte(0);                              //Send Address (0 = configuration lines)
    AddByte(static_cast<unsigned char>(length / 256));                   //Send Length
    AddByte(static_cast<unsigned char>(length % 256));

    AddBytes(0,static_cast<unsigned int>(length));

    return true;
}

//Pattern Generator is obsolete: replaced by functions in injection_config.h
//#########################
//### Pattern Generator ###
//#########################

void NexysIO::PatGenReset(bool reset)
{
    AddByte(NexysIO::HWrite);         //Send Header
    AddByte(PGreset);       //Send Address
    AddByte(0);             //Send Length
    AddByte(1);
    AddByte((reset)?1:0);
}

void NexysIO::PatGenSuspend(bool suspend)
{
    AddByte(NexysIO::HWrite);         //Send Header
    AddByte(PGsuspend);     //Send Address
    AddByte(0);             //Send Length
    AddByte(1);
    AddByte((suspend)?1:0);
}

void NexysIO::PatGen(int period, int cycle, int clkdiv, int initdelay, bool oldpatgen)
{

    /*
        pattern generator register file and description (old):

        $addr:	(8) toggle timestamps
            when subcounter reaches toggle timestamp,
            signal is toggled, and next toggle timestamp is loaded
        $addr+8: period
            when subcounter reaches period, subcounter
            is resetted (and first toggle timestamp loaded if desired)
        $addr+9: bitwise behaviour control
            0: rststate (0/1/tri?)
            1: initstate (while in start delay)
            2: rststate on done  (otherwise, keep last state)
            3: initstate on done (otherwise, keep last state)
            4: rststate on suspend  (otherwise, keep last state)
            5: initstate on suspend (otherwise, keep last state)
            6:
            7: do not reset timestamp ptr on period reach
        $addr+10/11:	runlen
            count of patternticks this generator shall run
            0: run infinite
        $addr+12/13:	fastclock initial delay
            start delay: count of fastticks until patternticks are generated
        $addr+14/15:	clkfac
            fire patterntick each clkfac fastticks

        Changes for new version:
            the toggle time stamps do not exist any more, but addr 7 contains the number of
            pulses in pulse trains, the value 0 is equal to 1 generating a "train" with 1 pulse
    */

    //Timestamps
    unsigned char timestamps[8] = {1,3,0,0,0,0,0,0};
    for(unsigned char i = 0; i < 8; ++i)
        PatGenWrite(i, timestamps[i]);

    //Period
    PatGenWrite(8, static_cast<unsigned char>(period));

    //Flags
    PatGenWrite(9, 0b010100); //0b1111); //0);  //initstate = 0

    //runlength
    int runlength = ((oldpatgen)?period:1) * cycle;
    PatGenWrite(10, static_cast<unsigned char>(runlength >> 8));
    PatGenWrite(11, static_cast<unsigned char>(runlength % 256));

    //initial delay
    PatGenWrite(12, static_cast<unsigned char>(initdelay >> 8));
    PatGenWrite(13, static_cast<unsigned char>(initdelay % 256));

    //clkdiv
    PatGenWrite(14, static_cast<unsigned char>(clkdiv >> 8));
    PatGenWrite(15, static_cast<unsigned char>(clkdiv % 256));
}

void NexysIO::RestartPatGen(bool flush)
{
    PatGenReset(1);
    PatGenReset(0);
    PatGenSuspend(0);
    if(flush)
        Flush();
}

void NexysIO::StartFastRead(unsigned char address, unsigned int readsize, unsigned int readingwait, bool zerosuppressed)
{
        workeraddress  = address;
        workerreadsize = 12240 * readsize;
        workersleep    = readingwait;
        workerzerosuppressed = zerosuppressed;
        if(workerrunning)
            return;

        workerrunning = true;

        worker = new std::thread(FastReadWorker, this);
}

void NexysIO::StopFastRead()
{
    if(!workerrunning)
        return;

    workerrunning = false;
    if(worker->joinable())
        worker->join();
    delete worker;
    worker = nullptr;

    return;
}

std::string NexysIO::FastRead(unsigned int characters)
{
    if(firstelementremains.length() < characters)
    {
        if((workerrunning && readbuffer.size() > 1) || (!workerrunning && readbuffer.size() > 0))
        {
            firstelementremains += readbuffer.front();
            readbuffer.pop_front();
        }
    }

    if(characters >= firstelementremains.length())
    {
        std::string answer = firstelementremains;
        firstelementremains = "";
        return answer;
    }
    else
    {
        std::string answer = firstelementremains.substr(0, characters);
        firstelementremains = firstelementremains.substr(characters);
        return answer;
    }
}

bool NexysIO::FastReadDataAvailable()
{
    //return readbuffer.size() > 0;
    return (firstelementremains.length() > 0 || (workerrunning && readbuffer.size() > 1)
            || (!workerrunning && readbuffer.size() > 0));
}

bool NexysIO::ClearFastReadBuffer()
{
    if(worker == nullptr)
    {
        readbuffer.clear();
        return true;
    }
    else
        return false;
}

bool NexysIO::PatGenWrite(unsigned char patgenaddress, unsigned char patgenvalue)
{
    bool result = true;

    //set address inside pattern generator for the next data byte:
    result &= Write(PGaddress, patgenaddress, false);
    //send the data byte for the address above:
    result &= Write(PGdata,    patgenvalue,   false);   //initstate = 0
    //WriteStrobe:
    result &= Write(PGwrite,   1,             false);
    result &= Write(PGwrite,   0,             false);

    return result;
}



void FastReadWorker(NexysIO *nexys)
{
    if(nexys == nullptr || !nexys->is_open())
    {
        std::cout << "Could not start Fast Reading as NexysIO is not ready!" << std::endl;
        return;
    }

    unsigned long long totaldata = 0;

    if(!nexys->workerzerosuppressed)
        while(nexys->workerrunning)
        {
            nexys->readbuffer.push_back(nexys->Read(nexys->workeraddress, int(nexys->workerreadsize), nexys->workersleep));
            totaldata += nexys->readbuffer.back().length();
        }
    else
    {
        std::string answer = "";
        std::string answerraw = "";

        while(nexys->workerrunning)
        {
            answerraw += nexys->Read(nexys->workeraddress, int(nexys->workerreadsize), nexys->workersleep);

            for(unsigned int i = 0; i < answerraw.length(); i += 8)
                if(answerraw.substr(i,8).compare("\xff\xff\xff\xff\xff\xff\xff\xff") != 0)
                    answer += answerraw.substr(i,8);

            totaldata += answerraw.length();

            if((answerraw.length() &  0b00000111) != 0)
                answerraw = answerraw.substr(answerraw.length() & 0xfffffff8);
                        //assuming size_type is an uint32 - correct for sizes up to 4GB
            else
                answerraw = "";

            if(answer != "")
            {
                nexys->readbuffer.push_back(answer);
                answer = "";
            }
        }
    }

    std::cout << "Fast Reading: Worker Ending\nTotal amount of data read: " << totaldata << " Bytes" << std::endl;
}

bool NexysIO::is_programmed(){
    //Write and read from Register 0x00
    Write(0x09,0b10101010);
    unsigned int i = int(Read(0x09,1).c_str()[0]) & 255;

    if (i == 0b10101010){
        return true;
    }
    return false;
}
