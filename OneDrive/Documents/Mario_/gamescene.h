#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include "player.h"
#include "inputhandler.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT   // REQUIRED for signals/slots
public:
    GameScene();

signals:
    void gameFinished();   // NEW signal emitted when Mario leaves the scene

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    Player* player;
    InputHandler* input;
    QTimer* loop;

    // NEW: Scene size for checking end-of-level
    int sceneWidth;
    int sceneHeight;
};

#endif // GAMESCENE_H
