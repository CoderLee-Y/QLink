#include "unittest.h"


UnitTest::UnitTest() {
    qlink = new QLinkWindow();
    qlink->setGameMode(onePlayer);
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            if (qlink->getBlock(i, j)->status == EMPTY) {
                // in origin program, map producing is random
                qlink->setBlock(i, j, OCCUPIED, 1);
            }
        }
    }
}

void UnitTest::initTestCase() {
    qDebug("QLink Unit Test Begins");
}

void UnitTest::testSuccess() {
    QVector < QBlock * > path;

    // scene 1: in one row
    qlink->setBlock(1, 3, OCCUPIED, 1);
    qlink->setBlock(1, 7, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 3, 1, 7, false, path));

    // scene 2: one row and link directly
    qlink->setBlock(1, 1, EMPTY, 0);
    qlink->setBlock(1, 2, OCCUPIED, 1);
    qlink->setBlock(2, 1, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 2, 2, 1, false, path));

    // scene 3: almost all empty and test
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, EMPTY, 0);
        }
    }
    qlink->setBlock(1, 7, OCCUPIED, 1);
    qlink->setBlock(7, 3, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 2, 2, 1, false, path));

    // recover
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, OCCUPIED, 1);
        }
    }

    // scene 4:

}

void UnitTest::testFailure() {

}

void UnitTest::testDifferentType() {

}

void UnitTest::testDifferentContent() {

}

void UnitTest::cleanupTestCase() {
    qDebug("QLink Unit Test Ends");
    delete qlink;
}

QTEST_MAIN(UnitTest)
