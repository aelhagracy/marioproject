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
    addItem(new QGraphicsPixmapItem(
        QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight)
        ));

    // Player
    player = new Player();
    addItem(player);
    player->setPos(100, -420);

    input = new InputHandler(player);

    // Floating platforms
    platform* b1 = new platform(90, 40, ":/images/brick.jpg"); b1->setPos(300, 300); addItem(b1);
    platform* b2 = new platform(100, 40, ":/images/brick.jpg"); b2->setPos(700, 290); addItem(b2);
    platform* b3 = new platform(90, 40, ":/images/brick.jpg"); b3->setPos(1100, 300); addItem(b3);
    platform* b4 = new platform(100, 40, ":/images/brick.jpg"); b4->setPos(1600, 290); addItem(b4);
    platform* b5 = new platform(90, 40, ":/images/brick.jpg"); b5->setPos(2000, 280); addItem(b5);
    platform* b6 = new platform(100, 40, ":/images/brick.jpg"); b6->setPos(2500, 300); addItem(b6);
    platform* b7 = new platform(90, 40, ":/images/brick.jpg"); b7->setPos(3000, 300); addItem(b7);
    platform* b8 = new platform(100, 40, ":/images/brick.jpg"); b8->setPos(3600, 290); addItem(b8);
    platform* b9 = new platform(90, 40, ":/images/brick.jpg"); b9->setPos(4100, 300); addItem(b9);
    platform* b10 = new platform(100, 40, ":/images/brick.jpg"); b10->setPos(4700, 290); addItem(b10);

    //Enemies
    Enemy* enemy1 = new Enemy(); enemy1->setPos(400, 404); addItem(enemy1);
    Enemy* enemy2 = new Enemy(); enemy2->setPos(1700, 404); addItem(enemy2);
    Enemy* enemy3 = new Enemy(); enemy3->setPos(2200, 404); addItem(enemy3);
    Enemy* enemy4 = new Enemy(); enemy4->setPos(2800, 404); addItem(enemy4);

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

    invincible = false;
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);
    connect(invincibleTimer, &QTimer::timeout, this, &GameScene::endInvincibility);

    // Spawn initial spikes
    spawnSpikes();

    // Main loop
    loop = new QTimer(this);
    connect(loop, &QTimer::timeout, this, &GameScene::updateGame);
    loop->start(16);
}

// Spawn spikes randomly
void GameScene::spawnSpikes()
{
    int minX = 600;
    int maxX = sceneWidth - 200;
    int spikesCount = 3;

    for (int i = 0; i < spikesCount; ++i)
    {
        Spike* spike = new Spike();
        int x = QRandomGenerator::global()->bounded(minX, maxX);
        int y = 404; // ground level
        spike->setPos(x, y);
        addItem(spike);
    }
}

// Key input
void GameScene::keyPressEvent(QKeyEvent* event)
{
    input->keyPressed(event->key());
}

void GameScene::keyReleaseEvent(QKeyEvent* event)
{
    input->keyReleased(event->key());
}

// Main game loop
void GameScene::updateGame()
{
    player->applyGravity();
    player->updatePosition();

    // Check collisions with spikes and enemies
    checkSpikeCollisions();
    checkEnemyCollisions();

    // Score progress
    int dx = player->x() - lastX;
    if (dx >= 10) {
        score->increase(dx / 10);
        lastX += (dx / 10) * 10;
    }

    // Keep score and lives fixed on screen
    if (!views().isEmpty()) {
        score->setPos(views().first()->mapToScene(20, 20));
        playerLives->setPos(views().first()->mapToScene(150, 20));
    }

    // Camera follow
    if (!views().isEmpty())
        views().first()->centerOn(player);

    // Level finish
    if (player->x() > sceneWidth - 100)
    {
        loop->stop();
        emit gameFinished();
    }
}

// Check for spike collisions and handle lives + invincibility
void GameScene::checkSpikeCollisions()
{
    if (invincible) return;

    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList) {
        if (dynamic_cast<Spike*>(item)) {

            // Start invincibility immediately
            invincible = true;
            invincibleTimer->start(1000); // 1 second

            // Deduct life
            playerLives->decrease();

            if (playerLives->getLives() <= 0) {
                onPlayerDied();
                return;
            } else {
                // Reset player slightly above current to avoid repeated collision
                player->setPos(player->x(), player->y() - 50);
            }

            break; // Only handle one spike per frame
        }
    }
}

//Check enemy collisions with player + handle lives
void GameScene::checkEnemyCollisions()
{
    if (invincible) return;

    QList<QGraphicsItem*> collidingItemsList = player->collidingItems();
    for (auto item : collidingItemsList) {
        if (dynamic_cast<Enemy*>(item)) {

            // Start invincibility immediately
            invincible = true;
            invincibleTimer->start(1000); // 1 second

            // Deduct life
            playerLives->decrease();

            if (playerLives->getLives() <= 0) {
                onPlayerDied();
                return;
            } else {
                // Reset player slightly above current to avoid repeated collision
                player->setPos(player->x(), player->y() - 50);
            }

            break; // Only handle one enemy per frame
        }
    }
}

// End invincibility
void GameScene::endInvincibility()
{
    invincible = false;
}

// Game over popup
void GameScene::onPlayerDied()
{
    loop->stop();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr,
                                  "Game Over",
                                  "You died! Would you like to retry again?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        // Reset player and score
        player->setPos(100, -420);
        score->reset();
        lastX = player->x();

        // Reset lives
        playerLives->reset();
        invincible = false;

        // Remove existing spikes
        QList<QGraphicsItem*> itemsToRemove;
        for (auto item : items()) {
            if (dynamic_cast<Spike*>(item)) {
                itemsToRemove.append(item);
            }
        }
        for (auto item : itemsToRemove) {
            removeItem(item);
            delete item;
        }

        // Spawn new spikes
        spawnSpikes();

        // Restart game loop
        loop->start(16);
    } else {
        if (!views().isEmpty()) {
            views().first()->window()->close();
        }
    }
}



