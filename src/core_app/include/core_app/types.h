#ifndef CORE_APP_TYPES_H
#define CORE_APP_TYPES_H

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QJsonObject>

#include "wled/wled_audio_reactive_server.h"
#include "analyzer/analyze/analyzer.h"
#include "analyzer/analyze/spectrogram.h"
#include "analyzer/loopback/windows_loopback.h"

using node_delegate_model_registry_t = std::shared_ptr<QtNodes::NodeDelegateModelRegistry>;
using data_flow_graph_model_t = std::shared_ptr<QtNodes::DataFlowGraphModel>;

struct UIApplicationConfig {
    bool move_to_primary_screen_on_start = false;
    bool autosave_enabled = true;
    bool show_raw_samples = false;
    bool show_samples = false;
    bool show_samples_after_window_function = false;
    bool show_window_function = false;
    bool show_amplitudes = true;
    bool show_test_amplitudes = true;
    bool show_surface = true;
};

struct WLEDARServerConfig {
    bool enabled = false;
    QString network_interface_name{};
    quint16 port{};
};

struct FileCoreApplicationConfig {
    bool run_loopback = true;
    bool run_analyser = true;
};

struct FileSaveApplicationConfig {
    FileCoreApplicationConfig core;
    UIApplicationConfig ui{};
    WLEDARServerConfig wled_ar_server{};
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
    Analyzer *analyzer = nullptr;
    Spectrogram *spectrogram = nullptr;
    audio::loopback::WASAPILoopback *loopback = nullptr;
    WLEDAudioReactiveServer *wled_ar_server = nullptr;
};

#endif // CORE_APP_TYPES_H
