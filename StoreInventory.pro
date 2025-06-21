QT += core gui
#This tells Qt to use the core library (basic functions) and
#the GUI library (for windows, buttons, etc.)
#HOLAAAAAAAAAAAAAAA

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += sql

CONFIG += c++17
#This sets the C++ version to C++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Main C++ code files that Qt will compile (build)
SOURCES += \
    Item.cpp \
    additemdialogue.cpp \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    startdialog.cpp \
    updateitemdialogue.cpp \
    usermanagerdialog.cpp

#Files that define the structure of the classes (what functions and variables they have)
HEADERS += \
    Item.h \
    additemdialogue.h \
    databasemanager.h \
    mainwindow.h \
    startdialog.h \
    updateitemdialogue.h \
    usermanagerdialog.h

#These are files created using Qt Designer (visual editor)
#They describe how the windows and dialogs look.
FORMS += \
    additemdialogue.ui \
    mainwindow.ui \
    startdialog.ui \
    updateitemdialogue.ui \
    usermanagerdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#lists other files that are not code but are part of the project
DISTFILES += \
    .gitignore \
    README.md
