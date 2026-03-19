#include "core_app/scheme_editor/editor_widget.h"

#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeDelegateModelRegistry>

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

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

SchemeEditorWidget::SchemeEditorWidget(QWidget *parent) :
            QWidget(parent),
            _registry(std::make_shared<NodeDelegateModelRegistry>()),
            _data_flow_graph_model(std::make_shared<DataFlowGraphModel>(_registry)),
            _scene(new DataFlowGraphicsScene(*_data_flow_graph_model, this)),
            _view(new GraphicsView(this)) {
    setStyle__();


    _scene->setGroupingEnabled(false);
    _view->setScene(_scene);

    _registry->registerModel<NumberSourceDataModel>();
    _registry->registerModel<NumberDisplayDataModel>();
    _registry->registerModel<AdditionModel>();



    auto menuBar = new QMenuBar(this);
    QMenu *menu = menuBar->addMenu("File");

    auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    auto loadAction = menu->addAction("Load Scene");
    loadAction->setShortcut(QKeySequence::Open);

    QVBoxLayout *l = new QVBoxLayout(this);


    l->addWidget(menuBar);

    l->addWidget(_view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    connect(saveAction, &QAction::triggered, _scene, [this]() {

        if (_scene->save())
            setWindowModified(false);
    });

    connect(loadAction, &QAction::triggered, _scene, &DataFlowGraphicsScene::load);

    connect(_scene, &DataFlowGraphicsScene::sceneLoaded, _view, &GraphicsView::centerScene);

    connect(_scene, &DataFlowGraphicsScene::modified, this, [this]() {

        // setWindowModified(true);
    });
    if (_scene->groupingEnabled()) {
        auto loadGroupAction = menu->addAction("Load Group...");
        connect(loadGroupAction, &QAction::triggered, [this] { _scene->loadGroupFile(); });
    }
}
