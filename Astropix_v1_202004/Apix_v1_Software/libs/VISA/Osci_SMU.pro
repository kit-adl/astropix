#
# Osci_SMU Readout Project
# Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu) and
#					   Felix Ehrler (felix.ehrler@kit.edu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#-------------------------------------------------
#
# Project created by QtCreator 2015-11-22T08:32:36
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += "-std=c++11 -pthread"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Osci
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    visadevice.cpp \
    tektronixosci.cpp \
    keysightosci.cpp \
    histogramm.cpp \
    functionplotter.cpp \
    wfmpreview.cpp

HEADERS  += wfmpreview.h \
    mainwindow.h \
    visadevice.h \
    tektronixosci.h \
    keysightosci.h \
    histogramm.h \
    functionplotter.h

FORMS    += mainwindow.ui \
    wfmpreview.ui

LIBS     += "C:/Windows/System32/visa32.dll"

INCLUDEPATH += "C:\Program Files\IVI Foundation\VISA\Win64\Include"

#win32 {
#    LIBS += -LC:/Windows/System32/ -lvisa32
#}
