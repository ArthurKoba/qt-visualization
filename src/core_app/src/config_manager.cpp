#include "core_app/config_manager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QDir>
#include <utility>

Q_DECLARE_LOGGING_CATEGORY(core_app_config)

Q_LOGGING_CATEGORY(core_app_config, "core_app.config")

ConfigManager::ConfigManager(QString config_path) : _config_path(std::move(config_path)) { }

bool ConfigManager::load(FileSaveApplicationConfig &config) const {
    QFile config_file(_config_path);

    if (!config_file.exists()) {
        qCWarning(core_app_config, "Config file not found: %s", qPrintable(_config_path));
        return false;
    }

    if (!config_file.open(QIODevice::ReadOnly)) {
        qCWarning(core_app_config, "Failed to open config file: %s", qPrintable(_config_path));
        return false;
    }

    QJsonParseError parse_error;
    const auto json_doc = QJsonDocument::fromJson(config_file.readAll(), &parse_error);
    config_file.close();

    if (json_doc.isNull()) {
        qCWarning(core_app_config, "Failed to parse JSON config: %s", qPrintable(parse_error.errorString()));
        return false;
    }

    const auto json_obj = json_doc.object();

    _parse_core_config(json_obj, config);
    _parse_ui_config(json_obj, config);
    _parse_diagram_config(json_obj, config);

    qCInfo(core_app_config, "Config loaded successfully from: %s", qPrintable(_config_path));
    return true;
}

bool ConfigManager::save(const FileSaveApplicationConfig &config) const {
    // Создаем директорию если её нет
    const QFileInfo file_info(_config_path);
    const QDir dir = file_info.absoluteDir();
    if (not dir.exists() and dir.mkpath(".")) {
        qCWarning(core_app_config, "Failed to create config directory: %s", qPrintable(dir.absolutePath()));
        return false;
    }

    QFile config_file(_config_path);
    if (not config_file.open(QIODevice::WriteOnly)) {
        qCWarning(core_app_config, "Failed to open config file for writing: %s", qPrintable(_config_path));
        return false;
    }

    QJsonObject json_obj;
    _save_aggregator_config(json_obj, config);
    _save_ui_config(json_obj, config);
    _save_scheme_config(json_obj, config);

    const QJsonDocument json_doc(json_obj);
    config_file.write(json_doc.toJson());
    config_file.close();

    qCInfo(core_app_config, "Config saved successfully to: %s", qPrintable(_config_path));
    return true;
}

QString ConfigManager::getConfigPath() const {
    return _config_path;
}

void ConfigManager::setConfigPath(const QString &path) {
    _config_path = path;
}

auto ConfigManager::_parse_core_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) -> void {
    constexpr FileCoreApplicationConfig default_;
    if (not json_obj.contains("core")) return;
    const QJsonObject agg_obj = json_obj["core"].toObject();
    config.core.run_loopback = agg_obj.value("run_loopback").toBool(default_.run_loopback);
    config.core.run_analyser = agg_obj.value("run_analyser").toBool(default_.run_analyser);
}

void ConfigManager::_parse_ui_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) {
    constexpr UIApplicationConfig default_;

    if (not json_obj.contains("ui"))return;
    const QJsonObject ui_obj = json_obj["ui"].toObject();
    config.ui.move_to_primary_screen_on_start = ui_obj.value("move_to_primary_screen_on_start").toBool(
        default_.move_to_primary_screen_on_start);
    config.ui.autosave_enabled = ui_obj.value("autosave_enabled").toBool(default_.autosave_enabled);

    config.ui.show_raw_samples = ui_obj.value("show_raw_samples").toBool(default_.show_raw_samples);
    config.ui.show_samples = ui_obj.value("show_samples").toBool(default_.show_samples);
    config.ui.show_samples_after_window_function = ui_obj.value("show_samples_after_window_function").toBool(
        default_.show_samples_after_window_function);
    config.ui.show_window_function = ui_obj.value("show_window_function").toBool(default_.show_window_function);
    config.ui.show_amplitudes = ui_obj.value("show_amplitudes").toBool(default_.show_amplitudes);
    config.ui.show_test_amplitudes = ui_obj.value("show_test_amplitudes").toBool(default_.show_test_amplitudes);
    config.ui.show_surface = ui_obj.value("show_surface").toBool(default_.show_surface);
}

void ConfigManager::_parse_diagram_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) {
    config.scene_graph = json_obj.value("scene_graph").toObject(QJsonObject());
}

void ConfigManager::_save_aggregator_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    QJsonObject agg_obj;
    agg_obj["run_loopback"] = config.core.run_loopback;
    agg_obj["run_analyser"] = config.core.run_analyser;
    json_obj["core"] = agg_obj;
}

void ConfigManager::_save_ui_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    QJsonObject ui_obj;
    ui_obj["move_to_primary_screen_on_start"] = config.ui.move_to_primary_screen_on_start;
    ui_obj["autosave_enabled"] = config.ui.autosave_enabled;
    ui_obj["show_raw_samples"] = config.ui.show_raw_samples;
    ui_obj["show_samples"] = config.ui.show_samples;
    ui_obj["show_samples_after_window_function"] = config.ui.show_samples_after_window_function;
    ui_obj["show_window_function"] = config.ui.show_window_function;
    ui_obj["show_raw_samples"] = config.ui.show_raw_samples;
    ui_obj["show_amplitudes"] = config.ui.show_amplitudes;
    ui_obj["show_test_amplitudes"] = config.ui.show_test_amplitudes;
    ui_obj["show_surface"] = config.ui.show_surface;
    json_obj["ui"] = ui_obj;
}

void ConfigManager::_save_scheme_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    json_obj["scene_graph"] = config.scene_graph;
}
