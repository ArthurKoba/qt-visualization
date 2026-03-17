---
inclusion: manual
---
# Карта проекта

## Обзор
Это C++ приложение на Qt6 для обработки и визуализации аудиосигналов в реальном времени. Проект обрабатывает аудио из захвата loopback Windows, генерирует тестовые сигналы, получает их по последовательному порту от микроконтроллера. Отображает данные через несколько типов графиков (линейный, столбчатый, частотный, 3D спектрограмма). Обработка сигнала использует FFT из библиотеки ESP-DSP Library, оптимизированную для встраиваемых систем ESP32.

## Архитектура

### Основные компоненты

#### 0. Сервер агрегации (`src/aggregator_server/`)

**Назначение:** Центральный компонент системы для отслеживания доступности и состояния всех компонентов архитектуры конвейера обработки данных. Использует протокол BDSP для надежной передачи пакетов.

**Основной класс: `AggregatorServer`**
- TCP сервер на статическом порту 8212
- Регистрация компонентов с проверкой уникальности UUID
- Мониторинг heartbeat через BDSP пакеты (каждые 100 мс, таймаут 500 мс)
- Публикация событий подключения/отключения компонентов через BDSP
- Отправка списка активных компонентов в JSON формате через BDSP
- Каждое TCP соединение обрабатывается отдельным `TcpBDSPSocket`
- Логирование подключений/отключений клиентов

**Протокол BDSP:**
- `TcpBDSPSocket` — обработчик пакетов для одного TCP соединения
- `PacketSerializer/PacketValidator` — утилиты сериализации и валидации
- `AggregatorClient` — пример клиента, использующего BDSP
- Типы пакетов: heartbeat, component_registration, event_notification, command_request/response

**Структура пакета BDSP:**
- Тип пакета (1 байт)
- UUID источника (16 байт)
- UUID назначения (16 байт)
- Номер последовательности (4 байта)
- Временная метка (8 байт)
- Данные пакета (переменная длина)

**Типы данных:**
- `PacketType` — типы BDSP пакетов (heartbeat, component_registration, event_notification, etc.)
- `packet_header_t` — заголовок BDSP пакета
- `registration_packet_t` — пакет регистрации компонента
- `heartbeat_packet_t` — пакет heartbeat
- `event_notification_packet_t` — пакет уведомления о событии
- `ComponentType` — тип компонента (data_source, data_processor, data_sink, core)
- `ComponentState` — состояние компонента (connecting, registered, active, inactive, _disconnected)
- `EventType` — тип события (component_connected, component_disconnected, component_state_changed)
- `socket_info_t` — информация о компоненте (QUuid UUID, тип, QHostAddress адрес, порт, состояние)
- `component_event_t` — событие изменения состояния компонента (QUuid UUID)
- `address_utils::format_address_ipv4_priority()` — утилита форматирования адресов с приоритетом IPv4
- `std::hash<QUuid>` — специализация хеш-функции для использования QUuid в unordered_map

**Исполняемые файлы:**
- `aggregator_server_app` — сервер агрегации с поддержкой аргументов:
  - `-s, --start` — запуск сервера (обязательно)
  - `-p, --component_server_port <PORT>` — порт прослушивания (по умолчанию 8212)
  - `-h, --help` — справка
- `aggregator_bdsp_test_client` — тестовый BDSP клиент для проверки работы сервера
- `aggregator_test_client` — устаревший тестовый клиент (текстовый протокол)

**Протокол взаимодействия BDSP:**
1. Клиент подключается к серверу по TCP
2. Клиент отправляет пакет `component_registration` с JSON данными компонента
3. Сервер отвечает пакетом `component_registration` с результатом (success/error)
4. При успешной регистрации клиент начинает отправлять `heartbeat` пакеты
5. Сервер рассылает `event_notification` пакеты всем клиентам при изменениях
6. Клиенты могут запрашивать список компонентов через `command_request`

**Примеры использования:**
```bash
# Запуск сервера на порту 8212
aggregator_server_app -s

# Запуск BDSP тестового клиента
aggregator_bdsp_test_client

# Вывод справки
aggregator_server_app -h
```

#### 1. Точка входа в приложение (`src/main.cpp`)
- Создает `Application` (наследуется от `QApplication`)
- Инициализирует `SurfaceGraph` для 3D визуализации
- Использует Qt6 с OpenGL бэкендом (`QSG_RHI_BACKEND=opengl`)

#### 2. Основное приложение (`src/app.h/cpp`)
**Структура Config** — переключение компонентов визуализации:
- `run_loopback` — захват системного аудио
- `run_serial` — обработка данных по последовательному порту от микроконтроллера
- `run_generator` — генерация тестовых сигналов
- `run_analyzer` — обработка аудио с помощью FFT
- Различные флаги `show_*` для управления видимостью UI

**Ключевые члены:**
- `Analyzer *analyzer` — обработка FFT и спектральный анализ
- `Generator *generator` — генератор тестового сигнала (синусоида)
- `audio::loopback::IAudioLoopback *loopback` — захват аудио Windows
- `QSerialPort *serial` — последовательная связь с микроконтроллером
- `BDSP::COBSZPEReceiver *receiver` — парсер пакетов по serial
- `SurfaceGraph *surfaceView` — 3D визуализация поверхности
- `Spectrogram *spectrogram` — структура данных спектрограммы

**Конвейер обработки:**
1. Loopback захватывает аудио → вызывает `analyzer->add_samples()`
2. Analyzer запускает FFT в фоновом потоке → вызывает обработчик обновления
3. Обработчик обновления отправляет данные в графики и спектрограмму
4. Спектрограмма обновляет 3D поверхность

#### 3. Анализатор (`src/analyzer/`)
**Основной класс: `Analyzer` (наследуется от `AbstractTask`)**
- Запускается в выделенном потоке (целевая частота кадров — 1000/45 FPS)
- Использует библиотеку BDSP для FFT (FFT2R/FFT4R/FHT2R/FHT4R)
- Обрабатывает стереоаудио (левый/правый каналы)

**Ключевые структуры данных:**
- `Samples samples` — буфер аудио (4096 сэмплов)
- `Amplitudes amplitudes` — выходные величины FFT (2048 бинов)
- `Amplitudes amplitudes_test` — амплитуды по шкале Mel (150 полос)
- `SpectralWhitening whitening` — нормализация спектра

**Ключевые методы:**
- `add_samples(left, right)` — подача аудиоданных
- `set_update_handler(handler)` — обратный вызов при готовности новых данных
- `update_sample_rate(rate)` — пересчет масштабных коэффициентов
- `get_freq_step()` — частотное разрешение (Гц/бин)

**Цепочка обработки сигнала:**
1. Аудиосэмплы → FFT → спектр величин
2. Применение весов шкалы Bark
3. Преобразование в шкалу Mel (40 полос) → `amplitudes_test`
4. Опциональная спектральная белизна (whitening)

**Файлы:**
- `analyzer.h/cpp` — основной класс анализатора
- `fft.h/cpp` — обертка FFT
- `audio_window.h/cpp` — применение окна Хеннинга (Hanning)
- `spectrogram.h/cpp` — структура данных спектрограммы
- `generator/generator.h/cpp` — генератор синусоиды

#### 4. Аудио loopback (`src/audio/`)
**Реализация для Windows: `WASAPILoopback`**
- Использует захват loopback в общем режиме WASAPI
- Захватывает вывод устройства по умолчанию
- Преобразует в формат IEEE FLOAT (32-бит)
- Вызывает `audio_handler_t` с аудиоданными

**Интерфейс:**
```cpp
struct loopback_audio {
    float *data;      // Чередующиеся сэмплы [L,R,L,R,...]
    size_t samples;   // Сэмплов на канал
    size_t sample_rate;
};
```

**Паттерн Factory:**
- `LoopbackFactory::get_loopback()` возвращает реализацию, специфичную для платформы

#### 5. Графики (`src/charts/`)
Все графики наследуются от `AbstractChartView` (расширяет `QChartView`):

**Базовые возможности:**
- Автоматическое изменение диапазона осей
- Авто-усиление с настраиваемой скоростью
- Частота обновления до 200 FPS

**Типы графиков:**
- `LineChartView` — линейная серия для сигналов во временной области
- `BarChartView` — столбчатая серия для спектральных данных
- `FreqChartView` — линейный график с частотной осью (Гц)
- `AbsChartView` — абстрактный базовый класс

**Примечание:** Текущая реализация использует Qt Charts. Для новых функций визуализации рекомендуется использовать [Qt Graphs](.kiro/steering/qt/qt-graphs.md) — современный модуль с поддержкой 2D и 3D графиков.

Для загрузки полной документации по Qt Charts используй: `#[[file:.kiro/steering/qt/qt-charts.md]]`


#### 6. Ядро утилит (`src/core/`)
**`AbstractTask`** — управление потоками:
- Создание потоков, специфичное для Windows (`CreateThread`)
- Виртуальный метод `_task()` для рабочей логики
- Хуки `_before_run_task()`, `_after_close_task()`
- `_sleep(ms)` для контроля частоты кадров

#### 7. 3D визуализация (`src/surfacegraph.*`)
**`SurfaceGraph`** — обертка Qt Data Visualization:
- `Q3DSurface` для 3D поверхностных графиков
- `SurfaceGraphModifier` для взаимодействия

**`Spectrogram`** — структура данных:
- Кольцевой буфер для спектрограммы временных рядов
- Методы: `push()`, `get_spectrogram()`, `get_channel_history()`
- Поддержка настройки размера истории

**Примечание:** Текущая реализация использует Qt Data Visualization (deprecated). Для новых проектов рекомендуется использовать [Qt Graphs](.kiro/steering/qt/qt-graphs.md) с поддержкой 3D графиков (`Q3DScatter`, `Q3DBars`, `Q3DSurface`).

Для загрузки полной документации по Qt Data Visualization используй: `#[[file:.kiro/steering/qt/qt-datavisualization.md]]`

## Система сборки

**Структура CMakeLists.txt:**
```
qt-visualization/
├── CMakeLists.txt (корневой)
├── src/
│   ├── CMakeLists.txt
│   ├── analyzer/CMakeLists.txt
│   ├── audio/CMakeLists.txt
│   ├── charts/CMakeLists.txt
│   ├── core/CMakeLists.txt
│   └── analyzer/generator/CMakeLists.txt
```

**Зависимости:**
- Qt6: Charts, Core, Gui, Multimedia, SerialPort, DataVisualization
- BDSP (через FetchContent).

**Цели сборки:**
- `qt-application` — основной исполняемый файл
- `analyzer` — статическая/разделяемая библиотека
- `audio` — платформо-специфичный захват аудио
- `charts` — библиотека визуализации графиков
- `core` — библиотека ядра утилит
- `dsp` — подкаталог библиотеки ESP-DSP

## Примечания по платформе

**Специфичное для Windows:**
- Захват аудио через WASAPI
- Последовательный порт: COM8 @ 1M бод
- Целевая платформа: win32

**Встраиваемая цель:**
- Библиотека BDSP разработана для ESP32
- FFT оптимизирована для микроконтроллеров
- Тот же код, разные цели компиляции

## Ключевые константы

- Размер FFT: 4096 сэмплов
- Выход FFT: 2048 частотных бинов
- Частота обновления: ~45 FPS (задержка 1000/45 мс)
- Частота дискретизации: динамическая (от аудиоустройства)
- Шаг частоты: `sample_rate / 4096` Гц/бин

## Конфигурация

Редактируйте `Application::Config` в `app.h` для включения/выключения компонентов:
```cpp
bool run_loopback = true;      // Включить захват аудио
bool run_serial = false;       // Включить serial вход
bool run_generator = false;    // Включить тестовый сигнал
bool run_analyser = true;      // Включить FFT анализ
bool show_amplitudes = true;   // Показать график величин FFT
bool show_surface = true;      // Показать 3D поверхность
```

## Внешние библиотеки

- **BDSP**: https://github.com/KobaProduction/BDSP
  - Оптимизирована для встраиваемых систем
  - Библиотека для реализации пакетной передачи в каналах связи где есть только методы отправки и получения байт или наборов байт, в которых отсутствует разделение на пакеты данных
  - Изначально разработана для последовательного порта, однако её можно адаптировать под любые каналы связи, в том числе TCP/IP, UDP, CAN, I2C, SPI
  - В проекте используется для надежной передачи пакетов между сервером агрегации и клиентами
  - Основные классы: `COBSZPETransceiver` (трансивер с COBS кодированием и Zero Padding Elimination)

- **ESP-DSP**:  https://github.com/espressif/esp-dsp/
  - Оптимизирована для микроконтроллеров ESP32
  - Используется через `dsps_fft2r_fc32`, `dsps_fft4r_fc32`
