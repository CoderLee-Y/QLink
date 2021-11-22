#include "linemask.h"

LineMask::LineMask() {
    num = 0;
}

void LineMask::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::blue, 5));

    for (unsigned int i = 0; i < num; ++i) {
        if (seen[i])
            painter.drawLines(lineGroup[i]);
    }
}

int LineMask::addLines(QVector <QLine> &q) {
    lineGroup.push_back(q);
    seen.push_back(true);
    ++num;

    return (lineGroup.size() - 1);
}

void LineMask::removeLines(int idx) {
    seen[idx] = false;
}

QDataStream &operator<<(QDataStream &output, const LineMask &lm) {

    output << lm.num;
    assert(lm.num == lm.seen.size());
    assert(lm.num == lm.lineGroup.size());
    for (unsigned int i = 0; i < lm.num; ++i) {
        output << lm.lineGroup[i] << lm.seen[i];
    }
    return output;
}

QDataStream &operator>>(QDataStream &input, LineMask &lm) {

    input >> lm.num;
    for (unsigned int i = 0; i < lm.num; ++i) {
        input >> lm.lineGroup[i] >> lm.seen[i];
    }
    return input;
}

