#include "app.h"
#include "surfacegraph.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qtabwidget.h>
#include <QtGui/qscreen.h>

using namespace Qt::StringLiterals;

int main(int argc, char **argv)
{
    qputenv("QSG_RHI_BACKEND", "opengl");


    Application a(argc, argv);

    return Application::exec();
    QApplication app(argc, argv);

    // Create a tab widget for creating own tabs for Q3DBars, Q3DScatter, and Q3DSurface
    QTabWidget tabWidget;
    tabWidget.setWindowTitle(u"Graph Gallery"_s);

    const QSize screenSize = tabWidget.screen()->size();
    const QSize minimumGraphSize{screenSize.width() / 2, qRound(screenSize.height() / 1.75)};

    SurfaceGraph surface;

    if (!surface.initialize(minimumGraphSize, screenSize)) {
        QMessageBox::warning(nullptr, u"Graph Gallery"_s, u"Couldn't initialize the OpenGL context."_s);
        return -1;
    }

    tabWidget.addTab(surface.surfaceWidget(), u"Surface Graph"_s);

    tabWidget.show();
    return app.exec();
}
