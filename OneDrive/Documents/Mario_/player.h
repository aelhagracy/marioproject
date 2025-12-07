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

signals:
    void hitSpike();   // Game over

private:
    float vx = 0;
    float vy = 0;
    bool onGround = false;
};

#endif // PLAYER_H
