#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>

class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = nullptr);
    virtual ~GameObject();

    enum ItemType { PlayerType = 1, EnemyType, BulletType };
    virtual int type() const override = 0;
    virtual void move() = 0;

    int health = 1;
    int speed = 1;
};

#endif // GAMEOBJECT_H