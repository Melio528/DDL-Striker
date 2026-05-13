#ifndef GAMELOGIC_H
#define GAMELOGIC_H

class GameLogic
{
public:
    GameLogic();

    void reset();               // 重置为新游戏状态
    void addScore(int points);
    int score() const;

    void takeDamage();
    int health() const;
    bool isGameOver() const;

    static int clampX(int x, int sceneWidth, int playerWidth); // 边界限制

private:
    int m_score;
    int m_health;
    static const int MAX_HEALTH = 5;
};

#endif // GAMELOGIC_H