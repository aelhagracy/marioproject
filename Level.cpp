#include "Level.h"

Level::Level(int levelNumber, Player *player, QObject *parent)
    : QObject(parent),
    levelNumber(levelNumber),
    player(player),
    completed(false)
{
}

Level::~Level() = default;

int Level::getLevelNumber()
{
    return levelNumber;
}

void Level::startLevel()
{
    changeLevel(levelNumber);
    emit levelStarted(levelNumber);
}

void Level::completeLevel()
{
    completed = true;
    emit levelCompleted(levelNumber);
}

void Level::restartLevel()
{
    completed = false;
    changeLevel(levelNumber);
    emit levelRestarted(levelNumber);
}


void Level::changeLevel(int newLevel)
{
    levelNumber = newLevel;
}
