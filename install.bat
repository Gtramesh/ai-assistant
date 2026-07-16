@echo off
echo ============================================
echo   AI Assistant - Installer
echo ============================================
echo.

where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Python not found! 
    echo Please install Python from https://www.python.org/downloads/
    echo Make sure to check "Add Python to PATH" during installation.
    pause
    exit /b 1
)

echo [1/2] Installing dependencies...
pip install -r requirements.txt --quiet
if %errorlevel% neq 0 (
    echo [ERROR] Failed to install dependencies
    pause
    exit /b 1
)

echo [2/2] Dependencies installed!
echo.
echo ============================================
echo   Ready to launch!
echo ============================================
echo.
echo Run 'start.bat' to launch the AI Assistant.
echo.
pause
