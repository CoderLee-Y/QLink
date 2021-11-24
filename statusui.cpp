#include "statusui.h"

StatusUI::StatusUI(QVBoxLayout *hand, int num) : playerNum(num) {
    layout = hand;
    initSideBar();

    layout->setAlignment(Qt::AlignCenter);
    layout->update();
}

void StatusUI::initSideBar() {
    QLabel *playerInfo = new QLabel();
    if (playerNum == 1)
        playerInfo->setText(PLAYER1MSG);
    else
        playerInfo->setText(PLAYER2MSG);
    playerInfo->setFixedSize(175, 30);
    setFont(playerInfo, 16);
    layout->addWidget(playerInfo);

    info_c = new QLabel();
    info_c->setText(INITINFOMSG);
    info_c->setFixedSize(175, 120);
    setFont(info_c, 20);
    layout->addWidget(info_c);

    QLabel *score = new QLabel();
    score->setText(SCOREMSG);
    score->setFixedSize(175, 30);
    setFont(score, 16);
    layout->addWidget(score);

    score_t = new QLabel();
    setFont(score_t, 16);
    score_t->setFixedSize(175, 30);
    layout->addWidget(score_t);

    QLabel *sol_num = new QLabel();
    sol_num->setText(SOL_NUM_MSG);
    sol_num->setFixedSize(175, 30);
    setFont(sol_num, 16);
    layout->addWidget(sol_num);

    sol_num_c = new QLabel();
    sol_num_c->setText("0");
    setFont(sol_num_c, 16);
    sol_num_c->setFixedSize(175, 30);
    layout->addWidget(sol_num_c);

    QLabel *time_l = new QLabel();
    time_l->setText(TIMEMSG);
    setFont(time_l, 16);
    time_l->setFixedSize(175, 30);
    layout->addWidget(time_l);

    time_c = new QLabel();
    setFont(time_c, 16);
    layout->addWidget(time_c);

}

void StatusUI::addText(const QString &str) {
    QLabel *text = new QLabel;
    text->setText(str);
    layout->update();
}

void StatusUI::setInfo(const QString &info) {
    info_c->setText(info);
    layout->update();
}

void StatusUI::setSolNum(const int &s) {
    QString sp;
    sp.setNum(s);
    sol_num_c->setText(sp);
    layout->update();
}

void StatusUI::setTime(const int &s) {
    QString sp;
    sp.setNum(s);
    time_c->setText(sp);
    layout->update();
}

void StatusUI::setFont(QLabel *q, const int &size) {
    QFont qf;
    qf.setBold(true);
    qf.setFamily("Microsoft YaHei");
    qf.setPixelSize(size);

    q->setFont(qf);
}

void StatusUI::setScore(const int &score) {
    QString sp;
    sp.setNum(score);
    score_t->setText(sp);
    layout->update();
}
