#include "menu.h"

Menu::Menu() : QWidget() {
    QSize fixedSize;
    fixedSize.setHeight(WINDOWHEIGHT);
    fixedSize.setWidth(WINDOWLEN);
    setFixedSize(fixedSize);
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *label = new QLabel;
    QFont font;
    font.setBold(true);
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(50);
    label->setFont(font);
    label->setText("QLink");

    layout->addWidget(label, 0, Qt::AlignCenter);
    initBtn(layout);


    this->setLayout(layout);
    this->setStyleSheet("Background: white");


}

void Menu::initBtn(QVBoxLayout *layout) {
    QVector < QPushButton * > vec;
    QPushButton *singlePlayer = new QPushButton("1 Player");
    QPushButton *doublePlayer = new QPushButton("2 Player");
    QPushButton *load = new QPushButton("Load");
    QPushButton *exit = new QPushButton("Exit");

    vec.push_back(singlePlayer);
    vec.push_back(doublePlayer);
    vec.push_back(load);
    vec.push_back(exit);

    for (auto i: vec) {
        QFont btnFont;
        btnFont.setFamily("Microsoft YaHei");
        btnFont.setPixelSize(20);
        i->setFont(btnFont);
        i->setFlat(true);
        i->setFixedSize(192, 108);
    }

    connect(singlePlayer, &QPushButton::clicked,
            this, &Menu::onClickSinglePlayer);
    connect(doublePlayer, &QPushButton::clicked,
            this, &Menu::onClickDoublePlayer);
    connect(load, &QPushButton::clicked,
            this, &Menu::onClickLoad);
    connect(exit, &QPushButton::clicked,
            this, &Menu::onClickExit);

    layout->addWidget(singlePlayer, 0, Qt::AlignCenter);
    layout->addWidget(doublePlayer, 0, Qt::AlignCenter);
    layout->addWidget(load, 0, Qt::AlignCenter);
    layout->addWidget(exit, 0, Qt::AlignCenter);
}

void Menu::onClickExit() {
    QApplication::quit();
}

void Menu::onClickLoad() {
    bool success = false;

    auto s = keyboardGrabber();
    if (s != nullptr)
        s->releaseKeyboard();

    QString text = QInputDialog::getText(this,
                                         tr("Input Game name"), tr("The Game name set when saving"),
                                         QLineEdit::Normal, "Game Name", &success);

    if (s != nullptr)
        s->grabKeyboard();
    if (!text.isEmpty() && success)
        emit showLoad(this, text);
}

void Menu::onClickDoublePlayer() {
    emit showGame(this, twoPlayer);
}

void Menu::onClickSinglePlayer() {
    emit showGame(this, onePlayer);
}
