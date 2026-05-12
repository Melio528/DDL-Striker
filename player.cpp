#include "player.h"
#include "bullet.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>

Player::Player(QObject *parent)
    : GameObject(parent)
{
    // 画一个 30x30 的青色方块作为临时飞机
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setBrush(Qt::cyan);
    painter.drawRect(0, 0, 30, 30);
    painter.end();
    setPixmap(pix);

    setPos(225, 720);
    speed = 5;
    health = maxHealth;
}

void Player::move() {}

void Player::shoot()
{
    Bullet *b = new Bullet(Bullet::Up);
    b->setPos(x() + pixmap().width()/2.0 - b->pixmap().width()/2.0,
              y() - b->pixmap().height());
    scene()->addItem(b);
}