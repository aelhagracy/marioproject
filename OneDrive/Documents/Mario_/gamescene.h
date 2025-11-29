#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include "player.h"
#include "inputhandler.h"

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

    // end level
    int sceneWidth;
    int sceneHeight;
};

#endif
