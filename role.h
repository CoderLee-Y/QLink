#ifndef ROLE_H
#define ROLE_H

#include "qblock.h"
#include "QLink.h"
#include "enum.h"
#include "statusui.h"

class QLinkWindow;

class Role
{
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

    QVBoxLayout* roleStatusBar;

    statusUI *board1;

private:
    void quickBackUp();

public:
    Role(QLinkWindow *, int xBound, int yBound,
            int x0 = 0, int y0 = 0, QVBoxLayout* = nullptr);

    direction_t move(direction_t dir);

    void plusScore(int score);

    void getPosition(int &, int &);

    bool rollback();

    void cancelBackup();

    void setActivated(QBlock *act);
};

#endif // ROLE_H
