INCLUDEPATH += $$PWD
# Add the required libraries
win32:LIBS += -luser32 -lkernel32
unix:LIBS += -lX11 -lGL -lpthread # 针对Linux系统添加必要的库
# Define the necessary preprocessor macros
win32:DEFINES += WIN32
unix:DEFINES += LINUX # 针对Linux系统添加宏定义
# Specify the target OS
win32:CONFIG += windows
unix:CONFIG += x11 # 针对Linux系统的配置

SOURCES += $$PWD/rasterwindow.cpp
HEADERS += $$PWD/rasterwindow.h
