#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "gameobject.h"
#include "enemy.h"
#include <QMessageBox>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("焦虑粉碎机 - DDL Strike");

    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::startGame);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionHelp, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "操作说明",
                                 "← → 或 A/D 移动飞机\n空格键发射子弹\n消灭下落的DDL和焦虑！");
    });

    // 场景
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 480, 800);
    view = new QGraphicsView(scene, this);
    view->setFixedSize(480, 800);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setBackgroundBrush(Qt::black);
    setCentralWidget(view);

    // 主循环
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);

    // 敌人生成器
    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy);

    player = nullptr;
}

MainWindow::~MainWindow() { delete ui; }

// 开始新游戏
void MainWindow::startGame()
{
    scene->clear();

    player = new Player();
    scene->addItem(player);
    player->setPos(225, 720);
    player->setFlag(QGraphicsItem::ItemIsFocusable);

    m_gameFrames = 0;
    timer->start(16);
    enemyTimer->start(1200);          // 第一个敌人稍快出现
}

// 主循环
void MainWindow::gameLoop()
{
    m_gameFrames++;

    if (player) {
        // 移动
        if (m_leftPressed)
            player->setPos(player->x() - player->speed, player->y());
        if (m_rightPressed)
            player->setPos(player->x() + player->speed, player->y());

        // 边界
        QRectF sr = scene->sceneRect();
        int pw = player->pixmap().width();
        qreal newX = qBound(sr.left(), player->x(), sr.right() - pw);
        player->setPos(newX, player->y());

        // 射击
        if (m_spacePressed && m_shootCooldown <= 0) {
            player->shoot();
            m_shootCooldown = SHOOT_DELAY;
        }
        if (m_shootCooldown > 0)
            m_shootCooldown--;
    }

    // 推动所有游戏对象移动（子弹、敌人等）
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (GameObject *obj = dynamic_cast<GameObject*>(item))
            obj->move();
    }
}

// 按键
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!player) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:  case Qt::Key_A: m_leftPressed  = true; break;
    case Qt::Key_Right: case Qt::Key_D: m_rightPressed = true; break;
    case Qt::Key_Space: m_spacePressed = true; break;
    default: QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!player) {
        QMainWindow::keyReleaseEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:  case Qt::Key_A: m_leftPressed  = false; break;
    case Qt::Key_Right: case Qt::Key_D: m_rightPressed = false; break;
    case Qt::Key_Space: m_spacePressed = false; break;
    default: QMainWindow::keyReleaseEvent(event);
    }
}

// 敌人生成
void MainWindow::spawnEnemy()
{
    double elapsed = m_gameFrames / 60.0;
    Enemy *enemy = new Enemy(elapsed);
    int x = rand() % (480 - 30);
    enemy->setPos(x, -30);
    scene->addItem(enemy);

    int next = 800 + rand() % 1201;
    enemyTimer->start(next);
}