#ifndef STATUSUI_H
#define STATUSUI_H

#include <QLayout>
#include <QLabel>
#include "constant.h"

/**
 * @brief The statusUI class, side bar of my game
 */
class statusUI
{
private:
    QVBoxLayout *layout;

    QVector<QVBoxLayout *> parts;

    int playerNum;

    void initSideBar();

    QLabel *info_c;

    QLabel *score_c;

    QLabel *time_c;

    QLabel *prop_time_c1;

    QLabel *prop_time_c2;

    QLabel *sol_num_c;

    QLabel *score_t;

public:
    statusUI(QVBoxLayout *hand, int num);

    void addText(QString str);

    void setInfo(QString info);

    void setSolNum(int s);

    void setTime(int s);

    void setScore(int score);

    void setFont(QLabel *q, int size);
};

#endif // STATUSUI_H
