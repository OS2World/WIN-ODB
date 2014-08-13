echo off
cd e:\users\staffan\odb\odb95
echo -- Making odb.def
echo off
impdef .\odb95.def ..\bin\odb.dll
implib .\xxx .\odb95.def
echo -- Done
