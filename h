@echo off
setlocal enabledelayedexpansion

:: Define standard directories
set "DOWNLOADS=%USERPROFILE%\Downloads"
set "MAIN_ZIP=%DOWNLOADS%\SNES.zip"
set "TEMP_EXTRACT=%DOWNLOADS%\SNES_TEMP"
set "FINAL_TARGET=%DOWNLOADS%\SNES"

echo [1/4] Extracting main SNES.zip...
if not exist "%MAIN_ZIP%" (
    echo Error: Could not find %MAIN_ZIP%
    pause
    exit /b
)

:: Extract the main outer zip
mkdir "%TEMP_EXTRACT%" 2>nul
tar -xf "%MAIN_ZIP%" -C "%TEMP_EXTRACT%"

:: Set path to the inner nested "SNES" folder
set "INNER_SNES=%TEMP_EXTRACT%\SNES"

echo [2/4] Processing and unzipping individual game files...
if not exist "%INNER_SNES%" (
    echo Error: Expected nested 'SNES' folder not found inside ZIP.
    rmdir /s /q "%TEMP_EXTRACT%"
    pause
    exit /b
)

:: Loop through every zip file inside the inner SNES folder
cd /d "%INNER_SNES%"
for %%G in (*.zip) do (
    set "FILENAME=%%~nG"
    
    :: Get the first letter of the game file name
    set "FIRSTCHAR=!FILENAME:~0,1!"
    
    :: Ensure characters like [ or ( from region tags don't break the letter sorting
    :: Force the folder name to be just the actual starting alphanumeric character
    for /f "delims=" %%A in ('powershell -NoProfile -Command "[System.Char]::ToUpper('!FIRSTCHAR!')"') do set "LETTER=%%A"
    
    :: Create the target alphabetical folder inside the final SNES directory
    mkdir "%FINAL_TARGET%\!LETTER!" 2>nul
    
    :: Unzip the game archive directly into its respective A-Z folder
    tar -xf "%%G" -C "%FINAL_TARGET%\!LETTER!"
)

echo [3/4] Cleaning up temporary files...
cd /d "%DOWNLOADS%"
rmdir /s /q "%TEMP_EXTRACT%"

echo [4/4] Done! Your games are organized in %FINAL_TARGET%
pause
