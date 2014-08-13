cd ..\lib
impdef odb.def odb.dll
REM implib odb.lib odb.dll
cd ..\bin
copy ..\lib\odb.dll .

