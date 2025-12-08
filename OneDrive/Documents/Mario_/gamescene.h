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
#include "enemy.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene();

signals:
    void gameFinished();   // level complete
    void playerDied();     // player death

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();
    void endInvincibility();

private:
    void spawnSpikes();
    void checkSpikeCollisions();
    void checkEnemyCollisions();
    void checkBonusCollisions();
    void loadLevel(int level);
    void showLevelPopup();
    void handleGameOver();
    void updateInvTimer();

private:
    Player* player;
    InputHandler* input;
    QTimer* loop;
    Score* score;
    int lastX = 0;

    int sceneWidth;
    int sceneHeight;

    static int farthestSpike;

    // Lives
    Life* playerLives;

    // Invincibility
    bool invincible;
    QTimer* invincibleTimer;
    QTimer* invCountdownTimer;
    QLCDNumber* invTimerDisplay;
    QGraphicsProxyWidget* invTimerProxy;
    int invSecondsRemaining;

    // Bonus
    bonusblock* bonus;

    // Leveling
    int currentLevel;
};

#endif // GAMESCENE_H
