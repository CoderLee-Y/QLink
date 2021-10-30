#ifndef LINEMASK_H
#define LINEMASK_H

#include <QFrame>
#include <QMap>
#include <QVector>
#include <QTime>
#include <QLine>
#include <QPainter>
#include "qblock.h"


class lineMask: public QFrame
{
private:

    QVector<QVector<QLine>> lineGroup;

    void paintEvent(QPaintEvent *) override;

    QVector<bool> seen;

    unsigned int num;

public:
    lineMask();

    int addLines(QVector<QLine> &);

    void removeLines(int idx);
};

#endif // LINEMASK_H
