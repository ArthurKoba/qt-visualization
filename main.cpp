#include <QApplication>
#include <QStyleFactory>
#include "serial_viewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Устанавливаем современный стиль для лучшего внешнего вида
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Устанавливаем информацию о приложении
    app.setApplicationName("COM Serial Viewer");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Serial Tools");
    
    SerialViewer viewer;
    viewer.show();
    
    return app.exec();
}