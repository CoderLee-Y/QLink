#include "menu.h"

menu::menu(): QWidget()
{
    QSize fixedSize;
    fixedSize.setHeight(WINDOWHEIGHT);
    fixedSize.setWidth(WINDOWLEN);
    setFixedSize(fixedSize);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(22);

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

void menu::initBtn(QVBoxLayout *layout){
    QVector<QPushButton *> vec;
    QPushButton *singlePlayer = new QPushButton("1 Player");
    QPushButton *doublePlayer = new QPushButton("2 Player");
    QPushButton *load = new QPushButton("Load");
    QPushButton *exit = new QPushButton("Exit");

    vec.push_back(singlePlayer);
    vec.push_back(doublePlayer);
    vec.push_back(load);
    vec.push_back(exit);

    for(auto i: vec){
        QFont btnFont;
        btnFont.setFamily("Microsoft YaHei");
        btnFont.setPixelSize(20);
        i->setFont(btnFont);
        i->setFlat(true);
        i->setFixedSize(192, 108);
    }

    connect(singlePlayer, &QPushButton::clicked,
            this, &menu::onClickSinglePlayer);
    connect(doublePlayer, &QPushButton::clicked,
            this, &menu::onClickDoublePlayer);
    connect(load, &QPushButton::clicked,
            this, &menu::onClickLoad);
    connect(exit, &QPushButton::clicked,
            this, &menu::onClickExit);

    layout->addWidget(singlePlayer, 0, Qt::AlignCenter);
    layout->addWidget(doublePlayer, 0, Qt::AlignCenter);
    layout->addWidget(load, 0, Qt::AlignCenter);
    layout->addWidget(exit, 0, Qt::AlignCenter);
}

void menu::onClickExit(){
    QApplication::quit();
}

void menu::onClickLoad(){
    emit showLoad(this);
}

void menu::onClickDoublePlayer(){
    emit showGame(this, twoPlayer);
}

void menu::onClickSinglePlayer(){
    emit showGame(this, onePlayer);
}
