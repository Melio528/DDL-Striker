#include <QtTest>
#include "gamelogic.h"

class TestGameLogic : public QObject
{
    Q_OBJECT
private slots:
    void testInitialState();
    void testAddScore();
    void testTakeDamage();
    void testGameOver();
    void testClampX();
};

void TestGameLogic::testInitialState()
{
    GameLogic logic;
    QCOMPARE(logic.score(), 0);
    QCOMPARE(logic.health(), 5);
    QVERIFY(!logic.isGameOver());
}

void TestGameLogic::testAddScore()
{
    GameLogic logic;
    logic.addScore(10);
    QCOMPARE(logic.score(), 10);
    logic.addScore(20);
    QCOMPARE(logic.score(), 30);
}

void TestGameLogic::testTakeDamage()
{
    GameLogic logic;
    logic.takeDamage();
    QCOMPARE(logic.health(), 4);
    for (int i = 0; i < 4; ++i) logic.takeDamage();
    QCOMPARE(logic.health(), 0);
    QVERIFY(logic.isGameOver());
    logic.takeDamage();
    QCOMPARE(logic.health(), 0);
}

void TestGameLogic::testGameOver()
{
    GameLogic logic;
    QVERIFY(!logic.isGameOver());
    for (int i = 0; i < 5; ++i) logic.takeDamage();
    QVERIFY(logic.isGameOver());
}

void TestGameLogic::testClampX()
{
    int sceneW = 480, playerW = 30;
    QCOMPARE(GameLogic::clampX(100, sceneW, playerW), 100);
    QCOMPARE(GameLogic::clampX(-10, sceneW, playerW), 0);
    QCOMPARE(GameLogic::clampX(500, sceneW, playerW), 450);
}

QTEST_MAIN(TestGameLogic)
#include "main.moc"