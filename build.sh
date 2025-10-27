#!/bin/bash

echo "Building COM Serial Viewer..."

# Проверяем наличие qmake
if ! command -v qmake &> /dev/null; then
    echo "Error: qmake not found. Please install Qt and add it to PATH."
    exit 1
fi

# Создаем директорию для сборки
mkdir -p build
cd build

# Создаем Makefile
echo "Creating Makefile..."
qmake ../COM_Serial_Viewer.pro
if [ $? -ne 0 ]; then
    echo "Error: Failed to create Makefile"
    exit 1
fi

# Собираем проект
echo "Building project..."
if [ -f Makefile ]; then
    make
    if [ $? -ne 0 ]; then
        echo "Error: Build failed"
        exit 1
    fi
    echo "Build completed successfully!"
    echo "Executable: build/COM_Serial_Viewer"
else
    echo "Error: Makefile not found"
    exit 1
fi