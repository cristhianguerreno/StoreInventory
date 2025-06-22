QT += core gui
# Use the Qt Core module (non-GUI functionality)
# and the GUI module (for windows, buttons, etc.)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# Include the Widgets module if using Qt version > 4

QT += sql
# Include the SQL module for database support

CONFIG += c++17
# Set the C++ language standard to C++17

# Uncomment the following line to disable deprecated APIs before Qt 6.0.0
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# C++ source files to be compiled
SOURCES += \
    Item.cpp \
    additemdialogue.cpp \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    startdialog.cpp \
    updateitemdialogue.cpp \
    usermanagerdialog.cpp

# Header files (class declarations)
HEADERS += \
    Item.h \
    additemdialogue.h \
    databasemanager.h \
    mainwindow.h \
    startdialog.h \
    updateitemdialogue.h \
    usermanagerdialog.h

# UI files designed with Qt Designer
FORMS += \
    additemdialogue.ui \
    mainwindow.ui \
    startdialog.ui \
    updateitemdialogue.ui \
    usermanagerdialog.ui

# Default rules for deployment depending on platform
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Other project-
