#ifndef QBLOCK_H
#define QBLOCK_H

#include <QWidget>
#include <QLayoutItem>
#include <QLabel>
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

    int xIndex, yIndex;

    prop_t prop_type;

public:
    QBlock(int len, int height, QWidget *parent);

    void paintEvent(QPaintEvent *event) override;

    void setColor(QColor color, QColor wordColor);

    void setStatus(block_t status);

    void setIndex(int x, int y);

};

#endif // QBLOCK_H
