# CLion install 

1. Install Qt from online installer;
2. Find path installed Qt;
3. Set `Path=C:\Qt\6.*.*\mingw_64\bin` to environment variables in run configuration;
4. Set `-DCMAKE_PREFIX_PATH=C:/Qt/6.*.*/mingw_64` to CMake options in CMake Profiles.

# COM Serial Viewer

## Описание
Оконное приложение на Qt для работы с COM портами на Windows с минимальными задержками при получении данных.

## Особенности
- Минимальные задержки при чтении данных (интервал 1-10 мс)
- Отдельный рабочий поток для чтения данных
- Современный UI с поддержкой всех параметров COM порта
- Отображение данных в hex и текстовом формате с временными метками

## Сборка
```bash
# Windows
build.bat

# Linux
./build.sh

# Или через Qt Creator
# Откройте COM_Serial_Viewer.pro
```

## Файлы проекта
- `main.cpp` - точка входа
- `serial_viewer.h/cpp` - главное окно приложения
- `serial_worker.h/cpp` - рабочий поток для чтения данных
- `COM_Serial_Viewer.pro` - файл проекта Qt