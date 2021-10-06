# Astropix 1 Firmware
The firmware has to be programmed first!  
Please see the [Nexys Programming Guide](https://reference.digilentinc.com/learn/programmable-logic/tutorials/nexys-video-programming-guide/start) for detailed instructions on USB, SD or Flash programming.

Maybe it would make sense to make an FPGA bit file release otherwise:

- The project is pushed with Vivado 2020.1 version config files, use this version to avoid warnings and IP block updates 
- The project is located under GECCO_Firmware


# Astropix 1 Software 

This software is used to configure the Astropix Asic internal Shift Register. 
For now only configuration for analog outputs is available, readout counters using SPI is a work in progress.


This project is best build using QT Creator under windows, Linux support requires a bit more support for the FTDI driver and some HTTP libraries.

One running, it will provide both a GUI to send configs to the Asic on the Shift Register interface, and also a small HTTP REST API to change to 
configuration values and update the ASIC. 
This allows easily writting scripts to run measurements. 

## Building under windows

- ~~Install IVI Compliance Package~~ (Not need with newest version)
- Install QT and QT Creator from QT official website using the community download packages
- Install QT 5 with Mingw 64bit runtime (no specific version, the latest 5.15.1 was tested)
- Open the Project located in this folder "gecco_top.pro" in QT Creator, it should build and run out of the box 
    - The project Contains two sub-projects: The Astropix_v1 Software, which is the GUI, and the qthttpserver library used for the REST API. 

## Building under Linux 

For Linux the FTID library must be installed manually, and some settings must be set for the user to have access to the Nexys Video USB Device. 

!! At the moment the qthttpserver library won't compile out of the box, the list of required packages is still being figured out !! 

The following instructions are courtesy of Rudolf Schimassek @ KIT 

### 0. Install Qt environment


Before starting with the drivers. make sure that the Qt framework is installed e.g. by starting Qt-Creator:
    qtcreator -version
    
If it is not installed, install it first (Qt is needed for the GUI tool to configure the FPGA):

    sudo apt-get update
    sudo apt-get install qt5-default qtcreator libqt5serialport5-dev qtmultimedia5-dev

For setting up a Raspberry Pi, refer to the Internship report of Lei Wang (in RaspberryPi/Praktikumbericht_Lei_Wang/thesis.pdf)

### 1. Install FTDI Drivers (Linux 64bits)


First, the FTD2XX driver has to be downloaded from ftdichip.com for the right operating system and processor
architecture. 

To extract the data, use the following commands:
    gunzip libftd2xx-x86_64-1.4.8.gz
    tar –xvf libftd2xx-x86_64-1.4.8

Then go to the build directory of the driver:
    cd release/build
Change to root user:
    sudo -s
and copy the driver files to the folder where the operating system expects them:
    cp libftd2xx.* /usr/local/lib/
Now, the driver needs to be accessible for users and a link has to be created to hide the version number:
    chmod 0755 /usr/local/lib/libftd2xx.so.1.4.8
    ln -s -T /usr/local/lib/libftd2xx.so.1.4.8 /usr/local/lib/libftd2xx.so
Exit the root terminal again:
    exit
    
This can also be found in the "release/ReadMe.txt" of the driver


### 2. Select FTDI Drivers


For FTDI devices, two drivers exist but exclude each other for usage. The FT2232H on the NexysVideo needs the FTD2XX driver
and will not work with the FTDI_SIO driver to which it is assigned to by default.

Therefore, a rule has to be created to not use the FTDI_SIO driver for the NexysVideo board.

Create a rules file at the operating system folder:
    sudo touch /etc/udev/rules.d/99-ft2232h.rules
This file needs to contain a rule to remove the FTDI_SIO driver for the vendor and product ID of the NexysVideo:
Start an editor to add the rule to the file:
    sudo nano /etc/udev/rules.d/99-ft2232h.rules
    
Enter the following lines to the file:

    ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010",\
    PROGRAM="/bin/sh -c '\
        echo -n $id:1.0 > /sys/bus/usb/drivers/ftdi_sio/unbind;\
        echo -n $id:1.1 > /sys/bus/usb/drivers/ftdi_sio/unbind\
    '"

    ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010",\
    MODE="0666"
    
The first four lines remove the FTDI_SIO driver from both channels of the FT2232h chip on the NexysVideo board.
The latter two lines make the device accessible also for normal users.
The product and vendor IDs are 0403 (Vendor) and 6010 (Product).
Save the file and then unplug and replug the USB cable of the NexysVideo.
To check that it worked, consider the output of dmesg:
    dmesg
There, you should find something like this in the output:

        ...
    [ 4535.964911] usb 1-10.1: new high-speed USB device number 19 using xhci_hcd
    [ 4536.069997] usb 1-10.1: New USB device found, idVendor=0403, idProduct=6010
    [ 4536.070002] usb 1-10.1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
    [ 4536.070006] usb 1-10.1: Product: Digilent USB Device
    [ 4536.070008] usb 1-10.1: Manufacturer: Digilent
    [ 4536.070011] usb 1-10.1: SerialNumber: 210276A79302
    [ 4536.073027] ftdi_sio 1-10.1:1.0: FTDI USB Serial Device converter detected
    [ 4536.073153] usb 1-10.1: Detected FT2232H
    [ 4536.073443] usb 1-10.1: FTDI USB Serial Device converter now attached to ttyUSB0
    [ 4536.075742] ftdi_sio 1-10.1:1.1: FTDI USB Serial Device converter detected
    [ 4536.075839] usb 1-10.1: Detected FT2232H
    [ 4536.076127] usb 1-10.1: FTDI USB Serial Device converter now attached to ttyUSB1
    [ 4536.090089] ftdi_sio ttyUSB0: FTDI USB Serial Device converter now disconnected from ttyUSB0
        ...
        
The important lines are the ones stating "...now disconnected from ...". This means, that the FTDI_SIO driver is not
used for this device.


