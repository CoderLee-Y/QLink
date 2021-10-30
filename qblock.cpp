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

void QBlock::paintEvent(QPaintEvent *event) {
    QString SS = "background: " + blockColor.name() + "; " +
                 "color: " + wordColor.name();
    QSize size;
    size.setWidth(blockWidth);
    size.setHeight(blockHeight);
    this->setFixedSize(size);
    this->setMargin(2);
    switch (status) {
        case OCCUPIED: {
            this->setText("");
            break;
        }
        case ACTIVATED: {
            this->setText("AC");
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
