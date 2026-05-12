#include "enemy.h"
#include <QPixmap>
#include <QPainter>
#include <QGraphicsScene>

Enemy::Enemy(double difficulty, QObject *parent)
    : GameObject(parent)
{
    // 随机颜色代表不同敌人类型
    int r = rand() % 3;
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    switch (r) {
    case 0: painter.setBrush(Qt::red);    break;
    case 1: painter.setBrush(Qt::magenta); break;
    case 2: painter.setBrush(Qt::darkRed); break;
    }
    painter.drawRect(0, 0, 30, 30);
    painter.end();
    setPixmap(pix);

    // 速度随游戏时长递增，保留随机性
    int baseSpeed = 1;
    int maxSpeed = 5;
    int extra = qMin((int)(difficulty * 0.4), maxSpeed - baseSpeed);
    this->speed = baseSpeed + extra + (rand() % 2);
}

void Enemy::move()
{
    setY(y() + speed);
    // 移出屏幕底部就自动删除
    if (y() > 820) {
        scene()->removeItem(this);
        deleteLater();
    }
}