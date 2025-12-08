#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QLCDNumber>
#include <QGraphicsProxyWidget>
#include "player.h"
#include "inputhandler.h"
#include "score.h"
#include "spike.h"
#include "platform.h"
#include "Life.h"
#include "bonusblock.h"

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
    void onPlayerDied();         // handle player death
    void endInvincibility();     // turn off invincibility
    void updateInvTimer();       // update countdown display

private:
    void spawnSpikes();
    void checkSpikeCollisions();
    void checkEnemyCollisions();
    void checkBonusCollisions(); // check player-bonus collision

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

    // Invincibility
    bool invincible = false;
    QTimer* invincibleTimer;

    // Bonus
    bonusblock* bonus;

    // Invincibility timer display
    QLCDNumber* invTimerDisplay;
    QGraphicsProxyWidget* invTimerProxy;
    QTimer* invCountdownTimer;
    int invSecondsRemaining = 0;
};

#endif // GAMESCENE_H
