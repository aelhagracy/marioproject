#ifndef bonusblock_h
#define bonusblock_h

#include <QGraphicsPixmapItem>

class bonusblock : public QGraphicsPixmapItem
{
public:
    bonusblock();

    bool collected = false;
};

#endif
