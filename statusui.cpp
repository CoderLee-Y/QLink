#include "statusui.h"

statusUI::statusUI(QVBoxLayout *hand)
{
    layout = hand;
    initSideBar();
    layout->setMargin(5);
    layout->setAlignment(Qt::AlignCenter);
    layout->update();
}

void statusUI::initSideBar(){
    QLabel *playerInfo = new QLabel();
    playerInfo->setText(PLAYER1MSG);
    layout->addWidget(playerInfo);

    QLabel *info = new QLabel();
    info->setText(INFOMSG);
    layout->addWidget(info);

    info_c = new QLabel();
    info_c->setText(INITINFOMSG);
    layout->addWidget(info_c);

    QLabel *score = new QLabel();
    score->setText(SCOREMSG);
    layout->addWidget(score);

    QLabel *sol_num = new QLabel();
    sol_num->setText(SOL_NUM_MSG);
    layout->addWidget(sol_num);

    sol_num_c = new QLabel();
    sol_num_c->setText("0");
    layout->addWidget(sol_num_c);

    QLabel *time_l = new QLabel();
    time_l->setText(TIMEMSG);
    layout->addWidget(time_l);

    time_c = new QLabel();
    layout->addWidget(time_c);
}


void statusUI::addText(QString str){
    QLabel *text = new QLabel;
    text->setText(str);
    layout->update();
}

void statusUI::setInfo(QString info){
    info_c->setText(info);
    layout->update();
}

void statusUI::setSolNum(int s){
    QString sp;
    sp.setNum(s);
    sol_num_c->setText(sp);
    layout->update();
}

void statusUI::setTime(int s){
    QString sp;
    sp.setNum(s);
    time_c->setText(sp);
    layout->update();
}

