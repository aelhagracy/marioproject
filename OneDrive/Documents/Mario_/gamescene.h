#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include "player.h"
#include "inputhandler.h"
#include "score.h"
#include "spike.h"
#include "platform.h"
#include "Life.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene();

signals:
    void gameFinished();   // level complete
    void playerDied();     // spike kill

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();
    void onPlayerDied();         // handle spike death
    void endInvincibility();     // turn off invincibility

private:
    void spawnSpikes();
    void checkSpikeCollisions(); // check player-spike collisions
    void checkEnemyCollisions();

private:
    Player* player;
    InputHandler* input;
    QTimer* loop;
    Score* score;
    int lastX = 0;

    // Scene boundaries
    int sceneWidth;
    int sceneHeight;

    // Spike spawning
    static int farthestSpike;

    // Lives system
    Life* playerLives;
    QGraphicsTextItem* livesDisplay;
    bool invincible = false;
    QTimer* invincibleTimer;
};

#endif // GAMESCENE_H
