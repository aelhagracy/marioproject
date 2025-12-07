#include "enemy.h"
#include "spike.h"
#include <QGraphicsScene>
#include <QList>

Enemy::Enemy(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/enemy.png").scaled(50, 50));

    direction = 1;
    speed = 2;

    //timer to move the enemy
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::moveEnemy);
    timer->start(30);
}

void Enemy::moveEnemy(){
    setX(x() + direction * speed);

    //everse direction at spikes
    QList<QGraphicsItem*> collidingEnemy = collidingItems();
    for (auto item: collidingEnemy){
        if (dynamic_cast<Spike*>(item)){
            direction *= -1;
            setX(x() + direction * speed);
            break;
        }

    //reverse direction at edge
    if (x() <= 0 || x() + boundingRect().width() >= scene()->width()){
        direction *= -1;
        }
    }
}
