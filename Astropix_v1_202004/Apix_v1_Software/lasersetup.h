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

/********************************************************************
 * Communication Class for the KIT-ADL Laser Setup                  *
 *                                                                  *
 * Author: Rudolf Schimassek                                        *
 * Version: 1.0 (08.05.2018)                                        *
 *          1.1 (04.06.2018)                                        *
 *            - added a parameter to make the scan pattern          *
 *              changeable (zick-zack or one directional). Default  *
 *              setting is the one directional scan                 *
 *            - added a function to initialise only one axis at a   *
 *              time                                                *
 *          1.2 (05.06.2018)                                        *
 *            - added functions to control the trigger of the laser *
 *            - added functions to control the speed of the stages  *
 *          1.3 (15.06.2018)                                        *
 *            - bugfix: position was not updated during non zick-   *
 *                  zack scan                                       *
 * Notes:                                                           *
 *    - When defining a measurement function using functionality    *
 *      from mainwindow, define the function as a friend function   *
 *      of MainWindow:                                              *
 *              friend double `measure`(double, double, void*);     *
 *    - The class is made for use with USB to serial adapters for   *
 *      both laser driver and stage drivers                         *
 *    - add "serialport" in your project file to QT:                *
 *              QT += serialport                                    *
 ********************************************************************/

#ifndef LASERSETUP_H
#define LASERSETUP_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QApplication>
#include <QString>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>

#include "libs/timing.h"


class LaserSetup
{
public:
    LaserSetup();
    /**
     * @brief ConfigureLaserPort sets the name of the port to use for the communication with
     *             laser driver, the only free parameter of the configuration is the port, the
     *             rest is fixed by the laser driver
     * @param portname       - name of the port as string (e.g. "COM1")
     * @return               - true if the port name was accepte, false otherwise
     */
    bool ConfigureLaserPort(std::string portname);
    /**
     * @brief ConfigureStagePort sets the name of the port to use for the communication with
     *             the stage drivers and the baud rate. The baud rate has to match the setting
     *             of the dip switches on the stage drivers. However, faster baud rates than
     *             9600 baud caused timeout problems. Therefore it is better to use 9600 baud.
     * @param portname       - name of the port as string (e.g. "COM2")
     * @param baudrate       - baud rate for the connection, only change if you know what you are
     *                            doing
     * @return               - true if the port/baud rate combination was accepted, false otherwise
     */
    bool ConfigureStagePort(std::string portname, int baudrate = 9600);

    /**
     * @brief GetPortList searches the hardware for serial ports and returns the list
     * @return               - list of available serial port names on the computer
     */
    static std::vector<std::string> GetPortList();
    /**
     * @brief IdentifyPorts searches for serial ports and then tests them one by one whether the
     *             laser driver or the stages are connected to it. If one of the devices is found,
     *             the connection is kept open, if not it is closed again. On success the setup is
     *             connected. Found connections are not closed: If one connection is not found, the
     *             stays open even though the function returns false.
     * @return               - true if both the laser driver and the stage drivers were found,
     *                            false otherwise
     */
    bool IdentifyPorts();

    //--- functions to test whether the connections are established ---
    bool Laser_is_open();
    bool Stages_is_open();
    ///test if both connections are open:
    bool is_open();

    //--- Laser functionality ---
    enum triggeredge{falling=0, rising=1};
    enum triggersource{internal=0, extAdj=1, extTTL=2};
    bool OpenLaserPort();
    bool CloseLaserPort();

    /**
     * @brief ActivateLaser changes the state of the laser turning it on or off
     * @param onoff          - new desired state (on=true, off=false)
     * @return               - true if the command was successfully sent, no guarantee that the
     *                            laser is actually on (e.g. due to interlock not closed)
     */
    bool ActivateLaser(bool onoff);
    /**
     * @brief GetLaserState retrieves the laser state from the laser driver
     * @return               - current status of the laser (true=on, false=off)
     */
    bool GetLaserState();

    /**
     * @brief GetTuneValue provides the tune value of the laser pulse
     * @return               - the tune value in percent or -1 on failure
     */
    double GetTuneValue();
    /**
     * @brief SetTuneValue sets the tune value of the laser pulse
     * @param tune           - new tune value in percent [0,100]
     * @return               - true if the value was successfully set, false if not
     */
    bool SetTuneValue(double tune);

    /**
     * @brief GetTriggerFrequency retrieves the internal trigger frequency of the laser driver
     * @return               - the trigger frequency in Hertz or -1 on failure
     */
    int GetTriggerFrequency();
    /**
     * @brief SetTriggerFrequency changes the internal trigger frequency of the laser driver
     * @param freq           - the desired frequency in Hertz
     * @return               - true on success, false otherwise. Only several specific values
     *                            are possible. A return value true does not mean that the trigger
     *                            frequency is the passed value but the closest possible value.
     *                            Call GetTriggerFrequency() to get the actual frequency.
     */
    bool SetTriggerFrequency(int freq);
    /**
     * @brief GetTriggerEdge retrieves the selected trigger edge from the laser driver
     * @return               - the values of the enum triggeredge on successul read or -1 on
     *                            an error
     */
    int GetTriggerEdge();
    /**
     * @brief SetTriggerEdge changes the trigger edge setting of the laser driver
     * @param edge           - the trigger edge setting to use (see enum triggeredge for possible
     *                            values)
     * @return               - true on success, false on wrong parameter value or failure
     */
    bool SetTriggerEdge(unsigned int edge);

    /**
     * @brief GetTriggerSource retrieves the trigger source set for the laser
     * @return               - the trigger source as value of the enum triggersource or -1 on an
     *                            error
     */
    int GetTriggerSource();
    /**
     * @brief SetTriggerSource changes the trigger source used by the laser source
     * @param source         - the new trigger source to use as entry of the enum triggersource
     * @return               - true if the value was accepted and transmitted, false otherwise
     */
    bool SetTriggerSource(unsigned int source);
    /**
     * @brief GetTriggerLevel retrieves the trigger threshold for external adjustable trigger
     * @return               - the trigger voltage in volt
     */
    double GetTriggerLevel();
    /**
     * @brief SetTriggerLevel changes the trigger level for adjustable trigger input
     * @param level          - the trigger value in volt (only -4.8V .. +4.8V are accepted)
     * @return               - true if the value was set, false if not
     */
    bool SetTriggerLevel(double level);



    /**
     * @brief WriteLaser sends a command to the laser driver. The function returns after the
     *             specified sleep
     *        CAUTION: only use if you know what you are doing
     *
     * @param command        - the command to send to the laser
     * @param extendsleep    - scaling factor for the sleep command after the write. The sleep
     *                            duration is set with SetLaserSleep().
     * @return               - true if the command was sent successfully, false otherwise
     */
    bool WriteLaser(std::string command, double extendsleep = 1);
    /**
     * @brief ReadLaser reads the passed number of characters from the laser driver port
     *        CAUTION: only use if you know what you are doing
     *
     * @param chars          - the number of characters to read at maximum
     * @return               - the read data from the laser port
     */
    std::string ReadLaser(unsigned int chars);

    /**
     * @brief GetLaserSleep provides the standard sleep duration after a write command to
     *             the laser driver
     * @return               - the sleep duration in milliseconds
     */
    int GetLaserSleep();
    /**
     * @brief SetLaserSleep sets a new value for the sleep duration after a write command
     *             to the laser driver
     * @param ms             - new standard sleep duration in milliseconds
     */
    void SetLaserSleep(unsigned int ms);

    //--- Movement functionality ---
    enum reference{negLimit, refPos, posLimit};  //reference position labels for the stages
    enum axislabels{z=1, y=2, x=3, all=4};       //axis labels

    bool OpenStagePort();
    bool CloseStagePort();

    /**
     * @brief InitialiseStages initiates movements of all stages to the specified reference point
     *             after turning on the motors and retrieving the sizes of the stages.
     *        CAUTION: the function sends all necessary commands for referencing the stages, but
     *                 it does not wait for the axes to be done referencing.
     *                 Call WaitForAxes(LaserSetup::all) after this function to wait for the
     *                 referencing to be finished
     * @param referencepoint - reference point to use (options: negLimit, refPos, posLimit)
     * @return               - true on success, false otherwise
     */
    bool InitialiseStages(int referencepoint);
    /**
     * @brief InitialiseStage initialtes movement of ONE axis to the specified reference point
     *             after turning on the motors and retrieving the sizes of the stages.
     *        CAUTION: the function sends all necessary commands for referencing the stages, but
     *                 it does not wait for the axes to be done referencing.
     *                 Call WaitForAxes(LaserSetup::all) after this function to wait for the
     *                 referencing to be finished
     * @param axis           - the axis to reference (LaserSetup::x,y,z)
     * @param referencepoint - reference point to use (options: negLimit, refPos, posLimit)
     * @return               - true on success, false otherwise
     */
    bool InitialiseStage(int axis, int referencepoint);
    /**
     * @brief GetInitStatus provides whether the axis has been referenced or not
     * @param axis           - the axis of interest (LaserSetup::x,y,z,all)
     * @return               - true if the axis was referenced,
     *                            false if not or axis index out of range
     */
    bool GetInitStatus(int axis);

    /**
     * @brief TurnOnMotor turns on the motor of the passed axis
     * @param axis           - index of the axis of choice (LaserSetup::x,y,z)
     * @return               - true of the command was sent, false if not (e.g. index out of range)
     */
    bool TurnOnMotor(int axis);
    /**
     * @brief TurnOffMotor turns off the motor of the passed axis
     * @param axis           - index of the axis of choice (LaserSetup::x,y,z)
     * @return               - true of the command was sent, false if not (e.g. index out of range)
     */
    bool TurnOffMotor(int axis);

    /**
     * @brief ReferenceAxis initiates a reference move of the specified axis to the specified
     *             reference point
     *        CAUTION: the function does not wait for the reference move to be finished.
     *                 Call WaitForAxes() to wait for the reference move to be finished.
     * @param axis           - axis of interest (LaserSetup::x,y,z)
     * @param referencepoint - desired reference point (LaserSetup::negLimit, refPos, posLimit)
     * @return               - true if the command was sent, false if not (e.g. invalid axis or
     *                            reference position or connection not open)
     */
    bool ReferenceAxis(int axis, int referencepoint);

    /**
     * @brief SetMinPosition set the mininum position of the stage allowed by software.
     *             a position smaller than this value will be changed to this value for
     *             movements
     * @param axis           - axis to set the minimum position (LaserSetup::x,y,z)
     * @param minvalue       - minimum position on the axis in mm
     * @return               - true if the value was set, false otherwise
     */
    bool SetMinPosition(int axis, double minvalue);
    /**
     * @brief GetMinPosition provides the minimum position allowed for the specified axis
     *             either from software or from the stage driver
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @param onhardware     - the value will be retrieved from the driver if this parameter
     *                            is set to true, also the minimum position will be changed
     *                            to the reveiced value
     * @return               - the minimum position for this axis in mm
     */
    double GetMinPosition(int axis, bool onhardware=false);
    /**
     * @brief SetMaxPosition set te maximum position of the stage allowed by software.
     *             a position larger than this value will be changed to this value for
     *             movements
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @param maxvalue       - maximum position on the axis in mm
     * @return               - true if the value was set, false otherwise
     */
    bool SetMaxPosition(int axis, double maxvalue);
    /**
     * @brief GetMaxPosition provides the maximum position allowed for the specified axis
     *             either from software or from the stage driver
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @param onhardware     - the value will be retrieved from the driver if this parameter
     *                            is set to true, also the minimum position will be changed
     *                            to the reveiced value
     * @return               - the maximum position for this axis in mm
     */
    double GetMaxPosition(int axis, bool onhardware=false);
    /**
     * @brief GetPosition provides the current position of the stage either from the software
     *             value or from hardware
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @param onhardware     - the value will be retrieved from the driver if this parameter
     *                            is set to true, also the minimum position will be changed
     *                            to the reveiced value
     * @return               - the current position for this axis in mm
     */
    double GetPosition(int axis, bool onhardware=false);

    /**
     * @brief GetSpeed retrieves the speed setting of the motor of interest
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @return               - the velocity of the axis in physical units / s
     */
    double GetSpeed(int axis);
    /**
     * @brief SetSpeed sets a new speed setting for the desired axis
     * @param axis           - the axis of interest (LaserSetup::x,y,z)
     * @param speed          - the speed setting in physical units / s (default: 1.5)
     * @return               - true on success, false otherwise
     */
    bool SetSpeed(int axis, double speed);
    /**
     * @brief Move initiates a movement of the specified axis by a lenght
     *        CAUTION: the function does not wait for the movement to be finished.
     *                 Call WaitForAxes() to wait for the movement to finish.
     * @param axis           - the desired axis (LaserSetup::x,y,z)
     * @param length         - travel distance in mm
     * @param backlashcompensation
     *                       - compensation for stage tolerance to ensure to always have the same
     *                            position not depending on play of the stage.
     *                            A positive number indicates that any position is approached from
     *                              a position with smaller value, smaller by this parameter
     *                            A negative number indicates that any position is approached from
     *                              a position with a larger value, larger by the absolute of this
     *                              parameter
     *                            For a value of "0", this feature is turned off
     * @return               - the new absolute target position in mm on success,
     *                            or -1e10 on failure
     */
    double Move(int axis, double length, double backlashcompensation = 0);
    /**
     * @brief MoveAbsolute initiates a movement of the specified axis to a target position
     * @param axis           - the desired axis (LaserSetup::x,y,z)
     * @param position       - new absolute target position in mm
     * @param backlashcompensation
     *                       - compensation for stage tolerance to ensure to always have the same
     *                            position not depending on play of the stage.
     *                            A positive number indicates that any position is approached from
     *                              a position with smaller value, smaller by this parameter
     *                            A negative number indicates that any position is approached from
     *                              a position with a larger value, larger by the absolute of this
     *                              parameter
     *                            For a value of "0", this feature is turned off
     * @return               - the new target position in mm on success,
     *                            or -1e10 on failure
     */
    double MoveAbsolute(int axis, double position, double backlashcompensation = 0);

    /**
     * @brief AxisOnTarget provides information whether an axis is on its target position
     * @param axis           - axis of interest (LaserSetup::x,y,z)
     * @return               - true if axis is on target position, false otherwise
     */
    bool AxisOnTarget(int axis);
    /**
     * @brief WaitforAxes returns when all specified axes reached their respective target positions
     * @param axis           - axis of interest (LaserSetup::x,y,z,all).
     *                            a number >= LaserSetup::all is interpreted as LaserSetup::all
     */
    void WaitforAxes(int axis);
    /**
     * @brief StopAxis aborts the movement of the specified axis
     * @param axis           - desired axis (LaserSetup::x,y,z)
     */
    void StopAxis(int axis);

    /**
     * @brief WriteStages sends a command to the stage drivers.
     *        CAUTION: only use if you know what you are doing
     *
     * @param command        - the command to send
     * @param extendsleep    - scaling factor for the sleep duration after sending the command
     * @return               - true if the command was sent, false otherwise
     */
    bool WriteStages(std::string command, double extendsleep = 1);
    /**
     * @brief ReadStages reads a number of characters from the port opened for the stage drivers
     *        CAUTION: only use if you know what you are doing
     *
     * @param chars          - the maximum number of characters to read from the port
     * @return               - a string containing the read data
     */
    std::string ReadStages(unsigned int chars);

    /**
     * @brief GetStageSleep provides the sleep duration after a write command for the
     *             stage driver port
     * @return               - the standard sleep duration in milliseconds
     */
    int GetStageSleep();
    /**
     * @brief SetStageSleep sets the standard sleep duration for commands sent to the stages
     * @param ms             - new standard sleep duration in milliseconds
     */
    void SetStageSleep(int ms);

    //--- Measurement Patterns ---
    /**
     * Scan2D scans the rectangle (xstart,ystart) - (xend, yend) with the precision (deltax,deltay)
     * where at each position after a delay (`measuredelay`) the passed measure function is called.
     * The return values of the measure function are stored in a file and the position with the
     * highest measure is returned at the end
     *
     * @param running        - monitoring flag, set to false to abort the measurement. The scan
     *                              will only run if it is set to true.
     * @param xstart         - start of the interval to scan on x axis
     * @param xend           - end of the interval to scan on x axis
     * @param ystart         - start of the interval to scan on y axis
     * @param yend           - end of the interval to scan on y axis
     * @param deltax         - step size on x axis
     * @param deltay         - step size on y axis
     * @param measuredelay   - delay before the measurement after the target position was reached
     *                              in ms
     * @param laseroff       - determines whether the laser is turned off during the movements
     * @param measure        - function pointer to be called at each position. The parameters are
     *                              the x and y position of the measurement and the return value
     *                              is supposed to be a measure of the result where a larger value
     *                              indicates a better result
     * @param mainwindow     - void pointer for passing a pointer - e.g. to mainwindow - to the
     *                              test function to avoid global variables
     * @param filename       - file name for storing the resulting measure values together with
     *                              the respective positions
     * @param measureunit    - string describing the unit of the result of measure() for output to
     *                              file
     * @param zickzack       - if this is set to true, the measurement area is scanned in a
     *                              zick-zack pattern if not the measurement direction is always
     *                              the same, avoiding shifts (setting this to true is only for
     *                              coarse scans)
     *
     * @return               - the position with the highest measure found during the scan
     */
    std::array<double,3> Scan2D(bool* running, int xaxis, double xstart, double xend,
                                    int yaxis, double ystart, double yend,
                                    double deltax, double deltay, int measuredelay,
                                    bool laseroff, double (*measure)(double, double, double, void*),
                                    void* mainwindow,
                                    std::string filename, std::string measureunit = "", bool zickzack = false);

private:
    QSerialPort laser;
    QSerialPort stages;

    //-- position and reference information for the stages --
    bool initialised[3];

    double position[3];
    double minposition[3];
    double maxposition[3];

    //-- standard delay after writing to the ports --
    int lasersleep;
    int stagesleep;
};

#endif // LASERSETUP_H
