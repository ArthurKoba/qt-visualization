#ifndef QT_VISUALIZATION_EDITOR_WIDGET_H
#define QT_VISUALIZATION_EDITOR_WIDGET_H

#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtCore/QString>

#include "core_app/types.h"

class SchemeEditorWidget final : public QWidget {
    Q_OBJECT

public:
    explicit SchemeEditorWidget(CoreApplicationContext &ctx, QWidget *parent = nullptr);
    ~SchemeEditorWidget() override = default;

private:

    QtNodes::DataFlowGraphicsScene *_scene = nullptr;
    QtNodes::GraphicsView *_view = nullptr;

    void _setupConnections();
};


#endif //QT_VISUALIZATION_EDITOR_WIDGET_H
