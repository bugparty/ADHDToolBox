set PATH=C:\Qt\6.7.2\msvc2019_64\bin;C:\Qt\Tools\QtCreator\bin\jom\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
if not exist build (
    mkdir build
)
cd build

qmake  CONFIG+=release CONFIG-=debug  ../AdhdToolBox.pro
jom /f Makefile
@REM nmake /f Makefile
if not exist packaging (
    mkdir packaging
)
copy   .\analogclock\release\analogclock.exe  ..\installer\packages\clock\data\adhdclock.exe 
windeployqt ..\installer\packages\clock\data\adhdclock.exe 

@REM misc\enigmavbconsole.exe misc\pack.evb