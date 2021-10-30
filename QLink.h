#ifndef QLINK_H
#define QLINK_H

#include <QtWidgets>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include "qblock.h"
#include "enum.h"
#include "role.h"
#include "statusui.h"
#include "linemask.h"

struct BLOCK_STRUCT {

    block_t type;

    QColor color;

    int group;

    QBlock *block;

};

class Role;

class QLinkWindow : public QWidget {
    Q_OBJECT

    friend class Role;

// for layout and sub-widget
private:
    QWidget window;

    QGridLayout *layout;

    QVBoxLayout *rightBarLayout, *leftBarLayout;

// meta data to store
private:
    // Not suitable name: vertical and horizanal
    // blocks number here
    int windowLength, windowHeight;

    QVector<QVector<BLOCK_STRUCT>> blockMap;

    QVector<QColor> colorSet;

    QVector<QColor> wordColorSet;

    Role *role1, *role2;

    int blockType;

    gameStatus_t globalStatus;

    gameMode_t gameMode;

    int gameTime1, hintTime1;

    lineMask *lines;

    QQueue<int> linesOnBoard;

    QQueue<QTimer*> timerList;

    QList<direction_t> keyPressed;

    QVector<QVector < QBlock * >> refAnswers;

    QVector<int> linesOfHint;

    int isInHint, hintTime;

    QVector<Role *> roles;

// necessary timer
private:

    QTimer *keyPressedTimer;

    QTimer *PlayerTimer;

    QTimer *HintTimer;

// procedure
private:

    void setColorSet();

    void initBlocks();

    void initTimers();

    void handleConflictBlocks();

    void drawBar();

    int isAnySol();

    void addProps();

    void dropProps();

    void handleHint();

    void handleReshuffle();

    void handlePlusSecond();

    void updateHint();

    void pauseGame();

    void startGame();

    void endGame();

    void stopHint();

// function
private:

    movingAction_t movingAction(Role *role, int &, int &);

    void setBlockStatus(int i,int j, block_t status, int group);

    void moveRole(int roleID, direction_t dir);

    bool isLeagalElimate(Role *, int x1, int y1, int x2,
                         int y2, bool changeInfo, QVector < QBlock * > &paths);

    QVector<direction_t> AStarFindDirection(int x1, int y1, int x2, int y2);

    int findLine(int x1, int y1, int x2, int y2, direction_t past_dir,
                 QVector<QBlock*> &path);

    bool isLegalPoint(int x, int y);

    bool canReachBorder(int x, int y, QMap<QPair<int, int>, bool> vis);

    void handleProps(prop_t prop);

    void shuffleBlocks(int);

    int drawAnswers(QVector < QBlock * > &);

    void setTime4Line(int lineNum, int msec);

    QLine getLine(QBlock *, QBlock *);

public:

    QLinkWindow(QWidget *parent = nullptr, int length = 10, int height = 10);

    void setGameMode(gameMode_t s);

// event monitor function here
protected:

    void keyPressEvent(QKeyEvent *event) override;

// handler function here
private slots:

    void handleKeyPressed();

    void handlePlayerTimer();

    void handleHintTimer();

    void handleLineTimer();

signals:

    void goToMenu(QWidget *);

};

#endif // QLINK_H
