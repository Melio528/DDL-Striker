#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>

class Player;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void startGame();
    void gameLoop();
    void spawnEnemy();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer *timer;
    QTimer *enemyTimer;
    Player *player;

    bool m_leftPressed = false;
    bool m_rightPressed = false;
    bool m_spacePressed = false;
    int m_shootCooldown = 0;
    int m_gameFrames = 0;
    static const int SHOOT_DELAY = 8;
};

#endif // MAINWINDOW_H