#include "bullet.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>

Bullet::Bullet(Direction dir, QObject *parent)
    : GameObject(parent), direction(dir)
{
    setPixmap(QPixmap("images/bullet.png"));
    speed = 7;
}

void Bullet::move()
{
    if (direction == Up)
        setY(y() - speed);
    else
        setY(y() + speed);

    // 飞出屏幕自我销毁
    if (y() < -20 || y() > 820) {
        scene()->removeItem(this);
        deleteLater();
    }
}