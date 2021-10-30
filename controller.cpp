#include "controller.h"

controller::controller(){

    menuWindow = new menu();
    qLinkWindow = new QLinkWindow();

    connect(menuWindow, (&menu::showLoad),
            this, (&controller::showLoad));
    connect(menuWindow, (&menu::showGame),
            this, (&controller::showGame));
    connect(qLinkWindow, (&QLinkWindow::goToMenu),
            this, (&controller::showMenu));
}

void controller::show(){
    menuWindow->show();
}


void controller::showGame(QWidget *from, gameMode_t s){
    qLinkWindow->setGameMode(s);
    from->hide();
    qLinkWindow->show();
}

void controller::showLoad(QWidget *from){
    from->hide();
    qLinkWindow->show();
}

void controller::showMenu(QWidget *from){
    from->hide();
    menuWindow->show();
}
