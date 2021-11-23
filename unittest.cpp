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

/**
 * @brief UnitTest::testSuccess
 * cases can be linked successfully
 */
void UnitTest::testSuccess() {

    // scene 1: in one row, but need 3 lines
    qlink->setBlock(1, 3, OCCUPIED, 1);
    qlink->setBlock(1, 7, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 3, 1, 7, false, path));

    // scene 2: one row and link directly
    qlink->setBlock(1, 4, EMPTY, 0);
    qlink->setBlock(1, 3, OCCUPIED, 1);
    qlink->setBlock(1, 5, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 3, 1, 5, false, path));

    // scene 3: nearly all empty and test use 2 lines link
    // almost any two blocks, common in sparse cases
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, EMPTY, 0);
        }
    }
    qlink->setBlock(1, 7, OCCUPIED, 1);
    qlink->setBlock(7, 3, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 7, 7, 3, false, path));

    // recover from scene 3
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, OCCUPIED, 1);
        }
    }

    // scene 4: turn once
    qlink->setBlock(1, 1, EMPTY, 0);
    qlink->setBlock(1, 2, OCCUPIED, 1);
    qlink->setBlock(2, 1, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 2, 2, 1, false, path));

    // scene 5: several solution
    qlink->setBlock(1, 1, OCCUPIED, 1);
    qlink->setBlock(1, 8, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 1, 1, 8, false, path));

    // scene 6: long U shape
    // very tiny but tricky bug, because 4 porints
    // main produce 3 lines or four lines
    qlink->setBlock(1, 3, EMPTY, 0);
    qlink->setBlock(2, 3, EMPTY, 0);
    qlink->setBlock(3, 3, EMPTY, 0);
    qlink->setBlock(4, 3, EMPTY, 0);

    qlink->setBlock(1, 6, EMPTY, 0);
    qlink->setBlock(2, 6, EMPTY, 0);
    qlink->setBlock(3, 6, EMPTY, 0);
    qlink->setBlock(4, 6, EMPTY, 0);

    qlink->setBlock(5, 3, OCCUPIED, 1);
    qlink->setBlock(5, 6, OCCUPIED, 1);
    QVERIFY(qlink->isLeagalElimate(player1, 5, 3, 5, 6, false, path));

    // recover from scene 6
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, OCCUPIED, 1);
        }
    }

    // scene 7: a z-shape route
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 4, 1, false, path));
    qlink->setBlock(2, 2, EMPTY, 0);
    qlink->setBlock(3, 2, EMPTY, 0);
    qlink->setBlock(3, 1, EMPTY, 0);
    QVERIFY(qlink->isLeagalElimate(player1, 1, 2, 4, 1, false, path));

}

/**
 * @brief UnitTest::testFailure
 * Test can't linked by 3 lines
 */
void UnitTest::testFailure() {
    // add all OCCUPIED
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, OCCUPIED, 1);
        }
    }

    // scene 1: right triangle, need 4 lines
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 2, 1, false, path));

    // scene 2: need 5 lines
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 8, 2, false, path));

    // scene 3: can't link at all
    QVERIFY(!qlink->isLeagalElimate(player1, 2, 2, 6, 6, false, path));

    // scene 4: long right triangle, need 4 lines
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 6, 6, 1, false, path));

    // scene 5: invalid z-shape
    qlink->setBlock(2, 3, EMPTY, 0);
    qlink->setBlock(3, 3, EMPTY, 0);
    qlink->setBlock(3, 2, EMPTY, 0);
    qlink->setBlock(4, 2, EMPTY, 0);

    QVERIFY(!qlink->isLeagalElimate(player1, 1, 3, 4, 1, false, path));
}

/**
 * @brief UnitTest::testDifferentType
 * if two blocks are in different groups, return false
 */
void UnitTest::testDifferentType() {
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            qlink->setBlock(i, j, OCCUPIED, 1);
        }
    }

    qlink->setBlock(1, 2, OCCUPIED, 3);
    qlink->setBlock(1, 7, OCCUPIED, 4);
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 1, 7, false, path));
}

/**
 * @brief UnitTest::testDifferentStatus
 * if any block has status of EMPTY, PLAYER, PROPS, return false
 */
void UnitTest::testDifferentStatus() {
    qlink->setBlock(1, 2, OCCUPIED, 1);
    qlink->setBlock(1, 7, OCCUPIED, 1);

    qlink->setBlock(1, 2, EMPTY, 0);
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 1, 7, false, path));

    qlink->setBlock(1, 7, OCCUPIED, 1);

    qlink->setBlock(1, 2, OCCUPIED, 0);
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 1, 7, false, path));

    qlink->setBlock(1, 7, OCCUPIED, 1);

    qlink->setBlock(1, 2, PROPS, 0);
    QVERIFY(!qlink->isLeagalElimate(player1, 1, 2, 1, 7, false, path));
}

void UnitTest::cleanupTestCase() {
    qDebug("QLink Unit Test Ends");
    delete qlink;
}

QTEST_MAIN(UnitTest)
