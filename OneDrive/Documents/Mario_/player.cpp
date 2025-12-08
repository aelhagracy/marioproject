#include "player.h"
#include "spike.h"
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsScene>

Player::Player()
{

    setPixmap(QPixmap(":/images/mario.png").scaled(50, 50));
    jumpSound.setSource(QUrl("qrc:/sound/jump.wav"));
    jumpSound.setLoopCount(0);
    jumpSound.setVolume(0.50);
    damageSound.setSource(QUrl("qrc:/sound/dmg.wav"));
    damageSound.setVolume(0.6f);
    damageSound.setLoopCount(1);

}

void Player::moveLeft() { vx = -4; }
void Player::moveRight(){ vx = 4; }
void Player::stop() { vx = 0; }

void Player::jump()
{
    if (onGround)
    {
        vy = -17;
        onGround = false;


        if (jumpSound.status() == QSoundEffect::Ready)
            jumpSound.play();
        else
            jumpSound.setSource(jumpSound.source());
    }
}

void Player::playDamage()
{
    if (damageSound.status() == QSoundEffect::Ready)
        damageSound.play();
    else
        damageSound.setSource(damageSound.source());
}

void Player::applyGravity()
{
    vy += 0.6;
    if (vy > 10) vy = 10;
}

void Player::updatePosition()
{

    setX(x() + vx);

    QList<QGraphicsItem*> colliding = collidingItems(Qt::IntersectsItemBoundingRect);

    for (QGraphicsItem* item : colliding) {
        if (item->type() == Spike::Type) {
            emit hitSpike();
            return;
        }
    }


    for (QGraphicsItem* item : colliding) {
        if (item->flags() & QGraphicsItem::ItemIsSelectable) {
            if (vx > 0)
                setX(item->sceneBoundingRect().left() - boundingRect().width());
            else if (vx < 0)
                setX(item->sceneBoundingRect().right());
            vx = 0;
        }
    }


    setY(y() + vy);
    colliding = collidingItems(Qt::IntersectsItemBoundingRect);

    onGround = false;
    for (QGraphicsItem* item : colliding) {
        if (item->type() == Spike::Type) {
            emit hitSpike();
            return;
        }
    }

    for (QGraphicsItem* item : colliding) {
        if (item->flags() & QGraphicsItem::ItemIsSelectable) {

            QRectF obs = item->sceneBoundingRect();

            if (vy > 0) {

                setY(obs.top() - boundingRect().height());
                vy = 0;
                onGround = true;
            }
            else if (vy < 0) {

                setY(obs.bottom());
                vy = 0;
            }
        }
    }

    if (y() >= 406) {
        setY(406);
        vy = 0;
        onGround = true;
    }
}

