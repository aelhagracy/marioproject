#include "gameview.h"

GameView::GameView(QGraphicsScene* scene) : QGraphicsView(scene)
{
    setFixedSize(1000, 500);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
