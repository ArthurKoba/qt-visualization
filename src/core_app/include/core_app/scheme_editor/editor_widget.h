#ifndef QT_VISUALIZATION_EDITOR_WIDGET_H
#define QT_VISUALIZATION_EDITOR_WIDGET_H

#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>

#include "core_app/types.h"

class SchemeEditorWidget final : public QWidget {
    Q_OBJECT

public:
    explicit SchemeEditorWidget(QWidget *parent = nullptr);

private:
    node_delegate_model_registry_t _registry;
    data_flow_graph_model_t _data_flow_graph_model;
    QtNodes::DataFlowGraphicsScene *_scene = nullptr;
    QtNodes::GraphicsView *_view = nullptr;
};


#endif //QT_VISUALIZATION_EDITOR_WIDGET_H
