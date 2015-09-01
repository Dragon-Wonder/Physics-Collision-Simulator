@echo off
:: This batch file will go through the folders and delete files that are not needed.
:: Really just for personal use
:: There are a lot of random files that get throw about, this batch file will delete them if they are there.
:: because I'm too lazy to go searching for them myself, but I want to keep my folders clean.

echo Cleaning Project Folder...
IF EXIST %~dp0Project\bin RMDIR /S /Q %~dp0Project\bin
IF EXIST %~dp0Project\obj RMDIR /S /Q %~dp0Project\obj
IF EXIST %~dp0Project\SDL-Cannon-Simulation.depend del /F %~dp0Project\SDL-Cannon-Simulation.depend
IF EXIST %~dp0Project\SDL-Cannon-Simulation.layout del /F %~dp0Project\SDL-Cannon-Simulation.layout
echo.

echo Cleaning res Folder...
IF EXIST %~dp0res\boilerplate.res del /F %~dp0res\boilerplate.res
echo.

echo Cleaning src Folder...
IF EXIST %~dp0src\main.o del /F %~dp0src\main.o
IF EXIST %~dp0src\cannonball.o del /F %~dp0src\cannonball.o
IF EXIST %~dp0src\config.o del /F %~dp0src\config.o
IF EXIST %~dp0src\tick.o del /F %~dp0src\tick.o
IF EXIST %~dp0src\screen.o del /F %~dp0src\screen.o
echo.

echo Cleaning Main Folder...
IF EXIST %~dp0Config.ini del /F %~dp0Config.ini
IF EXIST %~dp0gmon.out del /F %~dp0gmon.out
IF EXIST %~dp0Cannon.exe del /F %~dp0Cannon.exe
IF EXIST %~dp0Cannon-PRIVATE.exe del /F %~dp0Cannon-PRIVATE.exe
IF EXIST %~dp0Cannon del /F %~dp0Cannon
IF EXIST %~dp0Cannon-PRIVATE del /F %~dp0Cannon-PRIVATE
IF EXIST %~dp0logfile.log del /F %~dp0logfile.log
IF EXIST %~dp0time-log.log del /F %~dp0time-log.log 
echo.

echo All cleaning done.

