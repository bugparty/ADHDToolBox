INCLUDEPATH += $$PWD
# Add the required libraries
win32:LIBS += -luser32 -lkernel32
# Define the necessary preprocessor macros
win32:DEFINES += WIN32
unix:DEFINES += LINUX # 针对Linux系统添加宏定义
# Specify the target OS
win32:CONFIG += windows

SOURCES += $$PWD/rasterwindow.cpp
HEADERS += $$PWD/rasterwindow.h
