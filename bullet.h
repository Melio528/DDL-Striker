#ifndef BULLET_H
#define BULLET_H

#include "gameobject.h"

class Bullet : public GameObject
{
    Q_OBJECT
public:
    enum Direction { Up, Down };
    Bullet(Direction dir, QObject *parent = nullptr);
    int type() const override { return BulletType; }
    void move() override;

    Direction direction;
    int damage = 1;
};

#endif // BULLET_H