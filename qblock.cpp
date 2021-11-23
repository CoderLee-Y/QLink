#include "qblock.h"


QBlock::QBlock(int len, int height, QWidget *parent) : QLabel(parent) {
    // Default size
    blockWidth = (WINDOWLEN / len);
    blockHeight = (WINDOWHEIGHT / height);

    blockWidth = std::min(blockWidth, blockHeight);
    blockHeight = std::min(blockWidth, blockHeight);
    status = EMPTY;

    blockColor = QColor(255, 255, 255);
}

QBlock::QBlock() {
    blockColor = QColor(255, 255, 255);
    status = EMPTY;
}

void QBlock::paintEvent(QPaintEvent *event) {
    QString SS = "background: " + blockColor.name() + "; " +
                 "color: " + wordColor.name();
    setBlockSize(0);
    this->setAlignment(Qt::AlignCenter);
    switch (status) {
        case OCCUPIED: {
            this->setText("");
            break;
        }
        case ACTIVATED: {
            setBlockSize(1);
            break;
        }
        case PROPS: {
            switch (prop_type) {
                case plusSec: {
                    this->setText("+1s");
                    break;
                }
                case hint: {
                    this->setText("Hint");
                    break;
                }
                case reshuffle:
                    this->setText("Reshuffle");
                    break;
            }

            break;
        }
        case PLAYER: {
            this->setText("PLAYER");
            break;
        }
        case EMPTY: {
            this->setText("");
            break;
        }
    }

    this->setStyleSheet(SS);
    QLabel::paintEvent(event);
}

void QBlock::setBlockSize(int idx) {
    blockSize = idx;
    QSize size;
    if (idx == 0) {
        size.setWidth(blockWidth);
        size.setHeight(blockHeight);
    } else {
        size.setWidth(blockWidth * 0.98);
        size.setHeight(blockHeight * 0.98);

    }
    this->setFixedSize(size);
    this->setMargin(2);
}


void QBlock::setColor(QColor color, QColor wordColor) {
    this->blockColor = color;
    this->wordColor = wordColor;
}

void QBlock::setStatus(block_t status) {
    this->status = status;
}

void QBlock::setIndex(int x, int y) {
    this->xIndex = x;
    this->yIndex = y;
}

QDataStream &operator<<(QDataStream &output, const QBlock &b) {

    output << (qint32) b.status << b.xIndex << b.yIndex \
 << b.blockSize << (qint32) b.prop_type << (int) b.wordColor.red()
           << (int) b.wordColor.green() << (int) b.wordColor.blue()
           << (int) b.blockColor.red() << (int) b.blockColor.green()
           << (int) b.blockColor.blue() << b.blockWidth << b.blockHeight;
    return output;
}

QDataStream &operator>>(QDataStream &input, QBlock &b) {

    int r, g, ba;
    qint32 status_num, prop_num;
    input >> status_num >> b.xIndex >> b.yIndex \
 >> b.blockSize >> prop_num >> r >> g >> ba;

    b.status = block_t(status_num);
    b.prop_type = prop_t(prop_num);
    b.wordColor = QColor(r, g, ba);
    input >> r >> g >> ba >> b.blockWidth >> b.blockHeight;
    b.blockColor = QColor(r, g, ba);
    return input;
}
