#include "QLink.h"

/**
 * @brief QLinkWindow::QLinkWindow
 * @param parent
 * @param length number of blocks
 * @param height default 10 by 10
 */
QLinkWindow::QLinkWindow(QWidget *parent, int length, int height) :
        QWidget(parent), isInHint(0),
        keyPressedTimer(new QTimer(this)),
        PlayerTimer(new QTimer(this)), HintTimer(new QTimer(this)) {

    this->windowLength = length;
    this->windowHeight = height;
    blockType = std::min((int) (length * height / 10), 10);

    setColorSet();
    initTimers();

    globalStatus = unstarted;
}

void QLinkWindow::removePlayer() {
    for (int i = 0; i < windowHeight; ++i)
        for (int j = 0; j < windowLength; ++j) {
            if (blockMap[i][j].type == PLAYER)
                setBlockStatus(i, j, EMPTY, 0);
        }

    for (auto &m: roles) {
        delete m;
    }
    roles.clear();
}

void QLinkWindow::dataClear() {
    linesOfHint.clear();
    keyPressed.clear();
    timerList.clear();
    linesOnBoard.clear();
    hintTime1 = 0;
    isInHint = false;
    refAnswers.clear();
    roles.clear();
}

void QLinkWindow::setGameMode(gameMode_t s) {
    // not elegant at all!
    dataClear();
    if (globalStatus != unstarted) {
        removePlayer();
        removeAllFrom(gridLayout);
        removeAllFrom(rightBarLayout);
        if (gameMode == twoPlayer)
            removeAllFrom(leftBarLayout);
        removeAllFrom(this->layout());
    }
    rightBarLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();

    initBlocks();
    handleConflictBlocks();

    keyPressedTimer->start();
    gameTime1 = 10;

    QHBoxLayout *basicLayout;
    basicLayout = globalStatus == unstarted ?
                  new QHBoxLayout() : (QHBoxLayout * )
    this->layout();

    lines = new lineMask();
    lines->setFixedHeight(WINDOWHEIGHT);

    for (int i = 0; i < windowHeight; ++i)
        for (int j = 0; j < windowLength; ++j)
            gridLayout->addWidget(blockMap[i][j].block, i, j, 1, 1, Qt::AlignCenter);

    gridLayout->addWidget(lines, 0, 0, -1, -1);
    if (s == twoPlayer) {
        leftBarLayout = new QVBoxLayout();
        basicLayout->addLayout(leftBarLayout);
    }

    basicLayout->addLayout(gridLayout);
    basicLayout->addLayout(rightBarLayout);

    if (globalStatus == unstarted) {
        this->setLayout(basicLayout);
        this->setWindowTitle(WINDOWTITLE);
    }

    role1 = new Role(this, windowLength, windowHeight, 0, 0, rightBarLayout);
    roles.push_back(role1);
    if (s == twoPlayer) {
        role2 = new Role(this, windowLength, windowHeight,
                         windowLength - 1, windowHeight - 1, leftBarLayout);
        roles.push_back(role2);
    }

    for (auto &m: roles) {
        m->board1->setSolNum(isAnySol());
    }
    globalStatus = unstarted;
    gameMode = s;
}

void QLinkWindow::removeAllFrom(QLayout *layout) {
    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void QLinkWindow::initTimers() {
    keyPressedTimer->setInterval(20);
    PlayerTimer->setInterval(1000);
    HintTimer->setInterval(1000);

    connect(keyPressedTimer, &QTimer::timeout,
            this, &QLinkWindow::handleKeyPressed);
    connect(PlayerTimer, &QTimer::timeout,
            this, &QLinkWindow::handlePlayerTimer);
    connect(HintTimer, &QTimer::timeout,
            this, &QLinkWindow::handleHintTimer);
    this->grabKeyboard();
}

/**
 * @brief QLinkWindow::stopHint
 * stop hint timer and remove hint from line mask
 */
void QLinkWindow::stopHint() {
    HintTimer->stop();
    for (auto &m: roles) {
        m->board1->setInfo(HINT_END);
    }
    for (auto &lineNum: linesOfHint) {
        lines->removeLines(lineNum);
    }
    lines->update();
    isInHint = false;
}

/**
 * @brief QLinkWindow::handleHintTimer
 * handle Hint Timer
 */
void QLinkWindow::handleHintTimer() {
    --hintTime;
    if (hintTime == 0)
        stopHint();
}

/**
 * @brief QLinkWindow::handleConflictBlocks
 * make sure no same group block beside
 * // BUG: if grp number <= 3?
 */
void QLinkWindow::handleConflictBlocks() {
    for (int i = 1; i < windowHeight - 1; ++i) {
        for (int j = 1; j < windowLength - 1; ++j) {
            int curr = blockMap[i][j].group;
            while (!((curr == 0) ||
                     (blockMap[i - 1][j].group != curr &&
                      blockMap[i + 1][j].group != curr &&
                      blockMap[i][j + 1].group != curr &&
                      blockMap[i][j - 1].group != curr))) {
                curr = (curr + 1) % (blockType + 1);
                curr += (curr == 0 && blockMap[i][j].type != EMPTY) ? 1 : 0;
                setBlockStatus(i, j, blockMap[i][j].type,
                               curr);
            }
        }
    }
}

/**
 * @brief QLinkWindow::handleReshuffle
 * reshuffle props, with feature that won't stuck player at any moment
 */
void QLinkWindow::handleReshuffle() {
    int tol = 0;
    for (int i = 1; i < windowHeight - 1; ++i)
        for (int j = 1; j < windowLength - 1; ++j) {
            if (blockMap[i][j].type == OCCUPIED ||
                blockMap[i][j].type == ACTIVATED)
                ++tol;
        }
    bool isPlayerStuck = true;
    do {
        shuffleBlocks(tol);
        handleConflictBlocks();
        for (int i = 0; i < windowHeight; ++i)
            for (int j = 0; j < windowLength; ++j) {
                if (blockMap[i][j].type == PLAYER) {
                    QMap<QPair<int, int>, bool> vis;
                    isPlayerStuck = !canReachBorder(i, j, vis);
                    goto out;
                }
            }
        out:
        for (auto &m: roles) {
            m->board1->setSolNum(isAnySol());
        }
    } while (isPlayerStuck);

    for (int i = 1; i < windowHeight - 1; ++i)
        for (int j = 1; j < windowLength - 1; ++j) {
            if (blockMap[i][j].type == ACTIVATED)
                setBlockStatus(i, j, EMPTY, 0);
        }
}

/**
 * @brief QLinkWindow::setBlockStatus
 * @param i index of block
 * @param j
 * @param status : to be set
 * @param group : to be set, NOTE: if grp==0, seen empty
 */
void QLinkWindow::setBlockStatus(int i, int j, block_t status, int group) {
    blockMap[i][j].type = status;
    blockMap[i][j].group = group;
    blockMap[i][j].color = colorSet[group];
    blockMap[i][j].block->setColor(colorSet[group], wordColorSet[group]);
    blockMap[i][j].block->setStatus(status);
}

/**
 * @brief QLinkWindow::initBlocks init block status, color to draw init status
 * @param length how many blocks in horizantal orientation
 * @param height vertical?
 */
void QLinkWindow::initBlocks() {
    blockMap.resize(windowHeight);
    for (int i = 0; i < windowHeight; ++i) {
        blockMap[i].resize(windowLength);
        for (int j = 0; j < windowLength; ++j) {
            blockMap[i][j].block = new QBlock(windowLength, windowHeight, this);
        }
    }
    shuffleBlocks(-1);
}

void QLinkWindow::shuffleBlocks(int concret_num) {
    int tol = 0, threshold = 90;
    if (concret_num != -1) {
        threshold = ((double) concret_num / ((windowHeight - 2) * (windowLength - 2))) * 100;
    }
    for (int i = 0; i < windowHeight; ++i) {
        for (int j = 0; j < windowLength; ++j) {
            if (blockMap[i][j].type != PLAYER) {
                blockMap[i][j].type = EMPTY;
                blockMap[i][j].group = 0;
                blockMap[i][j].color = colorSet[0];
                blockMap[i][j].block->setIndex(i, j);
            }
            blockMap[i][j].block->setColor(blockMap[i][j].color, wordColorSet[blockMap[i][j].group]);
            blockMap[i][j].block->setStatus(blockMap[i][j].type);
        }
    }

    shuffle:
    for (int i = 1; i < windowHeight - 1; ++i) {
        for (int j = 1; j < windowLength - 1; ++j) {
            if (blockMap[i][j].type == EMPTY)
                // 90% not empty in central part
            {
                if (QRandomGenerator::global()->bounded(0, 100) < threshold) {
                    blockMap[i][j].type = OCCUPIED;
                    blockMap[i][j].group = QRandomGenerator::global()->
                            bounded(1, blockType + 1);
                    blockMap[i][j].color = colorSet[blockMap[i][j].group];
                    ++tol;
                    blockMap[i][j].block->setColor(blockMap[i][j].color, wordColorSet[blockMap[i][j].group]);
                    blockMap[i][j].block->setStatus(blockMap[i][j].type);
                    if (concret_num != -1 && concret_num == tol)
                        return;
                }
            }
        }
    }
    if (concret_num > tol) {
        goto shuffle;
    }
}

/**
 * @brief QLinkWindow::setColorSet
 * set bg color randomly and word color according to bg color
 */
void QLinkWindow::setColorSet() {
    wordColorSet.resize(blockType + 1);
    colorSet.resize(blockType + 1);

    wordColorSet[0] = QColor(0, 0, 0);
    colorSet[0] = QColor(255, 255, 255);

    for (int i = 1; i <= blockType; ++i) {
        colorSet[i] = QColor(QRandomGenerator::global()->bounded(4, 220),
                             QRandomGenerator::global()->bounded(4, 220),
                             QRandomGenerator::global()->bounded(4, 220));

        double luminance = (0.299 * colorSet[i].red() +
                            0.587 * colorSet[i].green() +
                            0.114 * colorSet[i].blue()) / 255;
        if (luminance > 0.5) {
            wordColorSet[i] = QColor(0, 0, 0);
        } else {
            wordColorSet[i] = QColor(255, 255, 255);
        }
    }

}

/**
 * @brief QLinkWindow::keyPressEvent
 * handle all key pressed event here
 * @param event
 */
void QLinkWindow::keyPressEvent(QKeyEvent *event) {
    switch (globalStatus) {
        case unstarted: {
            if (event->key() == Qt::Key_Enter) {
                startGame();
            }
            break;
        }
        case running: {
            switch (event->key()) {
                case Qt::Key_S: {
                    keyPressed.push_back(DIR_DOWN);
                    break;
                }
                case Qt::Key_A: {
                    keyPressed.push_back(DIR_LEFT);
                    break;
                }
                case Qt::Key_W: {
                    keyPressed.push_back(DIR_UP);
                    break;
                }
                case Qt::Key_D: {
                    keyPressed.push_back(DIR_RIGHT);
                    break;
                }
                case Qt::Key_Escape: {
                    pauseGame();
                    break;
                }
                case Qt::Key_Down: {
                    keyPressed.push_back(DIR_DOWN);
                    keyPressed.push_back(UNDEF);
                    break;
                }
                case Qt::Key_Left: {
                    keyPressed.push_back(DIR_LEFT);
                    keyPressed.push_back(UNDEF);
                    break;
                }
                case Qt::Key_Up: {
                    keyPressed.push_back(DIR_UP);
                    keyPressed.push_back(UNDEF);
                    break;
                }
                case Qt::Key_Right: {
                    keyPressed.push_back(DIR_RIGHT);
                    keyPressed.push_back(UNDEF);
                    break;
                }
            }
            break;
        }
        case paused: {
            if (event->key() == Qt::Key_Enter) {
                startGame();
            }
            break;
        }
        case gameOver: {
        }
    }
}

void QLinkWindow::endGame() {
    keyPressedTimer->stop();
    QMessageBox::information(this, "QLink", "Game over.");
    globalStatus = gameOver;
    PlayerTimer->stop();

    if (isInHint) {
        HintTimer->stop();
        for (auto &lineNum: linesOfHint) {
            lines->removeLines(lineNum);
        }
        lines->update();
        isInHint = false;
    }
    for (auto &m: roles) {
        m->board1->setInfo(GAME_OVER);
    }
    emit goToMenu(this);
}

void QLinkWindow::pauseGame() {
    globalStatus = paused;
    PlayerTimer->stop();
    if (isInHint) {
        HintTimer->stop();
    }
    for (auto &m: roles) {
        m->board1->setInfo(PAUSE);
    }
}

void QLinkWindow::startGame() {
    globalStatus = running;
    PlayerTimer->start();
    if (isInHint) {
        HintTimer->start();
    }
    for (auto &m: roles) {
        m->board1->setInfo(START);
    }
}

/**
 * @brief QLinkWindow::moveRole
 * @param roleID
 * @param dir
 * Main logic of moving a role
 */
void QLinkWindow::moveRole(int roleID, direction_t dir) {
    Role *role = (roleID == 1) ? role1 : role2;
    if (role->move(dir) != UNDEF) {
        return;
    } else {
        int roleX, roleY;
        movingAction(role, roleX, roleY);
    }
}

movingAction_t QLinkWindow::movingAction(Role *role, int &x, int &y) {
    role->getPosition(x, y);
    QVector < QBlock * > noUse;
    if (blockMap[x][y].type == EMPTY) {
        role->cancelBackup();
        setBlockStatus(x, y, PLAYER, blockMap[x][y].group);
        setBlockStatus(role->xLastIndex, role->yLastIndex, EMPTY, 0);
    } else if (blockMap[x][y].type == PLAYER) {
        role->rollback();
        return PLAYEROCCUPIED;
    } else if (blockMap[x][y].type == OCCUPIED) {
        if (role->hasActivated) {
            int xActivated = role->activated->xIndex;
            int yActivated = role->activated->yIndex;
            role->rollback();

            setBlockStatus(x, y, ACTIVATED, blockMap[x][y].group);
            if (isLeagalElimate(role, xActivated, yActivated, x, y, true, noUse)) {
                setBlockStatus(x, y, EMPTY, 0);
                setBlockStatus(xActivated, yActivated, EMPTY, 0);
                role->plusScore(10);
                role->hasActivated = false;
                for (auto &m: roles) {
                    m->board1->setSolNum(isAnySol());
                }
                return ELIMATE;
            } else {
                setBlockStatus(xActivated, yActivated, OCCUPIED,
                               blockMap[xActivated][yActivated].group);
                setBlockStatus(x, y, ACTIVATED, blockMap[x][y].group);
                role->setActivated(blockMap[x][y].block);
                return REPLACE;
            }
        } else {
            role->setActivated(blockMap[x][y].block);
            setBlockStatus(x, y, ACTIVATED,
                           blockMap[x][y].group);
            role->rollback();
            return ACTIVATE;
        }
    } else if (blockMap[x][y].type == ACTIVATED) {
        role->rollback();
        return ACTIVATE;
    } else if (blockMap[x][y].type == PROPS) {
        role->cancelBackup();
        setBlockStatus(x, y, PLAYER, blockMap[x][y].group);
        setBlockStatus(role->xLastIndex, role->yLastIndex,
                       EMPTY, 0);
        handleProps(blockMap[x][y].block->prop_type);
    }
    return NORMAL;
}

void QLinkWindow::handleProps(prop_t prop) {
    switch (prop) {
        case plusSec: {
            handlePlusSecond();
            break;
        }
        case reshuffle: {
            handleReshuffle();
            break;
        }
        case hint:
            handleHint();
    }
}

void QLinkWindow::handleHint() {
    if (isInHint) {
        hintTime += 10;
        for (auto &m: roles) {
            m->board1->setInfo(HINT_ADDED);
        }
    } else {
        isInHint = true;
        hintTime = 10;
        for (auto &m: roles) {
            m->board1->setInfo(HINT_BEGIN);
        }
        updateHint();
        HintTimer->start();
    }
}

void QLinkWindow::updateHint() {
    for (auto &lineNum: linesOfHint) {
        lines->removeLines(lineNum);
    }
    lines->update();
    linesOfHint.clear();

    for (auto &m: refAnswers) {
        int lineNum = drawAnswers(m);
        linesOfHint.push_back(lineNum);
    }

}

void QLinkWindow::handlePlusSecond() {
    gameTime1 += 30;
    for (auto &m: roles) {
        m->board1->setTime(gameTime1);
    }
}

void QLinkWindow::handleKeyPressed() {
    while (!keyPressed.empty()) {
        direction_t dir = keyPressed.front();
        keyPressed.pop_front();
        if (!keyPressed.empty() && keyPressed.front() == UNDEF) {
            keyPressed.pop_front();
            if (gameMode == twoPlayer)
                moveRole(2, dir);
        } else {
            moveRole(1, dir);
        }
    }
    this->update();
}

/**
 * @brief QLinkWindow::handlePlayerTimer
 * count time for player
 */
void QLinkWindow::handlePlayerTimer() {
    --gameTime1;
    for (auto &m: roles) {
        m->board1->setTime(gameTime1);
    }
    if (gameTime1 == 0) {
        endGame();
    }

    dropProps();
    addProps();
}

/**
 * @brief QLinkWindow::dropProps
 * elimate the prop for every 5 secs (in avg)
 */
void QLinkWindow::dropProps() {
    for (int i = 0; i < windowHeight; ++i)
        for (int j = 0; j < windowLength; ++j) {
            if (blockMap[i][j].type == PROPS) {
                int shouldDrop = QRandomGenerator::global()->bounded(0, 100);
                if (shouldDrop < 20) {
                    setBlockStatus(i, j, EMPTY, 0);
                }
            }
        }
}

/**
 * @brief QLinkWindow::addProps
 * modify probability here
 */
void QLinkWindow::addProps() {
    for (int i = 0; i < windowHeight; ++i)
        for (int j = 0; j < windowLength; ++j) {
            QMap<QPair<int, int>, bool> tmp;
            if (blockMap[i][j].type == EMPTY &&
                canReachBorder(i, j, tmp)) {
                int whichProp = QRandomGenerator::global()->bounded(0, 1000);
                prop_t prop_to_use;
                if (whichProp > 10)
                    continue;
                else if (whichProp > 5) {
                    prop_to_use = plusSec;
                } else if (whichProp > 3) {
                    prop_to_use = reshuffle;
                } else {
                    prop_to_use = hint;
                }
                blockMap[i][j].type = PROPS;
                blockMap[i][j].block->status = PROPS;
                blockMap[i][j].block->prop_type = prop_to_use;
                this->update();
            }
        }
}

/**
 * @brief QLinkWindow::isLeagalElimate
 * @param x1 y1 start point
 * @param x2 y2 end point
 * @param changeInfo if true, will change info board at the same time
 * @return return is these two point a pair of legal elimate.
 */
bool QLinkWindow::isLeagalElimate(Role *who, int x1, int y1, int x2, int y2,
                                  bool changeInfo, QVector<QBlock *> &path) {
    int ret;
    path.clear();
    if (blockMap[x1][y1].group == blockMap[x2][y2].group) {
        // change legal elimate method: find in list
        ret = findLine(x1, y1, x2, y2, UNDEF, path);
        if (changeInfo) {
            if (ret == 1) {
                setTime4Line(drawAnswers(path), 500);
                who->board1->setInfo(ELIMATE_SUCCESS);
            } else
                who->board1->setInfo(OVER_BROKEN_LINE);
        }
    } else {
        ret = false;
        if (changeInfo)
            who->board1->setInfo(NOT_SAME_GRP);
    }

    return ret;
}

void QLinkWindow::setTime4Line(int lineNum, int msec) {
    linesOnBoard.push_back(lineNum);
    QTimer *tmpTimer = new QTimer(this);
    tmpTimer->setInterval(msec);

    timerList.push_back(tmpTimer);
    connect(tmpTimer, &QTimer::timeout,
            this, &QLinkWindow::handleLineTimer);
    tmpTimer->start();
}

void QLinkWindow::handleLineTimer() {
    auto s = timerList.front();
    s->stop();
    timerList.pop_front();

    auto lineNum = linesOnBoard.front();
    lines->removeLines(lineNum);
    linesOnBoard.pop_front();
}

/**
 * @brief QLinkWindow::drawAnswers
 * @param path link all points in path to draw answer
 */
int QLinkWindow::drawAnswers(QVector<QBlock *> &path) {
    QVector <QLine> toDraw;
    int s = path.size();
    for (int i = 0; i < s - 1; ++i) {
        toDraw.push_back(getLine(path[i], path[i + 1]));
    }

    int lineNum = lines->addLines(toDraw);
    lines->update();
    return lineNum;
}

QLine QLinkWindow::getLine(QBlock *a, QBlock *b) {
    auto itemA = gridLayout->itemAtPosition(a->xIndex, a->yIndex);
    auto itemB = gridLayout->itemAtPosition(b->xIndex, b->yIndex);

    QLine ret(itemA->geometry().x(), itemA->geometry().y(),
              itemB->geometry().x(), itemB->geometry().y());
    if (gameMode == twoPlayer) {
        ret.setLine(itemA->geometry().x() - 175, itemA->geometry().y(),
                    itemB->geometry().x() - 175, itemB->geometry().y());
    }
    return ret;
}

/**
 * @brief QLinkWindow::isLegalPoint
 * @param x,y point requested
 * @return is it in legal area
 */
bool QLinkWindow::isLegalPoint(int x, int y) {
    return (x >= 0 && x < windowHeight && y >= 0 && y < windowLength);
}

/**
 * @brief QLinkWindow::AStarFindDirection
 * @param x1
 * @param y1 stat point
 * @param x2 user asserts x1 != x2 || y1 != y2
 * @param y2 end point
 * @return a vector whose len is 4, dfs should use dirs
 */
QVector <direction_t> QLinkWindow::AStarFindDirection(int x1, int y1,
                                                      int x2, int y2) {
    assert(x1 != x2 || y1 != y2);
    QVector <direction_t> ret;
    if (x1 == x2) {
        if (y1 > y2) {
            ret.push_back(DIR_LEFT);
        } else {
            ret.push_back(DIR_RIGHT);
        }
        ret.push_back(DIR_UP);
        ret.push_back(DIR_DOWN);
        if (y1 > y2) {
            ret.push_back(DIR_RIGHT);
        } else {
            ret.push_back(DIR_LEFT);
        }
    } else if (y1 == y2) {
        if (x1 > x2) {
            ret.push_back(DIR_UP);
        } else {
            ret.push_back(DIR_DOWN);
        }
        ret.push_back(DIR_LEFT);
        ret.push_back(DIR_RIGHT);
        if (x1 > x2) {
            ret.push_back(DIR_DOWN);
        } else {
            ret.push_back(DIR_UP);
        }
    } else {
        if (x1 > x2) {
            ret.push_back(DIR_UP);
            if (y1 > y2) {
                ret.push_back(DIR_LEFT);
                ret.push_back(DIR_RIGHT);
            } else {
                ret.push_back(DIR_RIGHT);
                ret.push_back(DIR_LEFT);
            }
            ret.push_back(DIR_DOWN);
        } else {
            ret.push_back(DIR_DOWN);
            if (y1 > y2) {
                ret.push_back(DIR_LEFT);
                ret.push_back(DIR_RIGHT);
            } else {
                ret.push_back(DIR_RIGHT);
                ret.push_back(DIR_LEFT);
            }
            ret.push_back(DIR_UP);
        }
    }
    assert(ret.size() == 4);
    return ret;
}

/**
 * @brief QLinkWindow::findLine for dfs's target
 * @param x1 y1 start point
 * @param x2 y2 end point
 * @param path ref to record path and draw lines according to this
 * @return -1: OVER 2 broken lines
 */
int QLinkWindow::findLine(int x1, int y1, int x2, int y2, direction_t past_dir,
                          QVector<QBlock *> &path) {
    int num = 0;
    // if arrive point, return success
    if (x1 == x2 && y1 == y2) {
        if (path.back()->xIndex != x2 || path.back()->yIndex != y2)
            path.push_back(blockMap[x2][y2].block);
        return 1;
    }
    // over bound or over to broken lines, return failure
    if (path.size() >= 3 || !isLegalPoint(x1, y1) ||
        (blockMap[x1][y1].type == OCCUPIED && past_dir != UNDEF))
        return 0;

    QVector <direction_t> dirs = AStarFindDirection(x1, y1, x2, y2);
    assert(dirs.size() == 4);
    for (auto &dir: dirs) {
        int new_x = x1, new_y = y1;
        // change direction
        if (past_dir != dir) {
            path.push_back(blockMap[x1][y1].block);
        }
        switch (dir) {
            case DIR_UP: {
                new_x -= 1;
                break;
            }
            case DIR_DOWN: {
                new_x += 1;
                break;
            }
            case DIR_LEFT: {
                new_y -= 1;
                break;
            }
            case DIR_RIGHT: {
                new_y += 1;
                break;
            }
            default: {
                assert(0);
                break;
            }
        }

        num = findLine(new_x, new_y, x2, y2, dir, path);
        if (num == 1)
            return num;
        else if (dir != past_dir) // 转过方向，就把多的去掉
            path.pop_back();
    }
    return num;
}

/**
 * @brief QLinkWindow::canReachBorder, FIND a point can be reached By player
 * if can reach column 0 / row 0, player can reach here.
 * @param x, y: point to find
 * @param vis: visited vector for DFS
 * @return
 */
bool QLinkWindow::canReachBorder(int x, int y, QMap<QPair<int, int>, bool> vis) {
    static QVector <QPair<int, int>> dirs = {{1,  0},
                                             {-1, 0},
                                             {0,  1},
                                             {0,  -1}};
    if (!isLegalPoint(x, y))
        return false;
    if (x == 0 || y == 0 || x == windowHeight - 1 || y == windowLength - 1)
        return true;


    vis[qMakePair(x, y)] = 1;
    bool found = false;
    for (auto &dir: dirs) {
        auto new_point = qMakePair(x + dir.first, y + dir.second);
        if (vis.count(new_point) || blockMap[new_point.first][new_point.second].type == OCCUPIED ||
            blockMap[new_point.first][new_point.second].type == ACTIVATED)
            continue;
        else if (canReachBorder(new_point.first, new_point.second, vis)) {
            return true;
        }
    }
    vis.remove(qMakePair(x, y));
    return found;
}

/**
 * @brief QLinkWindow::isAnySol
 * @return the number of sol, update the data structrue of answers simultaneously
 */
int QLinkWindow::isAnySol() {
    int ans = 0;
    refAnswers.clear();
    for (int k = 1; k <= blockType; ++k) {
        QVector <QPair<int, int>> members;
        for (int i = 1; i < windowHeight - 1; ++i)
            for (int j = 1; j < windowLength - 1; ++j) {
                QMap<QPair<int, int>, bool> vis;
                // only can reach by role and activated or occupied can be added to here
                if ((blockMap[i][j].type == OCCUPIED || blockMap[i][j].type == ACTIVATED)
                    && blockMap[i][j].group == k &&
                    canReachBorder(i, j, vis)) {
                    members.append(qMakePair(i, j));
                }
            }
        for (int i = 0; i < members.size(); ++i) {
            for (int j = i + 1; j < members.size(); ++j) {
                QVector < QBlock * > ansPath;
                if (isLeagalElimate(nullptr, members[i].first, members[i].second,
                                    members[j].first, members[j].second,
                                    false, ansPath)) {

                    refAnswers.push_back(ansPath);
                    ++ans;
                }
            }
        }
    }

    if (isInHint) {
        updateHint();
    }
    return ans;
}

void addNum(QString &m, int s, int level) {
    m.push_back(QString::number(s));
    switch (level) {
        case 1:
            m.push_back("&");
            break;
        case 2:
            m.push_back("|");
            break;
        case 3:
            m.push_back(",");
            break;
    }
}

void addStr(QString &m, QString s, int level) {
    m.push_back((s));
    switch (level) {
        case 1:
            m.push_back("&");
            break;
        case 2:
            m.push_back("|");
            break;
        case 3:
            m.push_back(",");
            break;
    }
}


/**
 * @brief QLinkWindow::loadFromDisk
 * @param name Load from file and start remaining game
 * @return success? false reason: archive not exist
 */
bool QLinkWindow::loadFromDisk(QString name) {
    name.push_back(".qlf");
    if (!QFile::exists(name))
        return false;

    QFile file(name);
    QString buffer = file.readAll();
    auto substrs = buffer.split('&', Qt::SkipEmptyParts);
    return true;
}

/**
 * @brief QLinkWindow::load2Disk
 * @param name archive name
 * @return success? false reason: 1. dup name, 2. illegal name
 */
bool QLinkWindow::load2Disk(QString name) {
    for (auto &q: name) {
        if (!((q >= 'a' && q <= 'z') || (q >= 'A' && q <= 'Z') ||
              (q >= '0' && q <= '9')))
            return false;
    }

    name.push_back(".qlf");
    if (QFile::exists(name))
        return false;

    QFile file(name);
    QString buffer = "";

    // divided by &, and if is 2-D vector, divided by |, if
    // 1-D vector, divided by ,
    // [1, 2]: windowLength, windowHeight
    addNum(buffer, (windowLength), 1);
    addNum(buffer, (windowHeight), 1);
    for(auto &f: blockMap){
        for(auto &s: f){
            addStr(buffer, fromBlockToStr(s), 2);
        }
    }
    addStr(buffer, "", 1);
    for(auto &c: colorSet){
        addStr(buffer, c.name(), 2);
    }
    addStr(buffer, "", 1);
    for(auto &c: wordColorSet){
        addStr(buffer, c.name(), 2);
    }
    addStr(buffer, "", 1);
    addNum(buffer, blockType, 1);
    addNum(buffer, globalStatus, 1);
    addNum(buffer, gameMode, 1);
    addNum(buffer, gameTime1, 1);
    addNum(buffer, hintTime1, 1);

    for(auto s: roles){
        addStr(buffer, fromRoleToStr(s), 2);
    }
    addStr(buffer, "", 1);

    file.write(buffer.toUtf8());
    return true;
}

QString QLinkWindow::fromRoleToStr(Role *role){
    QString buffer = "";

    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->xIndex, 3);
    addNum(buffer, role->yIndex, 3);
    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->playerID, 3);
    addNum(buffer, role->playerID, 3);
    return buffer;
}

QString QLinkWindow::fromBlockToStr(BLOCK_STRUCT b){
    QString buffer = "";

    addNum(buffer, (b.type), 3);
    addStr(buffer, b.color.name(), 3);
    addNum(buffer, b.group, 3);
    addNum(buffer, b.block->status, 3);
    addNum(buffer, b.block->xIndex, 3);
    addNum(buffer, b.block->yIndex, 3);
    addNum(buffer, b.block->blockSize, 3);
    addNum(buffer, b.block->prop_type, 3);
    addStr(buffer, b.block->wordColor.name(), 3);
    addStr(buffer, b.block->blockColor.name(), 3);
    addNum(buffer, b.block->blockWidth, 3);
    addNum(buffer, b.block->blockHeight, 3);

    return buffer;
}

QDataStream &operator<<(QDataStream &output, const QLinkWindow &qw){
    output << qw.windowLength <<qw.windowHeight << qw.blockMap \
           << qw.colorSet << qw.wordColorSet << qw.blockType \
           << qw.globalStatus << qw.gameMode << qw.gameTime1 \
           << qw.hintTime1 << (*(qw.roles[0]));

    if(qw.gameMode == twoPlayer){
        assert(qw.roles.size() == 2);
        output << (*(qw.roles[1]));
    }

    output << qw.linesOnBoard << qw.timerList.size() \
           << qw.linesOfHint \
           << qw.isInHint << qw.hintTime << *(qw.lines);

    return output;
}

/**
 * @brief operator >>
 * @param input
 * @param qw
 * Attention: you need to new all nullptr before you use this func!
 * @return
 */
QDataStream &operator>>(QDataStream &input, QLinkWindow &qw){

    input >> qw.windowLength >> qw.windowHeight >> qw.blockMap \
           >> qw.colorSet >> qw.wordColorSet >> qw.blockType \
           >> qw.globalStatus >> qw.gameMode >> qw.gameTime1 \
           >> qw.hintTime1 >> (*(qw.roles[0]));

    if(qw.gameMode == twoPlayer){
        qw.leftBarLayout = new QVBoxLayout();
        qw.role2 = new Role(&qw, 0, 0, 0, 0, qw.leftBarLayout);
        qw.roles.push_back(qw.role2);
        input >> (*(qw.roles[1]));
    }

    int timerNum;
    input >> qw.linesOnBoard >> timerNum \
           >> qw.linesOfHint \
           >> qw.isInHint >> qw.hintTime >> *(qw.lines);

    for(int i = 0; i < timerNum; ++i){
        qw.timerList.push_back(new QTimer());
    }

    return input;
}

QDataStream &operator<<(QDataStream &output, const BLOCK_STRUCT &bs){

    output << bs.type << bs.color << bs.group << (*bs.block);
    return output;
}

QDataStream &operator>>(QDataStream &input, BLOCK_STRUCT &bs){

    bs.block = new QBlock();
    input >> bs.type >> bs.color >> bs.group >> (*bs.block);
    return input;
}

QBlock *QLinkWindow::getBlock(int x, int y){
    return blockMap[x][y].block;
}

