#include "linemask.h"

lineMask::lineMask() {
    num = 0;
}

void lineMask::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 5));

    for (unsigned int i = 0; i < num; ++i) {
        if (seen[i])
            painter.drawLines(lineGroup[i]);
    }
}

int lineMask::addLines(QVector <QLine> &q) {
    lineGroup.push_back(q);
    seen.push_back(true);
    ++num;

    return (lineGroup.size() - 1);
}

void lineMask::removeLines(int idx) {
    seen[idx] = false;
}

QDataStream &operator<<(QDataStream &output, const lineMask &lm){

    output << lm.lineGroup << lm.seen << lm.num;
    return output;
}

QDataStream &operator>>(QDataStream &input, lineMask &lm){

    input >> lm.lineGroup >> lm.seen >> lm.num;
    return input;
}

