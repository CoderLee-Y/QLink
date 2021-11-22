#ifndef QBLOCK_H
#define QBLOCK_H

#include <QWidget>
#include <QLayoutItem>
#include <QLabel>
#include <QDataStream>
#include <QPushButton>
#include <QTextBlock>
#include <QSizePolicy>
#include <iostream>

#include "constant.h"
#include "enum.h"

class QLinkWindow;

class QBlock: public QLabel
{
    friend class QLinkWindow;

private:
    block_t status;

    QColor blockColor, wordColor;

    int blockWidth, blockHeight;

public:
    int xIndex, yIndex;

    prop_t prop_type;

private:

    void setBlockSize(int idx);

public:
    int blockSize;

    QBlock(int len, int height, QWidget *parent);

    QBlock();

    void paintEvent(QPaintEvent *event) override;

    void setColor(QColor color, QColor wordColor);

    void setStatus(block_t status);

    void setIndex(int x, int y);

public:

    friend QDataStream &operator<<(QDataStream & , const QBlock &);

    friend QDataStream &operator>>(QDataStream & , QBlock &);

};

#endif // QBLOCK_H
