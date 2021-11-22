#include "controller.h"

Controller::Controller(){

    menuWindow = new Menu();
    qLinkWindow = new QLinkWindow();

    connect(menuWindow, (&Menu::showLoad),
            this, (&Controller::showLoad));
    connect(menuWindow, (&Menu::showGame),
            this, (&Controller::showGame));
    connect(qLinkWindow, (&QLinkWindow::goToMenu),
            this, (&Controller::showMenu));

    qLinkWindow->releaseKeyboard();
    menuWindow->grabKeyboard();
}

void Controller::show(){
    menuWindow->show();
}


void Controller::showGame(QWidget *from, game_mode_t s){
    qLinkWindow->grabKeyboard();
    qLinkWindow->setGameMode(s);
    from->hide();
    qLinkWindow->show();
}

void Controller::showLoad(QWidget *from, QString text){
    from->hide();
    qLinkWindow->grabKeyboard();
    qLinkWindow->startFromFile(text);
    qLinkWindow->show();
}

void Controller::showMenu(QWidget *from){
    from->hide();
    qLinkWindow->releaseKeyboard();
    menuWindow->show();
}
