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

public:

    friend QDataStream &operator<<(QDataStream & , const lineMask &);

    friend QDataStream &operator>>(QDataStream & , lineMask &);

};

#endif // LINEMASK_H
