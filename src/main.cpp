#include "app.h"


int main(int argc, char *argv[]) {

    QApplicationCustom a(argc, argv);
    a.serial.connect("COM9",1000000);

    return QApplicationCustom::exec();
}
