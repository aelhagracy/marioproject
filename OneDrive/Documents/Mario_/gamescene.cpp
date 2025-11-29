#include "gamescene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include "score.h"

GameScene::GameScene()
{
    // background size
    sceneWidth = 6000;
    sceneHeight = 500;

    setSceneRect(0, 0, sceneWidth, sceneHeight);

    // image
    addItem(new QGraphicsPixmapItem(
        QPixmap(":/images/scene.png").scaled(sceneWidth, sceneHeight)
        ));

    // player
    player = new Player();
    addItem(player);
    player->setPos(100, -420);

    // score
    score = new Score();
    addItem(score);
    score->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    score->setPos(20, 20);
    lastX = player->x();

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


    int dx = player->x() - lastX;

    //score add
    if (dx >= 10) {
        int points = dx / 10;
        score->increase(points);
        lastX += points * 10;
    }


    if (!views().isEmpty())
        score->setPos(views().first()->mapToScene(20, 20));

    // camera
    if (!views().isEmpty())
        views().first()->centerOn(player);

    // end level
    if (player->x() > sceneWidth - 100)
    {
        loop->stop();
        emit gameFinished();
    }
}
