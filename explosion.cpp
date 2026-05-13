#include "explosion.h"
#include <QTimer>
#include <QPen>

Explosion::Explosion(QObject *parent)
    : QObject(parent)
{
    setRect(0, 0, 10, 10);
    setBrush(Qt::yellow);
    setPen(Qt::NoPen);
    setZValue(10); // 在最上层
}

void Explosion::start()
{
    // 动画：从 10x10 放大到 60x60，同时透明度降低
    auto *anim = new QPropertyAnimation(this, "rect");
    anim->setDuration(300);
    anim->setStartValue(QRectF(-5, -5, 10, 10));
    anim->setEndValue(QRectF(-30, -30, 60, 60));
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // 300 毫秒后删除对象
    QTimer::singleShot(300, this, &QObject::deleteLater);
}