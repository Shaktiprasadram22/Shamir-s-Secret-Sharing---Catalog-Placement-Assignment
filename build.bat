@echo off
echo Compiling Shamir's Secret Sharing solver...
g++ -std=c++11 -Wall -Wextra -O2 -o catalog_solver main.cpp
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the program...
    echo.
    catalog_solver.exe
) else (
    echo Compilation failed!
)