#include "player.h"
#include <QPixmap>

Player::Player()
{
    setPixmap(QPixmap(":/images/mario.png").scaled(50, 50));
}

void Player::moveLeft() { vx = -4; }
void Player::moveRight(){ vx =  4; }
void Player::stop()     { vx =  0; }

void Player::jump()
{
    if (onGround) {
        vy = -15;
        onGround = false;
    }
}

void Player::applyGravity()
{
    vy += 0.6;
    if (vy > 10) vy = 10;
}

void Player::updatePosition()
{
    setX(x() + vx);
    setY(y() + vy);

    if (y() >= 406) {
        setY(406);
        vy = 0;
        onGround = true;
    }
}
