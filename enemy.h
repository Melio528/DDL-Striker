#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

class Enemy : public GameObject
{
    Q_OBJECT
public:
    explicit Enemy(double difficulty = 0.0, QObject *parent = nullptr);
    int type() const override { return EnemyType; }
    void move() override;

    int scoreValue = 10; // 被击毁时玩家获得的分数
signals:
    void escaped();   // 敌人逃出屏幕时发出的信号
};

#endif // ENEMY_H