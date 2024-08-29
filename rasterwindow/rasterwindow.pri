INCLUDEPATH += $$PWD
# Add the required libraries
win32:LIBS += -luser32 -lkernel32
# Define the necessary preprocessor macros
DEFINES += WIN32
# Specify the target OS
win32:CONFIG += windows
SOURCES += $$PWD/rasterwindow.cpp
HEADERS += $$PWD/rasterwindow.h
