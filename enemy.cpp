#include "enemy.h"
#include <QPixmap>
#include <QPainter>
#include <QGraphicsScene>

Enemy::Enemy(double difficulty, QObject *parent)
    : GameObject(parent)
{
    int r = rand() % 4;
    QPixmap pix;
    switch (r) {
    case 0: pix.load("images/DDL.png"); break;
    case 1: pix.load("images/homework.png"); break;
    case 2: pix.load("images/PFT.png"); break;
    case 3: pix.load("images/finalexam.png"); break;
    }
    pix = pix.scaled(72, 72, Qt::KeepAspectRatio, Qt::FastTransformation);
    setPixmap(pix);

    // 速度随难度递增
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