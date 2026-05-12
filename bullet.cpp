#include "bullet.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>

Bullet::Bullet(Direction dir, QObject *parent)
    : GameObject(parent), direction(dir)
{
    // 画一个黄/红色子弹
    QPixmap pix(6, 12);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setBrush(dir == Up ? Qt::yellow : Qt::red);
    painter.drawRect(0, 0, 6, 12);
    painter.end();
    setPixmap(pix);

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