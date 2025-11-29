#include <cstdlib>
#include "gamescene.h"
#include "platform.h"
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

    // player spawn pos
    player->setPos(100, -420);

    input = new InputHandler(player);

    // floating blocks
    platform* float_b1 = new platform(90, 40, ":images/brick.jpg");
    float_b1->setPos(300, 300);
    addItem(float_b1);

    platform* float_b2 = new platform(100, 40, ":images/brick.jpg");
    float_b2->setPos(700, 290);
    addItem(float_b2);

    platform* float_b3 = new platform(90, 40, ":images/brick.jpg");
    float_b3->setPos(1100, 300);
    addItem(float_b3);

    platform* float_b4 = new platform(100, 40, ":images/brick.jpg");
    float_b4->setPos(1600, 290);
    addItem(float_b4);

    platform* float_b5 = new platform(90, 40, ":images/brick.jpg");
    float_b5->setPos(2000, 280);
    addItem(float_b5);

    platform* float_b6 = new platform(100, 40, ":images/brick.jpg");
    float_b6->setPos(2500, 300);
    addItem(float_b6);

    // score
    score = new Score();
    addItem(score);
    score->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    score->setPos(20, 20);
    lastX = player->x();

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

    //score add
    int dx = player->x() - lastX;
    if (dx >= 10) {
        int points = dx / 10;
        score->increase(points);
        lastX += points * 10;
    }

    if (!views().isEmpty())
        score->setPos(views().first()->mapToScene(20, 20));

    //platform pipes
    int rand_space = 150 + rand() % 520;
    if (player->x() + 900 > farthestPipe) {
        platform* pipe = new platform(150, 120, ":images/pipe.png");
        pipe->setPos(farthestPipe, 358);
        addItem(pipe);

        farthestPipe += rand_space + 150;
    }

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

// initialize static members
int GameScene::farthestPipe = 1200;
int GameScene::farthestB1 = 900;
int GameScene::farthestB2 = 1000;
