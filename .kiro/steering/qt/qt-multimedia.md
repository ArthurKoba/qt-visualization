---
inclusion: manual
---

# Qt Multimedia

## Описание

Qt Multimedia предоставляет классы и типы QML для работы с мультимедиа: воспроизведение аудио/видео, запись, работа с камерой, микрофоном и другими устройствами.

**Назначение:** Создание приложений с поддержкой аудио, видео, камеры и микрофона.

## Обзор

- [Qt Multimedia Overview](https://doc.qt.io/qt-6/qtmultimedia-index.html)
- [Qt Multimedia Module](https://doc.qt.io/qt-6/qtmultimedia-module.html)

## Ключевые концепции

### Основы мультимедиа
- [Multimedia Overview](https://doc.qt.io/qt-6/multimedia.html)
- [Audio Overview](https://doc.qt.io/qt-6/audiooverview.html)
- [Video Overview](https://doc.qt.io/qt-6/videooverview.html)
- [Camera Overview](https://doc.qt.io/qt-6/cameraoverview.html)

### Пространственный звук
- [Spatial Audio Overview](https://doc.qt.io/qt-6/spatialaudioverview.html)

### Поддерживаемые форматы
- [Supported Media Formats](https://doc.qt.io/qt-6/supported-media-formats.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QMediaPlayer` | Воспроизведение аудио/видео | [QMediaPlayer](https://doc.qt.io/qt-6/qmediaplayer.html) |
| `QAudioOutput` | Вывод аудио | [QAudioOutput](https://doc.qt.io/qt-6/qaudiooutput.html) |
| `QAudioInput` | Ввод аудио | [QAudioInput](https://doc.qt.io/qt-6/qaudioinput.html) |
| `QAudioDecoder` | Декодирование аудио | [QAudioDecoder](https://doc.qt.io/qt-6/qaudiodecoder.html) |
| `QMediaRecorder` | Запись аудио/видео | [QMediaRecorder](https://doc.qt.io/qt-6/qmediarecorder.html) |
| `QMediaCaptureSession` | Сессия захвата медиа | [QMediaCaptureSession](https://doc.qt.io/qt-6/qmediacapturesession.html) |
| `QCamera` | Работа с камерой | [QCamera](https://doc.qt.io/qt-6/qcamera.html) |
| `QImageCapture` | Захват изображений | [QImageCapture](https://doc.qt.io/qt-6/qimagecapture.html) |
| `QVideoWidget` | Виджет для отображения видео | [QVideoWidget](https://doc.qt.io/qt-6/qvideowidget.html) |
| `QVideoFrame` | Кадр видео | [QVideoFrame](https://doc.qt.io/qt-6/qvideoframe.html) |
| `QSoundEffect` | Воспроизведение звуковых эффектов | [QSoundEffect](https://doc.qt.io/qt-6/qsoundeffect.html) |
| `QAudioSink` | Приемник аудио | [QAudioSink](https://doc.qt.io/qt-6/qaudiosink.html) |
| `QAudioSource` | Источник аудио | [QAudioSource](https://doc.qt.io/qt-6/qaudiosource.html) |
| `QAudioBuffer` | Буфер аудио | [QAudioBuffer](https://doc.qt.io/qt-6/qaudiobuffer.html) |
| `QMediaDevices` | Информация об устройствах | [QMediaDevices](https://doc.qt.io/qt-6/qmediadevices.html) |
| `QAudioDevice` | Информация об аудиоустройстве | [QAudioDevice](https://doc.qt.io/qt-6/qaudiodevice.html) |
| `QCameraDevice` | Информация о камере | [QCameraDevice](https://doc.qt.io/qt-6/qcameradevice.html) |

## Примеры

- [Qt Multimedia Examples](https://doc.qt.io/qt-6/qtmultimedia-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
target_link_libraries(my_target PRIVATE Qt6::Multimedia)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt GUI](./qt-gui.md) — Графические классы
- [Qt Widgets](./qt-widgets.md) — Виджеты
