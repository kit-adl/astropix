TARGET = QtHttpServer
INCLUDEPATH += .
INCLUDEPATH += ../sslserver
QT = network core-private

qtHaveModule(websockets): QT += websockets-private
qtConfig(ssl): QT += sslserver

HEADERS += \
    qthttpserverglobal.h \
    qabstracthttpserver.h \
    qabstracthttpserver_p.h \
    qhttpserver.h \
    qhttpserver_p.h \
    qhttpserverliterals_p.h \
    qhttpserverrequest.h \
    qhttpserverrequest_p.h \
    qhttpserverresponder.h \
    qhttpserverresponder_p.h \
    qhttpserverresponse.h \
    qhttpserverresponse_p.h \
    qhttpserverrouter.h \
    qhttpserverrouter_p.h \
    qhttpserverrouterrule.h \
    qhttpserverrouterrule_p.h \
    qhttpserverrouterviewtraits.h



SOURCES += \
    qabstracthttpserver.cpp \
    qhttpserver.cpp \
    qhttpserverliterals.cpp \
    qhttpserverrequest.cpp \
    qhttpserverresponder.cpp \
    qhttpserverresponse.cpp \
    qhttpserverrouter.cpp \
    qhttpserverrouterrule.cpp

include(../3rdparty/http-parser.pri)

load(qt_module)
