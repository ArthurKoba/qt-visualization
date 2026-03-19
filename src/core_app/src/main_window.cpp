#include "core_app/main_window.h"

#include <QMenuBar>
#include <QVBoxLayout>
#include <QtWidgets/QStatusBar>
#include <QtCore/QLoggingCategory>
#include <QtGui/QShowEvent>

#include "core_app/settings_widget.h"
#include "core_app/scheme_editor/editor_widget.h"

Q_LOGGING_CATEGORY(core_app_window, "core_app.window")

MainWindow::MainWindow(CoreApplicationContext &context) : _context(context) {
    setWindowTitle("Data Processing System Core");
    setMinimumSize(800, 600);
    resize(1000, 700);

    auto *menu = menuBar()->addMenu("File");
    const auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    connect(saveAction, &QAction::triggered, this, [this] () {emit on_config_updated();});

    _tab_widget = new QTabWidget(this);
    _tab_widget->addTab(new SchemeEditorWidget(this), "Scheme Editor");
    // _tab_widget->addTab(new SettingsWidget(this), "Settings");

    setCentralWidget(_tab_widget);
}

