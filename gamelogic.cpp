#include "gamelogic.h"
#include <algorithm>

GameLogic::GameLogic()
{
    reset();
}

void GameLogic::reset()
{
    m_score = 0;
    m_health = MAX_HEALTH;
}

void GameLogic::addScore(int points)
{
    m_score += points;
}

int GameLogic::score() const
{
    return m_score;
}

void GameLogic::takeDamage()
{
    if (m_health > 0)
        m_health--;
}

int GameLogic::health() const
{
    return m_health;
}

bool GameLogic::isGameOver() const
{
    return m_health <= 0;
}

int GameLogic::clampX(int x, int sceneWidth, int playerWidth)
{
    return std::clamp(x, 0, sceneWidth - playerWidth);
}