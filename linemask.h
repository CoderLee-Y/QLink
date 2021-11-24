#ifndef LINEMASK_H
#define LINEMASK_H

#include <QFrame>
#include <QMap>
#include <QVector>
#include <QTime>
#include <QLine>
#include <QPainter>
#include "qblock.h"

class LineMask : public QFrame {
private:

    QVector <QVector<QLine>> lineGroup;

    void paintEvent(QPaintEvent *) override;

    QVector<bool> seen;

    unsigned int num;

public:
    LineMask();

    int addLines(const QVector <QLine> &);

    void removeLines(const int &idx);

public:

    friend QDataStream &operator<<(QDataStream &, const LineMask &);

    friend QDataStream &operator>>(QDataStream &, LineMask &);

};

#endif // LINEMASK_H
