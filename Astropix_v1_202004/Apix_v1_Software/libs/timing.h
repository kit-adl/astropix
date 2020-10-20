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

#ifndef TIMING_H
#define TIMING_H
/********************************************************************************
 * This class is a collection of several methods related to timing and time     *
 * measurement. This includes operating system "independent" implementations    *
 * for Sleep() and usleep() as well as time measurement as TimePoints or using  *
 * a stopwatch class. TimePoint differences can directly be converted into time *
 * strings ranging up to days.                                                  *
 * Everything is packed into the namespace Timing to avoid conflicts with other *
 * implementations.
 *                                                                              *
 * Author: Rudolf Schimassek                                                    *
 * Version: 1.0 (23.08.19)                                                      *
 ********************************************************************************/


#include <chrono>
#include <string>
#include <sstream>

#if defined(__linux__)
#include <unistd.h>
#else
#include <thread>
#include <windows.h>
#endif

namespace Timing
{    
    //typedef std::chrono::steady_clock::time_point TimePoint;
    typedef std::chrono::system_clock::time_point TimePoint;

    std::string TimePointToText(TimePoint time);
    std::string TimePointToTimeText(TimePoint time);

    /**
     * @brief GetTimeNow provides a time point for the time of the function call
     * @return              - a TimePoint which can be used for time measurement
     */
    TimePoint GetTimeNow();

    /**
     * @brief TimesToIntervalMS evaluates the time difference of two TimePoints at
     *      millisecond precision and returns the result
     * @param first         - earlier TimePoint
     * @param second        - later TimePoint
     * @return              - time difference between first and second in milliseconds
     */
    int TimesToIntervalMS(TimePoint first, TimePoint second);
    /**
     * @brief TimesToIntervalMS evaluates the time difference of two TimePoints at
     *      microsecond precision and returns the result
     * @param first         - earlier TimePoint
     * @param second        - later TimePoint
     * @return              - time difference between first and second in microseconds
     */
    int TimesToIntervalUS(TimePoint first, TimePoint second);

    /**
     * @brief TimesToInterval evaluates the time difference and converts the result into a
     *      human readable string including time units
     * @param first         - earlier TimePoint
     * @param second        - later TimePoint
     * @return              - time difference as a string with up to days as unit
     */
    std::string TimesToInterval(TimePoint first, TimePoint second);

    ///operating "independent" sleep method:
    void Sleep(int ms);
    ///operating "independent" usleep method:
    void usleep(int us);

    class StopWatch{
    public:
        StopWatch();
        ///Start() initiates time measurement:
        void Start();
        ///Stop() stops time measurent, returns time since start in s:
        double Stop();
        ///Lap() returns the time since last lap call or start,
        ///   does not stop the measurement:
        double Lap();
        ///Finish() returns the time from start to last lap call, stops the measurement:
        double Finish();
    private:
        bool running;
        TimePoint start;
        TimePoint lastlap;
    };
}

#endif // TIMING_H
