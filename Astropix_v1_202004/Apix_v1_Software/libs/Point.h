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

/****************************************************************
 * The Point class is part of a class structure to              *
 * manage and store measurement data. The data consists of      *
 * waveforms, histograms and other (text) data and is arranged  *
 * in events which are grouped into measurements.               *
 * The intention is to depict measured events from e.g. the     *
 * oscilloscope and to combine it with other data sources.      *
 * The data is stored in XML files and is compressed to save    *
 * space on the HDD.                                            *
 *                                                              *
 * The Point class simplifies the handling of data points in 2D *
 * space.                                                       *
 *                                                              *
 * Author:  Rudolf Schimassek                                   *
 * Version: 1.0 (08.05.18)                                      *
 ****************************************************************/

#ifndef POINT_H
#define POINT_H

#include <ostream>

class Point
{
    public:
        Point();
        Point(double x, double y);
        virtual ~Point();

        double getx() { return x; }
        void setx(double val) { x = val; }
        double gety() { return y; }
        void sety(double val) { y = val; }

        double x;
        double y;

        friend std::ostream& operator<<(std::ostream& os, const Point& pt);

    protected:

    private:
};

#endif // POINT_H
