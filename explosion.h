#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>

class Explosion : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
public:
    Explosion(QObject *parent = nullptr);
    void start();
};

#endif // EXPLOSION_H