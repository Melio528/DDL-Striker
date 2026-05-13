#include "player.h"
#include "bullet.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>

Player::Player(QObject *parent)
    : GameObject(parent)
{
    QPixmap pix("images/player.png");
    pix = pix.scaled(72, 72, Qt::KeepAspectRatio, Qt::FastTransformation);
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
