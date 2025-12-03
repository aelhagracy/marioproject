#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include "player.h"

class Level : public QObject
{
    Q_OBJECT

public:
    Level(int levelNumber, Player *player = nullptr, QObject *parent = nullptr);
    ~Level();

    int getLevelNumber();
    void startLevel();
    void completeLevel();
    void restartLevel();

signals:
    void levelStarted(int levelNumber);
    void levelCompleted(int levelNumber);
    void levelRestarted(int levelNumber);
    void levelFailed();
    void progressChanged(int percentage);

private:
   int levelNumber;
    Player* player;
    bool completed;
    void changeLevel(int newLevel);
};

#endif // LEVEL_H
