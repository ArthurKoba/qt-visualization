#ifndef QT_VISUALIZATION_SOURCE_DATA_MODEL_H
#define QT_VISUALIZATION_SOURCE_DATA_MODEL_H



#include <QtNodes/NodeDelegateModel>
#include <QLineEdit>

class DecimalData;

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;


class NumberSourceDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    NumberSourceDataModel();

    ~NumberSourceDataModel() override {}

public:
    QString caption() const override { return QStringLiteral("Number Source"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("NumberSource"); }

public:
    QJsonObject save() const override;

    void load(QJsonObject const &p) override;

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

    QWidget *embeddedWidget() override;

public:
    void setNumber(double number);

private Q_SLOTS:

    void onTextEdited(QString const &string);

private:
    std::shared_ptr<DecimalData> _number;

    QLineEdit *_lineEdit;
};

#endif //QT_VISUALIZATION_SOURCE_DATA_MODEL_H