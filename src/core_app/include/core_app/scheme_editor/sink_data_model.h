#ifndef QT_VISUALIZATION_SINK_DATA_MODEL_H
#define QT_VISUALIZATION_SINK_DATA_MODEL_H

#include <iostream>

#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>

#include "core_app/scheme_editor/types.h"


class QLabel;


class NumberDisplayDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    NumberDisplayDataModel();

    ~NumberDisplayDataModel() = default;

    QString caption() const override { return QStringLiteral("Result"); }

    bool captionVisible() const override { return true; }

    QString name() const override { return QStringLiteral("Result"); }

    unsigned int nPorts(QtNodes::PortType portType) const override;

    NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, QtNodes::PortIndex portIndex) override;

    QWidget *embeddedWidget() override;

    double number() const;

private:
    std::shared_ptr<DecimalData> _numberData;

    QLabel *_label;
};

#endif //QT_VISUALIZATION_SINK_DATA_MODEL_H
