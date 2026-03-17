#include "aggregator/aggregator_server.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mainServer)
Q_LOGGING_CATEGORY(mainServer, "aggregator.main_server")


int32_t main(int32_t argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("aggregator");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Aggregator Server - central component for tracking component availability");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption startOption(QStringList() << "s" << "start",
                                  "Start aggregation server (required)");
    parser.addOption(startOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                 "Port to listen on (default: 8212)",
                                 "PORT", QString::number(8212));
    parser.addOption(portOption);

    parser.process(app);

    bool should_start = parser.isSet(startOption);

    uint16_t port = 8212;
    if (parser.isSet(portOption)) {
        bool ok;
        uint32_t parsed_port = parser.value(portOption).toUInt(&ok);
        if (!ok || parsed_port > 65535) {
            qInfo("Error: invalid port number");
            return 1;
        }
        port = static_cast<uint16_t>(parsed_port);
    }

    if (!should_start) {
        qInfo("Error: option -s or --start is required to start the server");
        qInfo("Use -h or --help for help");
        return 1;
    }

    AggregatorServer server(&app);
    if (!server.start(port)) {
        qInfo("Error: failed to start server on port %d", port);
        return 1;
    }

    int32_t result = QCoreApplication::exec();
    server.stop();
    return result;
}
