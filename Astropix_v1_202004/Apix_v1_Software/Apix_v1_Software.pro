#
# ATLASPix3_SoftAndFirmware
# Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
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
# Project created by QtCreator 2017-02-06T09:27:09
#
#	created for LFoundry Chip4/5
#   adapted for ATLASPix3 on 09.08.2019
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Apix_v1_Software
TEMPLATE = app

QMAKE_CXXFLAGS += "-std=c++11"
QMAKE_CXXFLAGS += "-Wall"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    #libs/VISA/keysightosci.cpp \
    #libs/VISA/tektronixosci.cpp \
    #libs/VISA/visadevice.cpp \
    libs/cmdconfig.cpp \
    libs/configuration.cpp \
    libs/flagmanager.cpp \
    libs/ftdi.cpp \
    libs/nexysio.cpp\
    #lasersetup.cpp \
    libs/spinexys.cpp \
    libs/udp_config.cpp \
    mainwindow.cpp \
    libs/trimming.cpp \
    libs/tinyxml2.cpp \
    libs/tinyxml2_addon.cpp \
    libs/miniz.c \
    libs/zip_file.cpp \
    libs/Point.cpp \
    libs/WaveForm.cpp \
    libs/Histogram.cpp \
    libs/textdata.cpp \
    libs/Event.cpp \
    libs/ConfigPart.cpp \
    libs/Measurement.cpp \
    fastreadout.cpp \
    libs/asic_config_2.cpp \
    libs/vb_config.cpp \
    libs/timing.cpp \
    libs/src/alglibinternal.cpp \
    libs/src/alglibmisc.cpp \
    libs/src/ap.cpp \
    #libs/src/dataanalysis.cpp \
    #libs/src/diffequations.cpp \
    #libs/src/fasttransforms.cpp \
    #libs/src/integration.cpp \
    #libs/src/interpolation.cpp \
    libs/src/linalg.cpp \
    libs/src/optimization.cpp \
    libs/src/solvers.cpp \
    #lasercontrol.cpp \
    libs/injection_config.cpp \
    libs/tdac_config.cpp \
    libs/totcalibration.cpp
    #libs/src/specialfunctions.cpp \
    #libs/src/statistics.cpp

HEADERS  += mainwindow.h \
    #libs/VISA/keysightosci.h \
    #libs/VISA/tektronixosci.h \
    #libs/VISA/visadevice.h \
    libs/cmdconfig.h \
    libs/configuration.h \
    libs/flagmanager.h \
    libs/ftdi.h \
    libs/ftd2xx.h \
    libs/nexysio.h \
    libs/WinTypes.h \
    #lasersetup.h \
    libs/spinexys.h \
    libs/tinyxml2.h \
    libs/tinyxml2_addon.h \
    libs/miniz.h \
    libs/trimming.h \
    libs/udp_config.h \
    libs/zip_file.h \
    libs/Point.h \
    libs/WaveForm.h \
    libs/Histogram.h \
    libs/textdata.h \
    libs/Event.h \
    libs/ConfigPart.h \
    libs/Measurement.h \
    fastreadout.h \
    libs/asic_config_2.h \
    libs/vb_config.h \
    libs/timing.h \
    libs/src/alglibinternal.h \
    libs/src/alglibmisc.h \
    libs/src/ap.h \
    #libs/src/dataanalysis.h \
    #libs/src/diffequations.h \
    #libs/src/fasttransforms.h \
    #libs/src/integration.h \
    #libs/src/interpolation.h \
    libs/src/linalg.h \
    libs/src/optimization.h \
    libs/src/solvers.h \
    libs/src/specialfunctions.h \
    #lasercontrol.h \
    libs/injection_config.h \
    libs/tdac_config.h \
    libs/totcalibration.h
    #libs/src/statistics.h \
    #libs/src/stdafx.h

FORMS    += mainwindow.ui \
    lasercontrol.ui

win32 {
    LIBS     += -L$$PWD/libs -L./libs -L$$PWD/libs/ -lftd2xx
    #LIBS     += "C:/Windows/System32/visa32.dll"
    #INCLUDEPATH += "C:\Program Files\IVI Foundation\VISA\Win64\Include"
    INCLUDEPATH += "./includes"
    INCLUDEPATH += "./includes/QtHttpServer"
    INCLUDEPATH += "./includes/QtSslServer"
}
unix {
#LIBS += "/usr/lib/librsvisa.so"
LIBS += "/usr/local/lib/libftd2xx.so"
LIBS += "/usr/local/lib/libftd2xx.so"

#INCLUDEPATH += "/usr/include/rsvisa"
}

## Add QTHTTP Server library
LIBS += -L$$OUT_PWD/ -L$$OUT_PWD/../qthttpserver/lib -L$$OUT_PWD/ -lQt5HttpServer

RESOURCES  = "./resources.qrc"

RC_ICONS = "images/gecco_logo_3_square_4Qt_1.ico"

DISTFILES += \
    resources.qrc \
    test_qml_form.qml
