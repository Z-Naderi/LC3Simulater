QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lc3.cpp \
    lc3assembler.cpp \
    lc3binfile.cpp \
    lc3instructions.cpp \
    lc3memory.cpp \
    lc3registers.cpp \
    main.cpp

HEADERS += \
    lc3.h \
    lc3assembler.h \
    lc3binfile.h \
    lc3instructions.h \
    lc3memory.h \
    lc3registers.h

FORMS += \
    lc3.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    aks.qrc \
    image.qrc
