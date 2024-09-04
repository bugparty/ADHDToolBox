include(../rasterwindow/rasterwindow.pri)

# work-around for QTBUG-13496
CONFIG += no_batch
CONFIG += static
SOURCES += \
    main.cpp \
    util.cpp
QT += core gui widgets
target.path = $$[QT_INSTALL_EXAMPLES]/gui/analogclock
INSTALLS += target

RESOURCES += \
    resources.qrc

HEADERS += \
    util.h
