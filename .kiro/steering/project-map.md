---
inclusion: always
---
# Карта проекта

## Обзор
Это C++ приложение на Qt6 для обработки и визуализации аудиосигналов в реальном времени. Проект обрабатывает аудио из захвата loopback Windows, генерирует тестовые сигналы, получает их по последовательному порту от микроконтроллера. Отображает данные через несколько типов графиков (линейный, столбчатый, частотный, 3D спектрограмма). Обработка сигнала использует FFT из библиотеки ESP-DSP Library, оптимизированную для встраиваемых систем ESP32.

## Архитектура

### Основные компоненты

#### 0. Сервер агрегации (`src/aggregator/`)

**Назначение:** Центральный компонент системы для отслеживания доступности и состояния всех компонентов архитектуры конвейера обработки данных. Использует протокол BDSP для надежной передачи пакетов.

**Основной класс: `AggregatorServer`** (наследуется от `TcpBDSPServer` из `core`)
- TCP сервер на статическом порту 8212
- Регистрация компонентов с проверкой уникальности UUID и токенов сессии
- Восстановление сессий при переподключении через CRC16 токены
- Управление состояниями компонентов: `unknown`, `offline`, `disconnected`, `registered`, `inactive`, `idle`, `active`
- Автоматический перевод в состояние `offline` при отключении на 60 секунд
- Публикация событий изменения состояния компонентов через BDSP
- Отправка информации о новых компонентах всем подключенным клиентам
- Каждое TCP соединение обрабатывается отдельным `TcpBDSPSocket` (из `core`)
- Унифицированное логирование с текстовыми представлениями состояний

**Протокол обмена:**
- `PacketType::component_registration` — регистрация компонента с UUID, типом, портом и токеном сессии
- `PacketType::component_info_update` — полная информация о новом компоненте для всех клиентов
- `PacketType::component_state_change` — изменение состояния существующего компонента

**Файлы:**
- `include/aggregator/aggregator_server.h` / `src/aggregator_server.cpp`
- `include/aggregator/aggregator_client.h` / `src/aggregator_client.cpp`
- `include/aggregator/utils.h` / `src/utils.cpp` — утилиты форматирования и генерации токенов
- `include/aggregator/abstract/types.h` — типы: `component_info_t`, `registration_data_t`, `component_state_t`, `registered_component_t`, `ComponentType`, `ComponentState`, `PacketType`, `session_token_t`; константы: `DEFAULT_PORT`, `REGISTRATION_TIMEOUT_MS`, `CLIENT_RECONNECT_TIMEOUT_MS`, `RECONNECT_TRIES`, `COMPONENT_OFFLINE_TIMEOUT_MS`
- `src/main_server.cpp` — точка входа сервера агрегации
- `src/main_client.cpp` — точка входа клиента агрегации

**Вспомогательные компоненты:**
- `AggregatorClient` — клиент для подключения к серверу агрегации с автоматическим переподключением
- Утилиты форматирования: `format_component_info()`, `format_component_state_change()`, `format_component_registration()`
- Генерация токенов сессии: `generate_session_token()` на основе CRC16 от UUID, типа, порта и случайного числа
- Преобразование в текст: `component_state_to_string()`, `component_type_to_string()`




#### 1. Точка входа в приложение (`src/main.cpp`)
- Создает `Application` (наследуется от `QApplication`)
- Инициализирует `SurfaceGraph` для 3D визуализации
- Использует Qt6 с OpenGL бэкендом (`QSG_RHI_BACKEND=opengl`)

#### 2. Основное приложение (`src/app.h/cpp`)
**Структура Config** — переключение компонентов визуализации:
- `run_loopback` — захват системного аудио
- `run_serial` — обработка данных по последовательному порту от микроконтроллера
- `run_generator` — генерация тестовых сигналов
- `run_analyser` — обработка аудио с помощью FFT
- Флаги `show_*`: `show_serial_samples`, `show_generator_samples`, `show_raw_samples`, `show_samples`, `show_amplitudes`, `show_test_amplitudes`, `show_serial_fast_amplitudes`, `show_serial_audio_spectre`, `show_surface`

**Ключевые члены:**
- `Analyzer *analyzer` — обработка FFT и спектральный анализ
- `Generator *generator` — генератор тестового сигнала (синусоида)
- `audio::loopback::IAudioLoopback *loopback` — захват аудио Windows
- `QSerialPort *serial` — последовательная связь с микроконтроллером
- `BDSP::COBSZPEReceiver *receiver` — парсер пакетов по serial
- `SurfaceGraph *surfaceView` — 3D визуализация поверхности
- `Spectrogram *spectrogram` — структура данных спектрограммы
- `QSplitter *splitter`, `QTabWidget tabWidget` — UI-контейнеры
- Несколько `AbstractChartView*` для разных типов отображения

**Конвейер обработки:**
1. Loopback захватывает аудио → вызывает `analyzer->add_samples()`
2. Analyzer запускает FFT в фоновом потоке → вызывает обработчик обновления
3. Обработчик обновления отправляет данные в графики и спектрограмму
4. Спектрограмма обновляет 3D поверхность

#### 3. Анализатор (`src/analyzer/`)
**Основной класс: `Analyzer`** (наследуется от `AbstractTask`, `final`)
- Запускается в выделенном потоке (целевая частота кадров — 1000/45 FPS)
- Использует ESP-DSP для FFT (FFT2R/FFT4R/FHT2R/FHT4R)
- Обрабатывает стереоаудио (левый/правый каналы)

**Ключевые структуры данных:**
- `Samples samples` — два `AudioWindow` (left/right), размер задаётся при создании
- `Amplitudes amplitudes` — выходные величины FFT (2048 бинов)
- `Amplitudes amplitudes_test` — амплитуды по шкале Mel
- `SpectralWhitening whitening` — нормализация спектра

**Ключевые методы:**
- `add_samples(left, right)` — подача аудиоданных
- `set_update_handler(handler)` — обратный вызов при готовности новых данных
- `update_sample_rate(rate)` — пересчет масштабных коэффициентов
- `get_freq_step()` — частотное разрешение (Гц/бин)
- `generate_bark_scale()`, `generate_volume_scale()` — статические утилиты

**Цепочка обработки сигнала:**
1. Аудиосэмплы → FFT → спектр величин
2. Применение весов шкалы Bark
3. Преобразование в шкалу Mel → `amplitudes_test`
4. Опциональная спектральная белизна (whitening)

**Файлы** (плоская структура, без `include/src` разделения):
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

**Интерфейс** (`include/loopback/types.h`, namespace `audio::loopback`):
```cpp
struct loopback_audio {
    size_t sample_rate;
    float *data;      // Чередующиеся сэмплы [L,R,L,R,...]
    size_t samples;   // Сэмплов на канал
    size_t channels;
};
class IAudioLoopback { ... };  // set_audio_handler(), start(), stop()
```

**Файлы:**
- `include/loopback/types.h` — `loopback_audio`, `audio_handler_t`, `IAudioLoopback`
- `include/loopback/factory.h` — `LoopbackFactory`
- `factory.cpp` — реализация фабрики
- `windows_loopback.h/cpp` — реализация `WASAPILoopback`

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
- `FPSChartView` — график с отображением FPS (наследует `AbstractChartView` + `FPSMixin`)
- `AbstractChartView` — абстрактный базовый класс

**Файлы** (плоская структура `include/charts/*.h` / `*.cpp`):
- `include/charts/abs.h` / `abs.cpp`
- `include/charts/line.h` / `line.cpp`
- `include/charts/bar.h` / `bar.cpp`
- `include/charts/freq.h` / `freq.cpp`
- `include/charts/fps.h` / `fps.cpp`
- `include/charts/mixins.h` / `mixins.cpp` — `FPSMixin`

**Примечание:** Текущая реализация использует Qt Charts. Для новых функций визуализации рекомендуется использовать [Qt Graphs](.kiro/steering/qt/qt-graphs.md) — современный модуль с поддержкой 2D и 3D графиков.

Для загрузки полной документации по Qt Charts используй: `#[[file:.kiro/steering/qt/qt-charts.md]]`


#### 6. Ядро утилит (`src/core/`)

**`AbstractTask`** — управление потоками:
- Создание потоков, специфичное для Windows (`CreateThread`)
- Виртуальный метод `_task()` для рабочей логики
- Хуки `_before_run_task()`, `_after_close_task()`
- `_sleep(ms)` для контроля частоты кадров

**`TcpBDSPSocket`** (`final`) — обработчик BDSP-пакетов для одного TCP-соединения:
- Наследуется от `QTcpSocket`
- Инкапсулирует `COBSZPETransceiver` для кодирования/декодирования пакетов
- Сигналы: `on_got_packet`, `on_bdsp_packet_parsing_error`, `disconnected(TcpBDSPSocket*)`
- Метод `send_data(packet_id, data, size)` для отправки пакетов

**`TcpBDSPServer`** — базовый TCP-сервер с поддержкой BDSP:
- Наследуется от `QTcpServer`
- При входящем соединении создаёт `TcpBDSPSocket` и испускает сигнал `tcp_bdsp_socket_ready`
- Используется как базовый класс для `AggregatorServer`

**Файлы:**
- `include/core/tcp_packet_socket.h` / `src/tcp_packet_socket.cpp`
- `include/core/tcp_packet_server.h` / `src/tcp_packet_server.cpp`
- `include/core/abs_task.h` / `src/abs_task.cpp`
- `include/core/utils.h` / `src/utils.cpp`

#### 7. 3D визуализация (`src/surfacegraph.h/cpp`, `src/surfacegraphmodifier.h/cpp`)
**`SurfaceGraph`** — обертка Qt Data Visualization:
- `Q3DSurface` для 3D поверхностных графиков
- `SurfaceGraphModifier` для взаимодействия

**`Spectrogram`** (`src/analyzer/spectrogram.h/cpp`) — структура данных:
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
│   ├── aggregator/CMakeLists.txt
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
- `aggregator` — сервер и клиент агрегации
- `analyzer` — статическая/разделяемая библиотека
- `audio` — платформо-специфичный захват аудио
- `charts` — библиотека визуализации графиков
- `core` — библиотека ядра утилит (`AbstractTask`, `TcpBDSPSocket`, `TcpBDSPServer`)
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
bool run_loopback = true;
bool run_serial = false;
bool run_generator = false;
bool run_analyser = true;
bool show_serial_samples = false;
bool show_generator_samples = false;
bool show_raw_samples = false;
bool show_samples = false;
bool show_amplitudes = true;
bool show_test_amplitudes = true;
bool show_serial_fast_amplitudes = false;
bool show_serial_audio_spectre = false;
bool show_surface = true;
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
