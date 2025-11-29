#include "gamescene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>

GameScene::GameScene()
{
    // Make scene longer (background lasts longer)
    sceneWidth = 6000;      // NEW
    sceneHeight = 500;

    setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Background image scaled to new width
    addItem(new QGraphicsPixmapItem(
        QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight)
        ));

    // Player
    player = new Player();
    addItem(player);

    // Lower the player's position slightly
    player->setPos(100, -420);    // CHANGED from 350 → 380

    // Input Handler
    input = new InputHandler(player);

    // Game loop (60 FPS)
    loop = new QTimer(this);
    connect(loop, &QTimer::timeout, this, &GameScene::updateGame);
    loop->start(16);
}

void GameScene::keyPressEvent(QKeyEvent* event)
{
    input->keyPressed(event->key());
}

void GameScene::keyReleaseEvent(QKeyEvent* event)
{
    input->keyReleased(event->key());
}

void GameScene::updateGame()
{
    player->applyGravity();
    player->updatePosition();

    // Camera follows player
    if (!views().isEmpty())
        views().first()->centerOn(player);

    // GAME END CONDITION — Mario leaves the screen
    if (player->x() > sceneWidth - 100)   // NEW
    {
        loop->stop();  // Stop game loop

        emit gameFinished();  // Notify main window or controller
    }
}
