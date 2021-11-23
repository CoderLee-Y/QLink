#ifndef UNITTEST_H
#define UNITTEST_H

#include <QtTest/QtTest>
#include "controller.h"

class UnitTest : public QObject {
    Q_OBJECT
private
    slots:
            void initTestCase();

    void testSuccess();

    void testFailure();

    void testDifferentContent();

    void testDifferentType();

    void cleanupTestCase();

public:
    UnitTest();

private:
    Controller *window;
    QLinkWindow *qlink;

    Role *player1, *player2;
};

#endif // UNITTEST_H
