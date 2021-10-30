#include <QtWidgets>
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    controller window;
    window.show();
    return app.exec();
}
//! [main program]
