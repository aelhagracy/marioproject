#ifndef SPIKE_H
#define SPIKE_H

#include <QGraphicsPixmapItem>

class Spike : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 3 };
    int type() const override { return Type; }

    Spike();
};

#endif
