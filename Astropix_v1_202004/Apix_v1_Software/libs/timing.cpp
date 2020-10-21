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

#include "timing.h"

Timing::TimePoint Timing::GetTimeNow()
{
    //return std::chrono::steady_clock::now();
    return std::chrono::system_clock::now();
}

int Timing::TimesToIntervalMS(Timing::TimePoint first, Timing::TimePoint second)
{
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(second - first).count();
    return int(duration);
}

int Timing::TimesToIntervalUS(Timing::TimePoint first, Timing::TimePoint second)
{
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(second - first).count();
    return int(duration);
}

std::string Timing::TimesToInterval(Timing::TimePoint first, Timing::TimePoint second)
{
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(second - first).count();

    std::stringstream timetext("");
    bool started = false;

    if(duration > 86400000)	//1 day
    {
        timetext << duration/86400000 << " days";
        duration %= 86400000;
        started = true;
    }
    if(duration > 3600000)	//1 hour
    {
        if(started)
            timetext << " ";
        else
            started = true;
        timetext << duration/3600000 << " hours";
        duration %= 3600000;
    }
    if(duration > 60000)	//1 minute
    {
        if(started)
            timetext << " ";
        else
            started = true;
        timetext << duration/60000 << " minutes";
        duration %= 60000;
    }
    if(duration > 1000)		// 1 second
    {
        if(started)
            timetext << " ";
        else
            started = true;
        timetext << duration/1000 << " seconds";
        duration %= 1000;
    }

    if(started)
        timetext << " ";
    if(duration != 0)
        timetext << duration << " milliseconds";

    if(!started && duration == 0)
        return "0 milliseconds";

    return timetext.str();
}

#if defined(__linux__)
void Timing::Sleep(int ms)
{
    ::usleep(1000*ms);
}
#else
void Timing::Sleep(int ms)
{
    ::Sleep(static_cast<unsigned long>(ms));
}
#endif

#if defined(__linux__)
void Timing::usleep(int us)
{
    ::usleep(us);
}
#else
void Timing::usleep(int us)
{
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}
#endif


Timing::StopWatch::StopWatch() : running(false)
{

}

void Timing::StopWatch::Start()
{
    start = Timing::GetTimeNow();
    lastlap = start;
    running = true;
}

double Timing::StopWatch::Stop()
{
    if(running)
    {
        Timing::TimePoint ende = Timing::GetTimeNow();
        running = false;
        return Timing::TimesToIntervalUS(start, ende) / 1.e6;
    }
    else
        return -1;
}

double Timing::StopWatch::Lap()
{
    if(running)
    {
        Timing::TimePoint lap = Timing::GetTimeNow();
        double duration = Timing::TimesToIntervalUS(lastlap, lap) / 1.e6;
        lastlap = lap;
        return duration;
    }
    else
        return -1;
}

double Timing::StopWatch::Finish()
{
    if(running)
    {
        running = false;
        return Timing::TimesToIntervalUS(start, lastlap) / 1.e6;
    }
    else
        return -1;
}

std::string Timing::TimePointToText(TimePoint time)
{
    std::time_t timet = std::chrono::system_clock::to_time_t(time);
    return std::string(std::ctime(&timet));
}

std::string Timing::TimePointToTimeText(Timing::TimePoint time)
{
    std::time_t timet = std::chrono::system_clock::to_time_t(time);
    std::string text = std::string(std::ctime(&timet));
    unsigned int start = text.find(':') - 2;
    return text.substr(start, 8);
}
