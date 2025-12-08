#include "gamescene.h"
#include "platform.h"
#include "enemy.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>

int GameScene::farthestSpike = 1200;

GameScene::GameScene()
{

    sceneWidth = 6000;
    sceneHeight = 500;
    setSceneRect(0, 0, sceneWidth, sceneHeight);


    auto background = new QGraphicsPixmapItem(QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight));
    background->setZValue(0);
    addItem(background);


    player = new Player();
    player->setPos(100, -420);
    player->setZValue(5);
    addItem(player);

    input = new InputHandler(player);

    score = new Score();
    addItem(score);
    score->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    score->setPos(20, 20);
    lastX = player->x();

    playerLives = new Life(nullptr, 3);
    addItem(playerLives);
    playerLives->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    playerLives->setPos(150, 20);

    invincible = false;
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);
    connect(invincibleTimer, &QTimer::timeout, this, &GameScene::endInvincibility);
    invTimerDisplay = new QLCDNumber(2);
    invTimerDisplay->setSegmentStyle(QLCDNumber::Flat);
    invTimerDisplay->display(0);
    invTimerDisplay->setStyleSheet("background:transparent; color:yellow; font: 16pt 'Arial';");
    invTimerProxy = addWidget(invTimerDisplay);
    invTimerProxy->setZValue(5);
    invTimerProxy->setPos(300, 20);
    invCountdownTimer = new QTimer(this);
    connect(invCountdownTimer, &QTimer::timeout, this, &GameScene::updateInvTimer);
    invSecondsRemaining = 0;

    currentLevel = 1;

    themePlayer = new QMediaPlayer(this);
    themeAudio = new QAudioOutput(this);
    themePlayer->setAudioOutput(themeAudio);
    themeAudio->setVolume(0.5);
    themePlayer->setSource(QUrl("qrc:/sound/theme.wav"));
    themePlayer->setLoops(QMediaPlayer::Infinite);
    themePlayer->play();

    loadLevel(currentLevel);

    loop = new QTimer(this);
    connect(loop, &QTimer::timeout, this, &GameScene::updateGame);
    loop->start(16);
}
void GameScene::loadLevel(int level)
{
    QList<QGraphicsItem*> itemsToRemove;
    for (auto item : items())
    {
        if (dynamic_cast<platform*>(item) || dynamic_cast<Spike*>(item) ||
            dynamic_cast<Enemy*>(item) || dynamic_cast<bonusblock*>(item))
        {
            itemsToRemove.append(item);
        }
    }
    for (auto item : itemsToRemove)
    {
        removeItem(item);
        delete item;
    }

    int numPlatforms = 10 + level * 2;
    int x = 300;
    for (int i = 0; i < numPlatforms; ++i)
    {
        int width = 90 + (QRandomGenerator::global()->bounded(2) * 10);
        int height = 40;
        platform* plat = new platform(width, height, ":/images/brick.jpg");
        int y = 250 + QRandomGenerator::global()->bounded(120);
        plat->setPos(x, y);
        plat->setZValue(3);
        addItem(plat);
        x += 300 + QRandomGenerator::global()->bounded(100);
    }


    int baseSpikes = 3;
    int numSpikes = baseSpikes + level;
    for (int i = 0; i < numSpikes; ++i)
    {
        Spike* spike = new Spike();
        int sx = QRandomGenerator::global()->bounded(600, sceneWidth - 200);
        spike->setPos(sx, 404);
        spike->setZValue(4);
        addItem(spike);
    }


    int baseEnemies = 2;
    int numEnemies = baseEnemies + level;
    for (int i = 0; i < numEnemies; ++i)
    {
        Enemy* enemy = new Enemy();
        int ex = QRandomGenerator::global()->bounded(400, sceneWidth - 300);
        enemy->setPos(ex, 404);
        enemy->setZValue(4);
        addItem(enemy);
    }

    if (level >= 3)
    {
        bonus = new bonusblock();
        bonus->setPixmap(QPixmap(":/images/bonus.jpg").scaled(40, 40));
        int bx = QRandomGenerator::global()->bounded(200, sceneWidth - 200);
        int by = QRandomGenerator::global()->bounded(100, 350);
        bonus->setPos(bx, by);
        bonus->setZValue(10);
        addItem(bonus);
    }
    else
    {
        bonus = nullptr;
    }
    showLevelPopup();
}


void GameScene::showLevelPopup()
{
    if (currentLevel > 5)
    {
        if (themePlayer->playbackState() == QMediaPlayer::PlayingState)
            themePlayer->stop();
        QMediaPlayer* victoryPlayer = new QMediaPlayer(this);
        QAudioOutput* victoryAudio = new QAudioOutput(this);
        victoryPlayer->setAudioOutput(victoryAudio);
        victoryAudio->setVolume(0.7);
        victoryPlayer->setSource(QUrl("qrc:/sound/victory.wav"));
        victoryPlayer->play();

        QMessageBox::information(nullptr, "Game Complete",
                                 QString("Congratulations! You finished all 5 levels!\nTotal Score: %1").arg(score->getScore()));

        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Game Finished",
                                                                  "Do you want to restart the game?",
                                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (reply == QMessageBox::Yes)
        {
            currentLevel = 1;
            score->reset();
            playerLives->reset();
            player->setPos(100, -420);
            loadLevel(currentLevel);
            themePlayer->play();
            loop->start(16);
        }
        else
        {
            if (!views().isEmpty())
                views().first()->window()->close();
        }
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Level Start",
                                  QString("You will enter Level %1. Continue?").arg(currentLevel),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (reply == QMessageBox::No)
    {
        if (!views().isEmpty())
            views().first()->window()->close();
    }
}

void GameScene::keyPressEvent(QKeyEvent* event) { input->keyPressed(event->key()); }
void GameScene::keyReleaseEvent(QKeyEvent* event) { input->keyReleased(event->key()); }
void GameScene::updateGame()
{
    player->applyGravity();
    player->updatePosition();

    checkSpikeCollisions();
    checkEnemyCollisions();
    checkBonusCollisions();

    int dx = player->x() - lastX;
    if (dx >= 10)
    {
        score->increase(dx / 10);
        lastX += (dx / 10) * 10;
    }

    if (!views().isEmpty())
    {
        score->setPos(views().first()->mapToScene(20, 20));
        playerLives->setPos(views().first()->mapToScene(150, 20));
        invTimerProxy->setPos(views().first()->mapToScene(300, 20));
    }

    if (!views().isEmpty()) views().first()->centerOn(player);
    if (player->x() > sceneWidth - 100)
    {
        loop->stop();
        currentLevel++;
        loadLevel(currentLevel);
        player->setPos(100, -420);
        lastX = player->x();
        loop->start(16);
    }
}

void GameScene::checkSpikeCollisions()
{
    if (invincible) return;
    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList)
    {
        if (dynamic_cast<Spike*>(item))
        {
            invincible = true;
            player->playDamage();
            invincibleTimer->start(1000);
            playerLives->decrease();

            if (playerLives->getLives() <= 0)
            {
                handleGameOver();
                return;
            }
            else {
                player->setPos(player->x(), player->y() - 50);
            }
            break;
        }
    }
}

void GameScene::checkEnemyCollisions()
{
    if (invincible) return;
    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList)
    {
        if (dynamic_cast<Enemy*>(item))
        {
            invincible = true;
            player->playDamage();
            invincibleTimer->start(1000);
            playerLives->decrease();

            if (playerLives->getLives() <= 0)
            {
                handleGameOver();
                return;
            }
            else {
                player->setPos(player->x(), player->y() - 50);
            }
            break;
        }
    }
}

void GameScene::checkBonusCollisions()
{
    if (!bonus) return;
    if (player->collidesWithItem(bonus))
    {
        removeItem(bonus);
        delete bonus;
        bonus = nullptr;

        int powerupType = QRandomGenerator::global()->bounded(2);
        if (powerupType == 0){
            playerLives->increase();
        }
        else if (powerupType == 1){
            invincible = true;
            invSecondsRemaining = 10;
            invTimerDisplay->display(invSecondsRemaining);
            invCountdownTimer->start(1000);
            invincibleTimer->start(10000);
        }
    }
}

void GameScene::updateInvTimer()
{
    invSecondsRemaining--;
    if (invSecondsRemaining <= 0)
    {
        invCountdownTimer->stop();
        invTimerDisplay->display(0);
    }
    else
    {
        invTimerDisplay->display(invSecondsRemaining);
    }
}

void GameScene::endInvincibility()
{
    invincible = false;
    invSecondsRemaining = 0;
    invTimerDisplay->display(0);
    invCountdownTimer->stop();
}

void GameScene::handleGameOver()
{
    loop->stop();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Game Over",
                                  QString("You lost all lives!\nTotal Score: %1\nRestart?").arg(score->getScore()),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes)
    {
        currentLevel = 1;
        score->reset();
        playerLives->reset();
        player->setPos(100, -420);
        loadLevel(currentLevel);
        themePlayer->play();
        loop->start(16);
    }
    else
    {
        if (!views().isEmpty())
            views().first()->window()->close();
    }
}
