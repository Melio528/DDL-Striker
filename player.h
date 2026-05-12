#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

class Player : public GameObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    int type() const override { return PlayerType; }
    void move() override;
    void shoot();

    int maxHealth = 5;
    int currentHealth = 5;
    bool invincible = false;
};

#endif // PLAYER_H