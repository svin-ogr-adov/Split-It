QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

CONFIG(debug, debug|release) {
    unix|win32: LIBS += -L$$PWD/../Filtros01/Filtros01-Output/Products/x64Debug/
}
else {
   unix|win32: LIBS += -L$$PWD/../Filtros01/Filtros01-Output/Products/x64Release/
}
#unix|win32:

QTPLUGIN += qsqloci qgif
CONFIG += static


QT += widgets
CONFIG+= static


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculator.cpp \
    currenttime.cpp \
    file_info.cpp \
    main.cpp \
    mainwindow.cpp \
    printer.cpp \
    reader.cpp \
    trim.cpp

HEADERS += \
    calculator.h \
    currenttime.h \
    file_info.h \
    mainwindow.h \
    printer.h \
    reader.h \
    split_info.h \
    trim.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    r.qrc
