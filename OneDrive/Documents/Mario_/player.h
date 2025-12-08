#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QSoundEffect>

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

    void playJump();
    void playDamage();

signals:
    void hitSpike();

private:
    qreal vx = 0;
    qreal vy = 0;
    bool onGround = false;

    QSoundEffect jumpSound;
    QSoundEffect damageSound;
};

#endif
