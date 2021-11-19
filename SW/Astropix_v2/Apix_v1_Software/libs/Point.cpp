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

#include "Point.h"

Point::Point() : x(0), y(0)
{
    //ctor
}

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

Point::~Point()
{
    //dtor
}

std::ostream& operator<<(std::ostream& os, const Point& pt)
{
	os << "(" << pt.x << "|" << pt.y << ")";
	return os;
}
