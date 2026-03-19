#ifndef CORE_APP_TYPES_H
#define CORE_APP_TYPES_H

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QJsonObject>

struct UIApplicationConfig {
    bool move_to_primary_screen_on_start = true;
};

struct DiagramSceneData {
    QJsonObject scene_graph;
};

struct FileSaveApplicationConfig {
    bool enable_aggregator = false;
    UIApplicationConfig ui{};
    DiagramSceneData scheme_graph{};
};

struct CoreApplicationConfig {
    bool enable_ui = true;
};

struct CoreApplicationContext {
    CoreApplicationConfig configs;
    FileSaveApplicationConfig _file_configs;
    // AggregatorClient* _aggregator_client = nullptr;
};

using node_delegate_model_registry_t = std::shared_ptr<QtNodes::NodeDelegateModelRegistry>;
using data_flow_graph_model_t = std::shared_ptr<QtNodes::DataFlowGraphModel>;

#endif // CORE_APP_TYPES_H