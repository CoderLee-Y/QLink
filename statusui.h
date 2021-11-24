#ifndef STATUSUI_H
#define STATUSUI_H

#include <QLayout>
#include <QLabel>
#include "constant.h"

/**
 * @brief The statusUI class, side bar of my game
 */
class StatusUI {
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
    StatusUI(QVBoxLayout *hand, int num);

    void addText(const QString &str);

    void setInfo(const QString &info);

    void setSolNum(const int &s);

    void setTime(const int &s);

    void setScore(const int &score);

    void setFont(QLabel *q, const int &size);
};

#endif // STATUSUI_H
