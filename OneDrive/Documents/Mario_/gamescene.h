#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem> // Needed for on-screen text
#include "player.h"
#include "inputhandler.h"
#include "score.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene();

signals:
    void gameFinished();   // end game

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    Player* player;
    InputHandler* input;
    QTimer* loop;
    Score* score;
    int lastX = 0;

    QGraphicsTextItem* scoreText;

    // end level
    int sceneWidth;
    int sceneHeight;

    // static for pipe & block tracking
    static int farthestPipe;
    static int farthestB1;
    static int farthestB2;
};

#endif

