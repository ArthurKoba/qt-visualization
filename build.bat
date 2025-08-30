@echo off
echo Building COM Serial Viewer...

REM Проверяем наличие qmake
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: qmake not found. Please install Qt and add it to PATH.
    pause
    exit /b 1
)

REM Создаем директорию для сборки
if not exist build mkdir build
cd build

REM Создаем Makefile
echo Creating Makefile...
qmake ..\COM_Serial_Viewer.pro
if %errorlevel% neq 0 (
    echo Error: Failed to create Makefile
    pause
    exit /b 1
)

REM Собираем проект
echo Building project...
if exist Makefile (
    mingw32-make
    if %errorlevel% neq 0 (
        echo Error: Build failed
        pause
        exit /b 1
    )
    echo Build completed successfully!
    echo Executable: build\COM_Serial_Viewer.exe
) else (
    echo Error: Makefile not found
)

pause