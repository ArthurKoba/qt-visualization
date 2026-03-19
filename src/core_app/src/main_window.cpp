#include "core_app/main_window.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtCore/QLoggingCategory>
#include <QtCore/QUuid>
#include <QtGui/QFocusEvent>
#include <QtGui/QShowEvent>

Q_LOGGING_CATEGORY(core_app_window, "core_app.window")

MainWindow::MainWindow(AppConfigManager* config_manager,
                       AggregatorGuiAdapter* aggregator_adapter,
                       QWidget* parent)
    : QMainWindow(parent)
    , _tab_widget(nullptr)
    , _diagram_editor(nullptr)
    , _components_tab(nullptr)
    , _components_scroll_area(nullptr)
    , _components_container(nullptr)
    , _components_layout(nullptr)
    , _no_components_label(nullptr)
    , _settings_widget(nullptr)
    , _next_grid_position(0)
    , _config_manager(config_manager)
    , _autosave_timer(new QTimer(this))
    , _aggregator_adapter(aggregator_adapter)
    , _pending_load_diagram(false)
{
    _setup_ui();

    // Автосохранение — одиночный таймер, запускается при изменении диаграммы
    _autosave_timer->setSingleShot(true);
    connect(_autosave_timer, &QTimer::timeout, this, &MainWindow::_on_autosave_timer);

    // Подключаем сигнал изменения диаграммы
    connect(_diagram_editor, &DiagramEditorWidget::diagram_changed,
            this, &MainWindow::_on_diagram_changed);

    // Синхронизируем переименование из редактора с карточками
    connect(_diagram_editor, &DiagramEditorWidget::component_renamed,
            this, [this](const QUuid& uuid, const QString& new_name) {
                auto it = _component_cards.find(uuid);
                if (it != _component_cards.end()) {
                    component_display_info_t info = it.value()->component_info();
                    info.display_name = new_name;
                    it.value()->update_component_info(info);
                }
            });

    // Загружаем конфигурацию — диаграмма будет восстановлена в showEvent
    if (_config_manager and _config_manager->config_exists()) {
        if (_config_manager->load()) {
            _settings_widget->load_from_config(_config_manager->settings());
            _settings_widget->set_config_path(_config_manager->config_path());

            // Диаграмму загружаем отложенно в showEvent, когда сцена уже готова
            _pending_load_diagram = true;

            qCInfo(core_app_window, "Loaded config from: %s",
                   qPrintable(_config_manager->config_path()));
        }
    }

    if (_aggregator_adapter) {
        connect(_aggregator_adapter, &AggregatorGuiAdapter::component_registered_for_gui,
                this, &MainWindow::_on_component_registered);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::component_state_changed_for_gui,
                this, &MainWindow::_on_component_state_changed);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::component_connected_for_gui,
                this, &MainWindow::_on_component_connected);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::component_disconnected_for_gui,
                this, &MainWindow::_on_component_disconnected);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::aggregator_client_closed,
                this, &MainWindow::_on_aggregator_client_closed);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::focus_component_requested,
                this, &MainWindow::_on_focus_component_requested);
        connect(_aggregator_adapter, &AggregatorGuiAdapter::main_window_focus_requested,
                this, &MainWindow::set_main_window_focus);
        qCInfo(core_app_window, "MainWindow connected to AggregatorGuiAdapter");
    }
}


void MainWindow::_setup_ui()
{
    setWindowTitle("Data Processing System Core");
    setMinimumSize(800, 600);
    resize(1000, 700);

    // Меню
    QMenuBar* menu_bar = menuBar();
    QMenu* file_menu = menu_bar->addMenu("&File");
    file_menu->addAction("&Save", this, &MainWindow::_on_save_clicked, QKeySequence::Save);
    file_menu->addSeparator();
    file_menu->addAction("&Exit", this, &QWidget::close);

    QMenu* view_menu = menu_bar->addMenu("&View");
    view_menu->addAction("&Refresh Components", this, &MainWindow::_update_components_display);

    // Тулбар с кнопкой Save
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    toolbar->addAction(QIcon::fromTheme("document-save"), "Save", this, &MainWindow::_on_save_clicked);

    statusBar()->showMessage("Ready");

    _tab_widget = new QTabWidget(this);
    setCentralWidget(_tab_widget);

    // Вкладка 1 — редактор блок-схем
    _diagram_editor = new DiagramEditorWidget();
    _tab_widget->addTab(_diagram_editor, "Block Diagram Editor");

    // Вкладка 2 — компоненты
    _components_tab = _create_components_tab();
    _tab_widget->addTab(_components_tab, "System Components");

    // Вкладка 3 — настройки
    _settings_widget = new SettingsWidget();
    if (_config_manager) {
        _settings_widget->set_config_path(_config_manager->config_path());
    }
    connect(_settings_widget, &SettingsWidget::settings_changed,
            this, &MainWindow::_on_settings_changed);
    _tab_widget->addTab(_settings_widget, "Settings");

    qCDebug(core_app_window, "UI setup completed");
}

QWidget* MainWindow::_create_components_tab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* tab_layout = new QVBoxLayout(tab);
    tab_layout->setContentsMargins(10, 10, 10, 10);
    tab_layout->setSpacing(10);

    // Заголовок
    QLabel* title_label = new QLabel("System Components");
    title_label->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px;");
    tab_layout->addWidget(title_label);

    // Область прокрутки для компонентов
    _components_scroll_area = new QScrollArea();
    _components_scroll_area->setWidgetResizable(true);
    _components_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _components_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Контейнер для карточек компонентов
    _components_container = new QWidget();
    _components_layout = new QGridLayout(_components_container);
    _components_layout->setSpacing(15);
    _components_layout->setContentsMargins(10, 10, 10, 10);
    _components_layout->setAlignment(Qt::AlignTop);

    // Метка "Нет компонентов" - размещаем в центре сетки
    _no_components_label = new QLabel("No components connected");
    _no_components_label->setAlignment(Qt::AlignCenter);
    _no_components_label->setStyleSheet("color: #888; font-size: 14px; margin: 50px;");
    _components_layout->addWidget(_no_components_label, 0, 0, 1, 2); // span across 2 columns

    _components_scroll_area->setWidget(_components_container);
    tab_layout->addWidget(_components_scroll_area);

    return tab;
}

void MainWindow::add_component(const component_display_info_t& component_info)
{
    // Проверяем, не существует ли уже компонент с таким UUID
    if (_component_cards.contains(component_info.uuid)) {
        qCWarning(core_app_window, "Component with UUID %s already exists, updating instead",
                  qPrintable(component_info.uuid.toString()));
        update_component(component_info);
        return;
    }

    // Скрываем метку "Нет компонентов"
    _no_components_label->hide();

    // Создаем новую карточку компонента
    ComponentCardWidget* card = new ComponentCardWidget(component_info, _components_container);
    _component_cards.insert(component_info.uuid, card);

    // Размещаем карточку в сетке (2 колонки)
    int32_t row = _next_grid_position / 2;
    int32_t column = _next_grid_position % 2;
    _components_layout->addWidget(card, row, column);
    _next_grid_position++;

    // Синхронизируем с редактором диаграмм
    _diagram_editor->add_component_node(component_info);

    // Обновляем статусную строку
    statusBar()->showMessage(QString("Components: %1").arg(_component_cards.size()));

    qCInfo(core_app_window, "Added component: %s (%s)",
           qPrintable(component_info.uuid.toString()),
           qPrintable(component_info.display_name));
}

void MainWindow::update_component(const component_display_info_t& component_info)
{
    auto it = _component_cards.find(component_info.uuid);
    if (it != _component_cards.end()) {
        it.value()->update_component_info(component_info);
        _diagram_editor->update_component_node(component_info);
        qCDebug(core_app_window, "Updated component: %s",
                qPrintable(component_info.uuid.toString()));
    } else {
        qCWarning(core_app_window, "Component with UUID %s not found for update",
                  qPrintable(component_info.uuid.toString()));
    }
}

void MainWindow::remove_component(const QUuid& uuid)
{
    auto it = _component_cards.find(uuid);
    if (it != _component_cards.end()) {
        ComponentCardWidget* card = it.value();
        _components_layout->removeWidget(card);
        card->deleteLater();
        _component_cards.erase(it);

        // Пересчитываем позиции в сетке
        _recalculate_grid_positions();

        // Синхронизируем с редактором диаграмм
        _diagram_editor->remove_component_node(uuid);

        // Показываем метку "Нет компонентов" если список пуст
        if (_component_cards.isEmpty()) {
            _no_components_label->show();
        }

        // Обновляем статусную строку
        statusBar()->showMessage(QString("Components: %1").arg(_component_cards.size()));

        qCInfo(core_app_window, "Removed component: %s", qPrintable(uuid.toString()));
    } else {
        qCWarning(core_app_window, "Component with UUID %s not found for removal",
                  qPrintable(uuid.toString()));
    }
}

void MainWindow::clear_components()
{
    // Удаляем все карточки
    for (auto it = _component_cards.begin(); it != _component_cards.end(); ++it) {
        ComponentCardWidget* card = it.value();
        _components_layout->removeWidget(card);
        card->deleteLater();
    }
    _component_cards.clear();

    // Сбрасываем позицию сетки
    _next_grid_position = 0;

    // Синхронизируем с редактором диаграмм
    _diagram_editor->clear_nodes();

    // Показываем метку "Нет компонентов"
    _no_components_label->show();

    // Обновляем статусную строку
    statusBar()->showMessage("Components: 0");

    qCInfo(core_app_window, "Cleared all components");
}

void MainWindow::_update_components_display()
{
    qCDebug(core_app_window, "Refreshing components display");
    statusBar()->showMessage(QString("Components: %1 (refreshed)").arg(_component_cards.size()), 2000);
}

void MainWindow::_on_save_clicked() {
    if (not _config_manager) {
        qCWarning(core_app_window, "Cannot save: no config manager");
        return;
    }
    _settings_widget->save_to_config(_config_manager->settings());
    _config_manager->set_diagram_json(_diagram_editor->save_diagram());
    if (_config_manager->save()) {
        statusBar()->showMessage("Configuration saved", 3000);
    } else {
        statusBar()->showMessage("Failed to save configuration", 3000);
    }
}

void MainWindow::_on_autosave_timer() {
    if (not _config_manager) return;
    _settings_widget->save_to_config(_config_manager->settings());
    _config_manager->set_diagram_json(_diagram_editor->save_diagram());
    if (_config_manager->save()) {
        qCInfo(core_app_window, "Autosaved configuration");
        statusBar()->showMessage("Autosaved", 2000);
    }
}

void MainWindow::_on_diagram_changed() {
    if (not _config_manager) return;
    const app_config_t& cfg = _config_manager->settings();
    if (cfg.autosave_enabled) {
        _autosave_timer->start(cfg.autosave_delay_ms);
    }
}

void MainWindow::_on_settings_changed() {
    if (not _config_manager) return;
    _settings_widget->save_to_config(_config_manager->settings());
    // Перезапускаем таймер с новым интервалом если он активен
    if (_autosave_timer->isActive()) {
        _autosave_timer->start(_config_manager->settings().autosave_delay_ms);
    }
}

void MainWindow::_recalculate_grid_positions()
{
    // Временно удаляем все карточки из layout
    QVector<ComponentCardWidget*> cards;
    for (auto it = _component_cards.begin(); it != _component_cards.end(); ++it) {
        ComponentCardWidget* card = it.value();
        _components_layout->removeWidget(card);
        cards.append(card);
    }

    // Размещаем карточки заново в правильном порядке
    _next_grid_position = 0;
    for (ComponentCardWidget* card : cards) {
        int32_t row = _next_grid_position / 2;
        int32_t column = _next_grid_position % 2;
        _components_layout->addWidget(card, row, column);
        _next_grid_position++;
    }

    qCDebug(core_app_window, "Recalculated grid positions for %d components", cards.size());
}

void MainWindow::set_main_window_focus() {
    qCDebug(core_app_window, "Setting focus to main window");

    // Активируем окно и устанавливаем фокус
    activateWindow();
    raise();
    setFocus();
}

void MainWindow::focusInEvent(QFocusEvent* event) {
    QMainWindow::focusInEvent(event);

    // Уведомляем агрегатор о получении фокуса главным окном
    if (_aggregator_adapter) {
        // Можно добавить специальный UUID для главного окна
        QUuid main_window_uuid = QUuid::createUuidV5(QUuid(), "main_window");
        _aggregator_adapter->get_event_client()->notify_component_focused(main_window_uuid);
    }
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);

    if (_pending_load_diagram) {
        _pending_load_diagram = false;

        // Откладываем загрузку через singleShot(0) — к этому моменту event loop
        // завершит все pending layout/resize события и QGraphicsView будет готов
        QTimer::singleShot(0, this, [this]() {
            QSignalBlocker blocker(_diagram_editor);
            _diagram_editor->load_diagram(_config_manager->diagram_json());
            _diagram_editor->center_view();
            qCInfo(core_app_window, "Diagram restored from config after show");
        });
    }
}

// Обработчики событий агрегатора
void MainWindow::_on_component_registered(const component_display_info_t& component_info) {
    qCDebug(core_app_window, "Component registered via aggregator: %s", qPrintable(component_info.uuid.toString()));
    add_component(component_info);
}

void MainWindow::_on_component_state_changed(const component_display_info_t& component_info) {
    qCDebug(core_app_window, "Component state changed via aggregator: %s", qPrintable(component_info.uuid.toString()));
    update_component(component_info);
}

void MainWindow::_on_component_connected(const component_display_info_t& component_info) {
    qCDebug(core_app_window, "Component connected via aggregator: %s", qPrintable(component_info.uuid.toString()));
    add_component(component_info);
}

void MainWindow::_on_component_disconnected(const QUuid& uuid) {
    qCDebug(core_app_window, "Component disconnected via aggregator: %s", qPrintable(uuid.toString()));
    remove_component(uuid);
}

void MainWindow::_on_aggregator_client_closed() {
    qCInfo(core_app_window, "Aggregator client closed - clearing all components");
    clear_components();

    // Показываем сообщение пользователю
    statusBar()->showMessage("Соединение с сервером агрегации потеряно", 5000);
}

void MainWindow::_on_focus_component_requested(const QUuid& uuid) {
    qCDebug(core_app_window, "Focus requested for component: %s", qPrintable(uuid.toString()));

    // Находим карточку компонента и устанавливаем на неё фокус
    auto it = _component_cards.find(uuid);
    if (it != _component_cards.end()) {
        ComponentCardWidget* card = it.value();
        if (card) {
            // Прокручиваем к карточке и устанавливаем фокус
            _components_scroll_area->ensureWidgetVisible(card);
            card->setFocus();

            // Уведомляем агрегатор о том, что фокус установлен
            if (_aggregator_adapter) {
                _aggregator_adapter->get_event_client()->notify_component_focused(uuid);
            }
        }
    } else {
        qCWarning(core_app_window, "Cannot focus unknown component: %s", qPrintable(uuid.toString()));
    }
}
