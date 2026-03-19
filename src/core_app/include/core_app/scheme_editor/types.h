#ifndef QT_VISUALIZATION_CORE_APP_SCHEME_EDITOR_TYPES_H
#define QT_VISUALIZATION_CORE_APP_SCHEME_EDITOR_TYPES_H

#include <QtWidgets/QLabel>
#include <QtCore/QJsonObject>

#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModel>

#include <iostream>


class DecimalData : public QtNodes::NodeData {
public:
    DecimalData() = default;

    explicit DecimalData(double const number) : _number(number) { }

    QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"decimal", "Decimal"}; }

    double number() const { return _number; }

    QString numberAsText() const { return QString::number(_number, 'f'); }

private:
    double _number;
};




using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ~MathOperationDataModel() = default;

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override { return nullptr; }

protected:
    virtual void compute() = 0;

protected:
    std::weak_ptr<DecimalData> _number1;
    std::weak_ptr<DecimalData> _number2;

    std::shared_ptr<DecimalData> _result;
};

#endif //QT_VISUALIZATION_CORE_APP_SCHEME_EDITOR_TYPES_H