#include "aggregator/aggregator_client.h"
#include <QCoreApplication>
#include <QHostAddress>
#include <QTimer>

int main(int32_t argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    //    QLoggingCategory::setFilterRules("aggregator.abstract.debug=true");

    qInfo("BDSP Test Client for Aggregator Server");
    qInfo("======================================");

    ComponentType component_type = ComponentType::data_source;

    uint16_t component_port = 0;
    AggregatorClientConfig cfg = {.uuid=QUuid::createUuid(), .type=component_type, .component_server_port=component_port};;
    qInfo("Client UUID: %s, type: %d, port: %d", qPrintable(cfg.uuid.toString()), component_type,
          component_port);

    AggregatorClient client = AggregatorClient(cfg);


    QObject::connect(&client, &AggregatorClient::on_client_close, [&app] () {
        qInfo("Client shutting down...");
        app.exit(-1);
    });

    QHostAddress server_address = QHostAddress::LocalHost;
    uint16_t server_port = 8212;
    qInfo("Connecting to server %s:%d...", qPrintable(server_address.toString()), server_port);
    if (not client.connect_to_aggregator_server(server_address, server_port)) {
        qWarning("Failed to connect to server");
        return -1;
    }

    return app.exec();
}