#include "QLink.h"

void QLinkWindow::show(){
    window.resize(320, 240);
    window.setWindowTitle
          (QApplication::translate("QLink", "QLink"));
    window.show();

//! [create, position and show]
    QPushButton *button = new QPushButton(
        QApplication::translate("QLink", "Press me"), &window);
    button->move(100, 100);
    button->show();
//! [create, position and show]
}
