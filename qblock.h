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

class QBlock : public QLabel {
    friend class QLinkWindow;

public:
    block_t status;

    QColor blockColor, wordColor;

    int blockWidth, blockHeight;

public:
    int xIndex, yIndex;

    prop_t prop_type;

    int blockSize;

private:

    void setBlockSize(const int &idx);

public:

    QBlock(const int &len, const int &height, QWidget *parent);

    QBlock();

    void paintEvent(QPaintEvent *event) override;

    void setColor(const QColor &color, const QColor &wordColor);

    void setStatus(const block_t &status);

    void setIndex(const int &x, const int &y);

public:

    friend QDataStream &operator<<(QDataStream &, const QBlock &);

    friend QDataStream &operator>>(QDataStream &, QBlock &);

};

#endif // QBLOCK_H
