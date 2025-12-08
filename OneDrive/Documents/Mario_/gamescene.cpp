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

    // Floating platforms
    platform* b1 = new platform(90, 40, ":/images/brick.jpg"); b1->setPos(300, 300); b1->setZValue(3); addItem(b1);
    platform* b2 = new platform(100, 40, ":/images/brick.jpg"); b2->setPos(700, 290); b2->setZValue(3); addItem(b2);
    platform* b3 = new platform(90, 40, ":/images/brick.jpg"); b3->setPos(1100, 300); b3->setZValue(3); addItem(b3);
    platform* b4 = new platform(100, 40, ":/images/brick.jpg"); b4->setPos(1600, 290); b4->setZValue(3); addItem(b4);
    platform* b5 = new platform(90, 40, ":/images/brick.jpg"); b5->setPos(2000, 280); b5->setZValue(3); addItem(b5);
    platform* b6 = new platform(100, 40, ":/images/brick.jpg"); b6->setPos(2500, 300); b6->setZValue(3); addItem(b6);
    platform* b7 = new platform(90, 40, ":/images/brick.jpg"); b7->setPos(3000, 300); b7->setZValue(3); addItem(b7);
    platform* b8 = new platform(100, 40, ":/images/brick.jpg"); b8->setPos(3600, 290); b8->setZValue(3); addItem(b8);
    platform* b9 = new platform(90, 40, ":/images/brick.jpg"); b9->setPos(4100, 300); b9->setZValue(3); addItem(b9);
    platform* b10 = new platform(100, 40, ":/images/brick.jpg"); b10->setPos(4700, 290); b10->setZValue(3); addItem(b10);

    // Enemies
    Enemy* enemy1 = new Enemy(); enemy1->setPos(400, 404); enemy1->setZValue(4); addItem(enemy1);
    Enemy* enemy2 = new Enemy(); enemy2->setPos(1700, 404); enemy2->setZValue(4); addItem(enemy2);
    Enemy* enemy3 = new Enemy(); enemy3->setPos(2200, 404); enemy3->setZValue(4); addItem(enemy3);
    Enemy* enemy4 = new Enemy(); enemy4->setPos(2800, 404); enemy4->setZValue(4); addItem(enemy4);

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
    playerLives->setPos(150, 150);

    // Invincibility timer
    invincible = false;
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);
    connect(invincibleTimer, &QTimer::timeout, this, &GameScene::endInvincibility);

    // Invincibility display
    invTimerDisplay = new QLCDNumber(2);
    invTimerDisplay->setSegmentStyle(QLCDNumber::Flat);
    invTimerDisplay->display(0);
    invTimerDisplay->setStyleSheet("background:transparent; color:yellow;");

    invTimerProxy = addWidget(invTimerDisplay); // add via proxy
    invTimerProxy->setZValue(5);
    invTimerProxy->setPos(300, 20);

    invCountdownTimer = new QTimer(this);
    connect(invCountdownTimer, &QTimer::timeout, this, &GameScene::updateInvTimer);
    invSecondsRemaining = 0;

    // Spawn initial spikes
    spawnSpikes();

    // Bonus block (random location)
    bonus = new bonusblock();
    bonus->setPixmap(QPixmap(":/images/bonus.jpg").scaled(40, 40));

    int bonusX = QRandomGenerator::global()->bounded(200, sceneWidth - 100);
    int bonusY = QRandomGenerator::global()->bounded(150, 350);
    bonus->setPos(bonusX, bonusY);

    bonus->setZValue(10);
    addItem(bonus);

    // Main loop
    loop = new QTimer(this);
    connect(loop, &QTimer::timeout, this, &GameScene::updateGame);
    loop->start(16);
}

// Spawn spikes
void GameScene::spawnSpikes()
{
    int minX = 600;
    int maxX = sceneWidth - 200;
    int spikesCount = 3;

    for (int i = 0; i < spikesCount; ++i)
    {
        Spike* spike = new Spike();
        int x = QRandomGenerator::global()->bounded(minX, maxX);
        int y = 404;
        spike->setPos(x, y);
        spike->setZValue(4);
        addItem(spike);
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
    if (dx >= 10) { score->increase(dx / 10); lastX += (dx / 10) * 10; }

    if (!views().isEmpty())
    {
        score->setPos(views().first()->mapToScene(20, 20));
        playerLives->setPos(views().first()->mapToScene(150, 20));
        invTimerProxy->setPos(views().first()->mapToScene(300, 20));
    }

    if (!views().isEmpty()) views().first()->centerOn(player);

    if (player->x() > sceneWidth - 100) { loop->stop(); emit gameFinished(); }
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
            if (playerLives->getLives() <= 0) { onPlayerDied(); return; }
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
            if (playerLives->getLives() <= 0) { onPlayerDied(); return; }
            else { player->setPos(player->x(), player->y() - 50); }
            break;
        }
    }
}

// Bonus collision
void GameScene::checkBonusCollisions()
{
    if (!bonus) return;

    if (player->collidesWithItem(bonus))
    {
        removeItem(bonus);
        delete bonus;
        bonus = nullptr;

        invincible = true;
        invSecondsRemaining = 10;   // 10 seconds
        invTimerDisplay->display(invSecondsRemaining);
        invCountdownTimer->start(1000);
        invincibleTimer->start(10000);  // 10 seconds
    }
}

// Update invincibility countdown
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

// End invincibility
void GameScene::endInvincibility()
{
    invincible = false;
    invSecondsRemaining = 0;
    invTimerDisplay->display(0);
    invCountdownTimer->stop();
}

// Player died
void GameScene::onPlayerDied()
{
    loop->stop();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Game Over",
                                  "You died! Would you like to retry again?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes)
    {
        player->setPos(100, -420);
        score->reset();
        lastX = player->x();
        playerLives->reset();
        invincible = false;

        QList<QGraphicsItem*> itemsToRemove;
        for (auto item : items()) { if (dynamic_cast<Spike*>(item)) itemsToRemove.append(item); }
        for (auto item : itemsToRemove) { removeItem(item); delete item; }

        spawnSpikes();

        // Respawn bonus at random location
        bonus = new bonusblock();
        bonus->setPixmap(QPixmap(":/images/bonus.jpg").scaled(40, 40));

        int bonusX = QRandomGenerator::global()->bounded(200, sceneWidth - 100);
        int bonusY = QRandomGenerator::global()->bounded(150, 350);
        bonus->setPos(bonusX, bonusY);

        bonus->setZValue(10);
        addItem(bonus);

        loop->start(16);
    }
    else
    {
        if (!views().isEmpty()) views().first()->window()->close();
    }
}
