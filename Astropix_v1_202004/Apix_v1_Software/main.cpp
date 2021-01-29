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


#include "mainwindow.h"
#include <QApplication>
#include <QtHttpServer>


static MainWindow* myw = nullptr;

void myevents()
{
    myw->ProcessEvents();
}

int main(int argc, char *argv[])
{
    std::cout << "ATLASPix3_SoftAndFirmware  Copyright (C) 2019  Rudolf Schimassek\n"
              << "This program comes with ABSOLUTELY NO WARRANTY.\n"
              << "This is free software, and you are welcome to redistribute it\n"
              << "under certain conditions (GPL v3.0)" << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    myw = &w;

    w.setWaiter(myevents);

    w.show();

    // API
    //------------
    QHttpServer httpServer;

    //-- / path shows hello
    httpServer.route("/", []() {
        return QJsonObject{
            {
                {"software", "Astropix_v1"},
                {"hello", "world"}
            }
        };
    });

    //-- Config Setting
    //-------------------

    /**
        This route simulates a click to the Update ASIC button
      */
    httpServer.route("/asic/config/@send",
                     [&w]() {
        w.on_Update_clicked();
        return  QJsonObject {
            {
                {"result" , "success"},
                {"message","Config  Send"}
            }
        };
    });

    /**
      This Route sets a value to a Config SR and Parameter
      At the moment, it will fetch the registered UI component and set the value through it so that
      the UI automatically mirrors the new value without having to modify the original software much.

      IF GECCO were to be used without a GUI, we could change this procedure to update the Models directly
      in case no UI components are available

      Format: /asic/config/DEVICE/PARAMETER/set/VALUE
      DEVICE -> ASIC_Config set in software config, matches the Name of the Tab in the GUI
      PARAMETER -> One entry that contains a value to be send to the chip, matches the name of the editable
      elements in GUI
      VALUE -> The value, 0/1 for single bits, 0<value>63 for DACs
      */
    httpServer.route("/asic/config/<arg>/<arg>/set/<arg>",
                     [&w](QString configName,QString registerName,const unsigned int value) {

        // Get configu name
        std::vector<ASIC_Config2*>::iterator config = std::find_if(
             w.asicConfigs.begin(),
             w.asicConfigs.end(),
            [configName]( ASIC_Config2 *  x) {
            return x->GetDeviceName() == configName.toStdString();
        });

        if (config!=w.asicConfigs.end()) {
            //return QString("Found Config");

            // Search Register
            ASIC_Config2 * asicConfig = *config;

            int parameterIndex = asicConfig->GetParameterIndexToName(registerName.toStdString());
            if (parameterIndex==-1) {
                return QJsonObject {
                    {
                        {"result" , "error"},
                        {"message","Parameter not found"}
                    }
                };
            } else {

                // Change Parameter using UI
                //-----------
                QWidget * uiWidget = asicConfig->widgets[parameterIndex];
                //typ
                asicConfig->SetParameter(parameterIndex,value);

                if (QString(typeid(*uiWidget).name()).contains("QCheck")) {

                    static_cast<QCheckBox*>(uiWidget)->setChecked(value!=0);
                    return QJsonObject {
                        {
                            {"result" , "sucess"},
                            {"message","Parameter found and  set to Checkbox"}
                        }
                    };



                } else if (QString(typeid(*uiWidget).name()).contains("QSpin")) {

                    static_cast<QSpinBox*>(uiWidget)->setValue(value);
                    return QJsonObject {
                        {
                            {"result" , "sucess"},
                            {"message","Parameter found and  set to SpinBox"}
                        }
                    };


                }   else {
                    return QJsonObject {
                        {
                            {"result" , "error"},
                            {"message", "Parameter found but not  set"}
                        }
                    };

                }

                // return QString("Parameter found and set %1").arg(typeid(*uiWidget).name());
            }


        } else {
            return QJsonObject {
                {
                    {"result" , "error"},
                    {"message","Device not found"}
                }
            };
        }



       /*return QString("Setting Config %1 , register %2 to value %3")
                .arg(configName).arg(registerName).arg(value);*/
    });
    const auto port = httpServer.listen(QHostAddress::Any,8090);
    if (port == -1) {
        qDebug() << QCoreApplication::translate(
                "GECCO", "Could not run on http://127.0.0.1:%1/").arg(port);
        return 0;
    }

    qDebug() << QCoreApplication::translate(
            "GECCO", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);




    return a.exec();
}
