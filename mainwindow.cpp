#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "gameobject.h"
#include "enemy.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include "bullet.h"

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
    // 初始化状态
    m_gameFrames = 0;
    m_score = 0;
    m_health = 5;
    // 创建分数 UI (左上角)
    scoreText = new QGraphicsTextItem("分数: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 14));
    scoreText->setPos(10, 10);
    scene->addItem(scoreText);
    // 创建生命值 UI (右上角)
    healthText = new QGraphicsTextItem("生命: 5");
    healthText->setDefaultTextColor(Qt::white);
    healthText->setFont(QFont("Arial", 14));
    healthText->setPos(380, 10);
    scene->addItem(healthText);
    // 启动游戏循环
    timer->start(16);
    enemyTimer->start(1200);
}

// 主循环
void MainWindow::gameLoop()
{
    m_gameFrames++;

    if (player) {
        // ---------- 移动 ----------
        if (m_leftPressed)
            player->setPos(player->x() - player->speed, player->y());
        if (m_rightPressed)
            player->setPos(player->x() + player->speed, player->y());

        // 边界限制
        QRectF sr = scene->sceneRect();
        int pw = player->pixmap().width();
        qreal newX = qBound(sr.left(), player->x(), sr.right() - pw);
        player->setPos(newX, player->y());

        // ---------- 射击 ----------
        if (m_spacePressed && m_shootCooldown <= 0) {
            player->shoot();
            m_shootCooldown = SHOOT_DELAY;
        }
        if (m_shootCooldown > 0)
            m_shootCooldown--;

        // ---------- 无敌闪烁 ----------
        if (player->invincible) {
            player->invincibleFrames--;
            // 每 6 帧切换一次可见性，产生闪烁效果
            player->setVisible((player->invincibleFrames / 3) % 2);
            if (player->invincibleFrames <= 0) {
                player->invincible = false;
                player->setVisible(true);
            }
        }
    }

    // ---------- 推动所有对象移动 ----------
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (GameObject *obj = dynamic_cast<GameObject*>(item))
            obj->move();
    }

    // ---------- 碰撞检测 ----------
    QList<QGraphicsItem*> toRemove;

    for (QGraphicsItem *item : items) {
        GameObject *obj = dynamic_cast<GameObject*>(item);
        if (!obj) continue;

        // 1. 玩家子弹 vs 敌人
        if (obj->type() == GameObject::BulletType) {
            Bullet *bullet = dynamic_cast<Bullet*>(obj);
            if (bullet && bullet->direction == Bullet::Up) {
                QList<QGraphicsItem*> colliding = scene->collidingItems(obj);
                for (QGraphicsItem *other : colliding) {
                    GameObject *otherObj = dynamic_cast<GameObject*>(other);
                    if (otherObj && otherObj->type() == GameObject::EnemyType) {
                        // 击中敌人！
                        m_score += 10;               // 加分
                        toRemove.append(item);       // 删除子弹
                        toRemove.append(other);      // 删除敌人
                        break;
                    }
                }
            }
        }

        // 2. 敌人 vs 玩家
        else if (obj->type() == GameObject::EnemyType) {
            if (player && !player->invincible) {
                if (obj->collidesWithItem(player)) {
                    m_health--;                     // 扣血
                    player->invincible = true;
                    player->invincibleFrames = 90;  // 约 1.5 秒无敌
                    toRemove.append(item);           // 敌人也消失

                    // 游戏结束检查
                    if (m_health <= 0) {
                        timer->stop();
                        enemyTimer->stop();

                        QGraphicsTextItem *gameOverText = new QGraphicsTextItem(
                            "GAME OVER\n\n最终分数: " + QString::number(m_score));
                        gameOverText->setDefaultTextColor(Qt::white);
                        gameOverText->setFont(QFont("Arial", 22));
                        gameOverText->setPos(100, 300);
                        scene->addItem(gameOverText);

                        player = nullptr;   // 禁止继续操作
                    }
                }
            }
        }
    }

    // ---------- 删除待移除对象 ----------
    for (QGraphicsItem *item : toRemove) {
        scene->removeItem(item);
        delete item;
    }

    // ---------- 更新 UI 文字 ----------
    if (scoreText)
        scoreText->setPlainText("分数: " + QString::number(m_score));
    if (healthText)
        healthText->setPlainText("生命: " + QString::number(m_health));
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