QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = StudentManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    studentdialog.cpp \
    aboutdialog.cpp

HEADERS += \
    mainwindow.h \
    studentdialog.h \
    aboutdialog.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
