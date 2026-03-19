#ifndef QT_VISUALIZATION_PROCESSOR_DATA_MODEL_H
#define QT_VISUALIZATION_PROCESSOR_DATA_MODEL_H

#include "core_app/scheme_editor/types.h"

class AdditionModel : public MathOperationDataModel {
public:
    ~AdditionModel() override = default;
    QString caption() const override { return QStringLiteral("Addition"); }
    QString name() const override { return QStringLiteral("Addition"); }

private:
    void compute() override {
        constexpr PortIndex outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n1 && n2) {
            _result = std::make_shared<DecimalData>(n1->number() + n2->number());
        } else {
            _result.reset();
        }

        Q_EMIT dataUpdated(outPortIndex);
    }
};


#endif //QT_VISUALIZATION_PROCESSOR_DATA_MODEL_H
