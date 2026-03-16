---
inclusion: manual
---

# Qt GUI

## Описание

Qt GUI предоставляет базовые классы для графического интерфейса: окна, события, рисование, шрифты, изображения. Используется как основа для Qt Widgets и Qt Quick.

**Назначение:** Низкоуровневая работа с графикой, рисование, обработка событий.

## Обзор

- [Qt GUI Overview](https://doc.qt.io/qt-6/qtgui-index.html)
- [Qt GUI Module](https://doc.qt.io/qt-6/qtgui-module.html)

## Ключевые концепции

### Окна и события
- [Qt GUI Application Windows](https://doc.qt.io/qt-6/application-windows.html)
- [Event System](https://doc.qt.io/qt-6/eventsandfilters.html)

### Рисование
- [Qt GUI 2D Graphics](https://doc.qt.io/qt-6/paintsystem.html)
- [Painting Classes](https://doc.qt.io/qt-6/painting.html)

### Графика и рендеринг
- [Qt GUI Accelerated 2D and 3D Graphics using the Qt RHI](https://doc.qt.io/qt-6/rhi.html)
- [Qt GUI OpenGL and OpenGL ES Integration](https://doc.qt.io/qt-6/opengl.html)

## Ключевые классы

**Полный список всех классов модуля:** [Qt GUI C++ Classes](https://doc.qt.io/qt-6/qtgui-module.html)

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QGuiApplication` | Основной класс GUI приложения | [QGuiApplication](https://doc.qt.io/qt-6/qguiapplication.html) |
| `QWindow` | Окно верхнего уровня | [QWindow](https://doc.qt.io/qt-6/qwindow.html) |
| `QScreen` | Информация об экране | [QScreen](https://doc.qt.io/qt-6/qscreen.html) |
| `QSurface` | Абстракция рендеримой поверхности | [QSurface](https://doc.qt.io/qt-6/qsurface.html) |
| `QOffscreenSurface` | Offscreen поверхность | [QOffscreenSurface](https://doc.qt.io/qt-6/qoffsceensurface.html) |
| `QPainter` | Рисование на поверхности | [QPainter](https://doc.qt.io/qt-6/qpainter.html) |
| `QPaintDevice` | Базовый класс для устройств рисования | [QPaintDevice](https://doc.qt.io/qt-6/qpaintdevice.html) |
| `QPaintEngine` | Абстрактное определение рисования | [QPaintEngine](https://doc.qt.io/qt-6/qpaintengine.html) |
| `QPainterPath` | Контейнер для операций рисования | [QPainterPath](https://doc.qt.io/qt-6/qpainterpath.html) |
| `QPen` | Стиль линии для рисования | [QPen](https://doc.qt.io/qt-6/qpen.html) |
| `QBrush` | Стиль заполнения | [QBrush](https://doc.qt.io/qt-6/qbrush.html) |
| `QColor` | Цвет | [QColor](https://doc.qt.io/qt-6/qcolor.html) |
| `QColorSpace` | Цветовое пространство | [QColorSpace](https://doc.qt.io/qt-6/qcolorspace.html) |
| `QGradient` | Градиент для заполнения | [QGradient](https://doc.qt.io/qt-6/qgradient.html) |
| `QLinearGradient` | Линейный градиент | [QLinearGradient](https://doc.qt.io/qt-6/qlineargradient.html) |
| `QRadialGradient` | Радиальный градиент | [QRadialGradient](https://doc.qt.io/qt-6/qradialgradient.html) |
| `QConicalGradient` | Конический градиент | [QConicalGradient](https://doc.qt.io/qt-6/qconicalgradient.html) |
| `QFont` | Шрифт | [QFont](https://doc.qt.io/qt-6/qfont.html) |
| `QFontDatabase` | Информация о доступных шрифтах | [QFontDatabase](https://doc.qt.io/qt-6/qfontdatabase.html) |
| `QFontMetrics` | Метрики шрифта | [QFontMetrics](https://doc.qt.io/qt-6/qfontmetrics.html) |
| `QFontInfo` | Информация о шрифте | [QFontInfo](https://doc.qt.io/qt-6/qfontinfo.html) |
| `QImage` | Изображение в памяти | [QImage](https://doc.qt.io/qt-6/qimage.html) |
| `QPixmap` | Оптимизированное изображение | [QPixmap](https://doc.qt.io/qt-6/qpixmap.html) |
| `QBitmap` | Монохромное изображение | [QBitmap](https://doc.qt.io/qt-6/qbitmap.html) |
| `QIcon` | Иконка | [QIcon](https://doc.qt.io/qt-6/qicon.html) |
| `QIconEngine` | Абстрактный базовый класс для рендеров иконок | [QIconEngine](https://doc.qt.io/qt-6/qiconengine.html) |
| `QImageReader` | Чтение изображений | [QImageReader](https://doc.qt.io/qt-6/qimagereader.html) |
| `QImageWriter` | Запись изображений | [QImageWriter](https://doc.qt.io/qt-6/qimagewriter.html) |
| `QMovie` | Воспроизведение анимированных изображений | [QMovie](https://doc.qt.io/qt-6/qmovie.html) |
| `QKeySequence` | Комбинация клавиш | [QKeySequence](https://doc.qt.io/qt-6/qkeysequence.html) |
| `QCursor` | Курсор мыши | [QCursor](https://doc.qt.io/qt-6/qcursor.html) |
| `QClipboard` | Буфер обмена | [QClipboard](https://doc.qt.io/qt-6/qclipboard.html) |
| `QDrag` | Поддержка drag and drop | [QDrag](https://doc.qt.io/qt-6/qdrag.html) |
| `QOpenGLContext` | OpenGL контекст | [QOpenGLContext](https://doc.qt.io/qt-6/qopenglcontext.html) |
| `QOpenGLFunctions` | Доступ к OpenGL ES 2.0 API | [QOpenGLFunctions](https://doc.qt.io/qt-6/qopenglfunctions.html) |
| `QOpenGLExtraFunctions` | Доступ к OpenGL ES 3.0+ API | [QOpenGLExtraFunctions](https://doc.qt.io/qt-6/qopenglextrafunctions.html) |
| `QOpenGLTexture` | OpenGL текстура | [QOpenGLTexture](https://doc.qt.io/qt-6/qopengltexture.html) |
| `QVulkanInstance` | Vulkan инстанс | [QVulkanInstance](https://doc.qt.io/qt-6/qvulkaninstance.html) |
| `QVulkanWindow` | Окно для Vulkan рендеринга | [QVulkanWindow](https://doc.qt.io/qt-6/qvulkanwindow.html) |
| `QEvent` | Базовый класс для событий | [QEvent](https://doc.qt.io/qt-6/qevent.html) |
| `QInputEvent` | Базовый класс для событий ввода | [QInputEvent](https://doc.qt.io/qt-6/qinputevent.html) |
| `QKeyEvent` | Событие клавиатуры | [QKeyEvent](https://doc.qt.io/qt-6/qkeyevent.html) |
| `QMouseEvent` | Событие мыши | [QMouseEvent](https://doc.qt.io/qt-6/qmouseevent.html) |
| `QWheelEvent` | Событие колеса мыши | [QWheelEvent](https://doc.qt.io/qt-6/qwheelevent.html) |
| `QTouchEvent` | Событие касания | [QTouchEvent](https://doc.qt.io/qt-6/qtouchevent.html) |
| `QTabletEvent` | Событие графического планшета | [QTabletEvent](https://doc.qt.io/qt-6/qtabletevent.html) |
| `QHoverEvent` | Событие наведения мыши | [QHoverEvent](https://doc.qt.io/qt-6/qhoverevent.html) |
| `QDragEnterEvent` | Событие входа drag and drop | [QDragEnterEvent](https://doc.qt.io/qt-6/qdragenterevent.html) |
| `QDropEvent` | Событие drop | [QDropEvent](https://doc.qt.io/qt-6/qdropevent.html) |
| `QPaintEvent` | Событие рисования | [QPaintEvent](https://doc.qt.io/qt-6/qpaintevent.html) |
| `QResizeEvent` | Событие изменения размера | [QResizeEvent](https://doc.qt.io/qt-6/qresizeevent.html) |
| `QMoveEvent` | Событие перемещения | [QMoveEvent](https://doc.qt.io/qt-6/qmoveevent.html) |
| `QCloseEvent` | Событие закрытия окна | [QCloseEvent](https://doc.qt.io/qt-6/qcloseevent.html) |
| `QShowEvent` | Событие показа окна | [QShowEvent](https://doc.qt.io/qt-6/qshowevent.html) |
| `QHideEvent` | Событие скрытия окна | [QHideEvent](https://doc.qt.io/qt-6/qhideevent.html) |
| `QFocusEvent` | Событие фокуса | [QFocusEvent](https://doc.qt.io/qt-6/qfocusevent.html) |
| `QContextMenuEvent` | Событие контекстного меню | [QContextMenuEvent](https://doc.qt.io/qt-6/qcontextmenuevent.html) |
| `QInputMethod` | Доступ к методу ввода текста | [QInputMethod](https://doc.qt.io/qt-6/qinputmethod.html) |
| `QInputMethodEvent` | Событие метода ввода | [QInputMethodEvent](https://doc.qt.io/qt-6/qinputmethodevent.html) |
| `QTextDocument` | Документ с форматированным текстом | [QTextDocument](https://doc.qt.io/qt-6/qtextdocument.html) |
| `QTextCursor` | Курсор в текстовом документе | [QTextCursor](https://doc.qt.io/qt-6/qtextcursor.html) |
| `QTextBlock` | Блок текста в документе | [QTextBlock](https://doc.qt.io/qt-6/qtextblock.html) |
| `QTextCharFormat` | Форматирование символов | [QTextCharFormat](https://doc.qt.io/qt-6/qtextcharformat.html) |
| `QTextBlockFormat` | Форматирование блока текста | [QTextBlockFormat](https://doc.qt.io/qt-6/qtextblockformat.html) |
| `QTextTableFormat` | Форматирование таблицы | [QTextTableFormat](https://doc.qt.io/qt-6/qtexttableformat.html) |
| `QTextTable` | Таблица в текстовом документе | [QTextTable](https://doc.qt.io/qt-6/qtexttable.html) |
| `QTextLayout` | Раскладка текста | [QTextLayout](https://doc.qt.io/qt-6/qtextlayout.html) |
| `QSyntaxHighlighter` | Подсветка синтаксиса | [QSyntaxHighlighter](https://doc.qt.io/qt-6/qsyntaxhighlighter.html) |
| `QValidator` | Валидация ввода | [QValidator](https://doc.qt.io/qt-6/qvalidator.html) |
| `QIntValidator` | Валидация целых чисел | [QIntValidator](https://doc.qt.io/qt-6/qintvalidator.html) |
| `QDoubleValidator` | Валидация вещественных чисел | [QDoubleValidator](https://doc.qt.io/qt-6/qdoublevalidator.html) |
| `QRegularExpressionValidator` | Валидация по регулярному выражению | [QRegularExpressionValidator](https://doc.qt.io/qt-6/qregularexpressionvalidator.html) |
| `QAction` | Абстракция команды пользователя | [QAction](https://doc.qt.io/qt-6/qaction.html) |
| `QActionGroup` | Группа действий | [QActionGroup](https://doc.qt.io/qt-6/qactiongroup.html) |
| `QShortcut` | Клавиатурный ярлык | [QShortcut](https://doc.qt.io/qt-6/qshortcut.html) |
| `QUndoCommand` | Команда для отмены/повтора | [QUndoCommand](https://doc.qt.io/qt-6/qundocommand.html) |
| `QUndoStack` | Стек команд отмены/повтора | [QUndoStack](https://doc.qt.io/qt-6/qundostack.html) |
| `QUndoGroup` | Группа стеков отмены/повтора | [QUndoGroup](https://doc.qt.io/qt-6/qundogroup.html) |
| `QAccessible` | Поддержка доступности | [QAccessible](https://doc.qt.io/qt-6/qaccessible.html) |
| `QAccessibleInterface` | Интерфейс доступности | [QAccessibleInterface](https://doc.qt.io/qt-6/qaccessibleinterface.html) |
| `QFileSystemModel` | Модель файловой системы | [QFileSystemModel](https://doc.qt.io/qt-6/qfilesystemmodel.html) |
| `QStandardItemModel` | Стандартная модель данных | [QStandardItemModel](https://doc.qt.io/qt-6/qstandarditemmodel.html) |
| `QStandardItem` | Элемент стандартной модели | [QStandardItem](https://doc.qt.io/qt-6/qstandarditem.html) |
| `QTransform` | 2D трансформация координат | [QTransform](https://doc.qt.io/qt-6/qtransform.html) |
| `QMatrix4x4` | 4x4 матрица трансформации | [QMatrix4x4](https://doc.qt.io/qt-6/qmatrix4x4.html) |
| `QVector2D` | 2D вектор | [QVector2D](https://doc.qt.io/qt-6/qvector2d.html) |
| `QVector3D` | 3D вектор | [QVector3D](https://doc.qt.io/qt-6/qvector3d.html) |
| `QVector4D` | 4D вектор | [QVector4D](https://doc.qt.io/qt-6/qvector4d.html) |
| `QQuaternion` | Кватернион | [QQuaternion](https://doc.qt.io/qt-6/qquaternion.html) |
| `QPolygon` | Многоугольник с целыми координатами | [QPolygon](https://doc.qt.io/qt-6/qpolygon.html) |
| `QPolygonF` | Многоугольник с вещественными координатами | [QPolygonF](https://doc.qt.io/qt-6/qpolygonf.html) |
| `QRegion` | Область клипирования | [QRegion](https://doc.qt.io/qt-6/qregion.html) |
| `QPalette` | Цветовые группы для состояний виджета | [QPalette](https://doc.qt.io/qt-6/qpalette.html) |
| `QPageSize` | Размер страницы | [QPageSize](https://doc.qt.io/qt-6/qpagesize.html) |
| `QPageLayout` | Макет страницы | [QPageLayout](https://doc.qt.io/qt-6/qpagelayout.html) |
| `QPdfWriter` | Запись PDF | [QPdfWriter](https://doc.qt.io/qt-6/qpdfwriter.html) |
| `QDesktopServices` | Доступ к сервисам рабочего стола | [QDesktopServices](https://doc.qt.io/qt-6/qdesktopservices.html) |
| `QSessionManager` | Менеджер сессии | [QSessionManager](https://doc.qt.io/qt-6/qsessionmanager.html) |
| `QStyleHints` | Подсказки стиля платформы | [QStyleHints](https://doc.qt.io/qt-6/qstylehints.html) |
| `QSurfaceFormat` | Формат поверхности рендеринга | [QSurfaceFormat](https://doc.qt.io/qt-6/qsurfaceformat.html) |

## Обработка событий

- [Event System](https://doc.qt.io/qt-6/eventsandfilters.html)
- [Event Classes](https://doc.qt.io/qt-6/events.html)

## Примеры

- [Qt GUI Examples](https://doc.qt.io/qt-6/qtgui-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Gui)
target_link_libraries(my_target PRIVATE Qt6::Gui)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt Widgets](./qt-widgets.md) — Виджеты
- [Qt OpenGL](./qt-opengl.md) — OpenGL интеграция
