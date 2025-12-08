#include "gamescene.h"
#include "platform.h"
#include "enemy.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>

int GameScene::farthestSpike = 1200;

GameScene::GameScene()
{
    // Scene dimensions
    sceneWidth = 6000;
    sceneHeight = 500;
    setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Background
    auto background = new QGraphicsPixmapItem(QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight));
    background->setZValue(0);
    addItem(background);

    // Player
    player = new Player();
    player->setPos(100, -420);
    player->setZValue(5);
    addItem(player);

    input = new InputHandler(player);

    // Score
    score = new Score();
    addItem(score);
    score->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    score->setPos(20, 20);
    lastX = player->x();

    // Lives
    playerLives = new Life(nullptr, 3);
    addItem(playerLives);
    playerLives->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    playerLives->setPos(150, 20);

    // Invincibility timer
    invincible = false;
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);
    connect(invincibleTimer, &QTimer::timeout, this, &GameScene::endInvincibility);

    // Invincibility display
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

    // Load first level
    loadLevel(currentLevel);

    // Main loop
    loop = new QTimer(this);
    connect(loop, &QTimer::timeout, this, &GameScene::updateGame);
    loop->start(16);
}

void GameScene::loadLevel(int level)
{
    // Clear existing platforms, spikes, enemies, bonus
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

    // Platforms: closer together now
    int numPlatforms = 10 + level * 2;
    int x = 300;
    for (int i = 0; i < numPlatforms; ++i)
    {
        int width = 90 + (QRandomGenerator::global()->bounded(2) * 10);
        int height = 40;
        platform* plat = new platform(width, height, ":/images/brick.jpg");
        int y = 250 + QRandomGenerator::global()->bounded(120); // slightly less variation
        plat->setPos(x, y);
        plat->setZValue(3);
        addItem(plat);
        x += 300 + QRandomGenerator::global()->bounded(100); // closer spacing
    }

    // Spikes
    int numSpikes = 3 + level;
    for (int i = 0; i < numSpikes; ++i)
    {
        Spike* spike = new Spike();
        int sx = QRandomGenerator::global()->bounded(600, sceneWidth - 200);
        spike->setPos(sx, 404);
        spike->setZValue(4);
        addItem(spike);
    }

    // Enemies
    int numEnemies = 2 + level;
    for (int i = 0; i < numEnemies; ++i)
    {
        Enemy* enemy = new Enemy();
        int ex = QRandomGenerator::global()->bounded(400, sceneWidth - 300);
        enemy->setPos(ex, 404);
        enemy->setZValue(4);
        addItem(enemy);
    }

    // Bonus block: random
    bonus = new bonusblock();
    bonus->setPixmap(QPixmap(":/images/bonus.jpg").scaled(40, 40));
    int bx = QRandomGenerator::global()->bounded(200, sceneWidth - 200);
    int by = QRandomGenerator::global()->bounded(100, 350);
    bonus->setPos(bx, by);
    bonus->setZValue(10);
    addItem(bonus);

    showLevelPopup();
}

void GameScene::showLevelPopup()
{
    if (currentLevel > 5)
    {
        QMessageBox::information(nullptr, "Game Complete",
                                 QString("Congratulations! You finished all 5 levels!\nTotal Score: %1").arg(score->getScore()));
        // Ask restart or quit
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

// Key input
void GameScene::keyPressEvent(QKeyEvent* event) { input->keyPressed(event->key()); }
void GameScene::keyReleaseEvent(QKeyEvent* event) { input->keyReleased(event->key()); }

// Main loop
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

    // Level complete
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

// Spike collisions
void GameScene::checkSpikeCollisions()
{
    if (invincible) return;
    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList)
    {
        if (dynamic_cast<Spike*>(item))
        {
            invincible = true;
            invincibleTimer->start(1000);
            playerLives->decrease();
            if (playerLives->getLives() <= 0)
            {
                handleGameOver();
                return;
            }
            else { player->setPos(player->x(), player->y() - 50); }
            break;
        }
    }
}

// Enemy collisions
void GameScene::checkEnemyCollisions()
{
    if (invincible) return;
    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList)
    {
        if (dynamic_cast<Enemy*>(item))
        {
            invincible = true;
            invincibleTimer->start(1000);
            playerLives->decrease();
            if (playerLives->getLives() <= 0)
            {
                handleGameOver();
                return;
            }
            else { player->setPos(player->x(), player->y() - 50); }
            break;
        }
    }
}

// Handle bonus collisions
void GameScene::checkBonusCollisions()
{
    if (!bonus) return;
    if (player->collidesWithItem(bonus))
    {
        removeItem(bonus);
        delete bonus;
        bonus = nullptr;

        invincible = true;
        invSecondsRemaining = 10;
        invTimerDisplay->display(invSecondsRemaining);
        invCountdownTimer->start(1000);
        invincibleTimer->start(10000);
    }
}

// Invincibility timer
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

// Handle game over (lives = 0)
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
        loop->start(16);
    }
    else
    {
        if (!views().isEmpty())
            views().first()->window()->close();
    }
}
