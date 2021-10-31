#include "statusui.h"

statusUI::statusUI(QVBoxLayout *hand)
{
    layout = hand;
    initSideBar();

    layout->setAlignment(Qt::AlignCenter);
    layout->update();
}

void statusUI::initSideBar(){
    QLabel *playerInfo = new QLabel();
    playerInfo->setText(PLAYER1MSG);
    playerInfo->setFixedSize(175, 30);
    layout->addWidget(playerInfo);

    info_c = new QLabel();
    info_c->setText(INITINFOMSG);
    info_c->setFixedSize(175, 120);
    QFont qf;
    qf.setBold(true);
    qf.setFamily("Microsoft YaHei");
    qf.setPixelSize(20);

    info_c->setFont(qf);
    layout->addWidget(info_c);

    QLabel *score = new QLabel();
    score->setText(SCOREMSG);
    score->setFixedSize(175, 30);
    layout->addWidget(score);

    QLabel *sol_num = new QLabel();
    sol_num->setText(SOL_NUM_MSG);
    sol_num->setFixedSize(175, 30);
    layout->addWidget(sol_num);

    sol_num_c = new QLabel();
    sol_num_c->setText("0");
    sol_num_c->setFixedSize(175, 30);
    layout->addWidget(sol_num_c);

    QLabel *time_l = new QLabel();
    time_l->setText(TIMEMSG);
    time_l->setFixedSize(175, 30);
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

