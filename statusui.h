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

    void initSideBar();

    QLabel *info_c;

    QLabel *score_c;

    QLabel *time_c;

    QLabel *prop_time_c1;

    QLabel *prop_time_c2;

    QLabel *sol_num_c;

public:
    statusUI(QVBoxLayout *hand);

    void addText(QString str);

    void setInfo(QString info);

    void setSolNum(int s);

    void setTime(int s);
};

#endif // STATUSUI_H
