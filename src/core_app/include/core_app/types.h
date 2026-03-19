#ifndef CORE_APP_TYPES_H
#define CORE_APP_TYPES_H

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>

struct UIApplicationConfig {
    bool move_to_primary_screen_on_start = true;
};

struct CoreApplicationConfig {
    bool enable_aggregator = false;
    bool enable_ui = true;
    UIApplicationConfig ui{};
};

struct CoreApplicationContext {
    CoreApplicationConfig configs;
    // AggregatorClient* _aggregator_client = nullptr;
};

using node_delegate_model_registry_t = std::shared_ptr<QtNodes::NodeDelegateModelRegistry>;
using data_flow_graph_model_t = std::shared_ptr<QtNodes::DataFlowGraphModel>;

#endif // CORE_APP_TYPES_H