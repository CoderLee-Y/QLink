#ifndef QLINK_H
#define QLINK_H

#include <QtWidgets>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include <QDataStream>
#include "qblock.h"
#include "enum.h"
#include "statusui.h"
#include "role.h"
#include "linemask.h"

class BLOCK_STRUCT {

public:

    block_t type;

    QColor color;

    int group;

    QBlock *block;

    friend QDataStream &operator<<(QDataStream &, const BLOCK_STRUCT &);

    friend QDataStream &operator>>(QDataStream &, BLOCK_STRUCT &);

};

class Role;

class QLinkWindow : public QWidget {
    Q_OBJECT

    friend class Role;

private:
    QWidget window;

    QGridLayout *gridLayout;

    QVBoxLayout *rightBarLayout, *leftBarLayout;

// meta data to store
private:
    // Not suitable name: vertical and horizanal
    // blocks number here
    int windowLength, windowHeight;

    QVector <QVector<BLOCK_STRUCT>> blockMap;

    QVector <QColor> colorSet;

    QVector <QColor> wordColorSet;

    Role *role1, *role2;

    int blockType;

    game_status_t globalStatus;

    game_mode_t gameMode;

    int gameTime1, hintTime1;

    LineMask *lines;

    QQueue<int> linesOnBoard;

    QQueue<QTimer *> timerList;

    QList <direction_t> keyPressed;

    QVector <QVector<QBlock * >> refAnswers;

    QVector<int> linesOfHint;

    int isInHint, hintTime;


// necessary timer
private:

    QTimer *keyPressedTimer;

    QTimer *PlayerTimer;

    QTimer *HintTimer;

// procedure
private:

    void setColorSet();

    void dataClear();

    void initBlocks();

    void initTimers();

    void handleConflictBlocks();

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

    void removePlayer();

    void inputName();

    void clearAllAndRebuid();

// function
private:

    moving_action_t movingAction(Role *role, int &, int &);

    void setBlockStatus(const int &i, const int &j,
                        const block_t &status, const int &group);

    void moveRole(const int &roleID, const direction_t &dir);

    QVector <direction_t> AStarFindDirection(const int &x1, const int &y1,
                                             const int &x2, const int &y2);

    int findLine(const int &x1, const int &y1, const int &x2, const int &y2,
                 const direction_t &past_dir,
                 QVector<QBlock *> &path);

    bool isLegalPoint(const int &x, const int &y);

    bool canReachBorder(const int &x, const int &y,
                        QMap<QPair<int, int>, bool> vis);

    void handleProps(const prop_t &prop);

    void shuffleBlocks(const int&);

    int drawAnswers(QVector<QBlock *> &);

    void setTime4Line(const int &lineNum, const int &msec);

    QLine getLine(const QBlock *, const QBlock *);

    void removeAllFrom(QLayout *layout);

    bool load2Disk(QString name);

    bool loadFromDisk(QString name);

    void setLayoutRebuild(const game_mode_t &, const int &, const int &);

public:

    QLinkWindow(QWidget *parent = nullptr, int length = 10, int height = 10);

    void setGameMode(const game_mode_t &s);

    QBlock *getBlock(const int &x, const int &y);

    void startFromFile(QString fileName);

    void setBlock(const int &x, const int &y, const block_t &t,
                  const int &group);

// event monitor function here
protected:

    void keyPressEvent(QKeyEvent *event) override;

// handler function here
private
    slots:

    void handleKeyPressed();

    void handlePlayerTimer();

    void handleHintTimer();

    void handleLineTimer();

    signals:

    void goToMenu(QWidget * );

public:

    friend QDataStream &operator<<(QDataStream &, const QLinkWindow &);

    friend QDataStream &operator>>(QDataStream &, QLinkWindow &);

// have to be public because of unit test
public:

    QVector<Role *> roles;

    // Just public for test
    bool isLeagalElimate(const Role *, const int &x1, const int &y1, const int &x2,
                         const int &y2, const bool &changeInfo, QVector<QBlock *> &paths);
};


#endif // QLINK_H
