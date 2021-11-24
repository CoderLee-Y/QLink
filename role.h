#ifndef ROLE_H
#define ROLE_H

#include "qblock.h"
#include "enum.h"
#include "statusui.h"

class QLinkWindow;

class Role {
    friend class QLinkWindow;

private:
    int playerID;

    int xIndex, yIndex;

    int xLastIndex, yLastIndex;

    QBlock *activated;

    QLinkWindow *qLinkWindow;

    int xBoundary, yBoundary;

    int score;

    bool canRollBack, hasActivated;

    role_status_t status;

    QVBoxLayout *roleStatusBar;

    StatusUI *board1;

private:
    void quickBackUp();

public:

    bool rollback();

    void cancelBackup();

    void init();

public:
    Role(QLinkWindow *, int xBound, int yBound,
         int x0 = 0, int y0 = 0, QVBoxLayout * = nullptr);

    direction_t move(const direction_t &dir);

    void plusScore(const int &score);

    void getPosition(int &, int &);

    void setActivated(QBlock *act);

    void setQL(QLinkWindow *a);

    friend QDataStream &operator<<(QDataStream &, const Role &);

    friend QDataStream &operator>>(QDataStream &, Role &);
};

#endif // ROLE_H
