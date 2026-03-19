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

    _parse_aggregator_config(json_obj, config);
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

void ConfigManager::_parse_aggregator_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) {
    if (not json_obj.contains("aggregator")) return;
    const QJsonObject agg_obj = json_obj["aggregator"].toObject();
    config.enable_aggregator = agg_obj.value("enable").toBool(false);
}

void ConfigManager::_parse_ui_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) {
    if (not json_obj.contains("ui"))return;
    const QJsonObject ui_obj = json_obj["ui"].toObject();
    config.ui.move_to_primary_screen_on_start = ui_obj.value("move_to_primary_screen_on_start").toBool(false);
    config.ui.autosave_enabled = ui_obj.value("autosave_enabled").toBool(true);
}

void ConfigManager::_parse_diagram_config(const QJsonObject &json_obj, FileSaveApplicationConfig &config) {
    config.scene_graph = json_obj.value("scene_graph").toObject(QJsonObject());
}

void ConfigManager::_save_aggregator_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    QJsonObject agg_obj;
    agg_obj["enable"] = config.enable_aggregator;
    json_obj["aggregator"] = agg_obj;
}

void ConfigManager::_save_ui_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    QJsonObject ui_obj;
    ui_obj["move_to_primary_screen_on_start"] = config.ui.move_to_primary_screen_on_start;
    ui_obj["autosave_enabled"] = config.ui.autosave_enabled;
    json_obj["ui"] = ui_obj;
}

void ConfigManager::_save_scheme_config(QJsonObject &json_obj, const FileSaveApplicationConfig &config) {
    json_obj["scene_graph"] = config.scene_graph;
}
