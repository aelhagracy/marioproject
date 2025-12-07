#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Enemy : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent = nullptr);

    void setSpeed(int s) { speed = s; }
    int getSpeed() const { return speed; }

    void setDirection(int d) { direction = d; }
    int getDirection() const { return direction; }

private slots:
    void moveEnemy();

private:
    int direction;
    int speed;
};

#endif // ENEMY_H
