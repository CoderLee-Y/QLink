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
#include "QLink.h"
#include "menu.h"

class controller: public QObject
{
    Q_OBJECT
public:
    controller();

    void show();

private:

    QLinkWindow *qLinkWindow;

    menu *menuWindow;

private slots:

    void showGame(QWidget *from, gameMode_t s);

    void showMenu(QWidget *from);

    void showLoad(QWidget *from);

private slots:

};

#endif // CONTROLLER_H
