#include "role.h"

Role::Role(QLinkWindow *qLink, int xBound, int yBound,
           int x0, int y0, QVBoxLayout* sideBar): qLinkWindow(qLink),
    activated(nullptr), xIndex(x0), yIndex(y0), score(0),
    xBoundary(xBound), yBoundary(yBound),
    canRollBack(false), hasActivated(false), status(normal),
    roleStatusBar(sideBar)
{
    playerID = (x0 == 0 && y0 == 0);
    init();
}

void Role::init(){
    board1 = new statusUI(roleStatusBar, playerID);
    board1->setSolNum(qLinkWindow->isAnySol());
    board1->setTime(qLinkWindow->gameTime1);
    board1->setScore(score);
    qLinkWindow->setBlockStatus(xIndex, yIndex, PLAYER, 0);
}

/**
 * @brief Role::move
 * @param dir direction the role has to move
 * @return if move successfully, return UNDEF, if meet boundary
 * return the direction of boundary
 */
direction_t Role::move(direction_t dir){
    if(canRollBack){
        Sleep(10);
    }
    switch (dir) {
    case DIR_UP:{
        if(xIndex > 0){
            quickBackUp();
            --xIndex;
        }
        else{
            return dir;
        }
        break;
    }
    case DIR_DOWN:{
        if(xIndex < (xBoundary - 1)){
            quickBackUp();
            ++xIndex;
        }
        else{
            return dir;
        }
        break;
    }
    case DIR_LEFT:{
        if(yIndex > 0){
            quickBackUp();
            --yIndex;
        }
        else{
            return dir;
        }
        break;
    }
    case DIR_RIGHT:{
        if(yIndex < (yBoundary - 1)){
            quickBackUp();
            ++yIndex;
        }
        else{
            return dir;
        }
        break;
    }
    default:
        break;
    }
    return UNDEF;
}

void Role::plusScore(int score){
    this->score += score;
    board1->setScore(this->score);
}

void Role::getPosition(int &x, int &y){
    x = xIndex;
    y = yIndex;
}

void Role::quickBackUp(){
    xLastIndex = xIndex;
    yLastIndex = yIndex;
    canRollBack = true;
}

bool Role::rollback(){
    if(!canRollBack){
        return false;
    }
    else{
        canRollBack = false;
        xIndex = xLastIndex;
        yIndex = yLastIndex;
    }
    return true;
}

void Role::cancelBackup(){
    canRollBack = false;
}


void Role::setActivated(QBlock *act){
    activated = act;
    hasActivated = true;
}

QDataStream &operator<<(QDataStream &output, const Role &r){

    output << r.playerID << r.xIndex << r.yIndex \
           << r.activated->xIndex << r.activated->yIndex \
           << r.xBoundary << r.yBoundary \
           << r.xLastIndex << r.yLastIndex \
           << r.score << r.canRollBack << r.hasActivated \
           << r.status;

    return output;
}

/**
 * @brief operator >> no need constructor again
 * @param input
 * @param r
 * @return
 */
QDataStream &operator>>(QDataStream &input , Role &r){

    int x, y;
    input >> r.playerID >> r.xIndex >> r.yIndex \
           >> x >> y \
           >> r.xBoundary >> r.yBoundary \
           >> r.xLastIndex >> r.yLastIndex \
           >> r.score >> r.canRollBack >> r.hasActivated \
           >> r.status;

    if(r.hasActivated)
        r.activated = r.qLinkWindow->getBlock(x, y);

    // rebuild statusUI
    r.init();
    return input;
}
