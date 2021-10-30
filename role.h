#ifndef ROLE_H
#define ROLE_H

#include "qblock.h"
#include "QLink.h"
#include "enum.h"

class QLinkWindow;

class Role
{
friend class QLinkWindow;
private:
    int playerID;

    int xIndex, yIndex;

    int xLastIndex, yLastIndex;

    QBlock *activated;

    int xBoundary, yBoundary;

    int score;

    bool canRollBack, hasActivated;

    role_status_t status;

private:
    void quickBackUp();

public:
    Role(int xBound, int yBound,
            int x0 = 0, int y0 = 0);

    direction_t move(direction_t dir);

    void plusScore(int score);

    void getPosition(int &, int &);

    bool rollback();

    void cancelBackup();

    void setActivated(QBlock *act);


};

#endif // ROLE_H
