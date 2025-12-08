#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QLCDNumber>
#include <QGraphicsProxyWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
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
    void gameFinished();
    void playerDied();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();
    void endInvincibility();
    void updateInvTimer();

private:
    void spawnSpikes();
    void checkSpikeCollisions();
    void checkEnemyCollisions();
    void checkBonusCollisions();
    void loadLevel(int level);
    void showLevelPopup();
    void handleGameOver();

private:
    Player* player;
    InputHandler* input;
    QTimer* loop;
    Score* score;
    int lastX = 0;
    int sceneWidth;
    int sceneHeight;
    static int farthestSpike;
    Life* playerLives;
    bool invincible;
    QTimer* invincibleTimer;
    QTimer* invCountdownTimer;
    QLCDNumber* invTimerDisplay;
    QGraphicsProxyWidget* invTimerProxy;
    int invSecondsRemaining;
    bonusblock* bonus;
    int currentLevel;
    QMediaPlayer* themePlayer;
    QAudioOutput* themeAudio;
};

#endif
