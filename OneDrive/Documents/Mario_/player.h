#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Player();

    void moveLeft();
    void moveRight();
    void stop();
    void jump();

    void applyGravity();
    void updatePosition();

private:
    float vx = 0;
    float vy = 0;
    bool onGround = true;
};

#endif // PLAYER_H
