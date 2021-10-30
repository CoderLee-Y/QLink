#ifndef MENU_H
#define MENU_H

#include <QtWidgets>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include "enum.h"
#include "constant.h"

class menu : public QWidget
{
     Q_OBJECT
public:
    menu();

private:

    void initBtn(QVBoxLayout *layout);

private slots:

    void onClickSinglePlayer();

    void onClickDoublePlayer();

    void onClickLoad();

    void onClickExit();

signals:
    void showGame(QWidget *, gameMode_t);

    void showLoad(QWidget *);
};

#endif // MENU_H
