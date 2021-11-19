

#ifndef FTDI_H
#define FTDI_H
//#include "WinDef.h"
//#include "windows.h"
//#include "libs/ftd2xx.h"
#if defined(__linux__)
#include "WinTypes.h"
#include "ftd2xx.h"
#endif

#if defined(_WIN64)
    /* Microsoft Windows (64-bit). ------------------------------ */
#include "windows.h"
#include "FTD2XX.H"
#elif defined(_WIN32)
    /* Microsoft Windows (32-bit). ------------------------------ */
#include "windows.h"
#include "FTD2XX.H"
#endif

#include <iostream>

#include <vector>

/**
 * The FTDI User class holds the connection for single ftdi device.
 * It provides basically the same functions as the ftd2xx.h, but with a private ftdiHandle.
 */

class FTDI
{
 private:
  FT_HANDLE ftdiHandle;
  FT_STATUS status;

  int index_for_reopen;
 public:
  /**
   * Constructor. Initialize FTDI class. 
   * Sets ftdiHandle = 0  */
  FTDI(void);
  
  /** Destructor. */
  ~FTDI(void);
  
  /** Open a connection to device 0. */
  bool Open(void);
  
  /** Open a connection to the DeviceIndex. Requires the DeviceIndex of the board. */
  bool Open(int DeviceIndex);
  
  /**
   * Read from the FTDI device.
   * 
   * Requires an address of the buffer to be read in,
   * the buffer size (in other words bytes) to read 
   * and an address of a long integer bytesreturned
   */
  bool Read(void* lpBuffer, long unsigned int nBufferSize, long unsigned int* lpBytesReturned);
  /**
   * Write to the FTDI device.
   *
   * Requires the address of the buffer to write into the device,
   * the buffer size (in other words the bytes) the be written
   * and an address of a long integer variable, which returns the number of bytes written.
   * Returns TRUE, if successful.
   */
  bool Write(void* lpBuffer, long unsigned int nBufferSize, long unsigned int* lpBytesWritten);
  /**
   * Closes the USB connection to the FTDI device. 
   * Returns TRUE if successful.
   */
  bool Close(void);
  /**
   * Resets the device. 
   * Returns TRUE if successful. 
  */
  bool resetDevice(void);
  /**
   * Sets the mode. Refer to the FTDI programmer's manuel for more information.
   * Do not use, until you are very sure, what you do!
   * Returns TRUE if successful.
  */
  bool setBitMode(UCHAR Mask, UCHAR Mode);
  
  /**
   * Sets the baud rate for transmission. No need to set in standard mode. 
   * Returns TRUE if successful.
   */
  bool setBaudRate(DWORD BaudRate);

  /**
   * Gets the status of the read queue of the FTDI chip.
   * Requires the address of long unsigned int for the number of bytes in the queue.
   * Returns TRUE, if successful.
   */
  bool getQueueStatus(long unsigned int* queueBytes); // DWORD*

  /**
   * Get the Status of the FTDI device.
   *
   * Requires the address of 3 long unsigned integer variables.
   * 1. Number of bytes in receive queue.
   * 2. Number of byes in transmit queue.
   * 3. Eventstatus. Refer to programmer's manual for more information.
   *
   * Returns TRUE, if successful.
   */
  bool getStatus(DWORD* receive, DWORD* transmit, DWORD* eventstatus);

  /** Purge the buffer. */
  bool purge(bool RX, bool TX);

  /**
     Set the data characteristics. */
  bool setDataCharacteristics(/*unsigned char wordlength, unsigned char stopbits, unsigned char parity*/);

  /**
   * Set the timeouts for read and write.
   * 
   * Requires timeout in milliseconds for reading,
   * timeout in milliseconds for writing.
   *
   * Returns TRUE, if successful.
   */
  bool setTimeouts(long unsigned int readTimeout, long unsigned int writeTimeout);

  /**
   * Read back the instantaneous value of the FTDI pin out settings.
   */
  bool getBitMode(unsigned char* lpBuffer); // Read back the instantaneous values of the Ftdi chip

  /** Nanosleep is implemented in this class to have consistent sleep mode with all systems. sleep() will sleep for the given amount of milliseconds.
    *
    * Requires a time given in milliseconds.
    */
  void sleep(int milliseconds);

  bool getDeviceInfo(FT_DEVICE* Device, unsigned long int* ID, char* serialnumber, char* description);
  bool listDevices(void* arg1, void* arg2, unsigned long int Flags);
  bool OpenEx(void* arg1, unsigned long int flags);
  bool ReOpen();
};


#endif	/* FTDI_H */

