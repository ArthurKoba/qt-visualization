#include "core_app/scheme_editor/editor_widget.h"

#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeDelegateModelRegistry>

#include <QtWidgets/QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>

#include "core_app/scheme_editor/sink_data_model.h"
#include "core_app/scheme_editor/source_data_model.h"
#include "core_app/scheme_editor/processor_data_model.h"

using QtNodes::ConnectionStyle;
using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;

static void setStyle__() {
    ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

SchemeEditorWidget::SchemeEditorWidget(CoreApplicationContext &ctx, QWidget *parent) :
            QWidget(parent),
            _scene(new DataFlowGraphicsScene(*ctx.data_flow_graph, this)),
            _view(new GraphicsView(this)) {
    setStyle__();

    _scene->setGroupingEnabled(false);
    _view->setScene(_scene);



    _setupConnections();
}


void SchemeEditorWidget::_setupConnections() {
    connect(_scene, &DataFlowGraphicsScene::sceneLoaded, _view, &GraphicsView::centerScene);

    connect(_scene, &DataFlowGraphicsScene::modified, this, [this]() {
        // Можно добавить индикатор изменений
    });
}
