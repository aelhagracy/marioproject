#include "player.h"
#include "spike.h"
#include <QPixmap>
#include <QGraphicsItem>

Player::Player()
{
    setPixmap(QPixmap(":/images/mario.png").scaled(50, 50));
}

void Player::moveLeft() { vx = -4; }
void Player::moveRight(){ vx = 4; }
void Player::stop() { vx = 0; }

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
    // ------------------------
    // MOVE HORIZONTALLY
    // ------------------------
    setX(x() + vx);

    QList<QGraphicsItem*> colliding = collidingItems(Qt::IntersectsItemBoundingRect);

    // Check for spikes horizontally
    for (QGraphicsItem* item : colliding) {
        if (item->type() == Spike::Type) {
            emit hitSpike();
            return;
        }
    }

    // Handle horizontal collisions with platforms
    for (QGraphicsItem* item : colliding) {
        if (item->flags() & QGraphicsItem::ItemIsSelectable) {

            if (vx > 0) {
                setX(item->sceneBoundingRect().left() - boundingRect().width());
            }
            else if (vx < 0) {
                setX(item->sceneBoundingRect().right());
            }

            vx = 0;
        }
    }

    // ------------------------
    // MOVE VERTICALLY
    // ------------------------
    setY(y() + vy);
    colliding = collidingItems(Qt::IntersectsItemBoundingRect);

    onGround = false;

    // Check for spikes vertically
    for (QGraphicsItem* item : colliding) {
        if (item->type() == Spike::Type) {
            emit hitSpike();
            return;
        }
    }

    // Handle vertical collisions with platforms
    for (QGraphicsItem* item : colliding) {
        if (item->flags() & QGraphicsItem::ItemIsSelectable) {

            QRectF obsRect = item->sceneBoundingRect();

            if (vy > 0) {
                // Falling → land on platform
                setY(obsRect.top() - boundingRect().height());
                vy = 0;
                onGround = true;
            }
            else if (vy < 0) {
                // Jumping → hit the bottom of platform
                setY(obsRect.bottom());
                vy = 0;
            }
        }
    }

    // ------------------------
    // GROUND CHECK
    // ------------------------
    if (y() >= 406) {
        setY(406);
        vy = 0;
        onGround = true;
    }
}

