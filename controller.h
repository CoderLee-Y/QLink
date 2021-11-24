#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtWidgets>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include "enum.h"
#include "qlink.h"
#include "menu.h"

class Controller : public QObject {
    Q_OBJECT
public:
    Controller();

    void show();

private:

    QLinkWindow *qLinkWindow;

    Menu *menuWindow;

private
    slots:

    void showGame(QWidget * from, game_mode_t s);

    void showMenu(QWidget *from);

    void showLoad(QWidget *from, QString text);

private
    slots:

};

#endif // CONTROLLER_H
