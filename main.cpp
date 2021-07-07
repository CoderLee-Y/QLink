#include <QtWidgets>
#include "QLink.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLinkWindow window;
    window.show();
    return app.exec();
}
//! [main program]
