set PATH=C:\Qt\6.7.2\msvc2019_64\bin;C:\Qt\Tools\QtCreator\bin\jom\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
if not exist build (
    mkdir build
)
cd build

qmake  CONFIG+=release CONFIG-=debug  ../AdhdToolBox.pro
@REM jom /f Makefile
nmake /f Makefile
if not exist packaging (
    mkdir packaging
)
 copy   .\analogclock\release\analogclock.exe .\packaging\analogclock.exe 
windeployqt packaging/analogclock.exe
cd  packaging
mt.exe -manifest analogclock.exe.manifest -outputresource:analogclock.exe