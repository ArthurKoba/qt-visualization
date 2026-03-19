#ifndef CORE_APP_CONFIG_MANAGER_H
#define CORE_APP_CONFIG_MANAGER_H

#include "core_app/types.h"

class ConfigManager {
public:
    explicit ConfigManager(QString config_path = "config.json");
    
    bool load(FileSaveApplicationConfig& config) const;
    
    bool save(const FileSaveApplicationConfig& config) const;
    
    [[nodiscard]] QString getConfigPath() const;
    
    void setConfigPath(const QString& path);

private:
    QString _config_path;
    
    static void _parse_aggregator_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    static void _parse_ui_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    static void _parse_diagram_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    
    static void _save_aggregator_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
    static void _save_ui_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
    static void _save_scheme_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
};

#endif // CORE_APP_CONFIG_MANAGER_H
