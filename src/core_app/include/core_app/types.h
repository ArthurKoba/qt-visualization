#ifndef CORE_APP_TYPES_H
#define CORE_APP_TYPES_H

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QJsonObject>

using node_delegate_model_registry_t = std::shared_ptr<QtNodes::NodeDelegateModelRegistry>;
using data_flow_graph_model_t = std::shared_ptr<QtNodes::DataFlowGraphModel>;

struct UIApplicationConfig {
    bool move_to_primary_screen_on_start;
    bool autosave_enabled;
};

struct FileSaveApplicationConfig {
    bool enable_aggregator = false;
    UIApplicationConfig ui{};
    QJsonObject scene_graph;
};

struct CoreApplicationConfig {
    bool enable_ui = true;
};

struct CoreApplicationContext {
    CoreApplicationConfig configs;
    FileSaveApplicationConfig file_configs;
    node_delegate_model_registry_t delegate_model_registry;
    data_flow_graph_model_t data_flow_graph;
    // AggregatorClient* _aggregator_client = nullptr;
};


#endif // CORE_APP_TYPES_H