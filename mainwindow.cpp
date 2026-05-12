#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "gameobject.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("焦虑粉碎机 - DDL Strike");

    // 菜单信号绑定
    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::startGame);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionHelp, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "操作说明",
                                 "← → 或 A/D 移动飞机\n空格键发射子弹\n消灭下落的DDL和焦虑！");
    });

    // 创建图形场景（背景为黑色）
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 480, 800);
    view = new QGraphicsView(scene, this);
    view->setFixedSize(480, 800);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setBackgroundBrush(Qt::black);
    setCentralWidget(view);

    // 初始化游戏循环定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);

    player = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame()
{
    // 清空场景，创建新玩家
    scene->clear();

    player = new Player();
    scene->addItem(player);
    player->setPos(225, 720);
    player->setFlag(QGraphicsItem::ItemIsFocusable);

    timer->start(16);   // 大约60帧每秒
}

void MainWindow::gameLoop()
{
    // 推动所有游戏对象移动
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (GameObject *obj = dynamic_cast<GameObject*>(item)) {
            obj->move();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!player) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    // 玩家移动和射击
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        player->setPos(player->x() - player->speed, player->y());
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        player->setPos(player->x() + player->speed, player->y());
        break;
    case Qt::Key_Space:
        player->shoot();
        return;         // 射击不需要边界检查
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }

    // 边界限制
    QRectF sr = scene->sceneRect();
    int pw = player->pixmap().width();
    qreal newX = qBound(sr.left(), player->x(), sr.right() - pw);
    player->setPos(newX, player->y());
}