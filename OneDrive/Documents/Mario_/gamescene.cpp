#include "gamescene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>

GameScene::GameScene()
{
    // background size
    sceneWidth = 6000;      // NEW
    sceneHeight = 500;

    setSceneRect(0, 0, sceneWidth, sceneHeight);

    // image
    addItem(new QGraphicsPixmapItem(
        QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight)
        ));

    // player
    player = new Player();
    addItem(player);

    // player spawn pos
    player->setPos(100, -420);


    input = new InputHandler(player);


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

    // camera
    if (!views().isEmpty())
        views().first()->centerOn(player);

    // end level condition
    if (player->x() > sceneWidth - 100)
    {
        loop->stop();

        emit gameFinished();
    }
}
