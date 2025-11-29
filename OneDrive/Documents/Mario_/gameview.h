#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QTimer>

class GameView : public QGraphicsView
{
public:
    GameView(QGraphicsScene* scene);
};

#endif
