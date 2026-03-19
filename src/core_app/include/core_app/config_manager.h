#ifndef CORE_APP_CONFIG_MANAGER_H
#define CORE_APP_CONFIG_MANAGER_H

#include "core_app/types.h"

class ConfigManager {
public:
    explicit ConfigManager(QString config_path = "config.json");
    
    bool load(FileSaveApplicationConfig& config);
    
    bool save(const FileSaveApplicationConfig& config);
    
    [[nodiscard]] QString getConfigPath() const;
    
    void setConfigPath(const QString& path);

private:
    QString _config_path;
    
    void _parse_aggregator_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    void _parse_ui_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    void _parse_diagram_config(const QJsonObject& json_obj, FileSaveApplicationConfig& config);
    
    void _save_aggregator_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
    void _save_ui_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
    void _save_diagram_config(QJsonObject& json_obj, const FileSaveApplicationConfig& config);
};

#endif // CORE_APP_CONFIG_MANAGER_H
