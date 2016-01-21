@echo off
:: This batch file will go through the folders and delete files that are not needed.
:: Really just for personal use
:: There are a lot of random files that get throw about, this batch file will delete them if they are there.
:: because I'm too lazy to go searching for them myself, but I want to keep my folders clean.
set MAINDIR = %~dp0
set PROJECTDIR = %MAINDIR%Project\
set DOXYGENDIR = %PROJECTDIR%doxygen\
set RESDIR = %MAINDIR%res\
set SRCDIR = %MAINDIR%src\

echo Cleaning Project Folder...
IF EXIST %PROJECTDIR%SDL-Cannon-Simulation.depend del /F %PROJECTDIR%SDL-Cannon-Simulation.depend
IF EXIST %PROJECTDIR%SDL-Cannon-Simulation.layout del /F %PROJECTDIR%SDL-Cannon-Simulation.layout
echo.

echo Cleaning Doxygen Folder...
IF EXIST %DOXYGENDIR%html RMDIR /S /Q %DOXYGENDIR%html
IF EXIST %DOXYGENDIR%latex RMDIR /S /Q %DOXYGENDIR%latex
IF EXIST %DOXYGENDIR%man RMDIR /S /Q %DOXYGENDIR%man
IF EXIST %DOXYGENDIR%rtf RMDIR /S /Q %DOXYGENDIR%rtf
IF EXIST %DOXYGENDIR%xml RMDIR /S /Q %DOXYGENDIR%xml
IF EXIST %DOXYGENDIR%doxygen.log del /F %DOXYGENDIR%doxygen.log
echo.

echo Cleaning res Folder...
IF EXIST %RESDIR%boilerplate.res del /F %RESDIR%boilerplate.res
echo.

echo Cleaning Main Folder...
IF EXIST %MAINDIR%Config.ini del /F %MAINDIR%Config.ini
IF EXIST %MAINDIR%gmon.out del /F %MAINDIR%gmon.out
IF EXIST %MAINDIR%Cannon.exe del /F %MAINDIR%Cannon.exe
IF EXIST %MAINDIR%Physics-Collision-Simulator-PRIVATE.exe del /F %MAINDIR%Physics-Collision-Simulator-PRIVATE.exe
IF EXIST %MAINDIR%Physics-Collision-Simulator del /F %MAINDIR%Physics-Collision-Simulator
IF EXIST %MAINDIR%Physics-Collision-Simulator-PRIVATE del /F %MAINDIR%Physics-Collision-Simulator-PRIVATE
IF EXIST %MAINDIR%logfile.log del /F %MAINDIR%logfile.log
IF EXIST %MAINDIR%time-log.log del /F %MAINDIR%time-log.log 
IF EXIST %MAINDIR%*.stackdump del /F %MAINDIR%*.stackdump
echo.

echo Cleaning .o files where they might exist...
IF EXIST %MAINDIR%*.o del /F %MAINDIR%*.o
IF EXIST %SRCDIR%*.o del /F %SRCDIR%*.o
IF EXIST %DIR%Project\bin RMDIR /S /Q %DIR%Project\bin
IF EXIST %DIR%Project\obj RMDIR /S /Q %DIR%Project\obj
echo.

echo All cleaning done.

